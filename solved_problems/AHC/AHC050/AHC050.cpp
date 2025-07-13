#include <bits/stdc++.h>

#define rep(i, n) for (int i = 0; i < (n); i++)
using namespace std;
using ll = long long;
using P = pair<int, int>;

constexpr static int dx[4] = {0, 1, 0, -1}, dy[4] = {1, 0, -1, 0}; // 右、下、左、上

class ProbabilityBasedSimulator {
private:
  int N;
  vector<vector<bool>> grid;
  vector<P> empty_cells;
  vector<double> initial_neighbor_prob;

public:
  ProbabilityBasedSimulator(int n, const vector<string> &initial_grid) : N(n) {
    initialize_grid(initial_grid);
    precompute_neighbor_probabilities();
  }

  vector<P> solve() { return greedy_strategy(); }

private:
  void initialize_grid(const vector<string> &initial_grid) {
    grid.resize(N, vector<bool>(N, false));

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (initial_grid[i][j] == '#') {
          grid[i][j] = true;
        } else {
          empty_cells.push_back({i, j});
        }
      }
    }
  }

  void precompute_neighbor_probabilities() {
    int E = empty_cells.size();

    // 初期確率分布（均等）
    vector<vector<double>> init_prob(N, vector<double>(N, 0.0));
    for (auto &c : empty_cells) {
      init_prob[c.first][c.second] = 1.0 / E;
    }

    // 1ターン後の分布
    auto next_prob = calculate_next_probability(init_prob);

    // 各セルの周辺確率を事前計算
    initial_neighbor_prob.assign(E, 0.0);
    for (int i = 0; i < E; i++) {
      initial_neighbor_prob[i] = calculate_advanced_concentration_effect(empty_cells[i], next_prob);
    }
  }

  double calculate_neighbor_probability(const P &pos, const vector<vector<double>> &prob) {
    const int radius = 4;
    double sum = 0.0;
    int count = 0;

    // 基本的な周辺確率計算
    for (int di = -radius; di <= radius; di++) {
      for (int dj = -radius; dj <= radius; dj++) {
        if (di == 0 && dj == 0)
          continue;

        int nx = pos.first + di;
        int ny = pos.second + dj;

        if (nx < 0 || nx >= N || ny < 0 || ny >= N)
          continue;
        if (has_rock(nx, ny))
          continue;

        sum += prob[nx][ny];
        count++;
      }
    }

    double base_neighbor_prob = count > 0 ? sum / count : 0.0;

    // 岩密度による集中効果
    double rock_density_bonus = calculate_rock_density_bonus(pos, radius);

    // 方向別制限効果
    double directional_bonus = calculate_directional_restriction_bonus(pos);

    return base_neighbor_prob * (1.0 + rock_density_bonus + directional_bonus);
  }

  // 岩密度による集中効果を計算
  double calculate_rock_density_bonus(const P &pos, int radius) {
    int rock_count = 0;
    int total_cells = 0;

    for (int di = -radius; di <= radius; di++) {
      for (int dj = -radius; dj <= radius; dj++) {
        if (di == 0 && dj == 0)
          continue;

        int nx = pos.first + di;
        int ny = pos.second + dj;

        if (nx < 0 || nx >= N || ny < 0 || ny >= N)
          continue;

        total_cells++;
        if (has_rock(nx, ny)) {
          rock_count++;
        }
      }
    }

    if (total_cells == 0)
      return 0.0;

    double rock_density = double(rock_count) / total_cells;

    // 岩密度が高いほど集中効果が高い（最大50%のボーナス）
    return rock_density * 0.5;
  }

  // 方向別の移動制限効果を計算
  double calculate_directional_restriction_bonus(const P &pos) {
    double restriction_bonus = 0.0;

    // 4方向それぞれの制限度を計算
    for (int dir = 0; dir < 4; dir++) {
      int blocked_distance = 0;
      int x = pos.first + dx[dir];
      int y = pos.second + dy[dir];

      // この方向にどれだけ近くで岩にぶつかるかを計算
      while (x >= 0 && x < N && y >= 0 && y < N && !has_rock(x, y) && blocked_distance < 10) {
        blocked_distance++;
        x += dx[dir];
        y += dy[dir];
      }

      // 近くで制限される方向が多いほど集中効果が高い
      if (blocked_distance <= 3) {
        restriction_bonus += 0.1; // 各方向最大10%のボーナス
      } else if (blocked_distance <= 6) {
        restriction_bonus += 0.05;
      }
    }

    return restriction_bonus;
  }

  // さらに高度な版：岩の配置パターンを考慮
  double calculate_advanced_concentration_effect(const P &pos, const vector<vector<double>> &prob) {
    const int radius = 4;
    double sum = 0.0;
    int count = 0;

    // 基本的な周辺確率計算
    for (int di = -radius; di <= radius; di++) {
      for (int dj = -radius; dj <= radius; dj++) {
        if (di == 0 && dj == 0)
          continue;

        int nx = pos.first + di;
        int ny = pos.second + dj;

        if (nx < 0 || nx >= N || ny < 0 || ny >= N)
          continue;
        if (has_rock(nx, ny))
          continue;

        sum += prob[nx][ny];
        count++;
      }
    }

    double base_neighbor_prob = count > 0 ? sum / count : 0.0;

    // 複数の集中効果を組み合わせ
    double rock_density_bonus = calculate_rock_density_bonus(pos, radius);
    double directional_bonus = calculate_directional_restriction_bonus(pos);
    double corner_effect = calculate_corner_effect(pos);
    double corridor_effect = calculate_corridor_effect(pos);

    double total_bonus = rock_density_bonus + directional_bonus + corner_effect + corridor_effect;

    return base_neighbor_prob * (/*1.0*/ +total_bonus);
  }

  // 角・辺に近いことによる集中効果
  double calculate_corner_effect(const P &pos) {
    int dist_to_edge = min({pos.first, pos.second, N - 1 - pos.first, N - 1 - pos.second});

    if (dist_to_edge <= 2) {
      return (3 - dist_to_edge) * 0.1; // 角・辺近くで最大30%のボーナス
    }

    return 0.0;
  }

  // 通路効果（岩で囲まれた狭い通路）
  double calculate_corridor_effect(const P &pos) {
    double corridor_bonus = 0.0;

    // 水平方向の通路チェック
    bool left_blocked = false, right_blocked = false;
    for (int i = 1; i <= 5; i++) {
      if (pos.second - i < 0 || has_rock(pos.first, pos.second - i)) {
        left_blocked = true;
        break;
      }
    }
    for (int i = 1; i <= 5; i++) {
      if (pos.second + i >= N || has_rock(pos.first, pos.second + i)) {
        right_blocked = true;
        break;
      }
    }

    if (left_blocked && right_blocked) {
      corridor_bonus += 0.2; // 水平通路効果
    }

    // 垂直方向の通路チェック
    bool up_blocked = false, down_blocked = false;
    for (int i = 1; i <= 5; i++) {
      if (pos.first - i < 0 || has_rock(pos.first - i, pos.second)) {
        up_blocked = true;
        break;
      }
    }
    for (int i = 1; i <= 5; i++) {
      if (pos.first + i >= N || has_rock(pos.first + i, pos.second)) {
        down_blocked = true;
        break;
      }
    }

    if (up_blocked && down_blocked) {
      corridor_bonus += 0.2; // 垂直通路効果
    }

    return corridor_bonus;
  }

  vector<P> greedy_strategy() {
    int E = empty_cells.size();
    vector<P> result;
    result.reserve(E);
    vector<bool> used(E, false);

    // 動的確率分布
    vector<vector<double>> prob(N, vector<double>(N, 0.0));
    for (auto &c : empty_cells) {
      prob[c.first][c.second] = 1.0 / E;
    }

    ProbabilityBasedSimulator sim = *this;

    for (int turn = 0; turn < E; turn++) {
      int best_idx = select_best_candidate(prob, used, turn);

      auto [rx, ry] = empty_cells[best_idx];
      result.emplace_back(rx, ry);
      used[best_idx] = true;

      // 状態更新
      sim.place_rock(rx, ry);
      prob = sim.calculate_next_probability(prob);
      prob[rx][ry] = 0.0;
      normalize_probability(prob);
    }

    return result;
  }

  int select_best_candidate(const vector<vector<double>> &prob, const vector<bool> &used,
                            int turn) {
    int E = empty_cells.size();
    double progress = double(turn) / E;
    int max_candidates = (progress < 0.5) ? 30 : min(50, E);

    vector<pair<double, int>> candidates;
    candidates.reserve(E);

    for (int i = 0; i < E; i++) {
      if (used[i])
        continue;

      double score = calculate_score(prob, i, progress);
      candidates.emplace_back(score, i);
    }

    // 上位候補を選択
    sort(candidates.begin(), candidates.end(), greater<>());
    if ((int)candidates.size() > max_candidates) {
      candidates.resize(max_candidates);
    }

    return candidates[0].second;
  }

  double calculate_score(const vector<vector<double>> &prob, int cell_idx, double progress) {
    auto [x, y] = empty_cells[cell_idx];
    double robot_prob = prob[x][y];

    // 基本スコア：存在確率が低いほど高い
    double base_score = 1.0 - robot_prob;

    // 移動制限ボーナス：事前計算した周辺確率を使用
    double restriction_bonus = initial_neighbor_prob[cell_idx] * progress * 0.5;

    return base_score + restriction_bonus;
  }

  void normalize_probability(vector<vector<double>> &prob) {
    double sum = 0.0;
    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        sum += prob[i][j];
      }
    }

    if (sum > 1e-12) {
      for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
          prob[i][j] /= sum;
        }
      }
    }
  }

  // ユーティリティ関数
  bool has_rock(int x, int y) const {
    if (x < 0 || x >= N || y < 0 || y >= N)
      return true;
    return grid[x][y];
  }

  void place_rock(int x, int y) { grid[x][y] = true; }

  P simulate_move(int robot_x, int robot_y, int direction) const {
    int x = robot_x, y = robot_y;

    while (true) {
      int nx = x + dx[direction];
      int ny = y + dy[direction];

      if (has_rock(nx, ny))
        break;

      x = nx;
      y = ny;
    }

    return {x, y};
  }

  vector<vector<double>>
  calculate_next_probability(const vector<vector<double>> &current_prob) const {
    vector<vector<double>> next_prob(N, vector<double>(N, 0.0));

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (current_prob[i][j] > 0 && !has_rock(i, j)) {
          for (int dir = 0; dir < 4; dir++) {
            P new_pos = simulate_move(i, j, dir);
            next_prob[new_pos.first][new_pos.second] += current_prob[i][j] * 0.25;
          }
        }
      }
    }

    return next_prob;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N, M;
  cin >> N >> M;

  vector<string> S(N);
  for (int i = 0; i < N; i++) {
    cin >> S[i];
  }

  ProbabilityBasedSimulator simulator(N, S);
  vector<P> rock_sequence = simulator.solve();

  for (const auto &pos : rock_sequence) {
    cout << pos.first << " " << pos.second << "\n";
  }

  return 0;
}
