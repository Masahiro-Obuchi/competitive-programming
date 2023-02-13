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
  vector<int> A(N);
  for (auto &it : A)
    cin >> it;

  vector<int> leftMax(N + 1), rightMax(N + 1);
  for (int i = 0; i < N; i++) {
    leftMax[i + 1] = max(leftMax[i], A[i]);
  }

  for (int i = N; i > 0; i--) {
    rightMax[i - 1] = max(rightMax[i], A[i]);
  }

  int D;
  cin >> D;
  for (int i = 0; i < D; i++) {
    int L, R;
    cin >> L >> R;
    L--, R--;
    cout << max(leftMax[L], rightMax[R]) << endl;
  }
  return 0;
}
