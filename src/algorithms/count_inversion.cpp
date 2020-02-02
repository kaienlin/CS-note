int count_inv(int L, int R)
{
    if ( L == R )
        return 0;
    int M = (L + R) >> 1;
    int ans = count_inv(L, M) + count_inv(M+1, R);
    int p = M + 1;
    for ( int i = L; i <= M; i++) {
        while ( p <= R && A[i] > A[p] )
            ++p;
        ans += (p - M - 1);
    }
    merge(A+L, A+M+1, A+M+1, A+R+1, tmp);   // In C++ STL
    copy(tmp, tmp + (R - L + 1), A+L);
    return ans;
}
