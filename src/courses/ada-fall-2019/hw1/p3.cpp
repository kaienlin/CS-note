#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

using namespace std;
using LL = long long;

inline int read_char();
inline int read_int(int &);

const int MAXN = 5e5+5, NN = 1e6+5;
const int T = 40;

int N, A[MAXN], Mx[MAXN], Mn[MAXN];
int cnt[NN], cnt2[NN];
LL Ans;

void solve(int L, int R) {
    if ( L == R )
        return void(++Ans);
    if ( R - L <= T ) {
        for ( int l = L; l <= R; l++) {
            int mx = A[l], mn = A[l];
            for ( int r = l; r <= R; r++) {
                mx = max(mx, A[r]); mn = min(mn, A[r]);
                Ans += (mx - mn == r - l);
            }
        }
        return;
    }
    // D & C
    int m = (L + R) >> 1;
    solve(L, m);
    solve(m+1, R);

    // preprocess
    Mx[m] = Mn[m] = A[m];
    for ( int i = m-1; i >= L; i--)
        Mx[i] = max(Mx[i+1], A[i]), Mn[i] = min(Mn[i+1], A[i]);

    Mx[m+1] = Mn[m+1] = A[m+1];
    cnt[ Mn[m+1] + m + 1 ] = cnt2[ Mx[m+1] - m - 1 + N ] = 0;
    for ( int i = m+2; i <= R; i++) {
        Mx[i] = max(Mx[i-1], A[i]), Mn[i] = min(Mn[i-1], A[i]);
        cnt[ Mn[i] + i ] = cnt2[ Mx[i] - i + N ] = 0;
    }

    // cross subarray
    int pmx = m, pmn = m;
    for ( int r = m + 1; r <= R; r++) {
        int mx_r = Mx[r], mn_r = Mn[r];
        for ( ; pmx >= L && Mx[pmx] < mx_r; pmx--) {
            --cnt[ Mx[pmx] + pmx ];
            ++cnt2[ Mn[pmx] - pmx + N ];
        }
        for ( ; pmn >= L && Mn[pmn] > mn_r; pmn--) {
            ++cnt[ Mx[pmn] + pmn ];
            --cnt2[ Mn[pmn] - pmn + N ];
        }
        if ( pmn < pmx ) Ans += cnt[ mn_r + r ];
        else Ans += cnt2[ mx_r - r + N ];

        int idx = r - (mx_r - mn_r);
        Ans += ( idx <= m && idx > max(pmx, pmn) );
    }
    for ( int l = m; l >= L; l--) {
        int idx = l + (Mx[l] - Mn[l]);
        Ans += ( idx <= R && idx >= m+1 && Mx[idx] < Mx[l] && Mn[idx] > Mn[l] );
    }
}

int main()
{
    IOS;
    read_int(N);
    for ( int i = 1; i <= N; i++)
        read_int(A[i]);

    solve(1, N);
    printf("%lld\n", Ans);
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
