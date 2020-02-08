struct Edge
{
    int from, to, weight;
};

vector<Edge> edges;
int dist[MAXN];

bool relax(Edge e)
{
    if ( dist[e.from] != INF && dist[e.to] < dist[e.from] + e.weight ) {
        dist[e.to] = dist[e.from] + e.weight;
        return true;
    }
    return false;
}

bool bellman_ford(int src)
{
    fill(dist, dist+V, INF);
    dist[src] = 0;
    
    for ( int i = 1; i <= V-1; i++)
        for ( Edge e : edges )
            relax(e);
    
    for ( Edge e : edges )
        if ( relax(e) )
            return false;

    return true;
}