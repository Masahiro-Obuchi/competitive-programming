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

template <typename T, typename R>
long long syakutori(const vector<T> &vec, const function<bool(const R &)> &condition,
                    const function<R(const R &, const T &)> &op,
                    const function<R(const R &, const T &)> &inverse_op, const R &identity) {

  int n = vec.size();
  int right = 0;
  long long ans = 0;
  R result = identity;

  for (int left = 0; left < n; left++) {
    // rightを可能な限り右に伸ばす
    while (right < n) {
      R new_result = op(result, vec[right]);
      if (!condition(new_result)) {
        break;
      }
      result = new_result;
      right++;
    }

    // 区間[left, right)から始まる全ての部分区間の個数を加算
    ans += right - left;

    // leftを一つ右に進める準備
    if (right == left) {
      // 空区間の場合：rightも一緒に進める
      right++;
    } else {
      // 非空区間の場合：左端の要素を除去
      result = inverse_op(result, vec[left]);
    }
  }

  return ans;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  ll K;
  cin >> N >> K;
  vector<ll> A(N);
  for (auto &it : A) {
    cin >> it;
  }

  auto ans = syakutori<ll, ll>(
      A, [K](const ll &sum) { return sum <= K; },
      [](const ll &sum, const ll &x) { return sum + x; },
      [](const ll &sum, const ll &x) { return sum - x; }, 0LL);

  cout << ans << endl;

  return 0;
}
