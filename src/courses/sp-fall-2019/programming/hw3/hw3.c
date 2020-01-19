#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <setjmp.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#include "scheduler.h"
#define SIGUSR3 SIGWINCH

#define ERR_EXIT(msg) { char buf[1024]; \
                        snprintf(buf, 1024, "%d, %s", __LINE__, msg); \
                        perror(buf); exit(127); \
                      }

typedef void (*sighandler_t)(int);

void funct_5(int name) {
    int a[10000];
    switch ( name ) {
        case 1: funct_1(1); break;
        case 2: funct_2(2); break;
        case 3: funct_3(3); break;
        case 4: funct_4(4); break;
        default: ERR_EXIT("argument error");
    }
}

int P, Q, K, TASK;
int idx = 0, mutex = 0;
char arr[10000];

jmp_buf SCHEDULER;
jmp_buf main_env;

FCB fcb_list[5];
FCB_ptr Current, Head;

int in_queue[5];

sigset_t oldset;

void sigusr1_act(int signo) {
    //fprintf(stderr, "get SIGUSR1\n");
    char *ACK = "ACK1";
    write(STDOUT_FILENO, ACK, 5);
    sigprocmask(SIG_SETMASK, &oldset, NULL);
    longjmp(SCHEDULER, 1);
}

void sigusr2_act(int signo) {
    //fprintf(stderr, "get SIGUSR2\n");
    char *ACK = "ACK2";
    write(STDOUT_FILENO, ACK, 5);
    sigprocmask(SIG_SETMASK, &oldset, NULL);
    longjmp(SCHEDULER, 1);
}

void sigusr3_act(int signo) {
    //fprintf(stderr, "get SIGUSR3\n");
    char buf[BUFSIZ]; 
    char *p = buf;
    for ( int i = 1; i <= 4; i++) {
        if ( in_queue[i] ) {
            p += sprintf(p, "%d ", i);
        }
    }
    write(STDOUT_FILENO, buf, strlen(buf) + 1);
    sigprocmask(SIG_SETMASK, &oldset, NULL);
    longjmp(SCHEDULER, 1);
}

void funct_1(int n) {
    int sj = setjmp(fcb_list[n].Environment);
    if ( sj < 0 ) {
        ERR_EXIT("setjmp");
    } else if ( sj == 0 ) {
        if ( n != 4 )
            funct_5(n + 1);
        else // n = 4
            longjmp(main_env, 1);
    } else if ( sj > 0 ) {
        for ( int i = 1; i <= P; i++) {
            // check lock
            while ( 1 ) {
                if ( mutex != 0 && mutex != n ) {
                    in_queue[n] = 1;
                    int sj = setjmp(fcb_list[n].Environment);
                    if ( sj == 0 )
                        longjmp(SCHEDULER, 1);
                } else {
                    break;
                }
            }

            in_queue[n] = 0;
            mutex = n; // acquire lock

            for ( int j = 1; j <= Q; j++) {
                sleep(1);
                //fprintf(stderr, "append %d\n", n);
                arr[idx++] = '0' + n;
            }

            // For task 2
            if ( i % K == 0 ) {
                mutex = 0; // release lock
                int res = setjmp(fcb_list[n].Environment);
                if ( res < 0 ) {
                    ERR_EXIT("setjmp");
                } else if ( res == 0 && i != P ) {
                    longjmp(SCHEDULER, 1);
                }
            }

            // For task 3
            sigset_t set;
            int signo = -1;
            int res = sigpending(&set);
            if ( sigismember(&set, SIGUSR1) ) {
                signo = SIGUSR1;
            } else if ( sigismember(&set, SIGUSR2) ) {
                mutex = 0;
                signo = SIGUSR2;
            } else if ( sigismember(&set, SIGUSR3) ) {
                Current = Current->Previous;
                signo = SIGUSR3;
            }
            if ( signo == -1 )
                continue;
            if ( setjmp(fcb_list[n].Environment) != 0 )
                continue;
            sigprocmask(0, NULL, &oldset);
            sigset_t unblock_set;
            sigemptyset(&unblock_set);
            sigaddset(&unblock_set, signo);
            sigprocmask(SIG_UNBLOCK, &unblock_set, NULL); 
        }

        mutex = 0; // release lock
        longjmp(SCHEDULER, -2);
    }
}

void funct_2(int n) {
    int sj = setjmp(fcb_list[n].Environment);
    if ( sj < 0 ) {
        ERR_EXIT("setjmp");
    } else if ( sj == 0 ) {
        if ( n != 4 )
            funct_5(n + 1);
        else // n = 4
            longjmp(main_env, 1);
    } else if ( sj > 0 ) {
        for ( int i = 1; i <= P; i++) {
            // check lock
            while ( 1 ) {
                if ( mutex != 0 && mutex != n ) {
                    in_queue[n] = 1;
                    int sj = setjmp(fcb_list[n].Environment);
                    if ( sj == 0 )
                        longjmp(SCHEDULER, 1);
                } else {
                    break;
                }
            }

            in_queue[n] = 0;
            mutex = n; // acquire lock

            for ( int j = 1; j <= Q; j++) {
                sleep(1);
                //fprintf(stderr, "append %d\n", n);
                arr[idx++] = '0' + n;
            }

            // For task 2
            if ( i % K == 0 ) {
                mutex = 0; // release lock
                int res = setjmp(fcb_list[n].Environment);
                if ( res < 0 ) {
                    ERR_EXIT("setjmp");
                } else if ( res == 0 && i != P ) {
                    longjmp(SCHEDULER, 1);
                }
            }

            // For task 3
            sigset_t set;
            int signo = -1;
            int res = sigpending(&set);
            if ( sigismember(&set, SIGUSR1) ) {
                signo = SIGUSR1;
            } else if ( sigismember(&set, SIGUSR2) ) {
                mutex = 0;
                signo = SIGUSR2;
            } else if ( sigismember(&set, SIGUSR3) ) {
                Current = Current->Previous;
                signo = SIGUSR3;
            }
            if ( signo == -1 )
                continue;
            if ( setjmp(fcb_list[n].Environment) != 0 )
                continue;
            sigprocmask(0, NULL, &oldset);
            sigset_t unblock_set;
            sigemptyset(&unblock_set);
            sigaddset(&unblock_set, signo);
            sigprocmask(SIG_UNBLOCK, &unblock_set, NULL); 
        }

        mutex = 0; // release lock
        longjmp(SCHEDULER, -2);
    }
}

void funct_3(int n) {
    int sj = setjmp(fcb_list[n].Environment);
    if ( sj < 0 ) {
        ERR_EXIT("setjmp");
    } else if ( sj == 0 ) {
        if ( n != 4 )
            funct_5(n + 1);
        else // n = 4
            longjmp(main_env, 1);
    } else if ( sj > 0 ) {
        for ( int i = 1; i <= P; i++) {
            // check lock
            while ( 1 ) {
                if ( mutex != 0 && mutex != n ) {
                    in_queue[n] = 1;
                    int sj = setjmp(fcb_list[n].Environment);
                    if ( sj == 0 )
                        longjmp(SCHEDULER, 1);
                } else {
                    break;
                }
            }

            in_queue[n] = 0;
            mutex = n; // acquire lock

            for ( int j = 1; j <= Q; j++) {
                sleep(1);
                //fprintf(stderr, "append %d\n", n);
                arr[idx++] = '0' + n;
            }

            // For task 2
            if ( i % K == 0 ) {
                mutex = 0; // release lock
                int res = setjmp(fcb_list[n].Environment);
                if ( res < 0 ) {
                    ERR_EXIT("setjmp");
                } else if ( res == 0 && i != P ) {
                    longjmp(SCHEDULER, 1);
                }
            }

            // For task 3
            sigset_t set;
            int signo = -1;
            int res = sigpending(&set);
            if ( sigismember(&set, SIGUSR1) ) {
                signo = SIGUSR1;
            } else if ( sigismember(&set, SIGUSR2) ) {
                mutex = 0;
                signo = SIGUSR2;
            } else if ( sigismember(&set, SIGUSR3) ) {
                Current = Current->Previous;
                signo = SIGUSR3;
            }
            if ( signo == -1 )
                continue;
            if ( setjmp(fcb_list[n].Environment) != 0 )
                continue;
            sigprocmask(0, NULL, &oldset);
            sigset_t unblock_set;
            sigemptyset(&unblock_set);
            sigaddset(&unblock_set, signo);
            sigprocmask(SIG_UNBLOCK, &unblock_set, NULL); 
        }

        mutex = 0; // release lock
        longjmp(SCHEDULER, -2);
    }
}

void funct_4(int n) {
    int sj = setjmp(fcb_list[n].Environment);
    if ( sj < 0 ) {
        ERR_EXIT("setjmp");
    } else if ( sj == 0 ) {
        if ( n != 4 )
            funct_5(n + 1);
        else // n = 4
            longjmp(main_env, 1);
    } else if ( sj > 0 ) {
        for ( int i = 1; i <= P; i++) {
            // check lock
            while ( 1 ) {
                if ( mutex != 0 && mutex != n ) {
                    in_queue[n] = 1;
                    int sj = setjmp(fcb_list[n].Environment);
                    if ( sj == 0 )
                        longjmp(SCHEDULER, 1);
                } else {
                    break;
                }
            }

            in_queue[n] = 0;
            mutex = n; // acquire lock

            for ( int j = 1; j <= Q; j++) {
                sleep(1);
                //fprintf(stderr, "append %d\n", n);
                arr[idx++] = '0' + n;
            }

            // For task 2
            if ( i % K == 0 ) {
                mutex = 0; // release lock
                int res = setjmp(fcb_list[n].Environment);
                if ( res < 0 ) {
                    ERR_EXIT("setjmp");
                } else if ( res == 0 && i != P ) {
                    longjmp(SCHEDULER, 1);
                }
            }

            // For task 3
            sigset_t set;
            int signo = -1;
            int res = sigpending(&set);
            if ( sigismember(&set, SIGUSR1) ) {
                signo = SIGUSR1;
            } else if ( sigismember(&set, SIGUSR2) ) {
                mutex = 0;
                signo = SIGUSR2;
            } else if ( sigismember(&set, SIGUSR3) ) {
                Current = Current->Previous;
                signo = SIGUSR3;
            }
            if ( signo == -1 )
                continue;
            if ( setjmp(fcb_list[n].Environment) != 0 )
                continue;
            sigprocmask(0, NULL, &oldset);
            sigset_t unblock_set;
            sigemptyset(&unblock_set);
            sigaddset(&unblock_set, signo);
            sigprocmask(SIG_UNBLOCK, &unblock_set, NULL); 
        }

        mutex = 0; // release lock
        longjmp(SCHEDULER, -2);
    }
}

void build_linked_list() {
    Head = &fcb_list[1];
    for ( int i = 1; i <= 4; i++) {
        fcb_list[i].Name = i;
        fcb_list[i].Next = &fcb_list[i == 4 ? 1 : i + 1];
        fcb_list[i].Previous = &fcb_list[i == 1 ? 4 : i - 1];
    }
    Current = &fcb_list[4];
}

void set_action(int signo, sighandler_t handler) {
    struct sigaction act;
    act.sa_handler = handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    if ( sigaction(signo, &act, NULL) < 0 )
        ERR_EXIT("sigaction");
}

int main(int argc, char *argv[])
{
    if ( argc != 5 )
        ERR_EXIT("wrong number of arguments");

    P = atoi(argv[1]);
    Q = atoi(argv[2]);
    TASK = atoi(argv[3]);
    K = (TASK == 2) ? atoi(argv[4]) : 1000000007;

    if ( setjmp(main_env) == 0 )
        funct_5(1);

    build_linked_list();

    set_action(SIGUSR1, sigusr1_act);
    set_action(SIGUSR2, sigusr2_act);
    set_action(SIGUSR3, sigusr3_act);
    
    Scheduler();
}

