#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}

using namespace std;
using LL = long long;
using ULL = unsigned long long;

inline int read_int(int &x);

constexpr int MAXN = 1e5+5;
constexpr int MAXM = 300+5;

// dp[i][j] = min(dp[i-1][k] + cost(k, j) + cost(note[i-1], note[i]), dp[i-1][k] + cost(k, note[i]) + cost(note[i-1], j)) for all 0 <= k < M

int N, M, K;
int note[MAXN];
int _dp[2][MAXM];

inline int cost(int i, int j) { return max(0, abs(i - j) - K); }

int main()
{
    read_int(N); read_int(M); read_int(K);

    for ( int i = 1; i <= N; i++)
        read_int(note[i]);

    int *dp = _dp[0], *dp2 = _dp[1];

    int mn_b = 0;
    for ( int i = 2; i <= N; i++) {
        int mn_b_tmp = INT_MAX;
        for ( int j = 0; j < M; j++) {
            int mn_a = min(dp[max(0, j-K)], dp[min(M-1, j + K)]);
            dp2[j] = min(mn_a + cost(note[i-1], note[i]), cost(note[i-1], j) + mn_b);
            mn_b_tmp = min(mn_b_tmp, dp2[j] + cost(j, note[i+1]));
        }
        mn_b = mn_b_tmp;
        swap(dp, dp2);
    }

    cout << *min_element(dp, dp+M) << '\n';

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
