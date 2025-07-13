#ifndef INTEGER_DIVISOR_ENUMERATION_HPP_
#define INTEGER_DIVISOR_ENUMERATION_HPP_

#include <bits/stdc++.h>
using namespace std;

// 素数判定
template <typename T> bool isPrime(T N) {
  for (T i = 2; i * i <= N; i++) {
    if (N % i == 0) {
      return false;
    }
  }
  return true;
}

// 素数列挙　エラトステネスの篩
template <typename T> vector<T> Eratosthenes(const T N) {
  vector<bool> is_prime(N + 1);
  for (int i = 2; i <= N; i++) {
    is_prime[i] = true;
  }
  vector<T> P;
  for (int i = 2; i <= N; i++) {
    if (is_prime[i]) {
      for (int j = 2 * i; j <= N; j += i) {
        is_prime[j] = false;
      }
      P.emplace_back(i);
    }
  }

  return P;
}

// 素因数分解
template <typename T> vector<pair<T, int>> factorize(T N) {
  vector<pair<T, int>> res;
  for (T i = 2; i * i <= N; i++) {
    if (N % i != 0) {
      continue;
    }
    res.emplace_back(i, 0);
    while (N % i == 0) {
      N /= i;
      res.back().second++;
    }
  }
  if (N != 1) {
    res.emplace_back(N, 1);
  }
  return res;
}

// 高速な素因数分解
class FastFactorization {
public:
  FastFactorization(int n = kMaxN) : isPrime(n + 1), minFactor(n + 1) { preprocess(n + 1); }

  vector<pair<int, int>> prime_factor(int n) {
    vector<pair<int, int>> res;
    while (n != 1) {
      int prime = minFactor[n];
      int exp = 0;
      while (minFactor[n] == prime) {
        ++exp;
        n /= prime;
      }
      res.push_back(make_pair(prime, exp));
    }
    return res;
  }

  vector<int> prime_list() const { return primeList; }

  bool is_prime(int n) const {
    if (n < 0 || n >= kMaxN) {
      throw out_of_range("n must be in range [0, " + to_string(kMaxN) + ")");
    }
    return isPrime[n];
  }

  int min_factor(int n) const {
    if (n < 0 || n >= kMaxN) {
      throw out_of_range("n must be in range [0, " + to_string(kMaxN) + ")");
    }
    return minFactor[n];
  }

private:
  void preprocess(int n) {
    n = min(n, kMaxN);
    for (int i = 0; i < n; ++i) {
      isPrime[i] = true, minFactor[i] = -1;
    }

    isPrime[0] = false;
    isPrime[1] = false;

    minFactor[0] = 0;
    minFactor[1] = 1;

    for (int i = 2; i < n; ++i) {
      if (isPrime[i]) {
        minFactor[i] = i;
        primeList.push_back(i);
        for (int j = i * 2; j < n; j += i) {
          isPrime[j] = false;
          if (minFactor[j] == -1)
            minFactor[j] = i;
        }
      }
    }
  }

  constexpr static int kMaxN = 15001000;
  vector<bool> isPrime;
  vector<int> minFactor;
  vector<int> primeList;
};

#endif // INTEGER_DIVISOR_ENUMERATION_HPP_
