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

template <typename T, typename F> T bisect(T ok, T ng, F predicate) {
  // 初期条件チェック
  if (!predicate(ok)) {
    return ok;
  }

  while (abs(ng - ok) > epsilon) {
    T mid = std::midpoint(ok, ng);
    if (predicate(mid)) {
      ok = mid;
    } else {
      ng = mid;
    }
  }

  return ok;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;

  auto ok = bisect(0.0, 10000000.0, [&](double x) { return x * x * x + x - N <= epsilon; });

  cout << setprecision(12) << ok << endl;
  return 0;
}
