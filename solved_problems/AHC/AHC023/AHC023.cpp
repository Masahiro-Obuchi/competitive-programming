// #include <Eigen/Dense>
#include <bits/stdc++.h>
#include <boost/multi_array.hpp>
#include <boost/optional.hpp>
#include <boost/range/adaptors.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/range/irange.hpp>

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

constexpr static int dx[4] = {0, 0, -1, 0}, dy[4] = {0, 1, 0, -1}; // 右、下、左、上
// int dx[8] = {-1, -1, -1, 0, 0, 1, 1, 1}, dy[8] = {-1, 0, 1, 1, -1, 1, 0, -1};

uint64_t get_rand_range(uint64_t min_val, uint64_t max_val) {
  // 乱数生成器
  static std::mt19937_64 mt64(0);

  // [min_val, max_val] の一様分布整数 (int) の分布生成器
  std::uniform_int_distribution<uint64_t> get_rand_uni_int(min_val, max_val);

  // 乱数を生成
  return get_rand_uni_int(mt64);
}

double get_rand_range_real(double min_val, double max_val) {
  // 乱数生成器
  static std::mt19937_64 mt64(0);

  // [min_val, max_val] の一様分布整数 (int) の分布生成器
  std::uniform_real_distribution<double> get_rand_real(min_val, max_val);

  // 乱数を生成
  return get_rand_real(mt64);
}

struct InputData {
  int T, H, W, i0;
  vector<vector<bool>> h, v;
  int K;
  vector<int> S, D;

  InputData() {
    cin >> T >> H >> W >> i0;
    h.resize(H - 1, vector<bool>(W, false));
    for (int i = 0; i < H - 1; i++) {
      string s;
      cin >> s;
      for (int j = 0; j < W; j++)
        if (s[j] == '1')
          h[i][j] = true;
    }
    v.resize(H, vector<bool>(W - 1, false));
    for (int i = 0; i < H; i++) {
      string s;
      cin >> s;
      for (int j = 0; j < W - 1; j++)
        if (s[j] == '1')
          v[i][j] = true;
    }
    cin >> K;
    S.resize(K);
    D.resize(K);
    for (int i = 0; i < K; i++)
      cin >> S[i] >> D[i];
  }
};

struct Work {
  int k, i, j, s;
};

struct Solver {
  InputData input;
  vector<Work> plan;
  vector<vector<vector<pair<int, int>>>> adj;

  Solver() : input(), plan(), adj(input.H, vector<vector<pair<int, int>>>(input.W)){};

  void constructGraph(vector<vector<vector<pair<int, int>>>> &adj);
  bool reachable(int i, int j, const vector<vector<vector<pair<int, int>>>> &adj,
                 const vector<vector<bool>> &used);
  bool isValidPlan(const vector<Work> &plan, const vector<vector<vector<pair<int, int>>>> &adj);
  void solve();

  void output();
};

void Solver::constructGraph(vector<vector<vector<pair<int, int>>>> &adj) {
  for (int i = 0; i < input.H; i++) {
    for (int j = 0; j < input.W; j++) {
      if (i + 1 < input.H && !input.h[i][j]) {
        adj[i][j].emplace_back(i + 1, j);
        adj[i + 1][j].emplace_back(i, j);
      }
      if (j + 1 < input.W && !input.v[i][j]) {
        adj[i][j].emplace_back(i, j + 1);
        adj[i][j + 1].emplace_back(i, j);
      }
    }
  }
}

bool Solver::reachable(int i, int j, const vector<vector<vector<pair<int, int>>>> &adj,
                       const vector<vector<bool>> &used) {
  if (used[i][j] || used[input.i0][0]) {
    return false;
  } else if (i == input.i0 && j == 0) {
    return true;
  }
  queue<pair<int, int>> q;
  q.emplace(input.i0, 0);
  vector<vector<bool>> visited(input.H, vector<bool>(input.W, false));
  visited[input.i0][0] = true;
  while (!q.empty()) {
    const auto [i1, j1] = q.front();
    q.pop();
    for (const auto &[i2, j2] : adj[i1][j1]) {
      if (i2 == i && j2 == j) {
        return true;
      } else if (!used[i2][j2] && !visited[i2][j2]) {
        visited[i2][j2] = true;
        q.emplace(i2, j2);
      }
    }
  }

  return false;
}

bool Solver::isValidPlan(const vector<Work> &plan,
                         const vector<vector<vector<pair<int, int>>>> &adj) {
  vector<vector<Work>> plant_list(input.T + 1), harvest_list(input.T + 1);
  for (const Work &w : plan) {
    plant_list[w.s].push_back(w);
    harvest_list[input.D[w.k]].push_back(w);
  }

  vector<vector<bool>> used(input.H, vector<bool>(input.W, false));
  for (int t = 1; t <= input.T; t++) {
    // plaing phase
    for (const Work &w : plant_list[t]) {
      if (!reachable(w.i, w.j, adj, used)) {
        return false;
      }
    }
    for (const Work &w : plant_list[t]) {
      if (used[w.i][w.j]) {
        return false;
      } else {
        used[w.i][w.j] = true;
      }
    }

    // havesting phase
    for (const Work &w : harvest_list[t]) {
      used[w.i][w.j] = false;
    }
    for (const Work &w : harvest_list[t]) {
      if (!reachable(w.i, w.j, adj, used)) {
        return false;
      }
    }
  }

  return true;
}

void Solver::output() {
  cout << plan.size() << endl;
  for (const auto &w : plan) {
    cout << w.k + 1 << " " << w.i << " " << w.j << " " << w.s << endl;
  }
}

void Solver::solve() {
  constructGraph(adj);

  for (int k = 0; k < min(input.K, 100); k++) {
    // try to plant crop k
    bool found = false;
    for (int i = 0; i < input.H && !found; i++) {
      for (int j = 0; j < input.W && !found; j++) {
        plan.push_back({k, i, j, input.S[k]});
        if (!isValidPlan(plan, adj)) {
          plan.pop_back();
        } else {
          found = true;
        }
      }
    }
  }

  output();
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  Solver s;
  s.solve();

  return 0;
}
