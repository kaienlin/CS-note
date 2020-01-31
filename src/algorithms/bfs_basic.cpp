void bfs_util(int src)
{
    fill(dist, dist+N, INF);
    fill(prev, prev+N, -1);
    queue<int> Q;
    Q.push(src);
    dist[src] = 0;
    while ( !Q.empty() ) {
        int v = Q.front(); Q.pop();
        for ( int to : G[v] ) {
            if ( dist[to] == INF ) {
                dist[to] = dist[v] + 1;
                prev[to] = v;
                Q.push(to);
            }
        }
    }
}
