#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>

const char *const usage_msg = "usage: ./host [host_id] [random_key] [depth]";
const char *const write_fifo_name = "Host.FIFO";

#define FIFO_NAME_MAX 128
const int ROOT_HOST_DEP = 0;
const int CHILD_HOST_DEP = 1;
const int LEAF_HOST_DEP = 2;
const int ROUND_NUM = 10;

typedef struct {
    int player_id, score, rank;
} PlayerRecord;

typedef struct {
    pid_t pid;
    int wr_pipe_fd, rd_pipe_fd;
} Child;

void ERR_EXIT(char *msg) { perror(msg); exit(128); }

void flush_fsync(FILE *stream) { fflush(stream); fsync(fileno(stream)); }

void read_player_id(int player_id_list[], int n, int fd);

int sublist(int list[], int L, int R, char buf[]);

void fork_child(Child child_list[], int player_id_list[], int depth, char *argv[]);

void run_game(Child child_list[], int depth, void *extra);

void calc_rank(PlayerRecord player_record_list[]);

void clean_child(Child child_host_list[]);

int main(int argc, char *argv[])
{
    if ( argc != 4 ) {
        // the number of arguments should be exactly 3
        fprintf(stderr, usage_msg);
        exit(2);
    }

    int host_id = atoi(argv[1]);
    int random_key = atoi(argv[2]);
    int depth = atoi(argv[3]);

    if ( depth == ROOT_HOST_DEP ) {
        // fifo for read from bidding system
        char fifo_path[FIFO_NAME_MAX];
        snprintf(fifo_path, FIFO_NAME_MAX, "Host%d.FIFO", host_id);
        int read_fifo_fd = open(fifo_path, O_RDONLY);
        if ( read_fifo_fd == -1 )
            ERR_EXIT("open");

        // fifo for write to bidding system
        int write_fifo_fd = open(write_fifo_name, O_WRONLY);
        if ( write_fifo_fd == -1 )
            ERR_EXIT("open");

        // fork 2 child hosts
        Child child_host_list[2];
        fork_child(child_host_list, NULL, depth, argv);

        while ( 1 ) {
            int player_id_list[8];
            read_player_id(player_id_list, 8, read_fifo_fd);
            if ( player_id_list[0] == -1 ) {
                for ( int i = 0; i < 2; i++) {
                    char *msg = "-1 -1 -1 -1\n";
                    write(child_host_list[i].wr_pipe_fd, msg, strlen(msg));
                    fsync(child_host_list[i].wr_pipe_fd);
                }
                break;
            }

            // assign players to 2 child hosts
            for ( int i = 0; i < 2; i++) {
                char buf[BUFSIZ];
                int len = sublist(player_id_list, i * (4 >> depth), i * (4 >> depth) + (4 >> depth), buf);
                write(child_host_list[i].wr_pipe_fd, buf, len);
                fsync(child_host_list[i].wr_pipe_fd);
            }
            
            // init player record
            PlayerRecord player_record_list[8];
            for ( int i = 0; i < 8; i++) {
                player_record_list[i].player_id = player_id_list[i];
                player_record_list[i].score = 0;
            }
            run_game(child_host_list, depth, player_record_list);

            calc_rank(player_record_list);

            char buf[BUFSIZ];
            char *p = buf;
            p += snprintf(buf, BUFSIZ, "%d\n", random_key);
            for ( int i = 0; i < 8; i++) {
                p += snprintf(p, BUFSIZ, "%d %d\n", player_record_list[i].player_id, player_record_list[i].rank);
            }
            write(write_fifo_fd, buf, strlen(buf));
            fsync(write_fifo_fd);
        }

        clean_child(child_host_list);

        close(read_fifo_fd);
        close(write_fifo_fd);

    } else if ( depth == CHILD_HOST_DEP ) {
        Child leaf_host_list[2];
        fork_child(leaf_host_list, NULL, depth, argv);

        while ( 1 ) {
            int player_id_list[4];
            read_player_id(player_id_list, 4, STDIN_FILENO);
            if ( player_id_list[0] == -1 ) {
                for ( int i = 0; i < 2; i++) {
                    char *msg = "-1 -1\n";
                    write(leaf_host_list[i].wr_pipe_fd, msg, strlen(msg));
                    fsync(leaf_host_list[i].wr_pipe_fd);
                }
                break;
            }

            for ( int i = 0; i < 2; i++) {
                char buf[BUFSIZ];
                int len = sublist(player_id_list, i * (4 >> depth), i * (4 >> depth) + (4 >> depth), buf);
                write(leaf_host_list[i].wr_pipe_fd, buf, len);
                fsync(leaf_host_list[i].wr_pipe_fd);
            }
            
            run_game(leaf_host_list, depth, NULL);
        }

        clean_child(leaf_host_list);
    } else if ( depth == LEAF_HOST_DEP ) {
        while ( 1 ) {
            int player_id_list[2];
            read_player_id(player_id_list, 2, STDIN_FILENO);
            if ( player_id_list[0] == -1 ) {
                break;
            }

            Child player_list[2];

            fork_child(player_list, player_id_list, depth, argv);

            run_game(player_list, depth, NULL);

            clean_child(player_list);
        }
    }

    return EXIT_SUCCESS;
}

void read_player_id(int player_id_list[], int n, int fd) {
    char buf[BUFSIZ];
    char *p = buf;
    read(fd, p, BUFSIZ);
    for ( int i = 0; i < n; i++) {
        player_id_list[i] = strtol(p, &p, 10);
    }
}

int sublist(int list[], int L, int R, char buf[]) {
    char *ptr = buf;
    for ( int i = L; i < R; i++)
        ptr += snprintf(ptr, BUFSIZ, "%d%c", list[i], " \n"[i==R-1]);
    return (ptr - buf);
}

void fork_child(Child child_list[], int player_id_list[], int depth, char *argv[]) {
    for ( int i = 0; i < 2; i++) {
        // pipe
        int pfd[2][2];
        int pip_res1 = pipe(pfd[0]);  // parent read, child write
        int pip_res2 = pipe(pfd[1]);  // parent write, child read
        if ( pip_res1 == -1 || pip_res2 == -1 )
            ERR_EXIT("pipe");

        // fork
        child_list[i].pid = fork();
        if ( child_list[i].pid == -1 ) {
            ERR_EXIT("fork");
        } else if ( child_list[i].pid == 0 ) {
            // child
            close(pfd[0][0]);
            dup2(pfd[0][1], 1);
            close(pfd[0][1]);

            close(pfd[1][1]);
            dup2(pfd[1][0], 0);
            close(pfd[1][0]);

            if ( depth != LEAF_HOST_DEP ) {
                char next_dep[4];
                snprintf(next_dep, 4, "%d", depth + 1);
                execl("./host", "host", argv[1], argv[2], next_dep, NULL);
            } else {
                char buf[4];
                snprintf(buf, 4, "%d", player_id_list[i]);
                execl("./player", "player", buf, NULL);
            }

        } else {
            // parent
            close(pfd[0][1]);
            child_list[i].rd_pipe_fd = pfd[0][0];

            close(pfd[1][0]);
            child_list[i].wr_pipe_fd = pfd[1][1];
        }
    }
}

void run_game(Child child_list[], int depth, void *extra) {
    PlayerRecord *player_record_list = (PlayerRecord*)extra;
    int id_2_idx[32];
    for ( int i = 0; depth == 0 && i < 8; i++)
        id_2_idx[ player_record_list[i].player_id ] = i;

    for ( int round = 1; round <= ROUND_NUM; round++) {
        // read result from two child hosts
        int player_id[2], money[2];
        char buf[BUFSIZ];
        read(child_list[0].rd_pipe_fd, buf, BUFSIZ);
        sscanf(buf, "%d %d", &player_id[0], &money[0]);

        read(child_list[1].rd_pipe_fd, buf, BUFSIZ);
        sscanf(buf, "%d %d", &player_id[1], &money[1]);
        // compare two results
        int winner_id, win_money;
        if ( money[0] > money[1] ) {
            winner_id = player_id[0];
            win_money = money[0];
        } else {
            winner_id = player_id[1];
            win_money = money[1];
        }
        
        if ( depth == 0 ) {
            // if dep == 0: update player score and write winner to 2 child
            ++player_record_list[ id_2_idx[winner_id] ].score;
            if ( round != ROUND_NUM ) {
                for ( int i = 0; i < 2; i++) {
                    int len = sprintf(buf, "%d\n", winner_id);
                    write(child_list[i].wr_pipe_fd, buf, len);
                    fsync(child_list[i].wr_pipe_fd);
                }
            }
        } else {
            // if dep > 0: write winner, money to stdout and read final winner from stdin and write final winner to 2 child
            int len = sprintf(buf, "%d %d\n", winner_id, win_money);
            write(STDOUT_FILENO, buf, len);
            fsync(STDOUT_FILENO);

            if ( round != ROUND_NUM ) {
                int len = read(STDIN_FILENO, buf, BUFSIZ);
                for ( int i = 0; i < 2; i++) {
                    write(child_list[i].wr_pipe_fd, buf, len);
                    fsync(child_list[i].wr_pipe_fd);
                }
            }
        }
    }
}

int cmp(const void *a, const void *b) {
    int x = (*(PlayerRecord**)a)->score, y = (*(PlayerRecord**)b)->score;
    return (x < y) - (x > y);
}

void calc_rank(PlayerRecord player_record_list[]) {
    PlayerRecord* ptr[8];
    for ( int i = 0; i < 8; i++)
        ptr[i] = &player_record_list[i];
    qsort(ptr, 8, sizeof(PlayerRecord*), cmp);
    ptr[0]->rank = 1;
    for ( int i = 1; i < 8; i++)
        ptr[i]->rank = (ptr[i]->score == ptr[i-1]->score) ? ptr[i-1]->rank : i + 1;
}

void clean_child(Child child_list[]) {
    for ( int i = 0; i < 2; i++) {        
        int status;
        waitpid(child_list[i].pid, &status, 0);

        close(child_list[i].rd_pipe_fd);
        close(child_list[i].wr_pipe_fd);
    }
}