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
  int N, M;
  cin >> N >> M;
  vector<vector<int>> A(M, vector<int>(N));
  for (int i = 0; i < M; i++) {
    for (int j = 0; j < N; j++) {
      cin >> A[i][j];
    }
  }

  vector<vector<int>> dp(M + 1, vector<int>(1 << N, INT_INF));

  dp[0][0] = 0;
  for (int i = 0; i < M; i++) {
    for (int S = 0; S < (1 << N); S++) {
      // 前の状態から何もしない場合
      dp[i + 1][S] = min(dp[i + 1][S], dp[i][S]);

      int U = 0;
      for (int j = 0; j < N; j++) {
        if (A[i][j] == 1) {
          U |= (1 << j);
        }
      }
      dp[i + 1][S | U] = min(dp[i + 1][S | U], dp[i][S] + 1);
    }
  }

  cout << (dp[M][(1 << N) - 1] == INT_INF ? -1 : dp[M][(1 << N) - 1]) << endl;
  return 0;
}
