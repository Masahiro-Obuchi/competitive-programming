#ifndef BINARY_SEARCH_HPP_
#define BINARY_SEARCH_HPP_

#include <bits/stdc++.h>
using namespace std;

template <typename T, typename F> T bisect(T ok, T ng, F predicate) {
  // 初期条件チェック
  if (!predicate(ok)) {
    return ok;
  }

  while (abs(ng - ok) > 1) { // okとngの差が1未満になるまで繰り返す
    T mid = std::midpoint(ok, ng);
    if (predicate(mid)) {
      ok = mid;
    } else {
      ng = mid;
    }
  }

  return ok;
}

#endif
