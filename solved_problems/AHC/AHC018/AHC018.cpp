#include <Eigen/Dense>
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

template <typename T>
vector<vector<T>> dijkstra(const vector<vector<T>> &S, vector<vector<pair<T, T>>> &prev, int N,
                           int sy, int sx) {
  vector<vector<T>> dist(N, vector<T>(N, INT_INF));
  prev.resize(N);
  for (int i = 0; i < N; i++)
    prev[i].assign(N, {-1, -1});

  priority_queue<pair<T, pair<T, T>>, vector<pair<T, pair<T, T>>>, greater<pair<T, pair<T, T>>>>
      que;

  dist[sy][sx] = 0;
  que.push({dist[sy][sx], {sy, sx}});

  while (que.size()) {
    pair<T, T> p = que.top().second;
    T cost = que.top().first;

    que.pop();
    if (dist[p.first][p.second] < cost)
      continue;

    for (int i = 0; i < 4; i++) {
      int ny = p.first + dy[i], nx = p.second + dx[i];

      if (0 <= ny & ny < N && 0 <= nx && nx < N) {
        T weight = S[ny][nx];
        if (dist[ny][nx] <= cost + weight)
          continue;

        dist[ny][nx] = cost + weight;
        prev[ny][nx] = {p.first, p.second};
        que.push({dist[ny][nx], {ny, nx}});
      }
    }
  }

  return dist;
}

template <typename T>
deque<pair<T, T>> getDijkstraPath(const vector<vector<pair<T, T>>> &prev, P t) {
  deque<pair<T, T>> path;
  for (P cur = t; cur.first != -1 || cur.second != -1; cur = prev[cur.first][cur.second]) {
    // path.push_front(cur);
    path.push_back(cur);
  }
  return path;
}

enum class FieldType {
  House,
  WaterSource,
  Rock,
};

enum class CellState {
  Unexcavated,
  Excavating,
  Excavated,
  Water, // WaterはExcavatedの状態も含んでいる
};

enum class Responce {
  not_broken,
  broken,
  finish,
  invalid,
};

enum class LerpDirection {
  Horizontal,
  Vertical,
};

enum class ExcavationWay {
  Trial,
  Main,
};

struct UnionFind {
  vector<int> par;

  UnionFind(int n) : par(n, -1) {}
  UnionFind() {}

  void init(int n) { par.resize(n, -1); }

  int root(int x) {
    if (par[x] < 0)
      return x;
    else
      return par[x] = root(par[x]);
  }

  bool isSame(int x, int y) { return root(x) == root(y); }

  bool merge(int x, int y) {
    x = root(x);
    y = root(y);
    if (x == y)
      return false;
    if (par[x] > par[y])
      swap(x, y);
    par[x] += par[y];
    par[y] = x;
    return true;
  }

  int size(int x) { return -par[root(x)]; }
};

struct House {
  int id;
  P position;
  deque<P> path;
  int nowCost;

  bool operator<(const House &another) const { return nowCost < another.nowCost; }
};

struct WaterSource {
  int id;
  P position;
};

struct WaterCell {
  P position;
  WaterCell(P position) : position(position) {}
};

struct InputData {
  int N_; // 土地のサイズ N = 200
  int W_; // 水源の数 1 <= W <= 4
  int K_; // 家の数 1 <= K <= 10
  int C_; // 体力の消費に関係するパラメータ C: {1, 2, 4, 8, 16, 32, 64, 128}

  vector<vector<CellState>> isDigged_;
  vector<vector<FieldType>> field_;
  vector<vector<int>> estimatedS_;
  vector<vector<int>> realS_;

  deque<House> houseVec;
  vector<WaterSource> wsVec;
  vector<WaterCell> waterCellVec; // 水が通っているセルはこれを使う

  InputData(){};
  ~InputData() = default;

  void input() {
    cin >> N_ >> W_ >> K_ >> C_;
    cerr << "N: " << N_ << "  W: " << W_ << "  K: " << K_ << "  C: " << C_ << endl;

    isDigged_.resize(N_);
    field_.resize(N_);
    estimatedS_.resize(N_); // 10 <= S <= 5000 線形スケーリングされている
    realS_.resize(N_);

    houseVec.resize(K_);
    wsVec.resize(W_);

    for (int i = 0; i < N_; i++) {
      isDigged_[i].assign(N_, CellState::Unexcavated);
      field_[i].resize(N_, FieldType::Rock);
      estimatedS_[i].resize(N_);
      realS_[i].resize(N_);
    }

    /* this is used only when debugging */
    /*
    for (int i = 0; i < N_; i++) {
      for (int j = 0; j < N_; j++) {
        cin >> realS_[i][j];
        if (j == N_ - 1) {
          cerr << realS_[i][j] << " ";
        }
      }
    }
    cerr << endl;
    */

    for (int i = 0; i < W_; i++) {
      int a, b;
      cin >> a >> b;
      field_[a][b] = FieldType::WaterSource;
      wsVec[i].id = i;
      wsVec[i].position.first = a, wsVec[i].position.second = b;
      waterCellVec.emplace_back(WaterCell({a, b}));
    }

    for (int i = 0; i < K_; i++) {
      int c, d;
      cin >> c >> d;
      field_[c][d] = FieldType::House;
      houseVec[i].id = i;
      houseVec[i].position.first = c, houseVec[i].position.second = d;
    }
  }
};

class AHC018 {
  InputData input_;

  map<int, int> mapCToP;

  vector<vector<P>> excavatedCell;

  ll totalCost;

  // UnionFind uf;

public:
  AHC018() : totalCost(0){};

  ~AHC018() = default;

  void mapCToPInit() {
    vector<int> C = {1, 2, 4, 8, 16, 32, 64, 128};
    /*
    for (auto it : C) {
      mapCToP[it] = it * 3; // 10試した
    }
    */
    mapCToP[1] = 20;
    mapCToP[2] = 20;
    mapCToP[4] = 20;
    mapCToP[8] = 30;
    mapCToP[16] = 60;
    mapCToP[32] = 100;
    mapCToP[64] = 100;
    mapCToP[128] = 150;
  }

  void excavaingAllHouseAndWaterSourcePoints() {
    // 家と水源の岩盤をとりあえず開ける
    for (auto h : input_.houseVec) {
      destruct(h.position.first, h.position.second, ExcavationWay::Main);
    }

    for (auto w : input_.wsVec) {
      destruct(w.position.first, w.position.second, ExcavationWay::Main);
    }
  }

  void calculateInitialShortestPath() {
    for (auto &h : input_.houseVec) {
      vector<vector<P>> prev;
      vector<vector<int>> dist =
          dijkstra(input_.realS_, prev, input_.N_, h.position.first, h.position.second);

      int cost = INT_INF;
      for (auto w : input_.wsVec) {
        int nowCost = dist[w.position.first][w.position.second];
        if (nowCost < cost) {
          h.path = getDijkstraPath(prev, w.position);
          cost = nowCost;
          h.nowCost = cost;
        }
      }
    }
  }

  void updateShortestPath() {
    for (auto &h : input_.houseVec) {
      if (!h.path.size())
        continue;

      vector<vector<P>> prev;
      vector<vector<int>> dist =
          dijkstra(input_.realS_, prev, input_.N_, h.position.first, h.position.second);

      int cost = INT_INF;
      for (auto w : input_.waterCellVec) {
        int nowCost = dist[w.position.first][w.position.second];
        if (nowCost < cost) {
          h.path = getDijkstraPath(prev, w.position);
          cost = nowCost;
          h.nowCost = cost;
        }
      }
    }
  }

  // TODO: 試し堀の座標を行、列ごとに保持しておく
  void trialExcavation(int oneLineTrialNum = 10) {
    excavatedCell.resize(oneLineTrialNum);

    int exceptEdge = oneLineTrialNum - 1;
    int denom = exceptEdge;
    int skipNum = input_.N_ / denom;

    int excavatedCellRowIndex = 0;
    int breakCellRemRow = exceptEdge;
    for (int i = 0; i < input_.N_; i += skipNum) {
      int breakCellRemCol = exceptEdge;
      for (int j = 0; j < input_.N_; j += skipNum) {
        if (input_.isDigged_[i][j] != CellState::Excavated &&
            input_.isDigged_[i][j] != CellState::Water) {
          destruct(i, j, ExcavationWay::Trial);
          breakCellRemCol--;
          excavatedCell[excavatedCellRowIndex].push_back({i, j});
        }
        if (!breakCellRemCol) {
          break;
        }
      }
      // 最後の1列
      if (input_.isDigged_[i][input_.N_ - 1] != CellState::Excavated &&
          input_.isDigged_[i][input_.N_ - 1] != CellState::Water) {
        destruct(i, input_.N_ - 1, ExcavationWay::Trial);
        excavatedCell[excavatedCellRowIndex].push_back({i, input_.N_ - 1});
      }
      breakCellRemRow--;
      if (!breakCellRemRow) {
        break;
      }
      excavatedCellRowIndex++;
    }

    // 最後の1行
    int breakCellRemCol = exceptEdge;
    for (int j = 0; j < input_.N_; j += skipNum) {
      if (input_.isDigged_[input_.N_ - 1][j] != CellState::Excavated &&
          input_.isDigged_[input_.N_ - 1][j] != CellState::Water) {
        destruct(input_.N_ - 1, j, ExcavationWay::Trial);
        excavatedCell[oneLineTrialNum - 1].push_back({input_.N_ - 1, j});
        breakCellRemCol--;
      }
      if (!breakCellRemCol) {
        break;
      }
    }
    if (input_.isDigged_[input_.N_ - 1][input_.N_ - 1] != CellState::Excavated &&
        input_.isDigged_[input_.N_ - 1][input_.N_ - 1] != CellState::Water) {
      destruct(input_.N_ - 1, input_.N_ - 1, ExcavationWay::Trial);
      excavatedCell[oneLineTrialNum - 1].push_back({input_.N_ - 1, input_.N_ - 1});
    }

    /*
    for (int i = 0; i < input_.N_; i++) {
      for (int j = 0; j < input_.N_; j++) {
        cerr << input_.estimatedS_[i][j] << " ";
      }
      cerr << endl;
    }
    */
  }

  void lerp(P targetCell, P sampleCell1, P sampleCell2, LerpDirection ld) {
    int targetY = targetCell.first;
    int targetX = targetCell.second;
    int sampleCell1Y = sampleCell1.first;
    int sampleCell1X = sampleCell1.second;
    int sampleCell2Y = sampleCell2.first;
    int sampleCell2X = sampleCell2.second;

    if (ld == LerpDirection::Vertical) {
      input_.estimatedS_[targetY][targetX] = (input_.estimatedS_[sampleCell2Y][sampleCell2X] -
                                              input_.estimatedS_[sampleCell1Y][sampleCell1X]) *
                                                 (targetY - sampleCell1Y) /
                                                 (sampleCell2Y - sampleCell1Y) +
                                             input_.estimatedS_[sampleCell1Y][sampleCell1X];
    } else {
      input_.estimatedS_[targetY][targetX] = (input_.estimatedS_[sampleCell2Y][sampleCell2X] -
                                              input_.estimatedS_[sampleCell1Y][sampleCell1X]) *
                                                 (targetX - sampleCell1X) /
                                                 (sampleCell2X - sampleCell1X) +
                                             input_.estimatedS_[sampleCell1Y][sampleCell1X];
    }

    input_.realS_[targetY][targetX] = input_.estimatedS_[targetY][targetX];

    /*
    cerr << input_.estimatedS_[sampleCell1Y][sampleCell1X] << " "
         << input_.estimatedS_[targetY][targetX] << " "
         << input_.estimatedS_[sampleCell2Y][sampleCell2X] << endl;
    */
  }

  void bilinear(P upperLeft, P upperRight, P lowerLeft, P lowerRight) {
    // 上と下を先に補完する
    for (int j = upperLeft.second + 1; j < upperRight.second; j++) {
      lerp({upperLeft.first, j}, upperLeft, upperRight, LerpDirection::Horizontal);
    }
    for (int j = lowerLeft.second + 1; j < lowerRight.second; j++) {
      lerp({lowerLeft.first, j}, lowerLeft, lowerRight, LerpDirection::Horizontal);
    }

    // 次に間を全て補完する
    for (int j = upperLeft.second; j <= upperRight.second; j++) {
      for (int i = upperLeft.first + 1; i < lowerLeft.first; i++) {
        lerp({i, j}, {upperLeft.first, j}, {lowerLeft.first, j}, LerpDirection::Vertical);
      }
    }
  }

  void initialBilinearAll(int oneLineTrialNum = 10) {
    for (int i = 0; i < oneLineTrialNum - 1; i++) {
      for (int j = 0; j < oneLineTrialNum - 1; j++) {
        P upperLeft = excavatedCell[i][j];
        P upperRight = excavatedCell[i][j + 1];
        P lowerLeft = excavatedCell[i + 1][j];
        P lowerRight = excavatedCell[i + 1][j + 1];
        bilinear(upperLeft, upperRight, lowerLeft, lowerRight);
      }
    }

    /*
    for (int i = 0; i < input_.N_; i++) {
      for (int j = 0; j < input_.N_; j++) {
        cerr << input_.estimatedS_[i][j] << " ";
      }
      cerr << endl;
    }
    */
    /*
    cerr << endl;
    for (int i = 0; i < input_.N_; i++) {
      for (int j = 0; j < input_.N_; j++) {
        cerr << input_.realS_[i][j] << " ";
      }
      cerr << endl;
    }
    */
  }

  double calcBicubicH(int cellDist, int oneLineTrialNum) {
    int unitLength = input_.N_ / (oneLineTrialNum - 1);
    double t = cellDist / (double)unitLength;
    t = abs(t);
    if (t <= 1) {
      /*
      cerr << "t: " << t << endl;
      cerr << "res: " << t * t * t - 2 * t * t + 1 << endl;
      */
      return t * t * t - 2 * t * t + 1;
    } else if (1 < t && t <= 2) {
      /*
      cerr << "t: " << t << endl;
      cerr << "res: " << -t * t * t + 5 * t * t - 8 * t + 4 << endl;
      */
      return -t * t * t + 5 * t * t - 8 * t + 4;
    }
    return 0.;
  }

  // 囲まれる様に16点が存在しなければならない
  // すなわち、求めたい点の左側に2点、上側に2点、右側に2点、下側に2点ずつそれぞれ存在する必要がある
  void bicubic(P targetCell, const vector<P> &sampleCells, int oneLineTrialNum) {
    // sampleCellsは左上から0, 1, 2, 3,...となっている前提
    vector<double> hx(4);
    vector<vector<double>> sampleValue(4, vector<double>(4));
    vector<double> hy(4);

    for (int i = 0; i < 4; i++) {
      hx[i] = calcBicubicH(targetCell.second - sampleCells[i].second, oneLineTrialNum);
    }
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        sampleValue[i][j] =
            input_.estimatedS_[sampleCells[i * 4 + j].first][sampleCells[i * 4 + j].second];
      }
    }
    for (int i = 0; i < 4; i++) {
      hy[i] = calcBicubicH(targetCell.first - sampleCells[i * 4].first, oneLineTrialNum);
    }

    /*
    for (auto h : hx)
      cerr << h << " ";
    cerr << endl;
    for (auto h : hy)
      cerr << h << " ";
    cerr << endl;
    */

    vector<double> hxBySampleValue(4, 0.0);
    for (int i = 0; i < 4; i++) {
      for (int j = 0; j < 4; j++) {
        hxBySampleValue[i] += hx[j] * sampleValue[j][i];
      }
    }

    double result = 0.0;
    for (int i = 0; i < 4; i++) {
      result += hxBySampleValue[i] * hy[i];
    }

    input_.estimatedS_[targetCell.first][targetCell.second] = max(result, 100.);
    input_.realS_[targetCell.first][targetCell.second] = max(result, 100.);
  }

  void bicubicApply(const vector<P> &sampleCells, int oneLineTrialNum) {
    // 5, 6, 9, 10
    P upperLeft = sampleCells[5];
    P upperRight = sampleCells[6];
    P lowerLeft = sampleCells[9];
    P lowerRight = sampleCells[10];

    for (int i = upperLeft.first; i <= lowerLeft.first; i++) {
      for (int j = upperLeft.second; j <= upperRight.second; j++) {
        if (i == upperLeft.first && j == upperLeft.second)
          continue;
        if (i == upperRight.first && j == upperRight.second)
          continue;
        if (i == lowerLeft.first && j == lowerLeft.second)
          continue;
        if (i == lowerRight.first && j == lowerRight.second)
          continue;

        bicubic({i, j}, sampleCells, oneLineTrialNum);
      }
    }
  }

  void initialBicubicAll(int oneLineTrialNum) {
    for (int i = 0; i < oneLineTrialNum - 3; i++) {
      for (int j = 0; j < oneLineTrialNum - 3; j++) {
        vector<P> sampleCells;
        sampleCells.push_back({excavatedCell[i][j]});
        sampleCells.push_back({excavatedCell[i][j + 1]});
        sampleCells.push_back({excavatedCell[i][j + 2]});
        sampleCells.push_back({excavatedCell[i][j + 3]});
        sampleCells.push_back({excavatedCell[i + 1][j]});
        sampleCells.push_back({excavatedCell[i + 1][j + 1]});
        sampleCells.push_back({excavatedCell[i + 1][j + 2]});
        sampleCells.push_back({excavatedCell[i + 1][j + 3]});
        sampleCells.push_back({excavatedCell[i + 2][j]});
        sampleCells.push_back({excavatedCell[i + 2][j + 1]});
        sampleCells.push_back({excavatedCell[i + 2][j + 2]});
        sampleCells.push_back({excavatedCell[i + 2][j + 3]});
        sampleCells.push_back({excavatedCell[i + 3][j]});
        sampleCells.push_back({excavatedCell[i + 3][j + 1]});
        sampleCells.push_back({excavatedCell[i + 3][j + 2]});
        sampleCells.push_back({excavatedCell[i + 3][j + 3]});
        bicubicApply(sampleCells, oneLineTrialNum);
      }
    }
  }

  void decideInitialS() {
    // この関数の中をいい感じに実装する
    /*
    for (int i = 0; i < input_.N_; i++) {
      for (int j = 0; j < input_.N_; j++) {
        input_.estimatedS_[i][j] = 2500;
      }
    }
    */
    int oneLineTrialNum = 50;
    /*
    if (input_.K_ / input_.W_ >= 10) {
      oneLineTrialNum = 11;
    } else if (input_.K_ / input_.W_ >= 8) {
      oneLineTrialNum = 10;
    } else if (input_.K_ / input_.W_ >= 2) {
      oneLineTrialNum = 9;
    } else {
      oneLineTrialNum = 8;
    }
    */
    trialExcavation(oneLineTrialNum);
    initialBilinearAll(oneLineTrialNum);
    /*
    for (int i = 0; i < input_.N_; i++) {
      for (int j = 0; j < input_.N_; j++) {
        cerr << input_.estimatedS_[i][j] << " ";
      }
      cerr << endl;
    }
    cerr << endl;
    */
    initialBicubicAll(oneLineTrialNum);
    for (int i = 0; i < input_.N_; i++) {
      for (int j = 0; j < input_.N_; j++) {
        cerr << input_.estimatedS_[i][j] << " ";
      }
      cerr << endl;
    }
  }

  bool aroundCellWater(int y, int x) {
    for (int i = 0; i < 4; i++) {
      int ny = y + dy[i], nx = x + dx[i];
      if (ny < 0 || ny >= input_.N_ || nx > 0 || nx >= input_.N_)
        continue;
      if (input_.isDigged_[ny][nx] == CellState::Water) {
        return true;
      }
    }
    return false;
  }

  void destruct(int y, int x, ExcavationWay ew) {
    bool isFirstExcavation = true;

    int mainPower = input_.estimatedS_[y][x];
    int initPower = mapCToP[input_.C_];

    int nowCost = 0;
    int estimateCost = 0;
    while (input_.isDigged_[y][x] == CellState::Unexcavated ||
           input_.isDigged_[y][x] == CellState::Excavating) {
      Responce result;
      if (!isFirstExcavation && ew == ExcavationWay::Main) {
        nowCost += mainPower + input_.C_;
        estimateCost += mainPower;
        result = query(y, x, mainPower);
        isFirstExcavation = false;
      } else {
        nowCost += initPower + input_.C_;
        estimateCost += initPower;
        result = query(y, x, initPower);
      }
      if (result == Responce::finish) {
        cerr << "total_cost: " << totalCost << endl;
        exit(0);
      } else if (result == Responce::invalid) {
        cerr << "invalid: y=" << y << " x=" << x << endl;
        exit(1);
      }
    }

    // 上下左右を見て、水になっているマスがあれば(y, x)もCellState::Waterにする
    // またwaterCellVecにpush_back()
    if (aroundCellWater(y, x)) {
      input_.isDigged_[y][x] = CellState::Water;
      input_.waterCellVec.push_back(WaterCell({y, x}));
    }

    // updateEstimatedS(); // 推定されたS[y][x]からestimatedSを更新する
    input_.estimatedS_[y][x] = estimateCost;
    input_.realS_[y][x] = 0;
  }

  Responce query(int y, int x, int power) {
    int totalPower = power + input_.C_;
    totalCost += totalPower;
    cout << y << " " << x << " " << power << endl;
    int r;
    cin >> r;
    switch (r) {
    case 0:
      input_.isDigged_[y][x] = CellState::Excavating;
      return Responce::not_broken;
    case 1:
      input_.isDigged_[y][x] = CellState::Excavated;
      return Responce::broken;
    case 2:
      input_.isDigged_[y][x] = CellState::Water;
      return Responce::finish;
    }

    return Responce::invalid;
  }

  bool isFinished() {
    for (const auto &h : input_.houseVec) {
      if (h.path.size() > 0)
        return false;
    }
    return true;
  }

  void solve() {
    input_.input();
    // uf.init(input_.N_ * input_.N_);
    mapCToPInit();
    decideInitialS();                        // 初期Sを決める
    excavaingAllHouseAndWaterSourcePoints(); // とりあえず家と水源の位置の岩盤を壊す
    calculateInitialShortestPath();          // とりあえず最短経路の初期解を求める

    // main logic...
    // コストが小さい家から処理していく
    while (!isFinished()) {
      sort(input_.houseVec.begin(), input_.houseVec.end());
      auto h = input_.houseVec.front();
      input_.houseVec.pop_front();

      if (!h.path.size())
        continue;

      while (h.path.size()) {
        P targetCell = h.path.front();
        h.path.pop_front();
        if (input_.isDigged_[targetCell.first][targetCell.second] != CellState::Excavated &&
            input_.isDigged_[targetCell.first][targetCell.second] != CellState::Water) {
          destruct(targetCell.first, targetCell.second, ExcavationWay::Main);
        }
      }
      updateShortestPath();
    }
    /*
    while (!isFinished()) {
      for (auto &h : input_.houseVec) {
        if (!h.path.size()) {
          continue;
        }

        P targetCell = h.path.front();
        h.path.pop_front();
        if (input_.isDigged_[targetCell.first][targetCell.second] != CellState::Excavated &&
            input_.isDigged_[targetCell.first][targetCell.second] != CellState::Water) {
          destruct(targetCell.first, targetCell.second, ExcavationWay::Main);
        }
      }
    }
    */
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  AHC018 ahc018;
  ahc018.solve();

  return 0;
}
