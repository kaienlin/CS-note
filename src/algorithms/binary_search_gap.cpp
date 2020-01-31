bool is_valid(int val)
{
    /* return true if "val" is valid */
}

int my_binary_search()
{
    int L = LOWER_BOUND, R = UPPER_BOUND;
    while ( L + 1 < R ) {
        int M = L + ((R - L) >> 1);
        if ( is_valid(M) )
            L = M;
        else
            R = M;
    }
    return L;
}

