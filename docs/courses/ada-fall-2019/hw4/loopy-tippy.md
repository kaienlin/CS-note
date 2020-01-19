# Loopy Tippy

## Problem Description

[Yun-Nung (Vivian) Chen's website](https://www.csie.ntu.edu.tw/~yvchen/f108-ada/doc/ada19-hw4.pdf)

## TA's Hint

+ The constraints can be modeled as three parts:
    1. The number of edges
    2. Simple loops
    3. Connectivity
        - $n^4$ variables: $x_{i,j}^t$ denotes whether $(i, j)$ is reachable at time $t$.

## Online Resources

+ [Slitherlink Reloaded](https://david-westreicher.github.io/static/papers/ba-thesis.pdf)

## Code
```cpp
{{ code_from_file('src/courses/ada-fall-2019/hw4/pA.cpp') }}
```

