# Maximum Subarray Revisited

## Problem Description

[Yun-Nung (Vivian) Chen's website](https://www.csie.ntu.edu.tw/~yvchen/f108-ada/doc/ada19-hw1.pdf)

## Concept

+ Store the process of *divide and conquer* (just as *segment tree*)
+ In each node representing $[L, R]$, store 4 things: <br><br>
    1. lmx $= \max\limits_{L \leq r \leq R} \sum_{i=L}^{r} A_i$  <br><br>
    2. rmx $= \max\limits_{L \leq \ell \leq R} \sum_{i = \ell}^{R} A_i$ <br><br>
    3. ans $= \max\limits_{L \leq \ell \leq r \leq R} \sum_{i = \ell}^{r} A_i$ <br><br>
    4. sum $= \sum_{i=L}^{R} A_i$ <br><br>
+ The "merge" step is as the code.

## Code

```cpp
{{ code_from_file('src/courses/ada-fall-2019/hw1/p2.cpp') }}
```