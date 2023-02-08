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

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);
  int N;
  cin >> N;
  vector<int> A(N);
  for (auto &it : A)
    cin >> it;
  vector<int> sum(N + 1);
  for (int i = 0; i < N; i++)
    sum[i + 1] = sum[i] + A[i];

  int Q;
  cin >> Q;
  for (int i = 0; i < Q; i++) {
    int L, R;
    cin >> L >> R;
    int totalNum = R - L + 1;
    int winNum = sum[R] - sum[L - 1], loseNum = totalNum - winNum;
    if (winNum > loseNum) {
      cout << "win" << endl;
    } else if (winNum == loseNum) {
      cout << "draw" << endl;
    } else {
      cout << "lose" << endl;
    }
  }
  return 0;
}
