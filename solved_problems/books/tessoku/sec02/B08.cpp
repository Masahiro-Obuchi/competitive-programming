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
  int N;
  cin >> N;
  vector<vector<int>> field(1510, vector<int>(1510));
  for (int i = 0; i < N; i++) {
    int X, Y;
    cin >> X >> Y;
    field[X][Y]++;
  }

  vector<vector<int>> sum(1510, vector<int>(1510));
  for (int i = 1; i <= 1500; i++) {
    for (int j = 1; j <= 1500; j++) {
      sum[i][j] = sum[i][j - 1] + sum[i - 1][j] - sum[i - 1][j - 1] + field[i][j];
    }
  }

  int Q;
  cin >> Q;
  for (int i = 0; i < Q; i++) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    a--, b--;
    cout << sum[c][d] - sum[c][b] - sum[a][d] + sum[a][b] << endl;
  }
  return 0;
}
