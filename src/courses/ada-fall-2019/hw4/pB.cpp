#include <bits/stdc++.h>
#define IOS { ios::sync_with_stdio(false); cin.tie(nullptr); }

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

using namespace std;
using LL = long long;
using ULL = unsigned long long;
using pii = pair<int, int>;

inline int read_int(int &);
inline void write_int(LL);

constexpr int MAXN = 2e5+5;
constexpr int MOD = 1e9+7;
constexpr int K = 300;
constexpr int INF = 1e9+7;
constexpr int SEED = 8787;

int N, M, Q;

vector<int> G[MAXN];
bool vis[MAXN];

int H[MAXN];
int hk[MAXN][K + 5], tmp[2 * K + 5];

int unite(int *a, int *b, int *dst) {
    int *a_end = a; while ( *a_end != INF ) ++a_end;
    int *b_end = b; while ( *b_end != INF ) ++b_end;
    ++a_end; // preserve one INF
    int sz = set_union(a, a_end, b, b_end, tmp) - tmp;
    memcpy(dst, tmp, sizeof(int) * K);
    dst[K] = INF;
    return min(sz-1, K);
}

int intersect(int *a, int *b, int *dst) {
    int *a_end = a; while ( *a_end != INF ) ++a_end;
    int *b_end = b; while ( *b_end != INF ) ++b_end;
    ++a_end; ++b_end;
    int sz = set_intersection(a, a_end, b, b_end, tmp) - tmp;
    memcpy(dst, tmp, sizeof(int) * K);
    return sz-1;
}

void dfs(int v) {
    vis[v] = true;
    hk[v][0] = H[v], hk[v][1] = INF;
    for ( int u : G[v] ) {
        if ( !vis[u] )
            dfs(u);
        unite(hk[v], hk[u], hk[v]);
    }
}

int main()
{
    srand(SEED);
    read_int(N); read_int(M); read_int(Q);
    for ( int i = 0; i < M; i++) {
        int from, to; read_int(from); read_int(to);
        G[from].push_back(to);
    }

    for ( int i = 1; i <= N; i++) H[i] = i;
    random_shuffle(H+1, H+N+1);

    for ( int i = 1; i <= N; i++)
        if ( !vis[i] )
            dfs(i);

    for ( int q = 0; q < Q; q++) {
        int x, y; read_int(x); read_int(y);
        int Y[K + 5];
        int S = unite(hk[x], hk[y], Y);

        intersect(hk[x], Y, Y);
        int sz = intersect(hk[y], hk[x], Y);
        
        double ans = (double)sz / S;
        printf("%lf\n", ans);

    }

    return EXIT_SUCCESS;
}

// input optimize
inline int read_char() {
    constexpr int N = 1<<20;
    static char buf[N];
    static char *p = buf, *end = buf;
    if ( p == end ) {
        if ( (end = buf + fread_unlocked(buf, 1, N, stdin)) == buf ) return EOF;
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
// output optimize
#define _pc putchar_unlocked
char _buf[128];
inline void write_int(LL x) {
    if ( !x ) { _pc('0'); _pc('\n'); return; }
    int t = 0;
    while ( x ) {
        _buf[t++] = x % 10 + '0';
        x /= 10;
    }
    while ( t-- ) _pc(_buf[t]);
    _pc('\n');
}
#undef _pc

