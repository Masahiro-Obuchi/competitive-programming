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

template <typename T> struct Imos2D {
public:
  Imos2D(size_t rows, size_t cols) : imos(rows + 1, vector<T>(cols + 1, T{})), built(false) {}

  void add(int r1, int c1, int r2, int c2, T value) {
    if (built) {
      throw runtime_error("Cannot add after build() has been called");
    }
    if (r1 < 0 || c1 < 0 || r2 <= r1 || c2 <= c1 || r2 >= static_cast<int>(imos.size()) ||
        c2 >= static_cast<int>(imos[0].size())) {
      return;
    }

    imos[r1][c1] += value;
    imos[r1][c2] -= value;
    imos[r2][c1] -= value;
    imos[r2][c2] += value;
  }

  vector<vector<T>> build() {
    if (built) {
      throw runtime_error("build() can only be called once");
    }
    for (size_t i = 0; i < imos.size(); i++) {
      for (size_t j = 1; j < imos[0].size(); j++) {
        imos[i][j] += imos[i][j - 1];
      }
    }
    for (size_t i = 1; i < imos.size(); i++) {
      for (size_t j = 0; j < imos[0].size(); j++) {
        imos[i][j] += imos[i - 1][j];
      }
    }
    built = true;

    // 番兵を除いて返す（各行と各列から最後の要素を除く）
    vector<vector<T>> result(imos.size() - 1);
    for (size_t i = 0; i < imos.size() - 1; i++) {
      result[i] = vector<T>(imos[i].begin(), imos[i].end() - 1);
    }
    return result;
  }

  // サイズ取得メソッドを追加
  size_t rows() const { return imos.size() - 1; }
  size_t cols() const { return imos.empty() ? 0 : imos[0].size() - 1; }

private:
  vector<vector<T>> imos;
  bool built; // 構築済みフラグ
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  Imos2D<int> imos(1510, 1510);
  for (int i = 0; i < N; i++) {
    int A, B, C, D;
    cin >> A >> B >> C >> D;
    imos.add(A, B, C, D, 1);
  }

  auto result = imos.build();

  int ans = 0;
  for (int i = 0; i <= 1500; i++) {
    for (int j = 0; j <= 1500; j++) {
      if (result[i][j] > 0) {
        ans++;
      }
    }
  }

  cout << ans << endl;
  return 0;
}
