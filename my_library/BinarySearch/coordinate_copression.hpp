#ifndef COORDINATE_COMPRESSION_HPP_
#define COORDINATE_COMPRESSION_HPP_

#include <bits/stdc++.h>
using namespace std;

template <typename T> class CoordinateCompression {
public:
  CoordinateCompression() = default;

  CoordinateCompression(const vector<T> &values) {
    for (const T &val : values) {
      add(val);
    }
  }

  void add(const T &value) {
    coords.push_back(value);
    is_built = false;
  }

  void add_all(const vector<T> &values) {
    for (const T &val : values) {
      coords.push_back(val);
    }
    is_built = false;
  }

  void build() {
    sort(coords.begin(), coords.end());
    coords.erase(unique(coords.begin(), coords.end()), coords.end());
    is_built = true;
  }

  int compress(const T &value) {
    if (!is_built)
      build();
    return lower_bound(coords.begin(), coords.end(), value) - coords.begin();
  }

  T decompress(int index) {
    if (!is_built)
      build();
    assert(0 <= index && index < (int)coords.size());
    return coords[index];
  }

  int size() {
    if (!is_built)
      build();
    return coords.size();
  }

  T operator[](int i) { return decompress(i); }

  bool contains(const T &value) {
    if (!is_built)
      build();
    return binary_search(coords.begin(), coords.end(), value);
  }

  int lower_bound_index(const T &value) {
    if (!is_built)
      build();
    return lower_bound(coords.begin(), coords.end(), value) - coords.begin();
  }

  int upper_bound_index(const T &value) {
    if (!is_built)
      build();
    return upper_bound(coords.begin(), coords.end(), value) - coords.begin();
  }

  const vector<T> &get_coords() {
    if (!is_built)
      build();
    return coords;
  }

private:
  vector<T> coords;
  bool is_built = false;
};

template <typename T> vector<int> compress_array(const vector<T> &values) {
  CoordinateCompression<T> cc(values);
  cc.build();

  vector<int> compressed(values.size());
  for (int i = 0; i < (int)values.size(); i++) {
    compressed[i] = cc.compress(values[i]);
  }

  return compressed;
}

template <typename T> class CoordinateCompression2D {
public:
  CoordinateCompression2D() = default;

  CoordinateCompression2D(const vector<pair<T, T>> &points) { add_all(points); }

  CoordinateCompression2D(const vector<T> &x_values, const vector<T> &y_values) {
    x_comp.add_all(x_values);
    y_comp.add_all(y_values);
  }

  void add(const T &x, const T &y) {
    x_comp.add(x);
    y_comp.add(y);
  }

  void add_all(const vector<pair<T, T>> &points) {
    for (const auto &p : points) {
      add(p.first, p.second);
    }
  }

  void build() {
    x_comp.build();
    y_comp.build();
  }

  pair<int, int> compress(const T &x, const T &y) {
    return {x_comp.compress(x), y_comp.compress(y)};
  }

  pair<T, T> decompress(int x_idx, int y_idx) {
    return {x_comp.decompress(x_idx), y_comp.decompress(y_idx)};
  }

  pair<int, int> size() { return {x_comp.size(), y_comp.size()}; }

  const CoordinateCompression<T> &get_x_comp() const { return x_comp; }

  const CoordinateCompression<T> &get_y_comp() const { return y_comp; }

private:
  CoordinateCompression<T> x_comp, y_comp;
};

template <typename T> vector<pair<int, int>> compress_2d_array(const vector<pair<T, T>> &points) {
  CoordinateCompression2D<T> cc2d(points);
  cc2d.build();

  vector<pair<int, int>> compressed(points.size());
  for (int i = 0; i < (int)points.size(); i++) {
    compressed[i] = cc2d.compress(points[i].first, points[i].second);
  }

  return compressed;
}

#endif // COORDINATE_COMPRESSION_HPP_
