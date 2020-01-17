#include <bits/stdc++.h>
#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}
#define gc getchar_unlocked()

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

using namespace std;

const int MAXL = 2001;

int na, nb;
char Sa[MAXL], Sb[MAXL];
int dp[MAXL][MAXL];

inline int min(int a, int b) { return a < b ? a : b; }

int main()
{
    while ( (Sa[na] = gc) != '\n' ) ++na;
    while ( (Sb[nb] = gc) >= 'A' ) ++nb;

    for ( int a = na-1; a >= 0; --a) {
        for ( int b = nb-1; b >= 0; --b) {
            if ( Sa[a] == Sb[b] )
                dp[a][b] = 1 + min(dp[a+1][b+1], min(dp[a][b+1], dp[a+1][b]));
            else
                dp[a][b] = dp[a+1][b+1];
        }
    }

    cout << dp[0][0];
    return 0;
}

