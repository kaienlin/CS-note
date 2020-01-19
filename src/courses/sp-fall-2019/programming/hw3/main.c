#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>

#define MAXR 16

#define SIGUSR3 SIGWINCH

#define ERR_EXIT(msg) { char buf[1024]; \
                        snprintf(buf, 1024, "%d, %s", __LINE__, msg); \
                        perror(buf); exit(127); \
                      }
char buf[BUFSIZ];

int mp[4] = {0, SIGUSR1, SIGUSR2, SIGUSR3};

int main()
{
    int P, Q; scanf("%d %d", &P, &Q);
    int R; scanf("%d", &R);
    int sig[MAXR];
    for ( int i = 0; i < R; i++)
        scanf("%d", &sig[i]);

    int pfd[2];
    pipe(pfd);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGUSR3);
    sigprocmask(SIG_BLOCK, &set, NULL);

    pid_t pid = fork();
    if ( pid == -1 ) {
        ERR_EXIT("fork");
    } else if ( pid == 0 ) {
        close(pfd[0]);
        dup2(pfd[1], STDOUT_FILENO);
        char arg[3][16];
        snprintf(arg[1], 16, "%d", P);
        snprintf(arg[2], 16, "%d", Q);
        execl("./hw3", "./hw3", arg[1], arg[2], "3", "0", (char*)0);
    }
    close(pfd[1]);

    for ( int i = 0; i < R; i++) {
        sleep(5);
        kill(pid, mp[sig[i]]);
        read(pfd[0], buf, BUFSIZ);
        if ( sig[i] == 3 ) {
            printf("%s\n", buf);
        }
    }
    read(pfd[0], buf, BUFSIZ);
    printf("%s", buf);

    int status;
    waitpid(pid, &status, 0);

    close(pfd[0]);

    return EXIT_SUCCESS;
}
