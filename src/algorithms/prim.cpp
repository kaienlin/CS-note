int prim()
{
    // initializing
    fill(min_w, min_w+N, INF);
    fill(used, used+N, false);

    // first: weight of edge, second: id of vertex
    using pii = pair<int, int>;
    // min heap
    priority_queue<pii, vector<pii>, greater<pii>> pq;
    pq.push( pii(0, SRC) );
    min_w[SRC] = 1;
    
    int tot = 0;
    int tree_size = 0;
    while ( tree_size < N ) {
        if ( pq.empty() )
            return NOT_CONNECTED;
        
        int v = pq.top().second;
        int w = pq.top().first;
        pq.pop();

        if ( min_w[v] != w )
            continue;

        tot += w;
        used[v] = true;
        ++tree_size;

        for ( pii edge : G[v] ) {
            int to = edge.second;
            int wei = edge.first;
            if ( !used[to] && wei < min_w[to] ) {
                pq.push( pii(wei, to) );
                min_w[to] = wei;
            }
        }
    }
    
    return tot;
}