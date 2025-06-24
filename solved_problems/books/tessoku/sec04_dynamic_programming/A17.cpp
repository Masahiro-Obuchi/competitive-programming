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
  int N;
  cin >> N;
  vector<int> A(N - 1);
  for (auto &it : A) {
    cin >> it;
  }
  vector<int> B(N - 2);
  for (auto &it : B) {
    cin >> it;
  }

  vector<ll> dp(N, 0);
  dp[0] = 0;
  dp[1] = A[0];
  for (int i = 2; i < N; i++) {
    dp[i] = min(dp[i - 1] + A[i - 1], dp[i - 2] + B[i - 2]);
  }

  int now = N - 1;
  vector<int> ans;
  while (true) {
    ans.push_back(now);
    if (now == 0) {
      break;
    }
    if (dp[now] == dp[now - 1] + A[now - 1]) {
      now--;
    } else {
      now -= 2;
    }
  }

  cout << ans.size() << endl;
  for (int i = ans.size() - 1; i >= 0; i--) {
    cout << ans[i] + 1 << " ";
  }
  cout << endl;
  return 0;
}
