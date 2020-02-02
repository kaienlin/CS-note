const int MAXN = 250000 + 5;
const double eps = 0.01;
using pii = pair<int, int>;

struct Pt {
    int x, y, idx;
} arr[MAXN], tmp[MAXN];

bool cmp_x(Pt &a, Pt &b) {
    return a.x < b.x;
}
bool cmp_y(Pt &a, Pt &b) {
    return a.y < b.y;
}
bool eq(double a, double b) {
    return fabs(a - b) < eps;
}

int N;
double mn;
vector<pii> ans;

double get_dist(const Pt &a, const Pt &b) {
    return sqrt((double)(a.x-b.x)*(a.x-b.x) + (double)(a.y-b.y)*(a.y-b.y));
}
void upd_ans(const Pt &a, const Pt &b) {
    double d = get_dist(a, b);
    if ( d < mn ) {
        int aid = a.idx, bid = b.idx;
        if ( aid > bid ) swap(aid, bid);
        ans.clear(); ans.push_back( {aid, bid} );
        mn = d;
    } else if ( eq(mn, d) ) {
        int aid = a.idx, bid = b.idx;
        if ( aid > bid ) swap(aid, bid);
        ans.push_back( {aid, bid} );
    }
}
void DC(int L, int R) {
    if ( R - L + 1 <= 3 ) {
        for ( int i = L; i <= R; i++)
            for ( int j = i + 1; j <= R; j++)
                upd_ans(arr[i], arr[j]);
        sort(arr+L, arr+R+1, cmp_y);
        return;
    }

    int M = (L + R) >> 1;
    int midx = arr[M].x;
    DC(L, M);
    DC(M+1, R);
    merge(arr+L, arr+M+1, arr+M+1, arr+R+1, tmp, cmp_y);
    copy(tmp, tmp+(R-L+1), arr+L);

    size_t tsz = 0;
    for ( int i = L; i <= R; i++) {
        if ( abs(arr[i].x - midx) < mn + eps ) {
            for ( int j = tsz - 1; j >= 0 && arr[i].y - tmp[j].y < mn + eps; --j)
                upd_ans(arr[i], tmp[j]);
            tmp[tsz++] = arr[i];
        }
    }
}
void solve() {
    sort(arr+1, arr+N+1, cmp_x);
    mn = DBL_MAX;
    DC(1, N);
    sort(ans.begin(), ans.end());
    ans.erase(unique(ans.begin(), ans.end()), ans.end());
}