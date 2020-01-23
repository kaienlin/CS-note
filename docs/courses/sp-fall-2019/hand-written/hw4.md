# Assignment 7

## Signals
Please answer the questions according the following program and assumptions:

1. A blocked signal will be delivered immediately when unblocked.
2. If several signals are ready to be delivered at the same time, they will be delivered in the same order of generation.
3. If a blocked signal is generated several times, it will be delivered only once when unblocked.

```c
void catchit(int signo)
{
    // Line C
}
int main()
{
    pid_t pid;
    sigset_t newmask, waitmask, oldmask;

    if ( (pid = fork()) == 0 ) {
        struct sigaction action;

        sigemptyset( &newmask );  sigemptyset( &waitmask );
        sigaddset( &newmask, SIGUSR1 );
        sigaddset( &waitmask, SIGUSR2 );
        sigprocmask( SIG_BLOCK, &newmask, &oldmask );

        action.sa_flags = 0;
        sigemptyset( &action.sa_mask );
        action.sa_handler = catchit;
        sigaction( SIGUSR2, &action, NULL );
        sigaction( SIGINT, &action, NULL );
        sigaddset( &action.sa_mask, SIGINT );
        sigaction( SIGUSR1, &action, NULL );

        // Line A: critical section

        sigsuspend( &waitmask );    // Line B
    } else {
        int stat;
        kill( pid, SIGUSR1 ); kill( pid, SIGUSR2 ); kill( pid, SIGINT );
        pid = wait( &stat );
    }
    _exit(0);
}
```

+ (1) Suppose the child process receives SIGUSR2 at line A, SIGINT when blocked at line B, and SIGUSR1 when SIGINT is caught at line C. How does the child process' signal mask change

$$
\begin{array}[rl]x
\xrightarrow{\text{the program start}} &\ \text{empty} \\[2ex]
\xrightarrow{\text{block SIGUSR1 by sigprocmask}} &\ \texttt{SIGUSR1} \\[2ex]
\xrightarrow{\text{SIGUSR2 delivered at line A}} &\ \texttt{SIGUSR1 + SIGUSR2} \\[2ex]
\xrightarrow{\text{return to main}} &\ \texttt{SIGUSR1} \\[2ex]
\xrightarrow{\text{sigsuspend (change to waitmask)}} &\ \texttt{SIGUSR2} \\[2ex]
\xrightarrow{\text{SIGINT delivered in sigsuspend}} &\ \texttt{SIGUSR2 + SIGINT} \\[2ex]
\xrightarrow{\text{SIGUSR1 delivered in the handler of SIGINT}} &\ \texttt{SIGUSR1 + SIGUSR2 + SIGINT} \\[2ex]
\xrightarrow{\text{return to the handler of SIGINT}} &\ \texttt{SIGUSR2 + SIGINT} \\[2ex]
\xrightarrow{\text{return to main}} &\ \texttt{SIGUSR1}\\[2ex]
\end{array} \\
$$

+ (2) Suppose the child process receives SIGUSR1 at line A, SIGUSR2 when blocked at line B, and SIGINT when SIGUSR1 is caught at line C. How does the child process' signal mask change?

$$
\begin{array}[rl]x
\xrightarrow{\text{the program start}} &\ \text{empty} \\[2ex]
\xrightarrow{\text{block SIGUSR1 by sigprocmask}} &\ \texttt{SIGUSR1} \\[2ex]
\text{(}\texttt{SIGUSR1} \text{ generated} \text{ but blocked)} & \\[2ex]
\xrightarrow{\text{sigsuspend (change to waitmask)}} &\ \texttt{SIGUSR2} \\[2ex]
\text{(}\texttt{SIGUSR2}\text{ generated} \text{ but blocked)} & \\[2ex]
\xrightarrow{\text{SIGUSR1 delivered in sigsuspend}} &\ \texttt{SIGUSR1 + SIGUSR2 + SIGINT} \\[2ex]
\text{(}\texttt{SIGINT}\text{ generated} \text{ but blocked)} & \\[2ex]
\xrightarrow{\text{return to main}} &\ \texttt{SIGUSR1} \\[2ex]
\xrightarrow{\text{SIGUSR2 delivered}} &\ \texttt{SIGUSR1 + SIGUSR2} \\[2ex]
\xrightarrow{\text{SIGINT delivered in the handler of SIGUSR2}} &\ \texttt{SIGUSR1 + SIGUSR2 + SIGINT} \\[2ex]
\xrightarrow{\text{return to the handler of SIGUSR2}} &\ \texttt{SIGUSR1 + SIGUSR2} \\[2ex]
\xrightarrow{\text{return to main}} &\ \texttt{SIGUSR1}\\[2ex]
\end{array}
$$