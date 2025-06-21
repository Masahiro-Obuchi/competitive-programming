#ifndef SEGMENT_TREE_HPP_
#define SEGMENT_TREE_HPP_

#include <bits/stdc++.h>

using namespace std;

template <typename Monoid> class SegmentTree {
public:
  SegmentTree(const std::vector<Monoid> &v, std::function<Monoid(Monoid, Monoid)> fx, Monoid ex)
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

template <typename Monoid, typename Lazy> class LazySegmentTree {
public:
  LazySegmentTree(const std::vector<Monoid> &v, std::function<Monoid(Monoid, Monoid)> fx, Monoid ex,
                  std::function<Lazy(Lazy, Lazy)> lazy_fx, Lazy lazy_ex,
                  std::function<Monoid(Monoid, Lazy, int)> apply)
      : fx_(fx), ex_(ex), lazy_fx_(lazy_fx), lazy_ex_(lazy_ex), apply_(apply) {
    auto n = 1;
    const auto vector_size = static_cast<int>(v.size());
    while (n < vector_size) {
      n *= 2;
    }

    leaf_size_ = n;
    node_.resize(2 * n - 1, ex_);
    lazy_.resize(2 * n - 1, lazy_ex_);

    for (int i = 0; i < vector_size; i++) {
      set(i, v[i]);
    }

    build();
  }

  void lazy_update(int s, int t, Lazy v) { lazy_update_sub(s, t, v, 0, 0, leaf_size_); }

  Monoid query(int s, int t) { return query_sub(s, t, 0, 0, leaf_size_); }

private:
  void push(int now_data_index, int now_l, int now_r) {
    if (lazy_[now_data_index] != lazy_ex_) {
      node_[now_data_index] = apply_(node_[now_data_index], lazy_[now_data_index], now_r - now_l);

      if (now_data_index < leaf_size_ - 1) { // 葉でない場合
        auto left_child = now_data_index * 2 + 1;
        auto right_child = now_data_index * 2 + 2;
        lazy_[left_child] = lazy_fx_(lazy_[left_child], lazy_[now_data_index]);
        lazy_[right_child] = lazy_fx_(lazy_[right_child], lazy_[now_data_index]);
      }

      lazy_[now_data_index] = lazy_ex_;
    }
  }

  void lazy_update_sub(int s, int t, Lazy v, int now_data_index, int now_l, int now_r) {
    push(now_data_index, now_l, now_r);

    if (now_r <= s || t <= now_l) {
      return; // 範囲外
    } else if (s <= now_l && now_r <= t) {
      lazy_[now_data_index] = lazy_fx_(lazy_[now_data_index], v);
      push(now_data_index, now_l, now_r);
    } else {
      auto left_child = now_data_index * 2 + 1;
      auto right_child = now_data_index * 2 + 2;
      lazy_update_sub(s, t, v, left_child, now_l, (now_l + now_r) / 2);
      lazy_update_sub(s, t, v, right_child, (now_l + now_r) / 2, now_r);

      push(left_child, now_l, (now_l + now_r) / 2);
      push(right_child, (now_l + now_r) / 2, now_r);
      node_[now_data_index] = fx_(node_[left_child], node_[right_child]);
    }
  }

  Monoid query_sub(int s, int t, int now_data_index, int now_l, int now_r) {
    if (now_r <= s || t <= now_l) {
      return ex_; // 範囲外
    } else if (s <= now_l && now_r <= t) {
      push(now_data_index, now_l, now_r);
      return node_[now_data_index]; // 範囲内
    } else {
      push(now_data_index, now_l, now_r);
      auto left_child = query_sub(s, t, now_data_index * 2 + 1, now_l, (now_l + now_r) / 2);
      auto right_child = query_sub(s, t, now_data_index * 2 + 2, (now_l + now_r) / 2, now_r);
      return fx_(left_child, right_child); // 範囲の一部
    }
  }

  void set(int index, Monoid v) { node_[index + leaf_size_ - 1] = v; }

  void build() {
    for (int i = leaf_size_ - 2; i >= 0; --i) {
      node_[i] = fx_(node_[i * 2 + 1], node_[i * 2 + 2]);
    }
  }

  std::function<Monoid(Monoid, Monoid)> fx_;       // 二項演算
  Monoid ex_;                                      // 単位元
  std::function<Lazy(Lazy, Lazy)> lazy_fx_;        // 遅延伝播の二項演算
  Lazy lazy_ex_;                                   // 遅延伝播の単位元
  std::function<Monoid(Monoid, Lazy, int)> apply_; // 遅延値の適用
  std::vector<Monoid> node_;
  std::vector<Lazy> lazy_;
  int leaf_size_; // 葉の数
};

#endif
