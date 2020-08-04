# String Hashing

## Concept

Define the hash of a string $S$ of length $n$ as follow:
$$
\text{hash}(S) = \sum_{i=1}^{n} S[i-1] \times p^{n - i} \text{ mod } m
$$

To compute $H(S, m) = \text{hash}(S[0...m])$ for $m = 0$ to $n-1$, we can take use of the previous computed value:
$$
H(S, m) = H(S, m-1) \times p + S[m] \text{ mod } m
$$

With $H(S, m)$, we can compute $H(S, \ell, r) = \text{hash}(S[\ell...r])$ in $\Theta(1)$: 
$$
H(S, \ell, r) = H(S, r) - H(S, \ell-1) \times p^{r-\ell+1} \text{ mod } m
$$

## Implementation

```cpp
{{ code_from_file('src/algorithms/str_hash.cpp') }}
```