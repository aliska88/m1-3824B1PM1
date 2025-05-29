#pragma once

#include <algorithm>
#include <cmath>
#include <iostream>

#include "exception.h"
#include "matrix.h"
#include "vector.h"

template <typename T>
T abs_val(T x) {
  return (x < 0) ? -x : x;
}

// Метод Гаусса-Жордана с поиском базиса ядра матрицы
// A - расширенная матрица (n x m), b - вектор правой части (n)
// x - вектор (размер m)
// null_space_basis - базис ядра матрицы (векторы фундаментальной системы
// для однородной системы)
template <typename T>
void gauss_jordan(Matrix<T>& A, Vector<T>& b, Vector<T>& x,
                  Vector<Vector<T>>& null_space_basis, T epsilon = 1e-12,
                  size_t* operation_count = nullptr) {
  size_t n = A.rows();
  size_t m = A.cols();

  // Инициализация счетчика операций
  if (operation_count) *operation_count = 0;

  size_t row = 0;  // текущая строка
  Vector<int> where(m);
  for (size_t i = 0; i < m; ++i) {
    where[i] = -1;
  }

  for (size_t col = 0; col < m && row < n; ++col) {
    // Ищем максимальный элемент (по модулю)
    size_t sel = row;
    T max_val = abs_val(A(row, col));
    for (size_t i = row + 1; i < n; ++i) {
      if (operation_count) (*operation_count)++;
      if (abs_val(A(i, col)) > max_val) {
        max_val = abs_val(A(i, col));
        sel = i;
      }
    }
    if (max_val < epsilon) continue;  // столбец нулевой, пропускаем

    // Меняем строки
    if (sel != row) {
      A.swapRows(sel, row);
      std::swap(b[sel], b[row]);
      if (operation_count) (*operation_count) += 3;  // 3 операции обмена
    }

    where[col] = (int)row;

    // Нормализуем текущую строку
    T lead_val = A(row, col);
    for (size_t j = col; j < m; ++j) {
      A(row, j) /= lead_val;
      if (operation_count) (*operation_count)++;
    }
    b[row] /= lead_val;
    if (operation_count) (*operation_count)++;

    // Вычитаем из всех остальных строк
    for (size_t i = 0; i < n; ++i) {
      if (i != row && abs_val(A(i, col)) > epsilon) {
        T factor = A(i, col);
        for (size_t j = col; j < m; ++j) {
          A(i, j) -= factor * A(row, j);
          if (operation_count)
            (*operation_count) += 2;  // умножение и вычитание
        }
        b[i] -= factor * b[row];
        if (operation_count) (*operation_count) += 2;  // умножение и вычитание
      }
    }

    ++row;
  }

  // Проверяем несовместность системы
  for (size_t i = row; i < n; ++i) {
    if (operation_count) (*operation_count)++;
    if (abs_val(b[i]) > epsilon) {
      throw GaussException("Система несовместна - нет решений");
    }
  }

  // Восстанавливаем решение
  x.resize(m);
  for (size_t i = 0; i < m; ++i) {
    if (where[i] != -1)
      x[i] = b[where[i]];
    else
      x[i] = 0;  // свободная переменная, полагаем 0
  }

  // Находим базис ядра матрицы (векторы фундаментальной системы)
  null_space_basis.clear();
  for (size_t j = 0; j < m; ++j) {
    if (where[j] == -1) {  // свободный столбец
      Vector<T> vec(m);
      for (size_t k = 0; k < m; ++k) vec[k] = 0;
      vec[j] = 1;
      for (size_t i = 0; i < m; ++i) {
        int w = where[i];
        if (w != -1) {
          vec[i] = -A(w, j);
          if (operation_count) (*operation_count)++;
        }
      }
      null_space_basis.push_back(vec);
    }
  }
}

// Проверка решения
template <typename T>
void check_solution(const Matrix<T>& A, const Vector<T>& x, const Vector<T>& b,
                    T epsilon = 1e-8) {
  size_t n = A.rows();
  size_t m = A.cols();

  for (size_t i = 0; i < n; ++i) {
    T sum = 0;
    for (size_t j = 0; j < m; ++j) {
      sum += A(i, j) * x[j];
    }
    if (std::abs(sum - b[i]) > epsilon) {
      throw GaussException("Полученное решение не удовлетворяет системе");
    }
  }
}
