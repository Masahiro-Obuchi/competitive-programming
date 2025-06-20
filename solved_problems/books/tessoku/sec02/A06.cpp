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

template <typename T> struct AccumulatedSum {
public:
  AccumulatedSum(const vector<T> &data) {
    sum.resize(data.size() + 1);
    for (size_t i = 0; i < data.size(); ++i) {
      sum[i + 1] = sum[i] + data[i];
    }
  }

  // 区間[left, right)の和を取得
  T query(int left, int right) const {
    if (left < 0 || right < 0 || left >= right || right > static_cast<int>(sum.size()) - 1) {
      return T{}; // デフォルト値を返す
    }
    return sum[right] - sum[left];
  }

  // 単一要素の値を取得（元の配列の値）
  T get(int index) const { return query(index, index + 1); }

  // サイズを取得
  size_t size() const { return sum.size() - 1; }

private:
  vector<T> sum;
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N, Q;
  cin >> N >> Q;
  vector<ll> A(N);
  for (auto &it : A)
    cin >> it;

  AccumulatedSum<ll> acc(A);
  for (int i = 0; i < Q; i++) {
    int L, R;
    cin >> L >> R;
    cout << acc.query(L - 1, R) << endl; // 0-indexedに変換
  }

  return 0;
}
