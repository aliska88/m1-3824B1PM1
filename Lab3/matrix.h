#pragma once

#include "vector.h"

template <typename T>
class Matrix : public Vector<Vector<T>> {
 public:
  Matrix() = default;
  Matrix(size_t rows, size_t cols);

  size_t rows() const;
  size_t cols() const;

  T& operator()(size_t i, size_t j);
  const T& operator()(size_t i, size_t j) const;

  void swapRows(size_t i, size_t j);
};

template <typename T>
Matrix<T>::Matrix(size_t rows, size_t cols) {
  this->resize(rows);
  for (size_t i = 0; i < rows; ++i) (*this)[i].resize(cols);
}

template <typename T>
size_t Matrix<T>::rows() const {
  return this->size();
}

template <typename T>
size_t Matrix<T>::cols() const {
  return (*this)[0].size();
}

template <typename T>
T& Matrix<T>::operator()(size_t i, size_t j) {
  return (*this)[i][j];
}

template <typename T>
const T& Matrix<T>::operator()(size_t i, size_t j) const {
  return (*this)[i][j];
}

template <typename T>
void Matrix<T>::swapRows(size_t i, size_t j) {
  Vector<T> temp = (*this)[i];
  (*this)[i] = (*this)[j];
  (*this)[j] = temp;
}
