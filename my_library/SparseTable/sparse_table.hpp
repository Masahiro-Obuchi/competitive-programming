#ifndef SPARSE_TABLE_HPP_
#define SPARSE_TABLE_HPP_

#include <bits/stdc++.h>

using namespace std;

template <typename Band> class SparseTable {
public:
  SparseTable(const std::vector<Band> &v, std::function<Band(Band, Band)> fx) : fx_(fx) {
    const auto vector_size = static_cast<int>(v.size());

    // precompute log2 values
    log_table_.resize(vector_size + 1);
    for (auto i = 2; i < vector_size + 1; i++) {
      log_table_[i] = log_table_[i >> 1] + 1;
    }

    table_.resize(vector_size);
    for (auto i = 0; i < vector_size; i++) {
      table_[i].resize(log_table_[vector_size] + 1);
    }

    // initialize table_[i][0]
    for (auto i = 0; i < vector_size; i++) {
      table_[i][0] = v[i];
    }

    for (auto k = 1; (1 << k) <= vector_size; k++) {
      for (auto i = 0; i + (1 << k) <= vector_size; i++) {
        auto first_half = table_[i][k - 1];
        auto second_half = table_[i + (1 << (k - 1))][k - 1];
        // combine the two halves using the binary operation fx_
        table_[i][k] = fx_(first_half, second_half);
      }
    }
  }

  Band query(int begin, int end) {
    auto s = begin;
    auto t = end - 1;
    auto length_of_interval = t - s + 1;
    auto k = log_table_[length_of_interval];
    return fx_(table_[s][k], table_[t - (1 << k) + 1][k]);
  }

private:
  std::function<Band(Band, Band)> fx_; // 二項演算
  std::vector<std::vector<Band>> table_;
  std::vector<int> log_table_;
};

#endif
