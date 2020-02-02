struct BIT 
{
    int N, bit[MAXN];

    int lowbit(int x)
    {
        return x & -x;
    }

    int sum(int idx)
    {
        int ret = 0;
        for ( ; idx > 0; idx -= lowbit(idx) )
            ret += bit[idx];
        return ret;
    }

    int sum(int L, int R)
    {
        return sum(R) - sum(L-1);
    }

    void add(int idx, int delta)
    {
        for ( ; idx <= N; idx += lowbit(idx) )
            bit[idx] += delta;
    }

    void init(int n, int A[])
    {
        N = n;
        for ( int x = 1; x <= N; x++ ) {
            bit[x] = A[x];
            int y = x - lowbit(x);
            for ( int i = x-1; i > y; i -= lowbit(i) )
                bit[x] += bit[i];
        }
    }
}