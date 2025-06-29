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
  vector<int> X(N), Y(N);
  for (int i = 0; i < N; i++) {
    cin >> X[i] >> Y[i];
  }

  // dp[S][i]:= Sの頂点を訪問済みで、iが最後に訪れた頂点のときの最小コスト
  vector<vector<double>> dp(1 << N, vector<double>(N, 1e10));
  dp[0][0] =
      0; // 最後にスタート地点に戻るため、ここではスタート地点を訪問した都市の集合には含めない

  for (int S = 0; S < (1 << N); S++) {
    for (int i = 0; i < N; i++) { // now
      if (dp[S][i] >= 1e10) {
        continue; // iが訪問済みでない場合はスキップ
      }
      for (int j = 0; j < N; j++) { // next
        if (S & (1 << j)) {
          continue; // jが訪問済みの場合はスキップ
        }
        // Sにjを追加して、iからjへの距離を計算
        double dist = sqrt(pow(X[i] - X[j], 2) + pow(Y[i] - Y[j], 2));
        dp[S | (1 << j)][j] = min(dp[S | (1 << j)][j], dp[S][i] + dist);
      }
    }
  }

  cout << fixed << setprecision(10) << dp[(1 << N) - 1][0] << endl;

  return 0;
}
