#include <bits/stdc++.h>

namespace ada {

class Xoroshiro128 {
 public:
  using result_type = uint32_t;
  static constexpr result_type(min)() { return 0; }
  static constexpr result_type(max)() { return UINT32_MAX; }
  static inline result_type rotl(const result_type x, int k) {
    return (x << k) | (x >> (32 - k));
  }
  Xoroshiro128() : Xoroshiro128(1, 2, 3, 4) {}
  Xoroshiro128(result_type a, result_type b, result_type c, result_type d)
      : s{a, b, c, d} {}
  result_type operator()() {
    const result_type result = rotl(s[0] + s[3], 7) + s[0];
    const result_type t = s[1] << 9;
    s[2] ^= s[0];
    s[3] ^= s[1];
    s[1] ^= s[2];
    s[0] ^= s[3];
    s[2] ^= t;
    s[3] = rotl(s[3], 11);
    return result;
  }

 private:
  std::array<result_type, 4> s;
};

namespace {
int c_lead, c_team;
Xoroshiro128 rng;
}  // namespace

int Init() {
  int n;
  uint32_t s1, s2, s3, s4;
  std::cin >> n >> c_lead >> c_team >> s1 >> s2 >> s3 >> s4;
  rng = Xoroshiro128(s1, s2, s3, s4);
  return n;
}

int GetLeadership() { return uint64_t(rng()) * c_lead >> 32; }

int GetTeamValue() {
  int tmp = int(uint64_t(rng()) * c_team >> 32) + 1;
  return int(c_team / sqrt(tmp));
}

}  // namespace ada

#define IOS {ios::sync_with_stdio(false);cin.tie(nullptr);}
using namespace std;
using LL = int64_t;
using ULL = uint64_t;
using LD = long double;
using pii = pair<int, int>;

constexpr int MAXN = 2e6+5;
constexpr int MOD = 1e9+7;

int n;
int lead[MAXN], team[MAXN];
int dp[MAXN], dp2[MAXN];
LL psum[MAXN];

int my_bsearch(LL x) {
    int l = 1, r = n+1;
    while ( l + 1 < r ) {
        int m = (l + r) >> 1;
        (x < psum[m] ? r : l) = m;
    }
    return r;
}

int main()
{
    IOS;
    n = ada::Init();
    for (int i = 1; i <= n; i++) lead[i] = ada::GetLeadership();
    for (int i = 1; i <= n; i++) team[i] = ada::GetTeamValue();

    for ( int i = 1; i <= n; i++)
        psum[i] = psum[i-1] + team[i];

    dp2[n+1] = 1;
    for ( int i = n; i >= 1; i--) {
        int p = my_bsearch(lead[i] + psum[i]);
        dp[i] = (dp2[i + 1] - dp2[p + 1]);
        dp[i] +=  dp[i] < 0 ? MOD : 0;
        dp2[i] = (dp[i] + dp2[i+1]);
        dp2[i] -= dp2[i] >= MOD ? MOD : 0;
    }
    cout << dp[1] << '\n';
    return 0;
}
