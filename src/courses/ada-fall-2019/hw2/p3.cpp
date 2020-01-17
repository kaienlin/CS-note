#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

using namespace std;
using LL = int64_t;
using ULL = uint64_t;
using LD = long double;
using pii = pair<int, int>;

constexpr int MAXB = 100;
constexpr int MAXN = 105;
constexpr int MAXK = 105;

int dp[MAXK][MAXB * MAXN];
int A[MAXN], B[MAXN];
int N, K;

int main()
{
    IOS;
    cin >> N >> K;
    for ( int i = 1; i <= N; i++)
        cin >> A[i] >> B[i];
    
    vector<int> arr;
    for ( int i = 1; i <= N; i++) {
        int mx = -1, mx_idx;
        for ( int j = 0; j <= arr.size(); j++) {
            int bsum = 0;
            for ( int k = 0; k < j; k++)
                bsum += B[arr[k]];
            int asum = 0;
            for ( int k = j; k < arr.size(); k++)
                asum += A[arr[k]];
            int val = asum * B[i] + bsum * A[i];
            if ( val > mx ) {
                mx = val;
                mx_idx = j;
            }
        }
        if ( mx_idx == arr.size() ) arr.push_back(i);
        else arr.insert(arr.begin()+mx_idx, i);
    }

    memset(dp, -1, sizeof(dp));
    dp[0][0] = 0;
    for ( int i : arr )
        for ( int k = K-1; k >= 0; k--)
            for ( int xp = 10000 - B[i]; xp >= 0; xp--)
                if ( dp[k][xp] != -1 )
                    dp[k+1][xp + B[i]] = max(dp[k+1][xp + B[i]], dp[k][xp] + A[i] * xp);

    cout << *max_element(dp[K], dp[K]+10001) << '\n';

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

