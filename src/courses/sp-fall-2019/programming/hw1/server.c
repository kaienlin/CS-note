#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/select.h>

#define ERR_EXIT(a) { perror(a); exit(1); }

typedef struct {
    char hostname[512];  // server's hostname
    unsigned short port;  // port to listen
    int listen_fd;  // fd to wait for a new connection
} server;

typedef struct {
    char host[512];  // client's host
    int conn_fd;  // fd to talk with client
    char buf[512];  // data sent by/to client
    size_t buf_len;  // bytes used by buf
    // you don't need to change this.
	int item;
    int wait_for_write;  // used by handle_read to know if the header is read or not.
} request;

typedef struct {
    int id;
    int balance;
} Account;

server svr;  // server
request* requestP = NULL;  // point to a list of requests
int maxfd;  // size of open file descriptor table, size of request list

const char* accept_read_header = "ACCEPT_FROM_READ";
const char* accept_write_header = "ACCEPT_FROM_WRITE";

// Forwards

static void init_server(unsigned short port);
// initailize a server, exit for error

static void init_request(request* reqP);
// initailize a request instance

static void free_request(request* reqP);
// free resources used by a request instance

static int handle_read(request* reqP);
// return 0: socket ended, request done.
// return 1: success, message (without header) got this time is in reqP->buf with reqP->buf_len bytes. read more until got <= 0.
// It's guaranteed that the header would be correctly set after the first read.
// error code:
// -1: client connection error

// auxiliary function
static void end_conn(int fd) {
    close(fd);
    free_request(&requestP[fd]);
}

static void set_nonblock(int fd) {
    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);
}

// io multiplexing
static void init_io_multiplexing();
static void add_read_set(int fd);
static fd_set do_select();

// request handling
static void init_req_handling();
static void query_for_read(request *req);
static void query_for_write(request *req);
static void do_action(request *req, int fd);

int maxfd_now = 0; // the max fd among current open fds
fd_set master_set;

int main(int argc, char** argv) {
    int i, ret;

    struct sockaddr_in cliaddr;  // used by accept()
    int clilen;

    int conn_fd;  // fd for a new connection with client
    int file_fd;  // fd for file that we open for reading
    char buf[512];
    int buf_len;

    // Parse args.
    if (argc != 2) {
        fprintf(stderr, "usage: %s [port]\n", argv[0]);
        exit(1);
    }

    // Initialize server
    init_server((unsigned short) atoi(argv[1]));

    // change server to nonblocking mode
    set_nonblock(svr.listen_fd);

    // Get file descripter table size and initize request table
    maxfd = getdtablesize();
    requestP = (request*) malloc(sizeof(request) * maxfd);
    if (requestP == NULL) {
        ERR_EXIT("out of memory allocating all requests");
    }
    for (i = 0; i < maxfd; i++) {
        init_request(&requestP[i]);
    }
    requestP[svr.listen_fd].conn_fd = svr.listen_fd;
    strcpy(requestP[svr.listen_fd].host, svr.hostname);

    // Loop for handling connections
    fprintf(stderr, "\nstarting on %.80s, port %d, fd %d, maxconn %d...\n", svr.hostname, svr.port, svr.listen_fd, maxfd);

    init_io_multiplexing();
    add_read_set(svr.listen_fd);
    init_req_handling();

    while (1) {
        // select one ready to read fd
        fd_set working_set = do_select();

        for ( int fd = 3; fd <= maxfd_now; fd++) {
            if ( FD_ISSET(fd, &working_set) ) {
                if ( fd == svr.listen_fd ) {
                    // Check new connection
                    clilen = sizeof(cliaddr);
                    conn_fd = accept(svr.listen_fd, (struct sockaddr*)&cliaddr, (socklen_t*)&clilen);
                    if (conn_fd < 0) {
                        if (errno == EINTR || errno == EAGAIN) { continue; }
                        if (errno == ENFILE) {
                            (void) fprintf(stderr, "out of file descriptor table ... (maxconn %d)\n", maxfd);
                            continue;
                        }
                        ERR_EXIT("accept")
                    }

                    // add conn_fd to io multiplexing waiting queue
                    add_read_set(conn_fd);
                    set_nonblock(conn_fd);

                    requestP[conn_fd].conn_fd = conn_fd;
                    strcpy(requestP[conn_fd].host, inet_ntoa(cliaddr.sin_addr));
                    fprintf(stderr, "getting a new request... fd %d from %s\n", conn_fd, requestP[conn_fd].host);

                } else {
                    FD_CLR(fd, &master_set);
                    ret = handle_read(&requestP[fd]);
                    if (ret < 0) {
                        fprintf(stderr, "bad request from %s\n", requestP[fd].host);
                        continue;
                    } else {
#ifdef READ_SERVER
                        query_for_read(&requestP[fd]);
#else
                        query_for_write(&requestP[fd]);
#endif
                    }
                }
            }
        }
    }
    free(requestP);
    return 0;
}

// Start of IO Multiplexing Utilities
const static struct timeval timeout = { .tv_sec = 0, .tv_usec = 10000 };

static void init_io_multiplexing() {
    FD_ZERO(&master_set);
    maxfd_now = 0;
}

static void add_read_set(int fd) {
    FD_SET(fd, &master_set);
    if ( fd > maxfd_now )
        maxfd_now = fd;
}

static fd_set do_select() {
    fd_set fdset;
    struct timeval tmp_timeout;
    memcpy(&fdset, &master_set, sizeof(fd_set));
    memcpy(&tmp_timeout, &timeout, sizeof(timeout));
    if ( select(maxfd_now+1, &fdset, NULL, NULL, &tmp_timeout) == -1 )
        ERR_EXIT("select");
    return fdset;
}
// End of IO Multiplexing Utilities


// Start of Request Handling Utilities
static bool try_set_lock(short type, off_t start, off_t len);
static bool is_locked(short type, off_t start, off_t len);
static void un_lock(off_t start, off_t len);
static struct flock mk_lock(short type, off_t start, off_t len);
static void send_msg(request *req, char *msg) { write(req->conn_fd, msg, strlen(msg)); }

int account_fd;
int *wait_for_cmd;
bool locked[21];

static void init_req_handling() {
    account_fd = open("account_list", O_RDWR);
    wait_for_cmd = calloc(maxfd + 1, sizeof(int));
    memset(locked, 0, sizeof(locked));
}

static void query_for_read(request *req) {
    static char buf[1<<10];
    int id = atoi(req->buf);

    off_t pos = sizeof(Account) * (id - 1);
    off_t len = sizeof(Account);

    // try set read lock
    if ( locked[id] || !try_set_lock(F_RDLCK, pos, len) ) {
        send_msg(req, "This account is locked.\n");
        end_conn(req->conn_fd);
        return;
    }

    // read bytes
    lseek(account_fd, sizeof(Account) * (id - 1), SEEK_SET);
    Account ans;
    read(account_fd, &ans, sizeof(Account));

    un_lock(pos, len);

    // write to client
    sprintf(buf, "%d %d\n", ans.id, ans.balance);
    send_msg(req, buf);
    end_conn(req->conn_fd);
}

static void query_for_write(request *req) {
    if ( wait_for_cmd[req->conn_fd] ) {
        do_action(req, wait_for_cmd[req->conn_fd]);
        return;
    }

    char buf[1<<10];
    int id = atoi(req->buf);

    off_t pos = sizeof(Account) * (id - 1);
    off_t len = sizeof(Account);

    if ( locked[id] || !try_set_lock(F_WRLCK, pos, len) ) {
        send_msg(req, "This account is locked.\n");
        end_conn(req->conn_fd);
        return;
    }
    
    send_msg(req, "This account is modifiable.\n");

    locked[id] = true;
    wait_for_cmd[req->conn_fd] = id;

    // add to read set
    add_read_set(req->conn_fd);

    return;
}

static void do_action(request *req, int id) {
    char cmd[1<<10], buf[1<<10];
    int n;
    sscanf(req->buf, "%s %d", cmd, &n);

    off_t pos = sizeof(Account) * (id - 1);
    off_t len = sizeof(Account);

    Account a;
    lseek(account_fd, pos, SEEK_SET);
    read(account_fd, &a, sizeof(Account));

    bool failed = false;
    if ( strcmp(cmd, "save") == 0 ) {
        if ( n >= 0 ) {
            a.balance += n;
        } else {
            failed = true;
        }
    } else if ( strcmp(cmd, "withdraw") == 0 ) {
        if ( n >= 0 && n <= a.balance ) {
            a.balance -= n;
        } else {
            failed = true;
        }
    } else if ( strcmp(cmd, "transfer") == 0 ) {
        int bid;
        sscanf(req->buf, "%s %d %d", cmd, &bid, &n);
        const off_t bpos = sizeof(Account) * (bid - 1);
        if ( n < 0 || n > a.balance ) {
            failed = true;
        } else {
            a.balance -= n;
            lseek(account_fd, bpos, SEEK_SET);
            Account b;
            read(account_fd, &b, sizeof(Account));
            b.balance += n;
            lseek(account_fd, bpos, SEEK_SET);
            write(account_fd, &b, sizeof(Account));
        }
    } else if ( strcmp(cmd, "balance") == 0 ) {
        if ( n > 0 ) {
            a.balance = n;
        } else {
            failed = true;
        }
    }

    if ( failed ) {
        send_msg(req, "Operation failed.\n");
    } else {
        lseek(account_fd, pos, SEEK_SET);
        write(account_fd, &a, sizeof(Account));
    }

    un_lock(pos, len);

    locked[id] = 0;
    wait_for_cmd[req->conn_fd] = 0;
    end_conn(req->conn_fd);

    return;
}

static bool try_set_lock(short type, off_t start, off_t len) {
    struct flock flk = mk_lock(type, start, len);

    if ( fcntl(account_fd, F_SETLK, &flk) < 0 ) {
        if ( errno == EACCES || errno == EAGAIN ) {
            return false;
        } else {
            ERR_EXIT("fcntl");
        }
    }
    return true;
}

static void un_lock(off_t start, off_t len) {
    struct flock flk = mk_lock(F_UNLCK, start, len);
    if ( fcntl(account_fd, F_SETLK, &flk) < 0 )
        ERR_EXIT("un_lock");
}

static struct flock mk_lock(short type, off_t start, off_t len) {
    struct flock flk;
    flk.l_type = type;
    flk.l_whence = SEEK_SET;
    flk.l_start = start;
    flk.l_len = len;
    return flk;
}
// End of Request Handling Utilities

// ======================================================================================================
// You don't need to know how the following codes are working
#include <fcntl.h>

static void* e_malloc(size_t size);


static void init_request(request* reqP) {
    reqP->conn_fd = -1;
    reqP->buf_len = 0;
    reqP->item = 0;
    reqP->wait_for_write = 0;
}

static void free_request(request* reqP) {
    /*if (reqP->filename != NULL) {
        free(reqP->filename);
        reqP->filename = NULL;
    }*/
    init_request(reqP);
}

// return 0: socket ended, request done.
// return 1: success, message (without header) got this time is in reqP->buf with reqP->buf_len bytes. read more until got <= 0.
// It's guaranteed that the header would be correctly set after the first read.
// error code:
// -1: client connection error
static int handle_read(request* reqP) {
    int r;
    char buf[512];

    // Read in request from client
    r = read(reqP->conn_fd, buf, sizeof(buf));
    if (r < 0) return -1;
    if (r == 0) return 0;
	char* p1 = strstr(buf, "\015\012");
	int newline_len = 2;
	// be careful that in Windows, line ends with \015\012
	if (p1 == NULL) {
		p1 = strstr(buf, "\012");
		newline_len = 1;
		if (p1 == NULL) {
			ERR_EXIT("this really should not happen...");
		}
	}
	size_t len = p1 - buf + 1;
	memmove(reqP->buf, buf, len);
	reqP->buf[len - 1] = '\0';
	reqP->buf_len = len-1;
    return 1;
}

static void init_server(unsigned short port) {
    struct sockaddr_in servaddr;
    int tmp;

    gethostname(svr.hostname, sizeof(svr.hostname));
    svr.port = port;

    svr.listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (svr.listen_fd < 0) ERR_EXIT("socket");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(port);
    tmp = 1;
    if (setsockopt(svr.listen_fd, SOL_SOCKET, SO_REUSEADDR, (void*)&tmp, sizeof(tmp)) < 0) {
        ERR_EXIT("setsockopt");
    }
    if (bind(svr.listen_fd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        ERR_EXIT("bind");
    }
    if (listen(svr.listen_fd, 1024) < 0) {
        ERR_EXIT("listen");
    }
}

static void* e_malloc(size_t size) {
    void* ptr;

    ptr = malloc(size);
    if (ptr == NULL) ERR_EXIT("out of memory");
    return ptr;
}
