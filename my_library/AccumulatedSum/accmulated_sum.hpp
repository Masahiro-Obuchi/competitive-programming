#ifndef ACCUMULATED_SUM_HPP_
#define ACCUMULATED_SUM_HPP_

#include <bits/stdc++.h>

using namespace std;

template <typename T> struct AccumulatedSum {
public:
  AccumulatedSum(const vector<T> &data) {
    sum.resize(data.size() + 1);
    for (size_t i = 0; i < data.size(); ++i) {
      sum[i + 1] = sum[i] + data[i];
    }
  }

  // 区間[left, right)の和を取得
  T query(int left, int right) const {
    if (left < 0 || right < 0 || left >= right || right > static_cast<int>(sum.size()) - 1) {
      return T{}; // デフォルト値を返す
    }
    return sum[right] - sum[left];
  }

  // 単一要素の値を取得（元の配列の値）
  T get(int index) const { return query(index, index + 1); }

  // サイズを取得
  size_t size() const { return sum.size() - 1; }

  const vector<T> &get_sum_array() const { return sum; }

private:
  vector<T> sum;
};

template <typename T> struct AccumulatedSum2D {
public:
  AccumulatedSum2D(const vector<vector<T>> &data) {
    int row_size = data.size();
    int col_size = data[0].size();

    if (row_size == 0 || col_size == 0) {
      sum.resize(1, vector<T>(1, T{})); // 空の配列の場合、1x1の累積和を初期化
      return;
    }

    sum.resize(row_size + 1, vector<T>(col_size + 1, T{}));
    for (int i = 0; i < row_size; ++i) {
      for (int j = 0; j < col_size; ++j) {
        sum[i + 1][j + 1] = data[i][j] + sum[i][j + 1] + sum[i + 1][j] - sum[i][j];
      }
    }
  }

  // 区間[(r1, c1), (r2, c2))の和を取得（右下は含まない）
  T query(int r1, int c1, int r2, int c2) const {
    if (r1 < 0 || c1 < 0 || r2 <= r1 || c2 <= c1 || r2 > static_cast<int>(sum.size()) - 1 ||
        c2 > static_cast<int>(sum[0].size()) - 1) {
      return T{}; // デフォルト値を返す
    }
    return sum[r2][c2] - sum[r1][c2] - sum[r2][c1] + sum[r1][c1];
  }

  // 単一要素の値を取得（元の配列の値）
  T get(int r, int c) const {
    if (r < 0 || c < 0 || r >= static_cast<int>(sum.size()) - 1 ||
        c >= static_cast<int>(sum[0].size()) - 1) {
      return T{}; // デフォルト値を返す
    }
    return query(r, c, r + 1, c + 1);
  }

  // サイズを取得
  size_t rows() const { return sum.size() - 1; }
  size_t cols() const { return sum.empty() ? 0 : sum[0].size() - 1; }

  const vector<vector<T>> &get_sum_array() const { return sum; }

private:
  vector<vector<T>> sum; // 累積和の2D配列
};

template <typename T> struct Imos {
public:
  Imos(size_t size) : imos(size + 1, T{}), built(false) {}

  void add(int left, int right, T value) {
    if (built) {
      throw runtime_error("Cannot add after build() has been called");
    }

    if (left < 0 || right < 0 || left >= right || left >= static_cast<int>(imos.size()) - 1 ||
        right > static_cast<int>(imos.size()) - 1) {
      return; // 範囲外は無視
    }
    imos[left] += value;
    if (right < static_cast<int>(imos.size()) - 1) {
      imos[right] -= value;
    }
  }

  vector<T> build() {
    if (built) {
      throw runtime_error("build() can only be called once");
    }
    // 累積和を計算
    for (size_t i = 0; i < imos.size() - 1; i++) {
      imos[i + 1] += imos[i];
    }
    built = true;

    // 番兵を除いて返す
    return vector<T>(imos.begin(), imos.end() - 1);
  }

  size_t size() const { return imos.size() - 1; }

private:
  vector<T> imos;
  bool built; // 構築済みフラグ
};

template <typename T> struct Imos2D {
public:
  Imos2D(size_t rows, size_t cols) : imos(rows + 1, vector<T>(cols + 1, T{})), built(false) {}

  void add(int r1, int c1, int r2, int c2, T value) {
    if (built) {
      throw runtime_error("Cannot add after build() has been called");
    }
    if (r1 < 0 || c1 < 0 || r2 <= r1 || c2 <= c1 || r2 >= static_cast<int>(imos.size()) ||
        c2 >= static_cast<int>(imos[0].size())) {
      return;
    }

    imos[r1][c1] += value;
    imos[r1][c2] -= value;
    imos[r2][c1] -= value;
    imos[r2][c2] += value;
  }

  vector<vector<T>> build() {
    if (built) {
      throw runtime_error("build() can only be called once");
    }
    for (size_t i = 0; i < imos.size(); i++) {
      for (size_t j = 1; j < imos[0].size(); j++) {
        imos[i][j] += imos[i][j - 1];
      }
    }
    for (size_t i = 1; i < imos.size(); i++) {
      for (size_t j = 0; j < imos[0].size(); j++) {
        imos[i][j] += imos[i - 1][j];
      }
    }
    built = true;

    // 番兵を除いて返す（各行と各列から最後の要素を除く）
    vector<vector<T>> result(imos.size() - 1);
    for (size_t i = 0; i < imos.size() - 1; i++) {
      result[i] = vector<T>(imos[i].begin(), imos[i].end() - 1);
    }
    return result;
  }

  // サイズ取得メソッドを追加
  size_t rows() const { return imos.size() - 1; }
  size_t cols() const { return imos.empty() ? 0 : imos[0].size() - 1; }

private:
  vector<vector<T>> imos;
  bool built; // 構築済みフラグ
};

#endif // ACCUMULATED_SUM_HPP_
