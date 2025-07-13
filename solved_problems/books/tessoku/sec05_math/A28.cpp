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

// ModInt ライブラリ
template <int MOD> struct ModInt {
private:
  long long value;

  static constexpr long long umod() { return MOD; }

public:
  // コンストラクタ
  ModInt() : value(0) {}

  template <class T> ModInt(T v) { value = normalize(v); }

  template <class T> static long long normalize(T v) {
    v %= umod();
    if (v < 0)
      v += umod();
    return v;
  }

  long long val() const { return value; }

  ModInt &operator+=(const ModInt &rhs) {
    value += rhs.value;
    if (value >= umod()) {
      value -= umod();
    }
    return *this;
  }

  ModInt &operator-=(const ModInt &rhs) {
    value -= rhs.value;
    if (value < 0) {
      value += umod();
    }
    return *this;
  }

  ModInt &operator*=(const ModInt &rhs) {
    value = (value * rhs.value) % umod();
    return *this;
  }

  ModInt &operator/=(const ModInt &rhs) { return *this *= rhs.inv(); }

  ModInt operator+() const { return *this; }
  ModInt operator-() const { return ModInt() - *this; }

  // 二項演算子
  ModInt operator+(const ModInt &rhs) const { return ModInt(*this) += rhs; }
  ModInt operator-(const ModInt &rhs) const { return ModInt(*this) -= rhs; }
  ModInt operator*(const ModInt &rhs) const { return ModInt(*this) *= rhs; }
  ModInt operator/(const ModInt &rhs) const { return ModInt(*this) /= rhs; }

  // 比較演算子
  bool operator==(const ModInt &rhs) const { return value == rhs.value; }
  bool operator!=(const ModInt &rhs) const { return value != rhs.value; }
  bool operator<(const ModInt &rhs) const { return value < rhs.value; }

  // 累乗
  ModInt pow(long long exp) const {
    ModInt result(1);
    ModInt base(*this);
    while (exp > 0) {
      if (exp & 1)
        result *= base;
      base *= base;
      exp >>= 1;
    }
    return result;
  }

  // 逆元（フェルマーの小定理を使用、MODが素数の場合）
  ModInt inv() const { return pow(umod() - 2); }

  // 入出力
  friend ostream &operator<<(ostream &os, const ModInt &x) { return os << x.value; }

  friend istream &operator>>(istream &is, ModInt &x) {
    long long v;
    is >> v;
    x = ModInt(v);
    return is;
  }
};

using mint998 = ModInt<998244353>;
using mint1e9 = ModInt<1000000007>;

// 階乗・組み合わせ用の前計算クラス
template <class T> struct Combination {
  vector<T> fact, inv_fact;

  Combination(int n) : fact(n + 1), inv_fact(n + 1) {
    fact[0] = 1;
    for (int i = 1; i <= n; i++) {
      fact[i] = fact[i - 1] * i;
    }
    inv_fact[n] = fact[n].inv();
    for (int i = n; i >= 1; i--) {
      inv_fact[i - 1] = inv_fact[i] * i;
    }
  }

  T C(int n, int r) {
    if (r < 0 || r > n)
      return 0;
    return fact[n] * inv_fact[r] * inv_fact[n - r];
  }

  T P(int n, int r) {
    if (r < 0 || r > n)
      return 0;
    return fact[n] * inv_fact[n - r];
  }

  T H(int n, int r) { return C(n + r - 1, r); }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<char> T(N);
  vector<int> A(N);
  ModInt<10000> ans(0);
  for (int i = 0; i < N; i++) {
    cin >> T[i] >> A[i];
    if (T[i] == '+') {
      ans += A[i];
    } else if (T[i] == '-') {
      ans -= A[i];
    } else if (T[i] == '*') {
      ans *= A[i];
    }
    cout << ans << endl;
  }
  return 0;
}
