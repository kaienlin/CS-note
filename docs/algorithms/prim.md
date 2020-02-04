# Prim's Algorithm

## Implementation (with binary heap)

+ time complexity: $O(E\,log\,E) = O(V^2\,log\,V)$ (which is more suitable for sparse graph)
+ As for $O(V^2)$ implementation, just use array(random-access) and linear search to implement the min-heap 

```cpp
{{ code_from_file('src/algorithms/prim.cpp') }}
```