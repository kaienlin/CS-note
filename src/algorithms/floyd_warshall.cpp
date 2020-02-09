constexpr int INF = INT_MAX;

int dp[MAXV][MAXV];

void floyd_warshall()
{
    for ( int i = 0; i < N; i++ )
        for ( int j = 0; j < N; j++ )
            dp[i][j] = INF;

    for ( int m = 0; m < N; m++ )
        for ( int from = 0; from < N; from++ )
            for ( int to = 0; to < N; to++ )
                if ( dp[from][m] != INF && dp[m][to] != INF )
                    dp[from][to] = min(dp[from][to], dp[from][m] + dp[m][to]);
}