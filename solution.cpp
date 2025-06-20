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
  vector<int> A(N);
  for (auto &it : A) {
    cin >> it;
  }

  vector<int> left_max(N + 1), right_max(N + 1);
  for (int i = 0; i < N; i++) {
    left_max[i + 1] = max(left_max[i], A[i]);
  }
  for (int i = N - 1; i >= 0; i--) {
    right_max[i] = max(right_max[i + 1], A[i]);
  }

  int D;
  cin >> D;
  for (int i = 0; i < D; i++) {
    int L, R;
    cin >> L >> R;
    L--;
    auto ans = max(left_max[L], right_max[R]);
    cout << ans << endl;
  }

  return 0;
}
