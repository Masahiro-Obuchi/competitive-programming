#include <bits/stdc++.h>

#include <cassert>

#define rep(i, n) for (int i = 0; i < (n); i++)
using namespace std;
using ll = long long;
using P = pair<int, int>;
struct Edge {
  ll to;
  ll weight;
  Edge(ll t, ll w) : to(t), weight(w) {}
};
// using Graph = vector<vector<Edge>>;
using Graph = vector<vector<int>>;

constexpr long long INF = 1LL << 62;
constexpr int INT_INF = 1 << 30;
constexpr double epsilon = 1e-9;

int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1}; // 右、下、左、上
// int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1}, dy[8] = {-1, 0, 1, 1, -1, 1, 0, -1};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, W;
  cin >> N >> W;
  vector<ll> w(N), v(N);
  for (int i = 0; i < N; i++) {
    cin >> w[i] >> v[i];
  }

  vector<vector<ll>> dp(N + 1, vector<ll>(W + 1, -INF));
  dp[0][0] = 0;
  for (int i = 1; i <= N; i++) {
    for (int j = 0; j <= W; j++) {
      dp[i][j] = dp[i - 1][j];
      if (j >= w[i - 1]) {
        dp[i][j] = max(dp[i][j], dp[i - 1][j - w[i - 1]] + v[i - 1]);
      }
    }
  }

  ll ans = -INF;
  for (int j = 0; j <= W; j++) {
    ans = max(ans, dp[N][j]);
  }
  cout << ans << endl;
  return 0;
}
