# Assignment 1

## 1. File Redirection

1. Please give the meaning of the command:
```shell
./a.out < infile 2>&1 > outfile
```

    !!! success "my answer"
        + Redirect a.out's standard input to read from the file named infile.
        + Redirect a.out's standard input to read from the file named infile.
        + Redirect a.out's standard output to write to the file named outfile.

2. Please use <code>dup()</code> or <code>dup2()</code> to do the redirections of the command
```shell
./a.out < infile 2>&1 > outfile
```
in the following program fragment. Error checking could be ignored.
```c hl_lines="8 9 10 11 12 13"
int main(int argc, char *argv[])
{
    int fd1, fd2;

    fd1 = open(infile, O_RDONLY);
    fd2 = open(outfile, O_WRONLY | O_CREAT, 0666);

    // redirect standard input to infile
    dup2(fd1, 0);
    // redirect standard error to standard output
    dup2(1, 2);
    // redirect standard output to outfile
    dup2(fd2, 1);

    execlp("./a.out", "./a.out", (char*)0);
    return 0;
}
```

## 2. Atomic operation

To randomly write a file, a student develops two functions *write_to_fd()* and *write_to_fn()* by which users can write data to a given position in the file. The only difference between the two functions is the first parameter. One accepts a file descriptor while the other accepts a file name. Here are the two functions.

```c
ssize_t write_to_fd(int fd, void *buf, size_t nbytes, off_t offset)
{
    if ( lseek(fd, offset, SEEK_SET) < 0 ) return -1;
    return ( write(fd, buf, nbytes) );
}

ssize_t write_to_fn(char* filename, void *buf, size_t nbytes, off_t offset)
{
    int fd;
    ssize_t retval;
    if ( (fd = open(filename, O_WRONLY)) < 0 ) return -1;
    if ( lseek(fd, offset, SEEK_SET) < 0 ) return -1;
    retval = write(fd, buf, nbytes);
    close(fd);
    return retval;
}
```

(a) Should the <code>write_to_fd()</code> function be an atomic operation? Please give an example to clearly explain your answer. 

!!! success "my answer"
    **Yes, function <code>write_to_fd</code> should be an atomic operation.** Since the function accepts a file descriptor, the file is opened before the function is called. If the process fork a child process after opening the file and before calling this function, then there exists another process that inherits the descriptor table, whose </code>fd</code> is pointed to the same file table which containing the current file offset. Between the time the parent process calls <code>lseek(fd, offset, SEEK_SET)</code> and <code>write(fd, buf, nbytes)</code>, if the CPU context switch to that child process and it calls <code>lseek(fd, 0, SEEK_SET)</code> or <code>read(fd, &x, 1)</code>, i.e. anything that changes the current file offset, then after the CPU context switch back to parent process, the current file offset is not at the specified <code>offset</code> and thus it write to wrong position when calling <code>write(fd, buf, nbytes)</code>. Thus, we should make </code>write_to_fd</code> be an atomic operation to prevent it from misbehaving. 

(b) Should the <code>write_to_fn()</code> function be an atomic operation? Please give an example to clearly explain your answer. 

!!! success "my answer"
    **No, function <code>write_to_fn</code> is not necessarily be an atomic operation.** Every time this function is called, it use <code>open</code> function to create a brand new open file table entry and file table, which have its own current file offset. Though there might be another process write to the same file between <code>lseek</code> and <code>write</code>, the function will definitely write to the requested offset, no matter what contents are overwritten.