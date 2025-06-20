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

  vector<T> sum;
};

#endif // ACCUMULATED_SUM_HPP_
