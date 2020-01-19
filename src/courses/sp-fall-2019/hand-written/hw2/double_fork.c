pid_t double_fork()
{
    pid_t pid_child, pid_grandchild;
    int pfd[2];
    pipe(pfd);
    pid_child = vfork();
    if ( pid_child == 0 ) {
        pid_grandchild = fork();
        if ( pid_grandchild == 0 ) {
            // grandchild
            // close unused fd
            close(pfd[1]);
            // read message from parent to ensure child (grandchild's parent) is dead
            char msg[128];
            read(pfd[0], msg, 128);
            close(pfd[0]);
            /* do stuff */
            return 0;
        } else {
            // child
            _exit(0);
        }
    } else {
        // parent
        // close unused fd
        close(pfd[0]);
        // wait child process
        waitpid(pid_child, NULL, 0);
        // tell grandchild that child is dead
        char *msg = "The child is dead!";
        write(pfd[1], msg, strlen(msg));
        close(pfd[1]);
        // because vfork() make the child process share
        // the same memory space, the parent's variable
        // also has the pid of the grandchild
        return pid_grandchild;
    }
}
