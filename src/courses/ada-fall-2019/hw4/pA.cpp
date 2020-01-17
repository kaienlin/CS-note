#include <bits/stdc++.h>
#define IOS { ios::sync_with_stdio(false); cin.tie(nullptr); }

#pragma GCC optimize("Ofast", "no-stack-protector", "unroll-loops")
#pragma GCC target("sse,sse2,sse3,ssse3,sse4,sse4.2,popcnt,abm,mmx,avx,tune=native")

#ifndef DIMACS
#include <cryptominisat5/cryptominisat.h>
#else
#include <fstream>
#include <vector>
#endif

namespace sat {

#ifndef DIMACS

CMSat::SATSolver solver;
std::vector<CMSat::Lit> lit_buf;

void Init(int n) { solver.new_vars(n + 1); }

void AddClause(std::vector<int> v) {
	lit_buf.clear();
	lit_buf.reserve(v.size());
	for (int x : v) lit_buf.emplace_back(abs(x), x < 0);
	solver.add_clause(lit_buf);
}

bool Solve() { return solver.solve() == CMSat::l_True; }

int GetResult(int id) {
	auto r = solver.get_model()[id];
	return r == CMSat::l_True ? 1 : r == CMSat::l_False ? -1 : 0;
}

#else

std::vector<std::vector<int>> clauses;
int n_vars;

void Init(int n) { n_vars = n; }

void AddClause(std::vector<int> v) { clauses.emplace_back(std::move(v)); }

bool Solve() {
	std::fstream fs("out.dimacs", fs.trunc | fs.out);
	fs << "p cnf " << n_vars << ' ' << clauses.size() << '\n';
	for (auto &v : clauses) {
		for (auto x : v) fs << x << ' ';
		fs << "0\n";
	}
	fs.close();
	exit(0);
}

int GetResult(int id) { return 0; }

#endif // DIMACS

}  // namespace sat

using namespace std;
using LL = long long;
using ULL = unsigned long long;
using pii = pair<int, int>;

constexpr int MAXR = 128;
constexpr int MAXC = 128;

int r, c, board[MAXR][MAXC];
bool is_wall[MAXR + MAXC + 2 * MAXR * MAXC];

inline int id_h(int i, int j) { return i * (2 * c + 1) + j + 1; }
inline int id_v(int i, int j) { return c + i * (2 * c + 1) + j + 1; }

void all_false(vector<int>& S) {
    for ( int v : S ) {
        vector<int> vv = {-v};
        sat::AddClause(vv);
    }
}
void exactly_one(vector<int>& S) {
    sat::AddClause(S);
    int sz = S.size();
    for ( int i = 0; i < sz; i++)
        for ( int j = i+1; j < sz; j++) {
            vector<int> vv = {-S[i], -S[j]};
            sat::AddClause(vv);
        }
}
void exactly_one_false(vector<int> &S) {
    for ( int &v : S ) v = -v;
    exactly_one(S);
}
void exactly_two(vector<int> &S) {
    for ( int i = 0; i < 4; i++) {
        vector<int> vv;
        for ( int j = 0; j < 4; j++) if ( j != i ) vv.push_back(S[j]);
        sat::AddClause(vv);
        for ( int &v : vv ) v = -v;
        sat::AddClause(vv); 
    }
}
void exactly_two_or_none(vector<int> &S) {
    if ( S.size() == 2 ) {
        vector<int> v1 = {-S[0], S[1]}, v2 = {S[0], -S[1]};
        sat::AddClause(v1); sat::AddClause(v2);
    } else if ( S.size() == 3 ) {
        for ( int i = 0; i < 3; i++) {
            S[i] = -S[i];
            sat::AddClause(S);
            S[i] = -S[i];
        }
        for ( int &i : S ) i = -i;
        sat::AddClause(S);
    } else if ( S.size() == 4 ) {
        for ( int i = 0; i < 4; i++) {
            S[i] = -S[i];
            sat::AddClause(S);
            S[i] = -S[i];
        }
        for ( int i = 0; i < 4; i++) {
            vector<int> vv;
            for ( int j = 0; j < 4; j++) if ( j != i ) vv.push_back(-S[j]);
            sat::AddClause(vv);
        }
    }
}

bool vis[MAXR][MAXC];
stack<pii> stk;
void floodfill() {
    while ( !stk.empty() ) {
        pii v = stk.top(); stk.pop();
        int i = v.first, j = v.second;
        if ( vis[i][j] ) continue;
        vis[i][j] = true;
        if ( i > 0 && !is_wall[id_h(i, j)] ) stk.push(pii(i-1, j));
        if ( i < r-1 && !is_wall[id_h(i+1, j)] ) stk.push(pii(i+1, j));
        if ( j > 0 && !is_wall[id_v(i, j)] ) stk.push(pii(i, j-1));
        if ( j < c-1 && !is_wall[id_v(i, j+1)] ) stk.push(pii(i, j+1));
    }
}
pii first_loop;
bool valid() {
    for ( int i = 0; i < c; i++) if ( !is_wall[id_h(0, i)] ) stk.push(pii(0, i));
    for ( int i = 0; i < c; i++) if ( !is_wall[id_h(r, i)] ) stk.push(pii(r-1, i));
    for ( int i = 1; i < r-1; i++) if ( !is_wall[id_v(i, 0)] ) stk.push(pii(i, 0));
    for ( int i = 1; i < r-1; i++) if ( !is_wall[id_v(i, c)] ) stk.push(pii(i, c-1));
    bool f = true;
    for ( int i = 0; i < r && f; i++)
        for ( int j = 0; j < c; j++)
            if ( is_wall[id_v(i, j)] ) {
                stk.push(pii(i, j));
                first_loop = pii(i, j);
                f = false;
                break;
            }
    memset(vis, 0, sizeof(vis));
    floodfill();
    for ( int i = 0; i < r; i++)
        for ( int j = 0; j < c; j++)
            if ( !vis[i][j] )
                return false;
    return true;
}
void get_loop(int i, int j, vector<int>& L) {
    stk.push(pii(i, j));
    while ( !stk.empty() ) {
        pii v = stk.top(); stk.pop();
        int i = v.first, j = v.second;
        if ( vis[i][j] ) continue;
        vis[i][j] = true;
        if ( i > 0 && !is_wall[id_h(i, j)] ) stk.push(pii(i-1, j));
        if ( i < r-1 && !is_wall[id_h(i+1, j)] ) stk.push(pii(i+1, j));
        if ( j > 0 && !is_wall[id_v(i, j)] ) stk.push(pii(i, j-1));
        if ( j < c-1 && !is_wall[id_v(i, j+1)] ) stk.push(pii(i, j+1));

        if ( is_wall[id_h(i, j)] ) L.push_back(id_h(i, j));
        if ( is_wall[id_h(i+1, j)] ) L.push_back(id_h(i+1, j));
        if ( is_wall[id_v(i, j)] ) L.push_back(id_v(i, j));
        if ( is_wall[id_v(i, j+1)] ) L.push_back(id_v(i, j+1));
    }
}

int main()
{
    IOS;
    cin >> r >> c;
    for ( int i = 0; i < r; i++) {
        string s; cin >> s;
        for ( int j = 0; j < c; j++)
            board[i][j] = s[j] - '0';
    }
    int nd = 2*r*c + r + c;
    sat::Init(nd);

    for ( int i = 0; i < r; i++) {
        for ( int j = 0; j < c; j++) {
            vector<int> S = {id_h(i, j), id_h(i+1, j), id_v(i, j), id_v(i, j+1)};
            switch ( board[i][j] ) {
                case 0: all_false(S); break;
                case 1: exactly_one(S); break;
                case 2: exactly_two(S); break;
                case 3: exactly_one_false(S); break;
            }
        }
    }
    
    for ( int i = 0; i <= r; i++) {
        for ( int j = 0; j <= c; j++) {
            vector<int> v;
            if ( i < r ) v.push_back(id_v(i, j));
            if ( i > 0 ) v.push_back(id_v(i-1, j));
            if ( j < c ) v.push_back(id_h(i, j));
            if ( j > 0 ) v.push_back(id_h(i, j-1));
            exactly_two_or_none(v);
        }
    }

    while ( true ) {
        if ( !sat::Solve() ) {
            cerr << "cannot solve\n";
            exit(128);
        }

        memset(is_wall, 0, sizeof(is_wall));
        for ( int i = 1; i <= nd; i++)
            is_wall[i] = (sat::GetResult(i) == 1);

        if ( valid() ) {
            break;
        } else {
            for ( int i = 0; i < r; i++) {
                for ( int j = 0; j < c; j++) {
                    if ( !vis[i][j] ) {
                        vector<int> v;
                        get_loop(i, j, v);
                        for ( int &i : v ) i = -i;
                        sat::AddClause(v);
                    }
                }
            }
            memset(vis, 0, sizeof(vis));
            vector<int> v;
            get_loop(first_loop.first, first_loop.second, v);
            for ( int &i : v ) i = -i;
            sat::AddClause(v);
        }
    }

    for ( int i = 1; i <= nd; i++)
        cout << is_wall[i];
    cout << '\n';

#ifdef DEBUG
    for ( int i = 0; i < r; i++) {
        for ( int j = 0; j < c; j++)
            cout << "+" << (is_wall[id_h(i, j)] ? "-" : " ");
        cout << "+\n";
        for ( int j = 0; j < c; j++)
            cout << (is_wall[id_v(i, j)] ? "|" : " ") << (char)(board[i][j] + '0');
        cout << (is_wall[id_v(i, c)] ? "|\n" : " \n");
    }
    for ( int j = 0; j < c; j++)
        cout << "+" << (is_wall[id_h(r, j)] ? "-" : " ");
    cout << "+\n";
#endif
    return EXIT_SUCCESS;
}
