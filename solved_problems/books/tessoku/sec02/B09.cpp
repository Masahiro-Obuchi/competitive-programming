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
  vector<vector<int>> plain(1510, vector<int>(1510));
  for (int i = 0; i < N; i++) {
    int A, B, C, D;
    cin >> A >> B >> C >> D;
    plain[A][B]++;
    plain[C][D]++;
    plain[A][D]--;
    plain[C][B]--;
  }

  for (int i = 0; i <= 1500; i++) {
    for (int j = 0; j <= 1500; j++) {
      plain[i][j + 1] += plain[i][j];
    }
  }
  for (int j = 0; j <= 1500; j++) {
    for (int i = 0; i <= 1500; i++) {
      plain[i + 1][j] += plain[i][j];
    }
  }

  int ans = 0;
  for (int i = 0; i <= 1500; i++) {
    for (int j = 0; j <= 1500; j++) {
      if (plain[i][j])
        ans++;
    }
  }

  cout << ans << endl;
  return 0;
}
