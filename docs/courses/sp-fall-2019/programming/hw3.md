# Programming Assignment #3

## Important Techniques Learned in this Homework

1. how to use <code>longjmp()</code>, <code>setjmp()</code> to *switch* between multiple stack frame without hurting the integrity of data
2. how to use <code>sigprocmask()</code>, <code>sigaction()</code> to manipulate the **signal disposition** and **signal mask**

## report

+ [link on github](https://github.com/kaienlin/CS-note/blob/master/src/courses/sp-fall-2019/programming/hw3/report.pdf)

## main.c

```c
{{ code_from_file('src/courses/sp-fall-2019/programming/hw3/main.c') }}
```

## hw3.c

```c
{{ code_from_file('src/courses/sp-fall-2019/programming/hw3/hw3.c') }}
```
