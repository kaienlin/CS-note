#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

using namespace std;
using LL = int64_t;
using ULL = uint64_t;
using LD = long double;
using pii = pair<LL, LL>;

inline int read_int(int &);

constexpr int MAXN = 2e5+5, MAXM = 5e5+5;
constexpr LL INF = (1LL << 61) + 1;

int n;

struct DSU {
    int parent[MAXN], size[MAXN];
    void init(int n) {
        for ( int i = 0; i < n; i++)
            mk_set(i);
    }
    void mk_set(int v) {
        parent[v] = v;
        size[v] = 1;
    }
    int find_set(int v) {
        if ( v == parent[v] ) return v;
        return (parent[v] = find_set(parent[v]));
    }
    void unite_set(int v, int u) {
        v = find_set(v), u = find_set(u);
        if ( v != u ) {
            if ( size[v] < size[u] )
                swap(v, u);
            parent[u] = v;
            size[v] += size[u];
        }
    }
} dsu;

struct Dijkstra {
    vector<pii> adj[MAXN];  // first: weight, second: to
    LL min_d[MAXN];

    Dijkstra() = default;

    void add_edge(int u, int v, int w) {
        adj[u].push_back( {w, v} );
    }

    LL run(int src, int dst) {
        fill(min_d, min_d+n, INF);
        min_d[src] = 0;
        priority_queue< pii, vector<pii>, greater<pii> > pq;
        pq.push({0, src});

        while ( !pq.empty() ) {
            LL v = pq.top().second, d_v = pq.top().first;
            pq.pop();
            if ( d_v != min_d[v] ) continue;
            if ( v == dst ) return d_v;
            for ( auto e : adj[v] ) {
                LL to = e.second, wei = e.first;
                if ( d_v + wei < min_d[to] ) {
                    min_d[to] = min_d[v] + wei;
                    pq.push( {min_d[to], to} );
                }
            }
        }
    }
} G;

struct Edge {
    int x, y, d, l;
    bool operator < (const Edge &rhs) const { return l < rhs.l; }
};

Edge edges[MAXM];

int main()
{
    IOS;
    int m;
    read_int(n); read_int(m);
    int s, t; read_int(s); read_int(t);

    if ( s == t ) {
        cout << "0 0\n";
        return 0;
    }

    for ( int i = 0; i < m; i++) {
        read_int(edges[i].x); read_int(edges[i].y);
        read_int(edges[i].d); read_int(edges[i].l);
    }
    sort(edges, edges + m);

    if ( edges[0].l == edges[m-1].l ) {
        for ( auto e : edges ) {
            G.add_edge(e.x, e.y, e.d);
            G.add_edge(e.y, e.x, e.d);
        }
        cout << G.run(s, t) << ' ' << edges[0].l << '\n';
        return 0;
    }

    int L = 0;
    dsu.init(n);
    for ( Edge *it = edges; it != edges + m; ++it) {
        dsu.unite_set(it->x, it->y);
        G.add_edge(it->x, it->y, it->d);
        G.add_edge(it->y, it->x, it->d);
        if ( dsu.find_set(s) == dsu.find_set(t) ) {
            L = it->l;
            for ( ++it; it != edges + m && it->l == L; ++it) {
                G.add_edge(it->x, it->y, it->d);
                G.add_edge(it->y, it->x, it->d);
            }
            break;
        }
    }

    cout << G.run(s, t) << ' ' << L << '\n';

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

