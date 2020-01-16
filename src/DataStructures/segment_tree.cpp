struct SegTree {
    constexpr static int MAXN = 1e5+5;

    int N;
    int A[MAXN];
    int T[4 * MAXN], tag[4 * MAXN];

    void init(int _N) { N = _N; }

    int lch(int v) { return v << 1; }
    int rch(int v) { return (v << 1) | 1; }

    int combine(int a, int b) {
        return a + b;
    }

    void pull(int v) {
        T[v] = combine(T[lch(v)], T[rch(v)]);
    }

    void push(int v, int l, int r) {
        if ( tag[v] ) {
            int m = (l + r) >> 1, lc = lch(v), rc = rch(v);
            T[lc] += (m - l + 1) * tag[v];
            T[rc] += (r - m) * tag[v];
            tag[lc] += tag[v];
            tag[rc] += tag[v];
            tag[v] = 0;
        }
    }

    void build(int v, int tl, int tr) {
        if ( tl == tr ) {
            T[v] = A[tl];
        } else {
            int tm = (tl + tr) >> 1;
            build(lch(v), tl, tm);
            build(rch(v), tm+1, tr);
            pull(v);
        }
    }

    int qry(int v, int tl, int tr, int l, int r) {
        if ( l > tr || r < tl )
            return 0;
        if ( l <= tl && r >= tr )
            return T[v];
        int tm = (tl + tr) >> 1;
        push(v, tl, tr);
        return combine(qry(lch(v), tl, tm, l, r), qry(rch(v), tm+1, tr, l, r));
    }

    void upd(int v, int tl, int tr, int l, int r, int add) {
        if ( l > tr || r < tl )
            return;
        if ( l <= tl && r >= tr ) {
            tag[v] += add;
            T[v] += add * (r - l + 1);
            return;
        }
        int tm = (tl + tr) >> 1;
        push(v, tl, tr);
        upd(lch(v), tl, tm, l, r, add);
        upd(rch(v), tm+1, tr, l, r, add);
        pull(v);
    }
} seg;

