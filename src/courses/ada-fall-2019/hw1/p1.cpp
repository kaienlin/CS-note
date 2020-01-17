#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC optimize("no-stack-protector")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")
#pragma GCC diagnostic ignored "-W"

using namespace std;
using LL = int64_t;
using ULL = uint64_t;
using LD = long double;

// input optimize
inline int read_char() {
    constexpr int N = 1<<20;
    static char buf[N];
    static char *p = buf, *end = buf;
    if ( p == end ) {
        if ( (end = buf + fread(buf, 1, N, stdin)) == buf ) return EOF;
        p = buf;
    }
    return *p++;
}
inline int read_int(int &x) {
    static char c, neg;
    while ( (c = read_char()) < '-' )
        if ( c == EOF ) return 0;
    neg = (c == '-') ? -1 : 1;
    x = (neg == 1) ? c - '0' : 0;
    while ( (c = read_char()) >= '0' )
        x = (x<<3) + (x<<1) + (c-'0');
    x *= neg;
    return 1;
}

const int MAXN = 1e6+5;

int N;
int P[MAXN], F[MAXN], *ptr[MAXN], mp[MAXN];

bool cmp(int *a, int *b) {
    return *a < *b;
}

int bit[MAXN];
inline int lowbit(int x) { return x & -x; }
int sum(int idx) {
    int ret = 0;
    for ( ; idx > 0; idx -= lowbit(idx) )
        ret += bit[idx];
    return ret;
}
inline int sum(int L, int R) {
    if ( L < 1 ) L = 1;
    return sum(R) - sum(L-1);
}
void add(int idx, int delta, int n) {
    for ( ; idx <= n; idx += lowbit(idx) )
        bit[idx] += delta;
}

int main()
{
    read_int(N);
    for ( int i = 1; i <= N; i++)
        read_int(P[i]);
    for ( int i = 1; i <= N; i++)
        read_int(F[i]);
    for ( int i = 1; i <= N; i++)
        ptr[i] = &P[i];
    sort(ptr+1, ptr+N+1, cmp);
    int n = 0;
    for ( int i = 1; i <= N; ) {
        int v = *ptr[i];
        ++n;
        for ( ; i <= N && *ptr[i] == v; i++)
            mp[ ptr[i] - P ] = n;
    }
    long long ans = 0;
    add(mp[1], 1, n);
    for ( int i = 2; i <= N; i++) {
        int lb = P[i] - F[i], ub = P[i] + F[i];
        auto ll = lower_bound(ptr+1, ptr+1+N, &lb, cmp);
        auto rr = upper_bound(ptr+1, ptr+1+N, &ub, cmp) - 1;
        lb = mp[*ll - P];
        ub = mp[*rr - P];
        ans += sum(lb, ub);
        add(mp[i], 1, n);
    }
    printf("%lld\n", ans);
    return 0;
}