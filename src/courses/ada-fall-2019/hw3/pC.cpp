#include <bits/stdc++.h>
#include <bits/extc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}
#define unordered_map __gnu_pbds::gp_hash_table

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

using namespace std;
using LL = int64_t;
using ULL = uint64_t;
using LD = long double;
using pii = pair<int, int>;

inline int read_int(int &x);

constexpr int MAXN = 1e5+5, MAXM = 5e5+5;
constexpr int MAXV = 1e6+2e5+100;
constexpr LL INF = (1LL<<61) + 1;

int N, M;
int A[MAXN];
vector<int> V[MAXN];
vector<pii> G[MAXV];

void add_edge(int from, int to, int w) {
    G[from].push_back( {to, w} );
}

int nd = 0;
unordered_map<ULL, int> mp;

ULL get_hash(int a, int b) {
    return ((ULL)a << 31) | b;
}

int add_node(int s, int t) {
    auto res = mp.insert( make_pair( get_hash(s, t), nd) );
    if ( res.second ) {
        ++nd;
        V[s].push_back(t);
    }
    return res.first->second;
}

int get_node(int s, int t) {
    return mp[ get_hash(s, t) ];
}

LL dist[MAXV];
bool vis[MAXV], done[MAXV];
LL dfs(int v) {
    vis[v] = true;
    for ( pii p : G[v] ) {
        if ( !vis[p.first] )
            dfs(p.first);
        else if ( !done[p.first] )
            return (dist[v] = INF);
        dist[v] = max(dist[v], dist[p.first] + p.second);
        if ( dist[v] >= INF ) return INF;
    }
    done[v] = true;
    return dist[v];
}

int main()
{
    IOS;
    int T; read_int(T);
    while ( T-- ) {
        read_int(N); read_int(M);
        for ( int i = 1; i <= N; i++) {
            read_int(A[i]);
            add_node(i, A[i]);
        }

        for ( int i = 0; i < M; i++) {
            int s1, t1, s2, t2;
            read_int(s1); read_int(t1); read_int(s2); read_int(t2);
            int from = add_node(s1, t1);
            int to = add_node(s2, t2);
            add_edge(from, to, 1);
        }

        int S = nd;

        for ( int i = 1; i <= N; i++) {
            sort(V[i].begin(), V[i].end());
            int prev_id = S, prev_pos = 0;
            for ( int j = 0; j < V[i].size(); j++) {
                int now_id = get_node(i, V[i][j]), now_pos = V[i][j];
                add_edge(prev_id, now_id, now_pos - prev_pos);
                prev_id = now_id, prev_pos = now_pos;
            }
            V[i].clear();
        }

        LL ans = dfs(S);
        if ( ans >= INF ) cout << "LoveLive!\n";
        else cout << ans << '\n';

        memset(vis, 0, sizeof(vis));
        memset(done, 0, sizeof(done));
        memset(dist, 0, sizeof(dist));
        for ( int i = 0; i <= nd; i++)
            G[i].clear();
        mp.clear();
        nd = 0;
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

