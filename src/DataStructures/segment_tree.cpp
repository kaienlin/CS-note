constexpr int MAXN = 1e5+5;

int N, A[MAXN];
int seg[4 * MAXN], lazy[4 * MAXN];

inline int lch(int v) { return 2 * v; }
inline int rch(int v) { return 2 * v + 1; }

inline int combine(int a, int b) {
    return a + b;
}

inline void push(int v, int l, int r) {
    int m = (l + r) >> 1;
    seg[lch(v)] += (m - l + 1) * lazy[v];
    seg[rch(v)] += (r - m) * lazy[v];

    lazy[lch(v)] += lazy[v];
    lazy[rch(v)] += lazy[v];
    lazy[v] = 0;
}

void build(int v, int tl, int tr) {
    if ( tl == tr ) {
        seg[v] = A[tl];
    } else {
        int tm = (tl + tr) >> 1;
        build(lch(v), tl, tm);
        build(rch(v), tm+1, tr);
        seg[v] = combine(seg[lch(v)], seg[rch(v)]); 
    }
}

int range_q(int v, int tl, int tr, int l, int r) {
    if ( l > r )
        return 0;
    if ( l == tl && r == tr )
        return seg[v];
    push(v, tl, tr);
    int tm = (tl + tr) >> 1;
    return combine(range_q(lch(v), tl, tm, l, min(r, tm)), 
                   range_q(rch(v), tm+1, tr, max(l, tm+1), r));
}

void point_upd(int v, int tl, int tr, int pos, int new_val) {
    if ( tl == tr ) {
        seg[v] = new_val;
    } else {
        push(v, tl, tr);
        int tm = (tl + tr) >> 1;
        if ( pos <= tm )
            point_upd(lch(v), tl, tm, pos, new_val);
        else
            point_upd(rch(v), tm+1, tr, pos, new_val);
        seg[v] = combine(seg[lch(v)], seg[rch(v)]);
    }
}

void range_upd(int v, int tl, int tr, int l, int r, int add) {
    if ( l > r )
        return;
    if ( tl == l && tr == r ) {
        seg[v] += add * (r - l + 1);
        lazy[v] += add;
    } else {
        push(v, tl, tr);
        int tm = (tl + tr) >> 1;
        range_upd(lch(v), tl, tm, l, min(r, tm), add);
        range_upd(rch(v), tm+1, tr, max(l, tm+1), r, add);
        seg[v] = combine(seg[lch(v)], seg[rch(v)]);
    }
}

