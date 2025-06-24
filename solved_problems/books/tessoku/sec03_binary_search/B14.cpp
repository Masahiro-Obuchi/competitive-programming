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
  int N, K;
  cin >> N >> K;
  vector<ll> A(N);
  for (auto &it : A) {
    cin >> it;
  }

  vector<ll> first_half, second_half;
  int mid = N / 2;
  for (int bit = 0; bit < (1 << mid); bit++) {
    ll sum = 0;
    for (int i = 0; i < mid; i++) {
      if (bit & (1 << i)) {
        sum += A[i];
      }
    }
    first_half.push_back(sum);
  }

  for (int bit = 0; bit < (1 << (N - mid)); bit++) {
    ll sum = 0;
    for (int i = 0; i < N - mid; i++) {
      if (bit & (1 << i)) {
        sum += A[mid + i];
      }
    }
    second_half.push_back(sum);
  }

  sort(second_half.begin(), second_half.end());

  bool found = false;
  for (const ll &x : first_half) {
    ll target = K - x;
    if (binary_search(second_half.begin(), second_half.end(), target)) {
      found = true;
      break;
    }
  }

  cout << (found ? "Yes" : "No") << endl;

  return 0;
}
