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

constexpr int MAX_INVEST_LEVEL = 20;

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

struct Project {
  ll h;
  ll v;
};

enum class CardType {
  WORK_SINGLE = 0,
  WORK_ALL = 1,
  CANCEL_SINGLE = 2,
  CANCEL_ALL = 3,
  INVEST = 4,
};

ostream &operator<<(ostream &os, CardType ct) {
  switch (ct) {
  case CardType::WORK_SINGLE:
    os << "WORK_SINGLE";
    break;
  case CardType::WORK_ALL:
    os << "WORK_ALL";
    break;
  case CardType::CANCEL_SINGLE:
    os << "CANCEL_SINGLE";
    break;
  case CardType::CANCEL_ALL:
    os << "CANCEL_ALL";
    break;
  case CardType::INVEST:
    os << "INVEST";
    break;
  }
  return os;
}

struct Card {
  CardType t;
  ll w;
  ll p;
};

struct Work {
  int k, i, j, s;
};

struct Judge {
  const int n;
  const int m;
  const int k;

  Judge(int n, int m, int k) : n(n), m(m), k(k) {}

  vector<Card> read_initial_cards() {
    vector<Card> cards;
    for (int i = 0; i < n; i++) {
      int64_t t, w;
      cin >> t >> w;
      cards.push_back(Card{(CardType)t, w, 0ll});
    }
    return cards;
  }

  vector<Project> read_projects() {
    vector<Project> projects;
    for (int i = 0; i < m; i++) {
      int64_t h, v;
      cin >> h >> v;
      projects.push_back(Project{h, v});
    }
    return projects;
  }

  void use_card(int c, int m) { cout << c << " " << m << endl; }

  int64_t read_money() {
    int64_t money;
    cin >> money;
    return money;
  }

  vector<Card> read_next_cards() {
    vector<Card> cards;
    for (int i = 0; i < k; i++) {
      int64_t t, w, p;
      cin >> t >> w >> p;
      cards.push_back(Card{(CardType)t, w, p});
    }
    return cards;
  }

  void select_card(int r) { cout << r << endl; }

  void comment(const string &message) { cout << "# " << message << endl; }
};

struct Solver {
  const int n;
  const int m;
  const int k;
  const int t;
  Judge judge;
  int turn;
  int invest_level;
  int64_t money;
  vector<Project> projects;
  vector<Card> cards;

  int target_project_i = 0;

  Solver(int n, int m, int k, int t)
      : n(n), m(m), k(k), t(t), judge(n, m, k), turn(0), invest_level(0), money(0) {}

  int64_t solve() {
    cards = judge.read_initial_cards();
    projects = judge.read_projects();
    target_project_i = select_project(projects);
    for (turn = 0; turn < t; turn++) {
      auto [use_card_i, use_target] = select_action(cards, projects);
      const Card &use_card = cards[use_card_i];
      if (use_card.t == CardType::INVEST) {
        invest_level++;
      }

      // example for comments
      stringstream msg;
      msg << "used Card(t=" << use_card.t << " w=" << use_card.w << " p=" << use_card.p
          << ") to target " << use_target << " : ";
      msg << "now target project is " << target_project_i
          << "  target_project_h: " << projects[target_project_i].h
          << "  target_project_v: " << projects[target_project_i].v;
      judge.comment(msg.str());
      judge.use_card(use_card_i, use_target);
      assert(invest_level <= MAX_INVEST_LEVEL);

      projects = judge.read_projects();
      money = judge.read_money();

      target_project_i = select_project(projects);
      if (target_project_i == -1) {
        target_project_i = use_target;
      }

      vector<Card> next_cards = judge.read_next_cards();
      int select_card_i = select_next_card(next_cards);
      cards[use_card_i] = next_cards[select_card_i];
      judge.select_card(select_card_i);
      money -= next_cards[select_card_i].p;
      assert(money >= 0);
    }
    return money;
  }

  bool check_need_cancel_project(const vector<Project> &projects) {
    if (turn * pow(2, invest_level) < projects[target_project_i].h) {
      return true;
    }

    return false;
  }

  int select_action_card(const vector<Card> &cards, const vector<Project> &projects) { return 0; }

  // ターン数で戦略を変える、価値が高いプロジェクトを狙う、最初は残務量が小さいプロジェクトを狙ったほうが良い？
  int select_project(const vector<Project> &projects) {
    /*
    if (turn < 100) {
      ll min_h = INF;
      int min_h_i = -1;
      for (int i = 0; i < projects.size(); i++) {
        if (projects[i].h < min_h) {
          min_h = projects[i].h;
          min_h_i = i;
        }
      }
      return min_h_i;
    }
    */

    double max_score = -INF;
    int max_score_i = -1;
    for (int i = 0; i < projects.size(); i++) {
      double score = static_cast<double>(projects[i].v) / projects[i].h + turn / projects[i].h;
      if (score > max_score) {
        max_score = score;
        max_score_i = i;
      }
    }

    return max_score_i;
  }

  int select_cancel_project(const vector<Project> &projects) {
    double min_score = INF;
    int min_score_i = -1;
    for (int i = 0; i < projects.size(); i++) {
      double score = static_cast<double>(projects[i].v) / projects[i].h + turn / projects[i].h;
      if (score < min_score) {
        min_score = score;
        min_score_i = i;
      }
    }
    return min_score_i;
  }

  pair<int, int> select_action(const vector<Card> &cards, const vector<Project> &projects) {
    if (turn >= 750) {
      for (int i = 0; i < cards.size(); i++) {
        if (cards[i].t == CardType::CANCEL_ALL) {
          return {i, 0};
        }
      }

      for (int i = 0; i < cards.size(); i++) {
        if (cards[i].t == CardType::CANCEL_SINGLE) {
          return {i, select_cancel_project(projects)};
        }
      }
    }

    // WORK_SINGLEの場合、とりあえず残務量が少ないプロジェクトを選び、オーバーでも良いのでそこに投入する
    // 増資カードが使えるのであれば早い段階で使う
    for (int i = 0; i < cards.size(); i++) {
      if (cards[i].t == CardType::INVEST) {
        return {i, 0};
      }
    }

    // TODO: 関数化
    ll max_work_power = 0;
    int selected_work_all_card_i = -1;
    for (int i = 0; i < cards.size(); i++) {
      if (cards[i].t != CardType::WORK_ALL) {
        continue;
      }

      ll now_work_power = 0;
      for (int j = 0; j < projects.size(); j++) {
        auto [h, v] = projects[j];
        now_work_power += min(h, cards[i].w);
      }
      if (now_work_power > max_work_power) {
        max_work_power = now_work_power;
        selected_work_all_card_i = i;
      }
    }

    if (selected_work_all_card_i != -1) {
      return {selected_work_all_card_i, 0};
    }

    int selected_work_single_card_i = -1;
    int selected_work_single_project_i = -1;
    for (int i = 0; i < cards.size(); i++) {
      if (cards[i].t != CardType::WORK_SINGLE) {
        continue;
      }

      ll over_work_power = INF;
      for (int j = 0; j < projects.size(); j++) {
        auto [h, v] = projects[j];
        if (cards[i].w >= h) {
          ll now_over_work_power = cards[i].w - h;
          if (now_over_work_power < over_work_power) {
            over_work_power = now_over_work_power;
            selected_work_single_card_i = i;
            selected_work_single_project_i = j;
          }
        }
      }
    }

    if (selected_work_single_card_i != -1) {
      return {selected_work_single_card_i, selected_work_single_project_i};
    }

    ll single_work_power = 0;
    selected_work_single_card_i = -1;
    for (int i = 0; i < cards.size(); i++) {
      if (cards[i].t != CardType::WORK_SINGLE) {
        continue;
      }

      if (cards[i].w > single_work_power) {
        single_work_power = cards[i].w;
        selected_work_single_card_i = i;
      }
    }

    if (selected_work_single_card_i != -1) {
      return {selected_work_single_card_i, target_project_i};
    }

    // TODO: implement your strategy
    return {0, 0};
  }

  int select_best_work_card(const vector<Card> &next_cards) {
    // まず増資カードが買えるなら買う
    for (int i = 0; i < next_cards.size(); i++) {
      if (next_cards[i].t == CardType::INVEST && money >= next_cards[i].p &&
          invest_level < MAX_INVEST_LEVEL && turn < 800) {
        return i;
      }
    }

    // 終盤の場合
    if (turn >= 800 && check_need_cancel_project(projects)) {
      for (int i = 0; i < next_cards.size(); i++) {
        if (next_cards[i].t == CardType::CANCEL_ALL && money / 10 >= next_cards[i].p) {
          return i;
        }
      }

      for (int i = 0; i < next_cards.size(); i++) {
        if (next_cards[i].t == CardType::CANCEL_SINGLE && money / 10 >= next_cards[i].p) {
          return i;
        }
      }
    }

    int best_card_i = -1;
    double best_score = 0.0;
    for (int i = 0; i < next_cards.size(); i++) {
      if (next_cards[i].t != CardType::WORK_SINGLE && next_cards[i].t != CardType::WORK_ALL) {
        continue;
      }

      // コスト0の場合はturn数x3のコストを考える
      // double denom = next_cards[i].w == 0 ? next_cards[i].p + 10 / turn : next_cards[i].p;
      double denom = next_cards[i].p + 1;
      double numer = next_cards[i].t == CardType::WORK_ALL ? next_cards[i].w * projects.size()
                                                           : next_cards[i].w;
      double now_card_score = numer / denom;
      if (money >= next_cards[i].p && now_card_score >= best_score) {
        best_score = now_card_score;
        best_card_i = i;
      }
    }

    if (best_card_i == -1) {
      return 0;
    }

    return best_card_i;
  }

  int select_next_card(const vector<Card> &next_cards) {
    // TODO: implement your strategy
    return select_best_work_card(next_cards);
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int n, m, k, t;
  cin >> n >> m >> k >> t;
  Solver solver(n, m, k, t);
  int64_t score = solver.solve();
  cerr << "score: " << score << endl;
  return 0;
}
