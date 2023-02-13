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

bool check(vector<ll> &A, ll x, ll K, int N) {
  ll totalOutput = 0;
  for (int i = 0; i < N; i++) {
    totalOutput += x / A[i];
  }
  return totalOutput >= K;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  ll N, K;
  cin >> N >> K;
  vector<ll> A(N);
  for (auto &it : A)
    cin >> it;

  ll left = 0, right = INT_INF;
  while (left + 1 < right) {
    ll mid = (left + right) / 2;
    if (check(A, mid, K, N))
      right = mid;
    else
      left = mid;
  }
  cout << right << endl;
  return 0;
}
