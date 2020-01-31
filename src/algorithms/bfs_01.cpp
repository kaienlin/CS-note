void bfs01(int src)
{
    fill(dist, dist+N, INF);
    fill(prev, prev+N, -1);
    deque<int> Q;
    Q.push_front(src);
    dist[src] = 0;
    while ( !Q.empty() ) {
        int v = Q.front(); Q.pop_front();
        for ( auto to : G[v] ) {
            if ( dist[v] + to.w < dist[to.id] ) {
                dist[to.id] = dist[v] + to.w;
                if ( to.w == 1 )
                    Q.push_back(to.id);
                else if ( to.w == 0 )
                    Q.push_front(to.id);
            }
        }
    }
}
