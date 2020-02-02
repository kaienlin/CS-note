# Fenwick Tree (Binary Indexed Tree)

## Implementation

!!! note 
    Function <code>int lowbit(int x)</code> truncates all 1 bits in <code>x</code> except the rightmost one.

!!! success "Note"
    The time complexity of function <code>init()</code> is $O(n)$

```cpp
{{ code_from_file('src/algorithms/fenwick_tree.cpp') }}
```