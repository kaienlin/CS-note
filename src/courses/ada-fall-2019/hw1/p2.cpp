#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}

using namespace std;
using LL = int64_t;
using ULL = uint64_t;
using LD = long double;

inline int read_char();
inline int read_int(int &);
inline void write_int(LL);

constexpr int MAXN = 5e5+5;

struct Item {
    LL lmx, rmx, sum, ans;
};

int N, A[MAXN];
Item seg[4 * MAXN];

inline int lch(int v) { return (v << 1); }
inline int rch(int v) { return (v << 1) | 1; }

inline void combine(Item &v, Item &a, Item &b) {
    v.sum = a.sum + b.sum;
    v.lmx = max(a.lmx, a.sum + b.lmx);
    v.rmx = max(b.rmx, b.sum + a.rmx);
    v.ans = max(a.rmx + b.lmx, max(a.ans, b.ans));
}
void build(int v, int tl, int tr) {
    if ( tl == tr ) {
        seg[v] = {A[tl], A[tl], A[tl], A[tl]};
    } else {
        int tm = (tl + tr) >> 1;
        build(lch(v), tl, tm);
        build(rch(v), tm+1, tr);
        combine(seg[v], seg[lch(v)], seg[rch(v)]);
    }
}
void pt_upd(int v, int tl, int tr, int pos, int new_v) {
    if ( tl == tr ) {
        seg[v] = {new_v, new_v, new_v, new_v};
    } else {
        int tm = (tl + tr) >> 1;
        if ( pos <= tm ) pt_upd(lch(v), tl, tm, pos, new_v);
        else pt_upd(rch(v), tm+1, tr, pos, new_v);
        combine(seg[v], seg[lch(v)], seg[rch(v)]);
    }
}
inline LL qry() {
    return seg[1].ans < 0 ? 0 : seg[1].ans;
}

int main()
{
    int Q;
    read_int(N); read_int(Q);
    for ( int i = 1; i <= N; i++)
        read_int(A[i]);
    build(1, 1, N);
    write_int(qry());
    while ( Q-- ) {
        int pos, v; read_int(pos); read_int(v);
        pt_upd(1, 1, N, pos, v);
        write_int(qry());
    }
    return 0;
}

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

