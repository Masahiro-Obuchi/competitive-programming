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
  vector<pair<int, int>> XY(N);
  for (int i = 0; i < N; i++) {
    cin >> XY[i].first >> XY[i].second;
    XY[i].second = -XY[i].second; // Y座標を反転
  }
  sort(XY.begin(), XY.end());

  // dp[i]: 最後の要素がXY[i]である部分列のうち、最長のものの長さ
  vector<int> dp(N, 1);
  // L[x]: 長さxの部分列の最後の要素として考えられる最小値(Y)
  vector<int> L(N + 1, INT_INF);

  int length = 0;
  for (int i = 0; i < N; i++) {
    int pos = lower_bound(L.begin() + 1, L.begin() + 1 + length, -XY[i].second) - L.begin();
    dp[i] = pos;

    L[dp[i]] = min(L[dp[i]], -XY[i].second);
    if (dp[i] > length) {
      length = dp[i];
    }
  }

  cout << length << endl;
  return 0;
}
