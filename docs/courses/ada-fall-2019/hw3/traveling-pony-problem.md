# Traveling Pony Problem

## Problem Description

[Yun-Nung (Vivian) Chen's website](https://www.csie.ntu.edu.tw/~yvchen/f108-ada/doc/ada19-hw3.pdf)

## Concept

+ First Stage: Find the minimum possible protection <code>min_l</code>
    1. By altered *Dijkstra*
    2. By *Disjoint Set Union* (the usage is like *Kruskal*)
+ Second Stage: Run normal *Dijkstra* On the graph which all edges' weight $\leq$ <code>min_l</code>

## Code
```cpp
{{ code_from_file('src/courses/ada-fall-2019/hw3/pB.cpp') }}
```
