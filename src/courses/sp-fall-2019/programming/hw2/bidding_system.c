#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>

const char *const usage_msg = "usage: ./bidding_system [host_num] [player_num]\n";
const char *const read_fifo_name = "Host.FIFO";

#define HOST_NUM_MAX 10
#define PLAYER_NUM_MAX 14
#define FIFO_NAME_MAX 128

typedef struct {
    pid_t pid;
    int fd;
    int random_key;
    char fifo_name[FIFO_NAME_MAX];
} RootHost;

void ERR_EXIT(char *msg) { perror(msg); exit(128); }

void flush_fsync(FILE *stream) { fflush(stream); fsync(fileno(stream)); }

int rank_to_score(int rank) { return 8 - rank; }

int next_subset(int, int[]);

void create_fifo(RootHost root_host_list[], int host_num);

void fork_root_hosts(RootHost root_host_list[], int host_num);

void assign_competition(RootHost *root_host, int set[]);

void output_rank(int player_num, int player_score[]);

int main(int argc, char *argv[])
{
    if ( argc != 3 ) {
        // the number of arguments should be exactly 2
        fprintf(stderr, usage_msg);
        exit(2);
    }
    int host_num = atoi(argv[1]);  // 1 <= x <= 10
    int player_num = atoi(argv[2]);  // 8 <= x <= 14

    FILE *read_fifo;
    RootHost root_host_list[HOST_NUM_MAX + 5];
    create_fifo(root_host_list, host_num);
    
    // fork root hosts
    fork_root_hosts(root_host_list, host_num);
    if ( (read_fifo = fopen(read_fifo_name, "rt")) == NULL )
        ERR_EXIT("fopen");
    unlink(read_fifo_name);

    // assign every hosts a competition
    int set[8] = {1, 2, 3, 4, 5, 6, 7, 8};
    int active_host_num = 0;
    for ( int i = 1; i <= host_num; i++) {
        if ( (i == 1) || (i > 1 && next_subset(player_num, set)) ) {
            assign_competition(&root_host_list[i], set);
            active_host_num++;
        }
    }

    int player_score[PLAYER_NUM_MAX + 5] = {0};

    while ( active_host_num > 0 ) {
        int key;
        fscanf(read_fifo, "%d", &key);
        for ( int i = 0; i < 8; i++) {
            int id, rank;
            fscanf(read_fifo, "%d %d", &id, &rank);
            player_score[id] += rank_to_score(rank);
        }
        for ( int i = 1; i <= host_num; i++) {
            if ( root_host_list[i].random_key == key ) {
                if ( next_subset(player_num, set) ) {
                    assign_competition(&root_host_list[i], set);
                } else {
                    active_host_num--;
                }
            }
        }
    }

    for ( int i = 1; i <= host_num; i++) {
        char *msg = "-1 -1 -1 -1 -1 -1 -1 -1\n";
        write(root_host_list[i].fd, msg, strlen(msg));
        fsync(root_host_list[i].fd);
    }

    for ( int i = 1; i <= host_num; i++) {
        int term_status;
        waitpid(root_host_list[i].pid, &term_status, 0);
        if ( close(root_host_list[i].fd) == -1 )
            ERR_EXIT("close");
    }

    output_rank(player_num, player_score);

    exit(EXIT_SUCCESS);
}

void create_fifo(RootHost root_host_list[], int host_num) {
    if ( mkfifo(read_fifo_name, 0600) < 0 )
        ERR_EXIT("mkfifo");
    for ( int i = 1; i <= host_num; i++) {
        snprintf(root_host_list[i].fifo_name, FIFO_NAME_MAX, "Host%d.FIFO", i);
        if ( mkfifo(root_host_list[i].fifo_name, 0600) < 0 )
            ERR_EXIT("mkfifo");
    }
}

#define KEY_MAX 65536
void fork_root_hosts(RootHost root_host_list[], int host_num) {
    int used[KEY_MAX] = {0};
    srand(time(NULL));
    for ( int i = 1; i <= host_num; i++) {
        do {
            root_host_list[i].random_key = rand() % KEY_MAX;
        } while ( used[root_host_list[i].random_key] );
        
        used[root_host_list[i].random_key] = 1;

        root_host_list[i].pid = fork();
        if ( root_host_list[i].pid == -1 ) {
            ERR_EXIT("fork");
        } else if ( root_host_list[i].pid == 0 ) {
            char host_id_str[32], random_key_str[32];
            snprintf(host_id_str, 32, "%d", i);
            snprintf(random_key_str, 32, "%d", root_host_list[i].random_key);
            execl("./host", "host", host_id_str, random_key_str, "0", (char*)0);
        } else {
            if ( (root_host_list[i].fd = open(root_host_list[i].fifo_name, O_WRONLY)) == -1 )
                ERR_EXIT("open");
            unlink(root_host_list[i].fifo_name);
        }
    }
}

void assign_competition(RootHost *root_host, int set[]) {
    char set_str[BUFSIZ];
    int len = snprintf(set_str, BUFSIZ, "%d %d %d %d %d %d %d %d\n", 
                       set[0], set[1], set[2], set[3], set[4], set[5], set[6], set[7]);
    write(root_host->fd, set_str, len);
    fsync(root_host->fd);
}

int next_subset(int player_num, int set[]) {
    for ( int i = 7; i >= 0; i--) {
        if ( set[i] + (7 - i) < player_num ) {
            set[i]++;
            for ( int j = i + 1; j <= 7; j++)
                set[j] = set[j-1] + 1;
            return 1;
        }
    }
    return 0;
}

int cmp(const void *a, const void *b) {
    int x = **(int**)a, y = **(int**)b;
    return (x < y) - (x > y);
}

void output_rank(int player_num, int player_score[]) {
    int *ptr[PLAYER_NUM_MAX + 5];
    for ( int i = 1; i <= player_num; i++)
        ptr[i] = &player_score[i];
    qsort(ptr + 1, player_num, sizeof(int*), cmp);
    int rank[PLAYER_NUM_MAX + 5];
    rank[ptr[1] - player_score] = 1;
    for ( int i = 2; i <= player_num; i++)
        rank[ptr[i] - player_score] = (*ptr[i] == *ptr[i-1]) ? rank[ptr[i-1] - player_score] : i;
    for ( int i = 1; i <= player_num; i++)
        printf("%d %d\n", i, rank[i]);
}
