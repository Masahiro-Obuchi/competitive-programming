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
  string S;
  cin >> S;

  vector<vector<int>> dp(N, vector<int>(N, 0));
  for (int i = 0; i < N; i++) {
    dp[i][i] = 1; // 単一文字は回文
  }
  for (int i = 0; i < N - 1; i++) {
    if (S[i] == S[i + 1]) {
      dp[i][i + 1] = 2; // 2文字が同じなら回文
    } else {
      dp[i][i + 1] = 1; // 2文字が異なるなら回文ではない
    }
  }

  for (int len = 2; len <= N; len++) {
    for (int l = 0; l < N; l++) {
      int r = l + len - 1;
      if (l < 0 || l >= N || r < 0 || r >= N) {
        continue; // 範囲外チェック
      }

      if (S[l] == S[r]) {
        dp[l][r] = max(dp[l + 1][r - 1] + 2,
                       max(dp[l + 1][r], dp[l][r - 1])); // 両端が同じなら内側の回文長に2を足す
      } else {
        dp[l][r] = max(dp[l + 1][r], dp[l][r - 1]); // 両端が異なるなら、内側の回文長の最大値を取る
      }
    }
  }

  cout << dp[0][N - 1] << endl; // 全体の回文長を出力
  return 0;
}
