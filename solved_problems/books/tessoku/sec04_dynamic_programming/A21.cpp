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
  int N;
  cin >> N;
  vector<ll> A(N), P(N);
  for (int i = 0; i < N; i++) {
    cin >> P[i] >> A[i];
    P[i]--;
  }

  vector<vector<ll>> dp(N, vector<ll>(N, 0));
  dp[0][N - 1] = 0;
  for (int len = N - 1; len >= 0; len--) {
    for (int l = 0; l < N; l++) {
      int r = l + len - 1;
      if (l < 0 || l >= N || r < 0 || r >= N) {
        continue;
      }

      int score1 = 0, score2 = 0;
      if (l <= P[l - 1] && P[l - 1] <= r) {
        score1 = A[l - 1];
      }
      if (l <= P[r + 1] && P[r + 1] <= r) {
        score2 = A[r + 1];
      }

      if (l == 0) {
        dp[l][r] = dp[l][r + 1] + score2;
      } else if (r == N - 1) {
        dp[l][r] = dp[l - 1][r] + score1;
      } else {
        dp[l][r] = max(dp[l - 1][r] + score1, dp[l][r + 1] + score2);
      }
    }
  }

  ll ans = 0;
  for (int i = 0; i < N; i++) {
    ans = max(ans, dp[i][i]);
  }
  cout << ans << endl;
  return 0;
}
