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

template <typename T> struct Imos {
public:
  Imos(size_t size) : imos(size + 1, 0), built(false) {}

  void add(int left, int right, T value) {
    if (left < 0 || right < 0 || left >= static_cast<int>(imos.size()) - 1) {
      return; // 範囲外は無視
    }
    imos[left] += value;
    if (right < static_cast<int>(imos.size()) - 1) {
      imos[right] -= value;
    }
    built = false; // 再構築が必要
  }

  vector<T> build() {
    if (!built) {
      // 累積和を計算
      for (size_t i = 0; i < imos.size() - 1; i++) {
        imos[i + 1] += imos[i];
      }
      built = true;
    }
    // 番兵を除いて返す
    return vector<T>(imos.begin(), imos.end() - 1);
  }

private:
  vector<T> imos;
  bool built; // 構築済みフラグ
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int T, N;
  cin >> T >> N;
  Imos<int> imos(T);
  for (int i = 0; i < N; i++) {
    int L, R;
    cin >> L >> R;
    imos.add(L, R, 1);
  }
  auto result = imos.build();
  for (int i = 0; i < T; i++) {
    cout << result[i] << endl;
  }
  return 0;
}
