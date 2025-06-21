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

// 乱数生成
uint64_t get_rand_range(uint64_t min_val, uint64_t max_val) {
  static mt19937_64 mt64(0);
  uniform_int_distribution<uint64_t> dist(min_val, max_val);
  return dist(mt64);
}

enum class Move { UP, DOWN, LEFT, RIGHT };

struct Action {
  enum Type { PICKUP, PLACE, MOVE };
  Type type;
  Move direction;

  Action(Type t) : type(t) {}
  Action(Type t, Move dir) : type(t), direction(dir) {}
};

struct Cardboard {
  int weight;
  int durability;

  Cardboard() = default;
  Cardboard(int w, int d) : weight(w), durability(d) {}
};

struct Player {
  vector<Cardboard> holding_cardboards;
  int y = 0, x = 0;
};

struct GameState {
  vector<vector<vector<Cardboard>>> field_cardboards;
  string operation_history;
  Player player;
  int move_count = 0;
  int total_operations = 0;
  int remaining_cardboards = 0;

  GameState(int N, const vector<vector<ll>> &weights, const vector<vector<ll>> &durabilities)
      : field_cardboards(N, vector<vector<Cardboard>>(N)), remaining_cardboards(N * N - 1) {

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (i != 0 || j != 0) {
          field_cardboards[i][j].push_back(Cardboard(weights[i][j], durabilities[i][j]));
        }
      }
    }
  }

  bool is_complete() const { return remaining_cardboards == 0 && player.x == 0 && player.y == 0; }

  bool is_game_over() const {
    int N = field_cardboards.size();
    int max_operations = 2 * N * N * N;
    return total_operations >= max_operations || has_broken_cardboards();
  }

  bool has_broken_cardboards() const {
    for (const auto &card : player.holding_cardboards) {
      if (card.durability <= 0)
        return true;
    }
    return false;
  }

  bool advance(const Action &action) {
    if (is_game_over() || is_complete())
      return false;

    bool success = false;
    string op = "";

    switch (action.type) {
    case Action::PICKUP:
      success = pickup_cardboard();
      if (success)
        op = "1";
      break;
    case Action::PLACE:
      success = place_cardboard();
      if (success)
        op = "2";
      break;
    case Action::MOVE:
      success = move_player(action.direction);
      if (success) {
        switch (action.direction) {
        case Move::UP:
          op = "U";
          break;
        case Move::DOWN:
          op = "D";
          break;
        case Move::LEFT:
          op = "L";
          break;
        case Move::RIGHT:
          op = "R";
          break;
        }
      }
      break;
    }

    if (success && !op.empty()) {
      operation_history += op;
    }
    return success;
  }

  void output_operations() const {
    for (char c : operation_history) {
      cout << c << endl;
    }
  }

  GameState copy() const { return *this; }

  int distance_to_origin() const { return abs(player.y) + abs(player.x); }

  bool can_reach_origin_safely() const {
    if (player.holding_cardboards.empty())
      return true;

    int distance = distance_to_origin();
    for (int i = 0; i < player.holding_cardboards.size(); i++) {
      int weight_above = 0;
      for (int j = i + 1; j < player.holding_cardboards.size(); j++) {
        weight_above += player.holding_cardboards[j].weight;
      }
      if (player.holding_cardboards[i].durability - weight_above * distance <= 0) {
        return false;
      }
    }
    return true;
  }

  // 最も耐久値が高く、重い箱を探す関数
  pair<int, int> find_most_durable_box() const {
    int N = field_cardboards.size();
    pair<int, int> best_pos = {-1, -1};
    int best_durability = -1;
    int best_weight = -1;
    int max_distance = -1;

    for (int i = 0; i < N; i++) {
      for (int j = 0; j < N; j++) {
        if (!field_cardboards[i][j].empty()) {
          int durability = field_cardboards[i][j].back().durability;
          int weight = field_cardboards[i][j].back().weight;
          int distance = abs(i - player.y) + abs(j - player.x);

          bool is_better = false;

          // 1. 耐久値がより高い場合
          if (durability > best_durability) {
            is_better = true;
          }
          // 2. 耐久値が同じで重量がより重い場合
          else if (durability == best_durability && weight > best_weight) {
            is_better = true;
          }
          // 3. 耐久値・重量が同じで距離がより遠い場合
          else if (durability == best_durability && weight == best_weight &&
                   distance > max_distance) {
            is_better = true;
          }

          if (is_better) {
            best_durability = durability;
            best_weight = weight;
            max_distance = distance;
            best_pos = {i, j};
          }
        }
      }
    }
    return best_pos;
  }

  vector<Action> get_path_to_origin() const {
    vector<Action> path;
    int cy = player.y, cx = player.x;

    while (cy != 0 || cx != 0) {
      if (cy > 0) {
        path.push_back(Action(Action::MOVE, Move::UP));
        cy--;
      } else if (cy < 0) {
        path.push_back(Action(Action::MOVE, Move::DOWN));
        cy++;
      } else if (cx > 0) {
        path.push_back(Action(Action::MOVE, Move::LEFT));
        cx--;
      } else if (cx < 0) {
        path.push_back(Action(Action::MOVE, Move::RIGHT));
        cx++;
      }
    }
    return path;
  }

  vector<Action> get_path_to(int target_y, int target_x) const {
    vector<Action> path;
    int cy = player.y, cx = player.x;

    // Y方向優先
    while (cy != target_y) {
      if (cy < target_y) {
        path.push_back(Action(Action::MOVE, Move::DOWN));
        cy++;
      } else {
        path.push_back(Action(Action::MOVE, Move::UP));
        cy--;
      }
    }

    // X方向
    while (cx != target_x) {
      if (cx < target_x) {
        path.push_back(Action(Action::MOVE, Move::RIGHT));
        cx++;
      } else {
        path.push_back(Action(Action::MOVE, Move::LEFT));
        cx--;
      }
    }
    return path;
  }

  // 近傍で最も軽い箱を探す関数
  pair<int, int> find_lightest_box_in_range(int center_y, int center_x, int range = 2) const {
    int N = field_cardboards.size();
    pair<int, int> best_pos = {-1, -1};
    int best_weight = INT_MAX;
    int best_durability = -1;

    for (int dy = -range; dy <= range; dy++) {
      for (int dx = -range; dx <= range; dx++) {
        int ny = center_y + dy;
        int nx = center_x + dx;

        if (ny < 0 || ny >= N || nx < 0 || nx >= N)
          continue;
        if (!field_cardboards[ny][nx].empty()) {
          int weight = field_cardboards[ny][nx].back().weight;
          int durability = field_cardboards[ny][nx].back().durability;

          // より軽い箱、または同じ重量でより耐久値の高い箱を選択
          if (weight < best_weight || (weight == best_weight && durability > best_durability)) {
            best_weight = weight;
            best_durability = durability;
            best_pos = {ny, nx};
          }
        }
      }
    }
    return best_pos;
  }

  // 最適化された(0,0)への経路（軽い箱を経由）
  vector<Action> get_optimized_path_to_origin() const {
    vector<Action> path;

    // 現在位置から(0,0)への候補経路を生成
    vector<vector<Action>> candidate_paths = generate_candidate_paths();

    if (candidate_paths.empty()) {
      return get_path_to_origin(); // フォールバック
    }

    // 各経路で取得可能な箱の価値を評価
    vector<Action> best_path;
    double best_score = -1e9;

    for (const auto &candidate_path : candidate_paths) {
      double score = evaluate_path_for_pickup(candidate_path);
      if (score > best_score) {
        best_score = score;
        best_path = candidate_path;
      }
    }

    return best_path.empty() ? get_path_to_origin() : best_path;
  }

private:
  bool pickup_cardboard() {
    if (field_cardboards[player.y][player.x].empty())
      return false;

    Cardboard card = field_cardboards[player.y][player.x].back();
    field_cardboards[player.y][player.x].pop_back();
    player.holding_cardboards.push_back(card);
    remaining_cardboards--;
    total_operations++;
    return true;
  }

  bool place_cardboard() {
    if (player.holding_cardboards.empty() || !field_cardboards[player.y][player.x].empty()) {
      return false;
    }

    Cardboard card = player.holding_cardboards.back();
    player.holding_cardboards.pop_back();
    field_cardboards[player.y][player.x].push_back(card);
    remaining_cardboards++;
    total_operations++;
    return true;
  }

  bool move_player(Move direction) {
    int new_y = player.y, new_x = player.x;

    switch (direction) {
    case Move::UP:
      new_y--;
      break;
    case Move::DOWN:
      new_y++;
      break;
    case Move::LEFT:
      new_x--;
      break;
    case Move::RIGHT:
      new_x++;
      break;
    }

    int N = field_cardboards.size();
    if (new_y < 0 || new_y >= N || new_x < 0 || new_x >= N)
      return false;

    player.y = new_y;
    player.x = new_x;
    move_count++;
    total_operations++;

    // 耐久力減少
    for (int i = 0; i < player.holding_cardboards.size(); i++) {
      int weight_above = 0;
      for (int j = i + 1; j < player.holding_cardboards.size(); j++) {
        weight_above += player.holding_cardboards[j].weight;
      }
      player.holding_cardboards[i].durability -= weight_above;
    }

    // (0,0)到達時の配送処理
    if (player.y == 0 && player.x == 0 && !player.holding_cardboards.empty()) {
      int delivered_count = player.holding_cardboards.size();
      player.holding_cardboards.clear();

      // cerr << "Delivered " << delivered_count << " cardboards at origin" << endl;
    }

    return true;
  }

  // 候補経路を生成（X方向優先とY方向優先）
  vector<vector<Action>> generate_candidate_paths() const {
    vector<vector<Action>> paths;

    // パス1: Y方向優先
    vector<Action> path1;
    int cy = player.y, cx = player.x;
    while (cy != 0) {
      if (cy > 0) {
        path1.push_back(Action(Action::MOVE, Move::UP));
        cy--;
      } else {
        path1.push_back(Action(Action::MOVE, Move::DOWN));
        cy++;
      }
    }
    while (cx != 0) {
      if (cx > 0) {
        path1.push_back(Action(Action::MOVE, Move::LEFT));
        cx--;
      } else {
        path1.push_back(Action(Action::MOVE, Move::RIGHT));
        cx++;
      }
    }
    paths.push_back(path1);

    // パス2: X方向優先
    vector<Action> path2;
    cy = player.y;
    cx = player.x;
    while (cx != 0) {
      if (cx > 0) {
        path2.push_back(Action(Action::MOVE, Move::LEFT));
        cx--;
      } else {
        path2.push_back(Action(Action::MOVE, Move::RIGHT));
        cx++;
      }
    }
    while (cy != 0) {
      if (cy > 0) {
        path2.push_back(Action(Action::MOVE, Move::UP));
        cy--;
      } else {
        path2.push_back(Action(Action::MOVE, Move::DOWN));
        cy++;
      }
    }
    paths.push_back(path2);

    return paths;
  }

  // 経路での箱取得可能性を評価
  double evaluate_path_for_pickup(const vector<Action> &path) const {
    GameState test_state = *this;
    double score = 0.0;
    int pickups = 0;

    for (const auto &action : path) {
      if (!test_state.advance(action)) {
        return -1e9; // 実行できない経路
      }

      // 現在位置に箱があるかチェック
      if (!test_state.field_cardboards[test_state.player.y][test_state.player.x].empty()) {
        GameState pickup_test = test_state.copy();
        if (pickup_test.advance(Action(Action::PICKUP))) {
          if (pickup_test.can_reach_origin_safely()) {
            int weight =
                test_state.field_cardboards[test_state.player.y][test_state.player.x].back().weight;
            int durability = test_state.field_cardboards[test_state.player.y][test_state.player.x]
                                 .back()
                                 .durability;

            // 軽い箱ほど高スコア、耐久値も考慮
            score += (1000.0 / (weight + 1)) + durability * 10.0;
            pickups++;

            // 実際に取得をシミュレーション
            test_state.advance(Action(Action::PICKUP));
          }
        }
      }
    }

    // 取得可能な箱の数にボーナス
    score += pickups * 5000.0;

    return score;
  }
};

GameState solve_greedy(GameState &initial_state) {
  GameState state = initial_state.copy();

  while (!state.is_complete() && !state.is_game_over()) {
    // 最も耐久値が高く重い箱を探す
    pair<int, int> target = state.find_most_durable_box();
    if (target.first == -1)
      break;

    // 目標箱まで移動
    while (state.player.y != target.first || state.player.x != target.second) {
      if (state.is_game_over())
        break;

      vector<Action> path = state.get_path_to(target.first, target.second);
      if (path.empty())
        break;

      if (!state.advance(path[0]))
        break;
    }

    // 目標箱を取得
    if (state.player.y == target.first && state.player.x == target.second) {
      if (!state.field_cardboards[state.player.y][state.player.x].empty()) {
        GameState test = state.copy();
        if (test.advance(Action(Action::PICKUP)) && test.can_reach_origin_safely()) {
          state.advance(Action(Action::PICKUP));
        }
      }
    }

    // (0,0)に戻る（最適化された経路で軽い箱を多く取得）
    while ((state.player.y != 0 || state.player.x != 0) && !state.is_game_over()) {
      // 現在位置の箱をチェック
      if (!state.field_cardboards[state.player.y][state.player.x].empty()) {
        GameState test = state.copy();
        if (test.advance(Action(Action::PICKUP)) && test.can_reach_origin_safely()) {
          state.advance(Action(Action::PICKUP));
          continue; // 箱を取得したので次のループへ
        }
      }

      // 最適化された経路で(0,0)に向かう
      vector<Action> optimized_path = state.get_optimized_path_to_origin();
      if (optimized_path.empty()) {
        // 最適化経路が見つからない場合は通常の最短経路
        vector<Action> fallback_path = state.get_path_to_origin();
        if (fallback_path.empty())
          break;
        if (!state.advance(fallback_path[0]))
          break;
      } else {
        // 最適化経路の最初の1歩を実行
        if (!state.advance(optimized_path[0]))
          break;
      }
    }

    // デバッグ出力
    /*
    if (state.total_operations % 100 == 0) {
      cerr << "Operations: " << state.total_operations
           << ", Remaining: " << state.remaining_cardboards
           << ", Position: (" << state.player.y << "," << state.player.x << ")"
           << ", Holding: " << state.player.holding_cardboards.size() << endl;
    }
           */
  }

  // 最終処理: 残っている荷物を(0,0)に運搬
  if (!state.player.holding_cardboards.empty() && !state.is_game_over()) {
    while ((state.player.y != 0 || state.player.x != 0) && !state.is_game_over()) {
      vector<Action> path = state.get_path_to_origin();
      if (path.empty()) {
        break;
      }

      if (!state.advance(path[0])) {
        break;
      }
    }

    // (0,0)に到達した時点で自動的に荷物が配送される（move_player内で処理）
    if (state.player.y == 0 && state.player.x == 0) {
    }
  }

  return state;
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int N;
  cin >> N;

  vector<vector<ll>> weights(N, vector<ll>(N));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> weights[i][j];
    }
  }

  vector<vector<ll>> durabilities(N, vector<ll>(N));
  for (int i = 0; i < N; i++) {
    for (int j = 0; j < N; j++) {
      cin >> durabilities[i][j];
    }
  }

  GameState initial_state(N, weights, durabilities);
  GameState result = solve_greedy(initial_state);

  result.output_operations();

  return 0;
}
