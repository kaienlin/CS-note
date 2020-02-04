void dijkstra(int src)
{
    fill(min_d, min_d+N, INF);
    fill(prev, prev+N, -1);
    fill(used, used+N, false);
    min_d[src] = 0;

    for ( int i = 0; i < N; i++ ) {
        int v = -1;
        for ( int j = 0; j < N; j++ )
            if ( !used[j] && (v == -1 || min_d[j] < min_d[v]) )
                v = j;
        
        if ( min_d[v] == INF )
            break;

        used[v] = true;
        for ( auto e : adj[v] ) {
            int to = e.second, wei = e.first;
            if ( min_d[v] + wei < min_d[to] ) {
                min_d[to] = min_d[v] + wei;
                prev[to] = v;
            }
        }
    }
}
