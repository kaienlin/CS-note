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
}