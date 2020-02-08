template<int MAXN>
struct DSU
{
    int parent[MAXN], size[MAXN];

    void init(int n)
    {
        for ( int i = 0; i < n; i++)
            make_set(i);
    }

    void make_set(int v)
    {
        parent[v] = v;
        size[v] = 1;
    }

    int find_set(int v)
    {
        if ( v == parent[v] ) return v;
        return (parent[v] = find_set(parent[v]));
    }

    void unite_set(int v, int u)
    {
        v = find_set(v), u = find_set(u);
        if ( v != u ) {
            if ( size[v] < size[u] )
                swap(v, u);
            parent[u] = v;
            size[v] += size[u];
        }
    }
};

struct Edge
{
    int u, v, w;
    bool operator < (const Edge &rhs) const { return w < rhs.w; }
};

DSU<MAXV> dsu;

int kruskal(vector<Edge> &edges)
{
    sort(edges.begin(), edges.end());
    dsu.init(N);
    int cost = 0;
    for ( Edge e : edges ) {
        if ( dsu.find_set(e.u) != dsu.find_set(e.v) ) {
            cost += e.w;
            dsu.unite_set(e.u, e.v);
        }
    }
    if ( dsu.size[dsu.find_set(0)] != N )
        return NOT_CONNECTED;
    else
        return cost;
}