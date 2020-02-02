bool topo_cmp(const int a, const int b)
{
    /* tout[] is obtained by basic dfs */
    return tout[a] > tout[b];
}

vector<int> topo_sort()
{
    dfs();
    vector<int> topo(N);
    for ( int i = 0; i < N; i++ )
        topo[i] = i;

    // sort the vertices by descending order of the "end time" in DFS
    sort(topo.begin(), topo.end(), topo_cmp);
    return topo;
}