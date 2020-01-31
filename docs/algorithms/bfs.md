# Breadth First Search

## Basic BFS

This also finds the shortest path from the source vertex in an unweighted graph.

```cpp
{{ code_from_file('src/algorithms/bfs_basic.cpp') }}
```

## 0-1 BFS

Generally, BFS can find the shortest path only when the graph is unweighted, but if the weights have a constraint that their values are either 0 or 1, then the shortest path can be found by a modified version of BFS, which is called "0-1 BFS".

```cpp
{{ code_from_file('src/algorithms/bfs_01.cpp') }}
```