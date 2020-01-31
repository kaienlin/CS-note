int my_binary_search(int key)
{
    int L = LOWER_BOUND, R = UPPER_BOUND;
    while ( L <= R ) {
        int M = L + ((R - L) >> 1);
        if ( arr[M] == key )
            return M;
        if ( arr[M] < key )
            L = M + 1;
        else
            R = M - 1;
    }
    return -1;  // key is not present
}