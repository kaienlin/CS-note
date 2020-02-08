constexpr int LEN = 16;

void counting_sort(int arr[], int p=0)
{
    static int cnt[1 << LEN];
    static int tmp[MAXN];
    int shift = p * LEN;
    int mask = (1 << LEN) - 1;
    
    auto get_idx = [=](int x) { return (x >> shift) & mask; };

    fill(cnt, cnt+N, 0);
    for ( int i = 0; i < N; i++ )
        ++cnt[get_idx(arr[i])];
    for ( int i = 1; i < (1 << LEN); i++ )
        cnt[i] += cnt[i-1];
    for ( int i = N-1; i >= 0; i-- )
        tmp[ --cnt[get_idx(arr[i])] ] = arr[i];
    
    copy(tmp, tmp+N, arr);
}

void radix_sort(int arr[])
{
    int int_len = sizeof(int) * 8;
    int seg_num = int_len / LEN;
    for ( int p = 0; p < seg_num; p++ )
        counting_sort(arr, p);
}
