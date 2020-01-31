# Dijkstra's Algorithm

## On Sparse Graph (binary heap as pq)

This implementation has time complexity of $O(E\,log\,E)$, which is suitable for sparse graph ($E \ll V^2$).

```cpp
{{ code_from_file('src/algorithms/dijkstra_sparse.cpp') }}
```

## On Dense Graph (array as pq)

This implementation has time complexity of $O(V^2)$, which is suitable for dense graph ($E \approx V^2$). The only difference between this algorithm and the previous one is the implementation of *priority queue*.

```cpp
{{ code_from_file('src/algorithms/dijkstra_dense.cpp') }}
```
