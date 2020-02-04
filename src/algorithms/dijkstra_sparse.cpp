void dijkstra(int src)
{
    fill(min_d, min_d+N, INF);
    fill(prev, prev+N, -1)
    min_d[src] = 0;
    priority_queue< pii, vector<pii>, greater<pii> > pq;
    pq.push({0, src});

    while ( !pq.empty() ) {
        int v = pq.top().second, d_v = pq.top().first;
        pq.pop();
        if ( d_v != min_d[v] ) continue;
        for ( auto e : adj[v] ) {
            int to = e.second, wei = e.first;
            if ( d_v + wei < min_d[to] ) {
                min_d[to] = min_d[v] + wei;
                prev[to] = v;
                pq.push( {min_d[to], to} );
            }
        }
    }
}
