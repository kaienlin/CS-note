# System Programming

+ Instructor: Pu-Jen Cheng 鄭卜壬

## Programming Assignments

1. 
    - topics:
        1. I/O multiplexing
        2. advisory file lock
    - description: Given a simple server, you have to modify it to a banking system supporting **I/O Multiplexing** and **content protection**
    - [my implementation](./programming/hw1.md)

2. 
    - topics:
        1. multi-processing and handling zombie processes
        2. Inter process communication by **pipe** and **FIFO**
    - description: Implement a bidding system which will handle a sequence of competitions. The goal of this assignment is to practice how to communicate between processes through **pipe** and **FIFO**, and to understand how to use <code>fork()</code> to create processes.
    - [my implementation](./programming/hw2.md)

3. 
    - topics:
        1. signal handling
        2. non-local jump (<code>setjmp()</code>, <code>longjmp()</code>)
    - description: Simulate a user-thread library by using <code>longjmp()</code>, <code>setjmp()</code>, and implement a **scheduler** to control the action of these threads. Additionally, **signals** are used to send instructions to the scheduler, and **signal mask** is applied to handle the timing of the **delivery of signals**.
    - [my implementation](./programming/hw3.md)

4. 
    - topics:
        1. multi-threading
        2. pthread library
    - description: implement a multiclass classifier with thread. You should train a model to classify handwritten digits from MNIST dataset. And the most important part is that you have to accelerate the matrix multiplication (let it parallelly) by thread.
    - [my implementation](./programming/hw4.md)



## Hand-written Assignments

1. [Assignment 1](./hand-written/hw1.md)

2. [Assignment 4](./hand-written/hw2.md)

3. [Assignment 5](./hand-written/hw3.md)

4. [Assignment 7](./hand-written/hw4.md)
