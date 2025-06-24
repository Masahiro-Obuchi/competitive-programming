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

int dx[4] = {1, 0, -1, 0}, dy[4] = {0, 1, 0, -1}; // 右、下、左、上
// int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1}, dy[8] = {-1, 0, 1, 1, -1, 1, 0, -1};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int H, W;
  cin >> H >> W;
  vector<vector<int>> X(H, vector<int>(W));
  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      cin >> X[i][j];
    }
  }

  vector<vector<int>> sum(H + 1, vector<int>(W + 1));

  for (int i = 0; i < H; i++) {
    for (int j = 0; j < W; j++) {
      sum[i + 1][j + 1] = sum[i + 1][j] + sum[i][j + 1] - sum[i][j] + X[i][j];
    }
  }

  int Q;
  cin >> Q;
  for (int i = 0; i < Q; i++) {
    int A, B, C, D;
    cin >> A >> B >> C >> D;
    A--, B--;
    cout << sum[C][D] + sum[A][B] - sum[C][B] - sum[A][D] << endl;
  }
  return 0;
}
