#ifndef SEGMENT_TREE_HPP_
#define SEGMENT_TREE_HPP_

#include <bits/stdc++.h>

using namespace std;

template <typename Monoid> class SegmentTree {
public:
  SegmentTree(const std::vector<Monoid> &v, std::function::<Monoid(Monoid, Monoid)>fx, Monoid ex)
      : fx_(fx), ex_(ex) {
    auto n = 1;
    const auto vector_size = static_cast<int>(v.size());
    while (n < vector_size) {
      n *= 2;
    }

    leaf_size_ = n;
    node_.resize(2 * n - 1, ex_);

    for (int i = 0; i < vector_size; i++) {
      set(i, v[i]);
    }

    build();
  }

  void update(int index, Monoid v) {
    auto data_index = index + leaf_size_ - 1;
    node_[data_index] = v;
    while (data_index > 0) {
      data_index = (data_index - 1) / 2;
      data_index = fx_(node_[data_index * 2 + 1], node_[data_index * 2 + 2]);
    }
  }

  Monoid query(int s, int t) { return query_sub(s, t, 0, 0, leaf_size_); }

  void set(int index, Monoid v) { node_[index + leaf_size_ - 1] = v; }

  void build() {
    for (int i = leaf_size_ - 2; i >= 0; --i) {
      node_[i] = fx_(node_[i * 2 + 1], node_[i * 2 + 2]);
    }
  }

private:
  Monoid query_sub(int s, int t, int now_data_index, int now_l, int now_r) {
    if (now_r <= s || t <= now_l) {
      return ex_; // 範囲外
    } else if (s <= now_l && now_r <= t) {
      return node_[now_data_index]; // 範囲内
    } else {
      auto left_child = query_sub(s, t, now_data_index * 2 + 1, now_l, (now_l + now_r) / 2);
      auto right_child = query_sub(s, t, now_data_index * 2 + 2, (now_l + now_r) / 2, now_r);
      return fx_(left_child, right_child); // 範囲の一部
    }
  }
  std::function<Monoid(Monoid, Monoid)> fx_; // 二項演算
  Monoid ex_;                                // 単位元
  std::vector<Monoid> node_;
  int leaf_size_; // 葉の数
};

#endif
