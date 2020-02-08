struct Edge
{
    int to, weight;
};

vector<Edge> G;
int dist[MAXV], len[MAXV];

bool spfa(int src)
{
    fill(dist, dist+V, INF);
    fill(len, len+V, -1);
    dist[src] = 0;
    len[src] = 0;

    queue<int> q;
    q.push(src);
    vector<bool> in_queue(V, false);
    in_queue[src] = true;

    while ( !q.empty() ) {
        int from = q.front(); q.pop();
        in_queue[from] = false;
        for ( Edge &e : G[from] ) {
            if ( dist[from] != INF && dist[from] + e.weight < dist[e.to] ) {
                dist[e.to] = dist[from] + e.weight;
                len[e.to] = len[from] + 1;

                // if a path's length(# of edges) > (V - 1), then there exists a negative cycle
                if ( len[e.to] > V - 1 )
                    return false;

                if ( !in_queue[e.to] ) {
                    in_queue[e.to] = true;
                    q.push(e.to);
                }
            }
        }
    }

    return true;
}