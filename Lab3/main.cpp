#include <chrono>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <random>

#include "exception.h"
#include "gauss_jordan.h"
#include "matrix.h"
#include "vector.h"

template <typename T>
void manualInput(Matrix<T>& A, Vector<T>& b, size_t n, size_t m) {
  std::cout << "Введите коэффициенты матрицы A и правой части b:\n";
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      std::cin >> A(i, j);
    }
    std::cin >> b[i];
  }
}

template <typename T>
void randomInput(Matrix<T>& A, Vector<T>& b, size_t n, size_t m) {
  // Инициализация генератора случайных чисел
  srand(static_cast<unsigned int>(time(nullptr)));

  // Генерация случайных чисел в диапазоне [-10, 10]
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      A(i, j) = static_cast<T>((rand() % 2000 - 1000) / 100.0);
    }
    b[i] = static_cast<T>((rand() % 2000 - 1000) / 100.0);
  }

  std::cout << "Сгенерированная система:\n";
  for (size_t i = 0; i < n; ++i) {
    for (size_t j = 0; j < m; ++j) {
      std::cout << A(i, j) << " ";
    }
    std::cout << "| " << b[i] << "\n";
  }
}

template <typename T>
void solveSystem(size_t n, size_t m, int inputChoice) {
  Matrix<T> A(n, m);
  Vector<T> b(n);

  if (inputChoice == 1) {
    manualInput(A, b, n, m);
  } else {
    randomInput(A, b, n, m);
  }

  Vector<T> x;
  Vector<Vector<T>> null_space_basis;
  size_t operation_count = 0;

  // Измеряем время выполнения
  auto start_time = std::chrono::high_resolution_clock::now();

  gauss_jordan(A, b, x, null_space_basis, static_cast<T>(1e-12),
               &operation_count);

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
      end_time - start_time);

  check_solution(A, x, b);

  // Выводим статистику
  std::cout << "\nСтатистика выполнения:\n";
  std::cout << "Размер матрицы: " << n << "x" << m << "\n";
  std::cout << "Время выполнения: " << duration.count() << " микросекунд\n";
  std::cout << "Количество операций: " << operation_count << "\n";

  std::cout << "\nРешение:\n";
  for (size_t i = 0; i < x.size(); ++i) {
    std::cout << "x" << i << " = " << x[i] << "\n";
  }

  if (!null_space_basis.empty()) {
    std::cout << "\nБазис ядра матрицы (векторы фундаментальной системы):\n";
    for (size_t k = 0; k < null_space_basis.size(); ++k) {
      std::cout << "v" << k << ": (";
      for (size_t j = 0; j < null_space_basis[k].size(); ++j) {
        std::cout << null_space_basis[k][j];
        if (j + 1 < null_space_basis[k].size()) std::cout << ", ";
      }
      std::cout << ")\n";
    }
  } else {
    std::cout << "\nЯдро матрицы пусто.\n";
  }
}

int main() {
  try {
    setlocale(LC_ALL, "Russian");
    int dataTypeChoice;
    std::cout << "Выберите тип данных:\n";
    std::cout << "1. float\n";
    std::cout << "2. double\n";
    std::cout << "Ваш выбор: ";
    std::cin >> dataTypeChoice;

    int inputChoice;
    std::cout << "\nВыберите способ ввода:\n";
    std::cout << "1. Ручной ввод\n";
    std::cout << "2. Случайная генерация\n";
    std::cout << "Ваш выбор: ";
    std::cin >> inputChoice;

    size_t n, m;
    std::cout << "\nВведите число уравнений (строк): ";
    std::cin >> n;
    std::cout << "Введите число неизвестных (столбцов): ";
    std::cin >> m;

    if (dataTypeChoice == 1) {
      solveSystem<float>(n, m, inputChoice);
    } else {
      solveSystem<double>(n, m, inputChoice);
    }

  } catch (const GaussException& e) {
    std::cerr << "Ошибка: " << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Неизвестная ошибка" << std::endl;
  }

  return 0;
}
