#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

using namespace std;
using LL = int64_t;
using ULL = uint64_t;
using LD = long double;
using pii = pair<int, int>;

constexpr int MAXN = (1 << 18) + 5;

inline int read_int(int &x);
inline void write_int(LL x);

int N;
int G[MAXN], ref_cnt[MAXN], cnt[MAXN], label[MAXN], len[MAXN];
bool done[MAXN];

void dfs(int o) {
    int v = G[o], d = 1;
    while ( v && ref_cnt[v] == 1 )
        v = G[v], ++d;
    G[o] = v;
    len[o] = d;
    done[o] = true;
    if ( v && !done[v] ) dfs(v);
}

void compress() {
    for ( int i = 1; i <= N; i++)
        if ( ref_cnt[i] == 0 )
            dfs(i);
}

int main()
{
    read_int(N);
    for ( int i = 1; i <= N; i++) {
        read_int(G[i]);
        ref_cnt[G[i]]++;
    }

    compress();

    int Q; read_int(Q);
    for ( int q = 1; q <= Q; q++) {
        int ans = 0;
        int nx; read_int(nx);
        for ( int _ = 0; _ < nx; ++_) {
            int x; read_int(x);
            while ( cnt[x] == 0 ) {
                ans += len[x];
                x = G[x];
                if ( !x ) break;
                if ( label[x] != q ) {
                    label[x] = q;
                    cnt[x] = ref_cnt[x];
                }
                cnt[x]--;
            }
        }
        write_int(ans);
    }

    return 0;
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

