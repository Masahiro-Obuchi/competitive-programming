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
  int N, S;
  cin >> N >> S;
  vector<int> A(N);
  for (auto &it : A) {
    cin >> it;
  }
  vector<vector<bool>> dp(N + 1, vector<bool>(S + 1, false));
  dp[0][0] = true;
  for (int i = 1; i <= N; i++) {
    for (int j = 0; j <= S; j++) {
      dp[i][j] = dp[i - 1][j];
      if (j >= A[i - 1]) {
        dp[i][j] = dp[i][j] || dp[i - 1][j - A[i - 1]];
      }
    }
  }

  if (!dp[N][S]) {
    cout << -1 << endl;
    return 0;
  }

  int nowSum = S;
  vector<int> ans;
  for (int i = N; i >= 1; i--) {
    if (dp[i - 1][nowSum] == true) {
      continue;
    } else {
      nowSum -= A[i - 1];
      ans.push_back(i - 1);
    }
  }

  reverse(ans.begin(), ans.end());
  cout << ans.size() << endl;
  for (auto it : ans) {
    cout << it + 1 << " ";
  }
  cout << endl;
  return 0;
}
