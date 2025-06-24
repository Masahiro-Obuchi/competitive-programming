#ifndef SYAKUTORI_HPP_
#define SYAKUTORI_HPP_

#include <bits/stdc++.h>
using namespace std;

/**
 * @brief 尺取り法の抽象化
 * @tparam T 配列の要素型
 * @tparam R 結果の型
 * @param vec 入力配列
 * @param condition 区間が条件を満たすかを判定する関数
 * @param op rightを右に動かしたときの結果更新関数
 * @param inverse_op leftを右に動かしたときの結果更新関数
 * @param identity 空区間での初期値
 * @param process 各区間での処理関数 (left, right) -> void
 */
template <typename T, typename R>
void syakutori_with_process(const vector<T> &vec, const function<bool(const R &)> &condition,
                            const function<R(const R &, const T &)> &op,
                            const function<R(const R &, const T &)> &inverse_op, const R &identity,
                            const function<void(int, int, const R &)> &process) {

  int n = vec.size();
  int right = 0;
  R result = identity;

  for (int left = 0; left < n; left++) {
    while (right < n) {
      R new_result = op(result, vec[right]);
      if (!condition(new_result)) {
        break;
      }
      result = new_result;
      right++;
    }

    process(left, right, result);

    // leftを一つ右に進める準備
    if (right == left) {
      right++;
    } else {
      result = inverse_op(result, vec[left]);
    }
  }
}

/**
 * @brief 条件を満たす部分配列の個数を求める
 */
template <typename T, typename R>
long long syakutori_count(const vector<T> &vec, const function<bool(const R &)> &condition,
                          const function<R(const R &, const T &)> &op,
                          const function<R(const R &, const T &)> &inverse_op, const R &identity) {
  long long ans = 0;
  syakutori<T, R>(vec, condition, op, inverse_op, identity,
                  [&ans](int left, int right, const R &result) { ans += right - left; });
  return ans;
}

/**
 * @brief 条件を満たす最長区間の長さを求める
 */
template <typename T, typename R>
int syakutori_max_length(const vector<T> &vec, const function<bool(const R &)> &condition,
                         const function<R(const R &, const T &)> &op,
                         const function<R(const R &, const T &)> &inverse_op, const R &identity) {
  int max_length = 0;
  syakutori<T, R>(vec, condition, op, inverse_op, identity,
                  [&max_length](int left, int right, const R &result) {
                    max_length = max(max_length, right - left);
                  });
  return max_length;
}

#endif // SYAKUTORI_HPP_
