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
  vector<int> A(N - 1), B(N - 2);
  for (auto &it : A) {
    cin >> it;
  }
  for (auto &it : B) {
    cin >> it;
  }

  vector<ll> dp(N, INT_INF);
  dp[0] = 0;

  for (int i = 0; i < N - 1; i++) {
    if (dp[i] == INT_INF) {
      continue;
    }
    dp[i + 1] = min(dp[i + 1], dp[i] + A[i]);
    if (i < N - 2) {
      dp[i + 2] = min(dp[i + 2], dp[i] + B[i]);
    }
  }

  cout << dp[N - 1] << endl;
  return 0;
}
