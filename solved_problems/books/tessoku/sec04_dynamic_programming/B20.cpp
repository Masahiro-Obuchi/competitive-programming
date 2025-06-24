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
  string S, T;
  cin >> S >> T;
  int N = S.size(), M = T.size();
  vector<vector<int>> dp(N + 1, vector<int>(M + 1, 0));
  for (int i = 0; i <= N; i++) {
    dp[i][0] = i;
  }
  for (int j = 0; j <= M; j++) {
    dp[0][j] = j;
  }

  for (int i = 1; i <= N; i++) {
    for (int j = 1; j <= M; j++) {
      if (S[i - 1] == T[j - 1]) {
        dp[i][j] = min(dp[i - 1][j - 1], min(dp[i - 1][j] + 1, dp[i][j - 1] + 1));
      } else {
        dp[i][j] = min(dp[i - 1][j - 1] + 1, min(dp[i - 1][j] + 1, dp[i][j - 1] + 1));
      }
    }
  }

  cout << dp[N][M] << endl;
  return 0;
}
