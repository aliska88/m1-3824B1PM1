#pragma once

template <typename T>
class Vector {
 public:
  Vector();
  Vector(size_t);
  Vector(const Vector&);
  Vector& operator=(const Vector&);
  ~Vector();

  T& operator[](size_t);
  const T& operator[](size_t) const;

  size_t size() const;
  bool empty() const;
  size_t capacity() const;
  void resize(size_t);
  void reserve(size_t);
  void clear();
  void shrink_to_fit();

  void push_back(T value);

 protected:
  size_t size_, capacity_;
  T* data_;
};

template <typename T>
Vector<T>::Vector() : data_(nullptr), size_(0), capacity_(0) {}

template <typename T>
Vector<T>::Vector(size_t size) : size_(size), capacity_(size) {
  data_ = new T[size];
}

template <typename T>
Vector<T>::Vector(const Vector& other)
    : size_(other.size_), capacity_(other.capacity_) {
  data_ = new T[size_];
  for (size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
}

template <typename T>
Vector<T>& Vector<T>::operator=(const Vector<T>& other) {
  if (this == &other) return *this;
  delete[] data_;
  size_ = other.size_;
  capacity_ = other.capacity_;
  data_ = new T[size_];
  for (size_t i = 0; i < size_; ++i) data_[i] = other.data_[i];
  return *this;
}

template <typename T>
Vector<T>::~Vector() {
  delete[] data_;
}

template <typename T>
T& Vector<T>::operator[](size_t index) {
  return data_[index];
}

template <typename T>
const T& Vector<T>::operator[](size_t index) const {
  return data_[index];
}

template <typename T>
size_t Vector<T>::size() const {
  return size_;
}
template <typename T>
bool Vector<T>::empty() const {
  return size_ == 0;
}

template <typename T>
size_t Vector<T>::capacity() const {
  return capacity_;
}

template <typename T>
void Vector<T>::resize(size_t newSize) {
  if (newSize <= capacity_) {
    size_ = newSize;
  } else {
    T* newData = new T[newSize];
    for (size_t i = 0; i < size_; ++i) {
      newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    size_ = capacity_ = newSize;
  }
}

template <typename T>
void Vector<T>::reserve(size_t newCapacity) {
  if (newCapacity <= capacity_) return;
  T* newData = new T[newCapacity];
  for (size_t i = 0; i < size_; ++i) {
    newData[i] = data_[i];
  }
  delete[] data_;
  data_ = newData;
  capacity_ = newCapacity;
}

template <typename T>
void Vector<T>::clear() {
  size_ = 0;
}

template <typename T>
void Vector<T>::shrink_to_fit() {
  if (size_ < capacity_) {
    T* newData = new T[size_];
    for (size_t i = 0; i < size_; ++i) {
      newData[i] = data_[i];
    }
    delete[] data_;
    data_ = newData;
    capacity_ = size_;
  }
}

template <typename T>
void Vector<T>::push_back(T value) {
  if (size_ == capacity_) {
    size_t new_capacity = (capacity_ == 0) ? 1 : capacity_ * 2;
    reserve(new_capacity);
  }
  data_[size_++] = value;
}
