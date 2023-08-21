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
constexpr static int dx2[13] = {0, -1, -1, -1, 0, 0, 1, 1, 1, -2, 0, 2, 0},
                     dy2[13] = {0, -1, 0, 1, 1, -1, 1, 0, -1, 0, 2, 0, -2};
constexpr static int dx3[9] = {0, -1, -1, -1, 0, 0, 1, 1, 1},
                     dy3[9] = {0, -1, 0, 1, 1, -1, 1, 0, -1};
constexpr static int dx4[2] = {0, 0}, dy4[2] = {1, -1};
constexpr static int dx5[5] = {0, 1, 0, -1, 0}, dy5[5] = {0, 0, 1, 0, -1};
constexpr static int dx6[3] = {0, 0, 0}, dy6[3] = {0, 1, -1};
constexpr static int dx7[1] = {0}, dy7[1] = {0};
constexpr static int dx8[8] = {-1, -1, -1, 0, 0, 1, 1, 1}, dy8[8] = {-1, 0, 1, 1, -1, 1, 0, -1};

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

struct Pos {
  int x;
  int y;

  Pos(int x, int y) : x{x}, y{y} {};
  Pos() : x{0}, y{0} {};
};

struct Judge {
  void set_temperature(const vector<vector<int>> &temperature) {
    for (const vector<int> &row : temperature) {
      for (int i = 0; i < row.size(); i++) {
        cout << row[i] << (i == row.size() - 1 ? "\n" : " ");
      }
    }
    cout.flush();
  }

  int measure(int i, int y, int x) {
    cout << i << " " << y << " " << x << endl; // endl does flush
    int v;
    cin >> v;
    if (v == -1) {
      cerr << "something went wrong. i=" << i << " y=" << y << " x=" << x << endl;
      exit(1);
    }
    return v;
  }

  void answer(const vector<int> &estimate) {
    cout << "-1 -1 -1" << endl;
    for (int e : estimate) {
      cout << e << endl;
    }
  }
};

struct FourDirectionTemp {
  int rightTemp;
  int downTemp;
  int leftTemp;
  int upTemp;

  FourDirectionTemp(int r, int d, int l, int u)
      : rightTemp{r}, downTemp{d}, leftTemp{l}, upTemp{u} {};
  FourDirectionTemp() : rightTemp{0}, downTemp{0}, leftTemp{0}, upTemp{0} {};
};

struct Solver {
  const int L;
  const int N;
  const int S;
  const vector<Pos> landing_pos;
  Judge judge;
  vector<FourDirectionTemp> landingPosFourDirectionTemp;
  vector<vector<bool>> setTempCell; // 焼きなましで使う
  const int *dxPointer;
  const int *dyPointer;

  Solver(int L, int N, int S, const vector<Pos> &landing_pos)
      : L(L), N(N), S(S), landing_pos(landing_pos), judge() {
    setTempCell.resize(L);
    for (int i = 0; i < L; i++) {
      setTempCell[i].resize(L);
    }
    std::cerr << "L: " << L << "  N: " << N << "  S: " << S << std::endl;
  }

  void solve() {
    vector<vector<int>> temperature = create_temperature();
    // calcPlacementCost(temperature);
    simulatedAnnealingTempBoard(temperature);
    judge.set_temperature(temperature);
    const vector<int> estimate = predict(temperature);
    judge.answer(estimate);
  }

  int decideTemperatureStep() { return 10; }

  vector<int> decideRowFactor() {
    vector<int> rowFactor(L);
    for (int i = 0; i < L; i++) {
      rowFactor[i] = decideTemperatureStep() * i;
    }
    return rowFactor;
  }

  vector<int> decideColFactor() {
    vector<int> colFactor(L);
    for (int i = 0; i < L; i++) {
      colFactor[i] = decideTemperatureStep() * i;
    }
    return colFactor;
  }

  vector<vector<int>> create_temperatureRowCol() {
    vector<vector<int>> temperature(L, vector<int>(L, 0));
    vector<int> rowFactor = decideRowFactor();
    vector<int> colFactor = decideColFactor();
    // rowFactorとcolFactorの和を設定温度として設定する
    for (int i = 0; i < L; i++) {
      for (int j = 0; j < L; j++) {
        temperature[i][j] = rowFactor[i] + colFactor[j];
      }
    }
    return temperature;
  }

  constexpr pair<int, int> decideTemperatureRange() {
    if (S == 1) {
      return {475, 525};
    } else if (S <= 4) {
      return {470, 530};
    } else if (S <= 9) {
      return {465, 535};
    } else if (S <= 16) {
      return {460, 540};
    } else if (S <= 25) {
      return {455, 545};
    } else if (S <= 36) {
      return {425, 575};
    } else if (S <= 49) {
      return {375, 625};
    } else if (S <= 64) {
      return {375, 625};
    } else if (S <= 81) {
      return {300, 700};
    } else if (S <= 100) {
      return {300, 700};
    } else if (S <= 225) {
      return {200, 800};
    } else if (S <= 324) {
      return {150, 850};
    } else if (S <= 400) {
      return {100, 900};
    }
    return {0, 1000};
  }

  constexpr int decidePlacementNumber() {
    if (S <= 5) {
      return 4;
    } else {
      return 8;
    }
  }

  // 近傍をループするたびにリセットのため実行
  void setNeiborhoodPointer() {
    if (decidePlacementNumber() == 4) {
      dxPointer = dx, dyPointer = dy;
    } else if (decidePlacementNumber() == 13) {
      dxPointer = dx2, dyPointer = dy2;
    } else if (decidePlacementNumber() == 9) {
      dxPointer = dx3, dyPointer = dy3;
    } else if (decidePlacementNumber() == 2) {
      dxPointer = dx4, dyPointer = dy4;
    } else if (decidePlacementNumber() == 5) {
      dxPointer = dx5, dyPointer = dy5;
    } else if (decidePlacementNumber() == 3) {
      dxPointer = dx6, dyPointer = dy6;
    } else if (decidePlacementNumber() == 1) {
      dxPointer = dx7, dyPointer = dy7;
    } else if (decidePlacementNumber() == 8) {
      dxPointer = dx8, dyPointer = dy8;
    }
  }

  pair<int, int> calcPositionInTable(int ny, int nx) {
    int exceed = 0;
    if (ny < 0) {
      exceed = 0 - ny;
      ny = L - exceed;
    } else if (ny >= L) {
      exceed = ny - (L - 1);
      ny = -1 + exceed;
    }

    if (nx < 0) {
      exceed = 0 - nx;
      nx = L - exceed;
    } else if (nx >= L) {
      exceed = nx - (L - 1);
      nx = -1 + exceed;
    }

    return {ny, nx};
  }

  constexpr int scaleNeighborhood() { return 1; }

  constexpr int tryNumberSetTemperature() { return 1000; }

  int calcL2Distance(int idx1, int idx2, const vector<vector<int>> &temperature) {
    int sum = 0;
    setNeiborhoodPointer();
    for (int dir = 0; dir < decidePlacementNumber(); dir++) {
      int ny1 = landing_pos[idx1].y + *dyPointer * scaleNeighborhood();
      int nx1 = landing_pos[idx1].x + *dxPointer * scaleNeighborhood();
      int ny2 = landing_pos[idx2].y + *dyPointer * scaleNeighborhood();
      int nx2 = landing_pos[idx2].x + *dxPointer * scaleNeighborhood();

      auto [nyIdx1, nxIdx1] = calcPositionInTable(ny1, nx1);
      auto [nyIdx2, nxIdx2] = calcPositionInTable(ny2, nx2);

      // 比較対象のCellの温度が決定していたら
      if (setTempCell[nyIdx2][nxIdx2]) {
        sum += (temperature[nyIdx1][nxIdx1] - temperature[nyIdx2][nxIdx2]) *
               (temperature[nyIdx1][nxIdx1] - temperature[nyIdx2][nxIdx2]);
      }
      dyPointer++, dxPointer++;
    }
    return sum;
  }

  constexpr int L2DistanceLowerLimit() {
    int lowerLimit = S * S;
    // std::cerr << lowerLimit << std::endl;
    return lowerLimit;
  }

  vector<vector<int>> create_temperature() {
    vector<vector<int>> temperature(L, vector<int>(L, 500));

    for (int i = 0; i < N; i++) {
      int tryTime = tryNumberSetTemperature();
      while (tryTime--) {
        setNeiborhoodPointer();
        for (int dir = 0; dir < decidePlacementNumber(); dir++) {
          int ny = landing_pos[i].y + *dyPointer * scaleNeighborhood();
          int nx = landing_pos[i].x + *dxPointer * scaleNeighborhood();

          auto [ny2, nx2] = calcPositionInTable(ny, nx);
          auto [tempMin, tempMax] = decideTemperatureRange();

          if (!setTempCell[ny2][nx2]) {
            temperature[ny2][nx2] = get_rand_range(tempMin, tempMax);
            setTempCell[ny2][nx2] = true; // 焼きなまし用
          }
          dyPointer++, dxPointer++;
        }

        bool ok = true;
        for (int j = 0; j < i; j++) {
          int L2Distance = calcL2Distance(i, j, temperature);
          if (L2Distance < L2DistanceLowerLimit()) {
            // std::cerr << "L2Distance: " << L2Distance << "  i: " << i << "  j: " << j <<
            // std::endl;
            ok = false;
          }
          // std::cerr << "L2Distance: " << L2Distance << std::endl;
        }
        if (ok) {
          // std::cerr << "i: " << i << "  ok!" << std::endl;
          break;
        }
      }
    }

    return temperature;
  }

  ll calcPlacementCost(const vector<vector<int>> &temperature) {
    ll costSum = 0;

    for (int i = 0; i < L; i++) {
      for (int j = 0; j < L; j++) {
        costSum += (temperature[i][j] - temperature[(i + 1) % L][j]) *
                   (temperature[i][j] - temperature[(i + 1) % L][j]);
        costSum += (temperature[i][j] - temperature[i][(j + 1) % L]) *
                   (temperature[i][j] - temperature[i][(j + 1) % L]);
      }
    }

    // std::cerr << "calculated cost sum: " << costSum << std::endl;

    return costSum;
  }

  void simulatedAnnealingTempBoard(vector<vector<int>> &temperature) {
    const double TIME_LIMIT = 3.0;
    auto startTime = chrono::system_clock::now();
    double startTemp = 5000, endTemp = 100;
    double nowTime = 0.0;

    int simulatingCnt = 0;
    ll bestScore = -calcPlacementCost(temperature);
    // std::cerr << "initial best score: " << -bestScore << std::endl;
    while (nowTime < TIME_LIMIT) {
      const double progressRatio = nowTime / TIME_LIMIT;
      const double temp = startTemp + (endTemp - startTemp) * progressRatio;

      simulatingCnt++;

      int newRowIndex = get_rand_range(0, L - 1);
      int newColIndex = get_rand_range(0, L - 1);

      if (setTempCell[newRowIndex][newColIndex]) {
        nowTime =
            (chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - startTime)
                 .count() *
             1e-6);
        continue;
      }

      auto [tempMin, tempMax] = decideTemperatureRange();
      int newTemp = get_rand_range(tempMin, tempMax);

      ll plusValue = 0, minusValue = 0;
      if (newRowIndex == 0) {
        plusValue += (newTemp - temperature[newRowIndex + 1][newColIndex]) *
                     (newTemp - temperature[newRowIndex + 1][newColIndex]);
        plusValue += (newTemp - temperature[L - 1][newColIndex]) *
                     (newTemp - temperature[L - 1][newColIndex]);

        minusValue +=
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex + 1][newColIndex]) *
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex + 1][newColIndex]);
        minusValue += (temperature[newRowIndex][newColIndex] - temperature[L - 1][newColIndex]) *
                      (temperature[newRowIndex][newColIndex] - temperature[L - 1][newColIndex]);
      } else if (newRowIndex == L - 1) {
        plusValue +=
            (newTemp - temperature[0][newColIndex]) * (newTemp - temperature[0][newColIndex]);
        plusValue += (newTemp - temperature[newRowIndex - 1][newColIndex]) *
                     (newTemp - temperature[newRowIndex - 1][newColIndex]);

        minusValue += (temperature[newRowIndex][newColIndex] - temperature[0][newColIndex]) *
                      (temperature[newRowIndex][newColIndex] - temperature[0][newColIndex]);
        minusValue +=
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex - 1][newColIndex]) *
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex - 1][newColIndex]);
      } else {
        plusValue += (newTemp - temperature[newRowIndex + 1][newColIndex]) *
                     (newTemp - temperature[newRowIndex + 1][newColIndex]);
        plusValue += (newTemp - temperature[newRowIndex - 1][newColIndex]) *
                     (newTemp - temperature[newRowIndex - 1][newColIndex]);

        minusValue +=
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex + 1][newColIndex]) *
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex + 1][newColIndex]);
        minusValue +=
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex - 1][newColIndex]) *
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex - 1][newColIndex]);
      }

      if (newColIndex == 0) {
        plusValue += (newTemp - temperature[newRowIndex][newColIndex + 1]) *
                     (newTemp - temperature[newRowIndex][newColIndex + 1]);
        plusValue += (newTemp - temperature[newRowIndex][L - 1]) *
                     (newTemp - temperature[newRowIndex][L - 1]);

        minusValue +=
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][newColIndex + 1]) *
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][newColIndex + 1]);
        minusValue += (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][L - 1]) *
                      (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][L - 1]);
      } else if (newColIndex == L - 1) {
        plusValue +=
            (newTemp - temperature[newRowIndex][0]) * (newTemp - temperature[newRowIndex][0]);
        plusValue += (newTemp - temperature[newRowIndex][newColIndex - 1]) *
                     (newTemp - temperature[newRowIndex][newColIndex - 1]);

        minusValue += (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][0]) *
                      (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][0]);
        minusValue +=
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][newColIndex - 1]) *
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][newColIndex - 1]);
      } else {
        plusValue += (newTemp - temperature[newRowIndex][newColIndex + 1]) *
                     (newTemp - temperature[newRowIndex][newColIndex + 1]);
        plusValue += (newTemp - temperature[newRowIndex][newColIndex - 1]) *
                     (newTemp - temperature[newRowIndex][newColIndex - 1]);

        minusValue +=
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][newColIndex + 1]) *
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][newColIndex + 1]);
        minusValue +=
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][newColIndex - 1]) *
            (temperature[newRowIndex][newColIndex] - temperature[newRowIndex][newColIndex - 1]);
      }

      double deltaScore = -(plusValue - minusValue);
      // double prob = exp(deltaScore / temp);
      if (deltaScore >= 0 /* prob > get_rand_range_real(0.0, 1.0) */) {
        // 受容
        bestScore += deltaScore;
        temperature[newRowIndex][newColIndex] = newTemp;
      } else {
        // 棄却 特に何もしない
      }

      nowTime =
          (chrono::duration_cast<chrono::microseconds>(chrono::system_clock::now() - startTime)
               .count() *
           1e-6);
    }
    // std::cerr << "Simulate count: " << simulatingCnt << std::endl;
    // std::cerr << "Result Best Score: " << -bestScore << std::endl;
  }

  vector<vector<int>> createRowDirectionAccumulatedSum(const vector<vector<int>> &temperature) {
    vector<vector<int>> accumulatedSumRow(L, vector<int>(L + 1));
    for (int i = 0; i < L; i++) {
      for (int j = 0; j < L; j++) {
        accumulatedSumRow[i][j + 1] = accumulatedSumRow[i][j] + temperature[i][j];
      }
    }
    return accumulatedSumRow;
  }

  vector<vector<int>> createColDirectionAccumulatedSum(const vector<vector<int>> &temperature) {
    vector<vector<int>> accumulatedSumCol(L + 1, vector<int>(L));
    for (int j = 0; j < L; j++) {
      for (int i = 0; i < L; i++) {
        accumulatedSumCol[i + 1][j] = accumulatedSumCol[i][j] + temperature[i][j];
      }
    }
    return accumulatedSumCol;
  }

  // まずは動かす距離は5で決め打ち
  int decideMoveDistance() { return 5; }

  int predictRowFactor(const vector<vector<int>> &accumulatedSumRowDirection, int cellNumber) {
    int valueSum = 0;
    for (int i = 0; i < decideMoveDistance(); i++) {
      int measuredValue = judge.measure(cellNumber, 0, i);
      valueSum += measuredValue;
    }

    int minDiff = INT_INF;
    int ansRow = -1;
    for (int i_out = 0; i_out < N; i_out++) {
      const auto &pos = landing_pos[i_out];
      int temperatureSum = 0;
      if (pos.x + decideMoveDistance() >= L) {
        int exceed = pos.x + decideMoveDistance() - (L - 1);
        temperatureSum +=
            accumulatedSumRowDirection[pos.y][L] - accumulatedSumRowDirection[pos.y][pos.x];
        temperatureSum +=
            accumulatedSumRowDirection[pos.y][exceed] - accumulatedSumRowDirection[pos.y][0];
      } else {
        temperatureSum = accumulatedSumRowDirection[pos.y][pos.x + decideMoveDistance()] -
                         accumulatedSumRowDirection[pos.y][pos.x];
      }

      int diff = abs(temperatureSum - valueSum);
      if (diff < minDiff) {
        minDiff = diff;
        ansRow = pos.y;
      }
    }

    if (ansRow == -1) {
      std::cerr << "predictRowFactor: something wrong." << std::endl;
    }
    return ansRow;
  }

  int predictColFactor(const vector<vector<int>> &accumulatedSumColDirection, int cellNumber) {
    int valueSum = 0;
    for (int i = 0; i < decideMoveDistance(); i++) {
      int measuredValue = judge.measure(cellNumber, i, 0);
      valueSum += measuredValue;
    }

    int minDiff = INT_INF;
    int ansCol = -1;
    for (int i_out = 0; i_out < N; i_out++) {
      const auto &pos = landing_pos[i_out];
      int temperatureSum = 0;
      if (pos.y + decideMoveDistance() >= L) {
        int exceed = pos.y + decideMoveDistance() - (L - 1);
        temperatureSum +=
            accumulatedSumColDirection[L][pos.x] - accumulatedSumColDirection[pos.y][pos.x];
        temperatureSum +=
            accumulatedSumColDirection[exceed][pos.x] - accumulatedSumColDirection[0][pos.x];
      } else {
        temperatureSum = accumulatedSumColDirection[pos.y + decideMoveDistance()][pos.x] -
                         accumulatedSumColDirection[pos.y][pos.x];
      }

      int diff = abs(temperatureSum - valueSum);
      if (diff < minDiff) {
        minDiff = diff;
        ansCol = pos.x;
      }
    }

    if (ansCol == -1) {
      std::cerr << "predictColFactor: something wrong." << std::endl;
    }
    return ansCol;
  }

  vector<int> predictRowCol(const vector<vector<int>> &temperature) {
    vector<int> estimate(N);

    vector<vector<int>> accumulatedSumRowDirection = createRowDirectionAccumulatedSum(temperature);
    vector<vector<int>> accumulatedSumColDirection = createColDirectionAccumulatedSum(temperature);

    for (int i_in = 0; i_in < N; i_in++) {
      // you can output comment
      // cout << "# measure i=" << i_in << " y=0 x=0" << endl; //
      // これと同じものをRowFactorとColFactorに入れる
      int predictRow = predictRowFactor(accumulatedSumRowDirection, i_in);
      int predictCol = predictColFactor(accumulatedSumColDirection, i_in);
      std::cerr << "i_in: " << i_in << "  predictRow: " << predictRow
                << "  predictCol: " << predictCol << std::endl;

      for (int i_out = 0; i_out < N; i_out++) {
        const Pos &pos = landing_pos[i_out];
        if (pos.y == predictRow && pos.x == predictCol) {
          std::cerr << "estimate[i_in]: " << i_out << std::endl;
          estimate[i_in] = i_out;
          break;
        }
      }
    }
    return estimate;
  }

  // L, N, Sで変更出来るようにする
  constexpr int decideNumberOfMeasure() {
    if (S == 1) {
      return 1;
    } else if (S <= 4) {
      return 2;
    } else if (S <= 9) {
      return 2;
    } else if (S <= 16) {
      return 4;
    } else if (S <= 25) {
      return 5;
    } else if (S <= 36) {
      return 5;
    } else if (S <= 49) {
      return 5;
    } else if (S <= 64) {
      return 5;
    } else if (S <= 81) {
      return 6;
    } else if (S <= 100) {
      return 7;
    } else if (S <= 121) {
      return 8;
    } else if (S <= 144) {
      return 8;
    } else if (S <= 169) {
      return 8;
    } else if (S <= 196) {
      return 8;
    } else if (S <= 225) {
      return 9;
    } else if (S <= 334) {
      return 10;
    } else {
      return 12;
    }
  }

  vector<int> predict(const vector<vector<int>> &temperature) {
    vector<int> estimate(N);
    vector<bool> alreadyEstimated(N);
    for (int i_in = 0; i_in < N; i_in++) {
      vector<int> diffSquareSum(N);

      setNeiborhoodPointer();
      for (int dir = 0; dir < decidePlacementNumber(); dir++) {
        cout << "# measure i =" << i_in << " y = " << *dyPointer * scaleNeighborhood()
             << " x = " << *dxPointer * scaleNeighborhood() << endl;

        int measuredValueSum = 0;
        for (int m = 0; m < decideNumberOfMeasure(); m++) {
          int measuredValueNow = judge.measure(i_in, *dyPointer * scaleNeighborhood(),
                                               *dxPointer * scaleNeighborhood());
          measuredValueSum += measuredValueNow;
          /*
          if (measuredValueNow > 0 && measuredValueNow < 1000) {
            measuredValueSum += measuredValueNow;
          } else if (measuredValueNow == 0) {
            measuredValueNow += get_rand_range(0, S);
            measuredValueSum += measuredValueNow;
          } else {
            measuredValueNow -= get_rand_range(0, S);
            measuredValueSum += measuredValueNow;
          }
          */
        }
        int measuredValue = measuredValueSum / decideNumberOfMeasure();

        for (int i_out = 0; i_out < N; i_out++) {
          const Pos &pos = landing_pos[i_out];
          int ny = pos.y + *dyPointer * scaleNeighborhood(),
              nx = pos.x + *dxPointer * scaleNeighborhood();
          auto [ny2, nx2] = calcPositionInTable(ny, nx);

          int diffSquare = abs(temperature[ny2][nx2] - measuredValue) *
                           abs(temperature[ny2][nx2] - measuredValue);
          diffSquareSum[i_out] += diffSquare;
        }
        dyPointer++, dxPointer++;
      }

      int minDiff = INT_INF;
      for (int i_out = 0; i_out < N; i_out++) {
        int diff = diffSquareSum[i_out];
        if (diff < minDiff && !alreadyEstimated[i_out]) {
          minDiff = diff;
          estimate[i_in] = i_out;
        }
      }
      alreadyEstimated[estimate[i_in]] = true;
    }
    return estimate;
  }
};

int main() {
  ios::sync_with_stdio(false);
  cin.tie(nullptr);

  int L, N, S;
  cin >> L >> N >> S;
  vector<Pos> landingPos(N);
  for (int i = 0; i < N; i++) {
    cin >> landingPos[i].y >> landingPos[i].x;
  }

  Solver solver(L, N, S, landingPos);
  solver.solve();

  return 0;
}
