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

template <typename T> vector<T> Eratosthenes(const T N) {
  vector<bool> is_prime(N + 1);
  for (int i = 2; i <= N; i++) {
    is_prime[i] = true;
  }
  vector<T> P;
  for (int i = 2; i <= N; i++) {
    if (is_prime[i]) {
      for (int j = 2 * i; j <= N; j += i) {
        is_prime[j] = false;
      }
      P.emplace_back(i);
    }
  }

  return P;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;
  auto res = Eratosthenes(N);
  for (auto p : res) {
    cout << p << endl;
  }

  return 0;
}
