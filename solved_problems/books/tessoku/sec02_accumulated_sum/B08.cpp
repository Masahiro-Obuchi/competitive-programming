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

template <typename T> struct AccumulatedSum2D {
public:
  AccumulatedSum2D(const vector<vector<T>> &data) {
    int row_size = data.size();
    int col_size = data[0].size();

    sum.resize(row_size + 1, vector<T>(col_size + 1, T{}));
    for (int i = 0; i < row_size; ++i) {
      for (int j = 0; j < col_size; ++j) {
        sum[i + 1][j + 1] = data[i][j] + sum[i][j + 1] + sum[i + 1][j] - sum[i][j];
      }
    }
  }

  // 区間[(r1, c1), (r2, c2))の和を取得（右下は含まない）
  T query(int r1, int c1, int r2, int c2) const {
    if (r1 < 0 || c1 < 0 || r2 <= r1 || c2 <= c1 || r2 > static_cast<int>(sum.size()) - 1 ||
        c2 > static_cast<int>(sum[0].size()) - 1) {
      return T{}; // デフォルト値を返す
    }
    return sum[r2][c2] - sum[r1][c2] - sum[r2][c1] + sum[r1][c1];
  }

  // 単一要素の値を取得（元の配列の値）
  T get(int r, int c) const {
    if (r < 0 || c < 0 || r >= static_cast<int>(sum.size()) - 1 ||
        c >= static_cast<int>(sum[0].size()) - 1) {
      return T{}; // デフォルト値を返す
    }
    return query(r, c, r + 1, c + 1);
  }

  // サイズを取得
  size_t rows() const { return sum.size() - 1; }
  size_t cols() const { return sum.empty() ? 0 : sum[0].size() - 1; }

  const vector<vector<T>> &get_sum_array() const { return sum; }

private:
  vector<vector<T>> sum; // 累積和の2D配列
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<vector<int>> field(1510, vector<int>(1510));
  for (int i = 0; i < N; i++) {
    int X, Y;
    cin >> X >> Y;
    X--, Y--;
    field[X][Y]++;
  }

  AccumulatedSum2D<int> acc(field);

  int Q;
  cin >> Q;
  for (int i = 0; i < Q; i++) {
    int a, b, c, d;
    cin >> a >> b >> c >> d;
    a--, b--;
    cout << acc.query(a, b, c, d) << endl;
  }
  return 0;
}
