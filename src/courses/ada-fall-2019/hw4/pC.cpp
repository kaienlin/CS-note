#include <bits/stdc++.h>
#define IOS { ios::sync_with_stdio(false); cin.tie(nullptr); }

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

using namespace std;
using ll = long long;
using ULL = unsigned long long;
using pii = pair<int, int>;

struct Vertex {
    int id, deg;
};

constexpr int MAXN = 120;
using bst = bitset<MAXN>;

int N;
int A[MAXN][MAXN];
bool G[MAXN][MAXN];
vector<int> sol, Q;

vector<int> C[MAXN];

void add_edge(int u, int v) {
    G[u][v] = G[v][u] = 1;
}

bool test(const int e, const vector<int>& S) {
    for ( int i : S ) if ( G[i][e] ) return true;
    return false;
}

void color_sort(vector<Vertex>& R) {
    int max_no = 1, j = 0;
    int k_min = sol.size() - Q.size() + 1;
    C[1].clear(); C[2].clear();
    for ( int i = 0; i < R.size(); i++) {
        int p = R[i].id;
        int k = 1;
        while ( test(p, C[k]) )
            ++k;
        if ( k > max_no ) {
            max_no = k;
            C[max_no+1].clear();
        }
        C[k].push_back(p);
        if ( k < k_min ) {
            R[j].id = p;
            ++j;
        }
    }
    if ( j > 0 ) R[j-1].deg = 0;
    if ( k_min <= 0 ) k_min = 1;
    for ( int k = k_min; k <= max_no; ++k) {
        int sz = C[k].size();
        for ( int i = 0; i < sz; i++) {
            R[j].id = C[k][i];
            R[j].deg = k;
            ++j;
        }
    }
}

bool deg_cmp(const Vertex u, const Vertex v) { return u.deg > v.deg; }

void calc_deg(vector<Vertex>& V) {
    for ( Vertex &v : V ) {
        v.deg = 0;
        for ( int j = 0; j < N; j++)
            v.deg += (int)G[v.id][j];
    }
}

void degree_sort(vector<Vertex>& R) {
    calc_deg(R);
    sort(R.begin(), R.end(), deg_cmp);
}

int S[MAXN], S_old[MAXN], ALLSTEP;
const double T_limit = 0.025;
void expand_dyn(vector<Vertex>& R, int level) {
    S[level] = S[level] + S[level-1] - S_old[level];
    S_old[level] = S[level-1];
    while ( R.size() ) {
        if ( Q.size() + R.back().deg <= sol.size() ) return;

        int p = R.back().id; R.pop_back();
        Q.push_back(p);
        vector<Vertex> Rp;
        for ( Vertex v : R )
            if ( G[p][v.id] )
                Rp.push_back(v);

        if ( Rp.size() ) {
            if ( (double)S[level] / ++ALLSTEP < T_limit )
                degree_sort(Rp);
            color_sort(Rp);
            ++S[level];
            expand_dyn(Rp, level + 1);
        } else if ( Q.size() > sol.size() ) {
            sol = vector<int>(Q);
        }
        Q.pop_back();
    }
}

void init_V(vector<Vertex>& V) {
    calc_deg(V);
    sort(V.begin(), V.end(), deg_cmp);
    int mx_deg = V[0].deg;
    for ( int i = 0; i < mx_deg; i++)
        V[i].deg = i + 1;
    for ( int i = mx_deg; i < N; i++)
        V[i].deg = mx_deg + 1;
}

void MCQdyn() {
    ALLSTEP = 0;
    vector<Vertex> V;
    for ( int i = 0; i < N; i++) V.push_back({i, 0});
    init_V(V);
    memset(S, 0, sizeof(S)); memset(S_old, 0, sizeof(S_old));
    expand_dyn(V, 1);
}

int main()
{
    IOS;
    cin >> N;
    for ( int i = 0; i < N; i++) {
        string s; cin >> s;
        for ( int j = 0; j < N; j++) {
            A[i][j] = s[j] - '0';
        }
    }

    for ( int i = 0; i < N; i++) {
        for ( int j = i + 1; j < N; j++) {
            if ( A[i][j] && A[j][i] && A[i][i] && A[j][j] )
                add_edge(i, j);
        }
    }

    MCQdyn();

    int ans = sol.size();
    bool used[MAXN] = {0};
    for ( int i = 0; i < sol.size(); i++) {
        used[sol[i]] = true;
        sol[i]++;
    }
    for ( int i = 0; i < N; i++)
        if ( !used[i] )
            sol.push_back(i+1);

    for ( int i = 0; i < sol.size(); i++)
        cout << sol[i] << " \n"[i==sol.size()-1];

#ifdef DEBUG
    cout << "max clique size = " << ans << '\n';
    for ( int i = 0; i < N; i++) {
        for ( int j = 0; j < N; j++)
            cout << A[sol[i]-1][sol[j]-1];
        cout << '\n';
    }
#endif

    return EXIT_SUCCESS;
}
