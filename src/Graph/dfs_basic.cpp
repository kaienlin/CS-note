constexpr int MAXN = 1e3+5;

int N;
vector<int> G[MAXN];
int color[MAXN], tin[MAXN], tout[MAXN], timer;

const int WHITE = 0, GREY = 1, BLACK = 2;

void is_back_edge(int from, int to) {}
void is_cross_edge(int from, int to) {}

bool is_ancestor(int anc, int des) {
    return tin[anc] < tin[des] && tout[anc] > tout[des];
}

void dfs_util(int v) {
    color[v] = GREY;
    tin[v] = timer++;
    for ( int u : G[v] ) {
        if ( color[u] == WHITE )
            dfs_util(u);
        else if ( color[u] == GREY )
            is_back_edge(v, u);
        else if ( color[u] == BLACK )
            is_cross_edge(v, u);
    }
    color[v] = BLACK;
    tout[v] = timer++;
}

void dfs() {
    fill(color, color + N, WHITE);
    timer = 0;
    for ( int i = 0; i < N; i++)
        if ( color[i] == WHITE )
            dfs_util(i);
}