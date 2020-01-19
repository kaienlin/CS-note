# Magic Song

## Problem Description

[Yun-Nung (Vivian) Chen's website](https://www.csie.ntu.edu.tw/~yvchen/f108-ada/doc/ada19-hw3.pdf)

## Concept

+ methods to convert the input to a graph:
    1. hash table (<code>__gnu_pbds::gp_hash_table</code>)
    2. 離散化 + <code>std::lower_bound</code>
+ longest path on DAG
+ dynamic programming

## Code
```cpp
{{ code_from_file('src/courses/ada-fall-2019/hw3/pC.cpp') }}
```
