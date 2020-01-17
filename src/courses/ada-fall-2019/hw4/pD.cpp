#include <bits/stdc++.h>
#include "hex.h"

using namespace std;
using pii = pair<int, int>;

constexpr int MAXN = 8;
constexpr int Me = 1, He = 2;

const vector<pii> Adj = {pii(-1, 1), pii(-1, 0), pii(1, -1), pii(1, 0), pii(-1, 1), pii(0, 1)};
const vector<pii> GoodUp = {pii(-1, 1), pii(-1, 0)};
const vector<pii> GoodDown = {pii(1, -1), pii(1, 0)};
const vector<pii> BadAdj = {pii(0, -1), pii(1, -1), pii(-1, 1), pii(0, 1)};
const vector<pii> GoodBri = {pii(-2, 1), pii(2, -1)};
const vector<pii> BadBri = {pii(1, -2), pii(-1, 2)};
const vector<pii> Block = {pii(1, 0), pii(0, 1)};

int board[MAXN][MAXN];

int n;
pii SRC;
int W[MAXN][MAXN];

int cnt;

void init2(int _n) {
    n = _n;
    cnt = 0;
    memset(board, 0, sizeof board);
}


void init(int _n) {
    if ( _n == 5 ) { init2(_n); return; }
    srand(7902064);
    n = _n;
    memset(board, 0, sizeof board);
    memset(W, 0, sizeof(W));
    int x = n / 2, y = n / 2;
    if ( n % 2 == 0 ) --x;
    W[x][y] += 100;
    SRC = pii(x, y);
}

bool in_board(int i, int j) {
    return i >= 0 && j >= 0 && i < n && j < n;
}

int how_many(int i, int j, int label, vector<pii> DIR) {
    int cnt = 0;
    for ( pii d : DIR ) {
        int ni = i + d.first, nj = j + d.second;
        if ( in_board(ni, nj) && board[ni][nj] == label )
            ++cnt;
    }
    return cnt;
}

int plus_adj(int i, int j, int add, vector<pii> DIR) {
    int cnt = 0;
    for ( pii d : DIR ) {
        int ni = i + d.first, nj = j + d.second;
        if ( in_board(ni, nj) && board[ni][nj] == 0 )
            W[ni][nj] += add, ++cnt;
    }
    return cnt;
}

void good(int i, int j) {
    int up_me = how_many(i, j, Me, GoodUp);
    if ( up_me >= 1 || i == 0 ) ++W[i][j];

    int down_me = how_many(i, j, Me, GoodDown);
    if ( down_me >= 1 || i == n-1 ) ++W[i][j];
}

void bad(int i, int j) {
    int up_he = how_many(i, j, He, GoodUp);
    int down_he = how_many(i, j, He, GoodDown);
    if ( up_he > 0 ) W[i][j]--;
    if ( down_he > 0 ) W[i][j]--;
}

void block(int i, int j) {
    int a = how_many(i, j, He, BadAdj);
    if ( a > 0 ) W[i][j] += 2;
}

pii decide2(pii p) {
    if ( p != make_pair(-1, -1) )
        board[p.first][p.second] = He;

    if ( cnt == 0 ) {
        ++cnt;
        int x = n / 2, y = n / 2;
        if ( n % 2 == 0 )
            --x;
        board[x][y] = Me;
        return pii(x, y);
    }

    memset(W, 0, sizeof(W));
    for ( int i = 0; i < n; i++) {
        for ( int j = 0; j < n; j++) {
            good(i, j);
            bad(i, j);
            block(i, j);
        }
    }
    
    int best = 0;
    pii best_pos = {0, 0};
    for ( int i = 0; i < n; i++) {
        for ( int j = 0; j < n; j++) {
            if ( board[i][j] == 0 && W[i][j] > best ) {
                best = W[i][j];
                best_pos = pii(i, j);
            }
        }
    }

    board[best_pos.first][best_pos.second] = Me;
    ++cnt;
    return best_pos;
}


pii decide(pii p) {
    if ( n == 5 ) return decide2(p);
    if ( p != make_pair(-1, -1) )
        board[p.first][p.second] = He;
    
    if ( p.second > 0 )
        W[p.first][p.second-1] += 1;
    
    plus_adj(p.first, p.second, 1, Block);

    for ( int i = 0; i < n; i++)
        for ( int j = 0; j < n; j++)
            if ( board[i][j] == Me ) {
                if ( how_many(i, j, Me, GoodUp) == 0 )
                    plus_adj(i, j, 2, GoodUp);
                if ( how_many(i, j, Me, GoodDown) == 0 )
                    plus_adj(i, j, 2, GoodDown);
            }

    for ( int i = 0; i < n; i++)
        for ( int j = 0; j < n; j++)
            if ( board[i][j] == 0 && (how_many(i, j, He, GoodUp) == 2 || how_many(i, j, He, GoodDown) == 2) )
                W[i][j] = 0;

    int best = -1;
    vector<pii> L;
    for ( int i = 0; i < n; i++) {
        for ( int j = 0; j < n; j++) {
            if ( board[i][j] == 0 && W[i][j] > best ) {
                best = W[i][j];
                L.clear();
                L.push_back(pii(i, j));
            } else if ( board[i][j] == 0 && W[i][j] == best ) {
                L.push_back(pii(i, j));
            }
        }
    }

    pii best_pos = L[rand() % L.size()];

    board[best_pos.first][best_pos.second] = Me;
    memset(W, 0, sizeof(W));

    return best_pos;
}
