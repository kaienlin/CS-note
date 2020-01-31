int N;
vector<int> G[MAXN];  // an "undirected graph"

int timer, tin[MAXN], low[MAXN];
bool vis[MAXN];

void is_bridge(int u, int v) {}

void dfs(int v, int parent=-1)
{
    vis[v] = true;
    tin[v] = low[v] = timer++;
    for ( int to : G[v] ) {
        if ( to == parent ) continue;
        if ( vis[to] ) {
            // back edge
            low[v] = min(low[v], tin[to]);
        } else {
            // tree edge
            dfs(to, v);
            low[v] = min(low[v], low[to]);
            if ( low[to] > tin[v] )
                is_bridge(v, to);
        }
    }
}

void find_bridges()
{
    timer = 0;
    fill(vis, vis+N, false);
    for ( int i = 0; i < N; ++i )
        if ( !vis[i] )
            dfs(i);
}