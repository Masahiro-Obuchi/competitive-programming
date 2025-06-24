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
  ll K;
  cin >> N >> K;
  vector<ll> A(N), B(N), C(N), D(N);
  for (auto &it : A) {
    cin >> it;
  }
  for (auto &it : B) {
    cin >> it;
  }
  for (auto &it : C) {
    cin >> it;
  }
  for (auto &it : D) {
    cin >> it;
  }

  vector<ll> AB, CD;
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      AB.push_back(A[i] + B[j]);
      CD.push_back(C[i] + D[j]);
    }
  }

  sort(AB.begin(), AB.end());
  sort(CD.begin(), CD.end());

  bool found = false;
  for (int i = 0; i < N * N; i++) {
    ll target = K - AB[i];
    // 二分探索でCDの中にtargetが存在するか確認
    auto it = lower_bound(CD.begin(), CD.end(), target);
    if (it != CD.end() && *it == target) {
      found = true;
      break;
    }
  }

  cout << (found ? "Yes" : "No") << endl;
  return 0;
}
