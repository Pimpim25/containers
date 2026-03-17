#ifndef my_VECTOR
#define my_VECTOR
#include <climits>
#include <memory>

namespace my {

template <typename T>

class vector {
  using value_type = T;
  using reference = T &;
  using const_reference = const T &;
  using size_type = size_t;
  using VectorPtr = std::unique_ptr<value_type[]>;
  class VectorIterator {
    T *value;
    friend class vector;
    explicit VectorIterator(T *value);

   public:
    VectorIterator() noexcept;
    VectorIterator(const VectorIterator &it) noexcept;
    VectorIterator &operator=(const VectorIterator &it) noexcept;
    bool operator==(const VectorIterator &it) const noexcept;
    bool operator!=(const VectorIterator &it) const noexcept;
    VectorIterator &operator--() noexcept;
    VectorIterator &operator++() noexcept;
    reference operator*() const noexcept;
  };
  class VectorConstIterator {
    const T *value;
    friend class vector;
    explicit VectorConstIterator(const T *value);

   public:
    VectorConstIterator() noexcept;
    VectorConstIterator(const VectorConstIterator &it) noexcept;
    VectorConstIterator &operator=(const VectorConstIterator &it) noexcept;
    bool operator==(const VectorConstIterator &it) const noexcept;
    bool operator!=(const VectorConstIterator &it) const noexcept;
    VectorConstIterator &operator--() noexcept;
    VectorConstIterator &operator++() noexcept;
    const_reference operator*() const noexcept;
  };

  using iterator = VectorIterator;
  using const_iterator = VectorConstIterator;

  size_type d_capacity{};
  size_type length{};
  VectorPtr v_data;

  void resize();

 public:
  vector() noexcept;
  explicit vector(size_type n);
  vector(std::initializer_list<value_type> const &items) noexcept;
  vector(const vector &v) noexcept;
  vector(vector &&v) noexcept;
  ~vector();

  vector &operator=(const vector &v) noexcept;
  vector &operator=(vector &&v) noexcept;
  reference at(size_type pos);
  reference operator[](size_type pos);
  const_reference front() const noexcept;
  const_reference back() const noexcept;
  T *data() noexcept;
  iterator begin();
  iterator end();
  const_iterator cbegin() const;
  const_iterator cend() const;
  bool empty() const noexcept;
  size_type capacity() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void reserve(size_type size) noexcept;
  void shrink_to_fit() noexcept;
  void clear() noexcept;
  iterator insert(iterator pos, const_reference value);
  void erase(iterator pos);
  void push_back(const_reference value) noexcept;
  void pop_back() noexcept;
  void swap(vector &other);

  template <typename... Args>
  iterator insert_many(const_iterator pos, Args &&...args);

  template <typename... Args>
  void insert_many_back(Args &&...args);
};

template <typename T>
vector<T>::VectorIterator::VectorIterator() noexcept = default;

template <typename T>
vector<T>::VectorIterator::VectorIterator(T *value) : value(value) {}

template <typename T>
vector<T>::VectorIterator::VectorIterator(
    const vector<T>::VectorIterator &it) noexcept
    : VectorIterator(it.value) {}

template <typename T>
typename vector<T>::VectorIterator &vector<T>::VectorIterator::operator=(
    const vector<T>::VectorIterator &it) noexcept {
  if (this != &it) {
    this->value = it.value;
  }
  return *this;
}

template <typename T>
bool vector<T>::VectorIterator::operator==(
    const VectorIterator &it) const noexcept {
  return (value == it.value);
}

template <typename T>
bool vector<T>::VectorIterator::operator!=(
    const VectorIterator &it) const noexcept {
  return (value != it.value);
}

template <typename T>
typename vector<T>::VectorIterator &
vector<T>::VectorIterator::operator--() noexcept {
  value--;
  return *this;
}

template <typename T>
typename vector<T>::VectorIterator &
vector<T>::VectorIterator::operator++() noexcept {
  value++;
  return *this;
}

template <typename T>
typename vector<T>::reference vector<T>::VectorIterator::operator*()
    const noexcept {
  return *value;
}

template <typename T>
vector<T>::VectorConstIterator::VectorConstIterator() noexcept = default;

template <typename T>
vector<T>::VectorConstIterator::VectorConstIterator(const T *value)
    : value(value) {}

template <typename T>
typename vector<T>::VectorConstIterator &
vector<T>::VectorConstIterator::operator=(
    const vector<T>::VectorConstIterator &it) noexcept {
  if (this != &it) {
    this->value = it.value;
  }
  return *this;
}

template <typename T>
vector<T>::VectorConstIterator::VectorConstIterator(
    const vector<T>::VectorConstIterator &it) noexcept
    : VectorConstIterator(it.value) {}

template <typename T>
bool vector<T>::VectorConstIterator::operator==(
    const VectorConstIterator &it) const noexcept {
  return (value == it.value);
}

template <typename T>
bool vector<T>::VectorConstIterator::operator!=(
    const VectorConstIterator &it) const noexcept {
  return (value != it.value);
}

template <typename T>
typename vector<T>::VectorConstIterator &
vector<T>::VectorConstIterator::operator--() noexcept {
  value--;
  return *this;
}

template <typename T>
typename vector<T>::VectorConstIterator &
vector<T>::VectorConstIterator::operator++() noexcept {
  value++;
  return *this;
}

template <typename T>
typename vector<T>::const_reference vector<T>::VectorConstIterator::operator*()
    const noexcept {
  return *value;
}

template <typename T>
void vector<T>::resize() {
  d_capacity = d_capacity == 0 ? 1 : d_capacity * 2;
  auto a = std::make_unique<value_type[]>(d_capacity);
  if (v_data)
    for (size_type i = 0; i < length; i++) a[i] = std::move(v_data[i]);
  v_data = std::move(a);
}

// конструктор и деструктор

template <typename T>
vector<T>::vector() noexcept = default;

template <typename T>
vector<T>::vector(size_type n) : d_capacity(n) {
  v_data = std::make_unique<T[]>(d_capacity);
};

template <typename T>
vector<T>::vector(std::initializer_list<value_type> const &items) noexcept
    : d_capacity(items.size()),
      length(items.size()),
      v_data(d_capacity ? std::make_unique<T[]>(d_capacity) : nullptr) {
  size_type i = 0;
  for (const auto &item : items) {
    v_data[i++] = item;
  }
};

template <typename T>
vector<T>::vector(const vector &v) noexcept
    : d_capacity(v.d_capacity), length(v.length) {
  v_data = std::make_unique<T[]>(d_capacity);
  for (size_type i = 0; i < length; i++) {
    v_data[i] = v.v_data[i];
  }
};

template <typename T>
vector<T>::vector(vector &&v) noexcept
    : d_capacity(v.d_capacity), length(v.length) {
  v_data = std::move(v.v_data);
  v.d_capacity = 0;
  v.length = 0;
};

template <typename T>
vector<T>::~vector() = default;

template <typename T>
vector<T> &vector<T>::operator=(const vector &v) noexcept {
  if (this != &v) {
    d_capacity = v.d_capacity;
    length = v.length;
    v_data = std::make_unique<T[]>(d_capacity);
    for (vector<T>::size_type i = 0; i < length; i++) {
      v_data[i] = v.v_data[i];
    }
  }
  return *this;
};

template <typename T>
vector<T> &vector<T>::operator=(vector &&v) noexcept {
  if (this != &v) {
    d_capacity = v.d_capacity;
    length = v.length;
    v_data = std::move(v.v_data);

    v.d_capacity = 0;
    v.length = 0;
  }
  return *this;
};

// методы

template <typename T>
typename vector<T>::reference vector<T>::at(size_type pos) {
  if (pos > length - 1)
    throw std::out_of_range("vector::at: pos >= size of the vector");
  return v_data[pos];
};

template <typename T>
typename vector<T>::reference vector<T>::operator[](size_type pos) {
  return v_data[pos];
};

template <typename T>
typename vector<T>::const_reference vector<T>::front() const noexcept {
  return const_cast<vector<T>::reference>(v_data[0]);
};

template <typename T>
typename vector<T>::const_reference vector<T>::back() const noexcept {
  return const_cast<vector<T>::reference>(v_data[length - 1]);
}

template <typename T>
T *vector<T>::data() noexcept {
  return v_data.get();
}

template <typename T>
typename vector<T>::iterator vector<T>::begin() {
  return iterator(v_data.get());
}

template <typename T>
typename vector<T>::iterator vector<T>::end() {
  return iterator(v_data.get() + length);
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cbegin() const {
  return const_iterator(v_data.get());
}

template <typename T>
typename vector<T>::const_iterator vector<T>::cend() const {
  return const_iterator(v_data.get() + length);
}

template <typename T>
bool vector<T>::empty() const noexcept {
  return (length == 0);
}

template <typename T>
typename vector<T>::size_type vector<T>::capacity() const noexcept {
  return d_capacity;
}

template <typename T>
typename vector<T>::size_type vector<T>::size() const noexcept {
  return length;
}

template <typename T>
typename vector<T>::size_type vector<T>::max_size() const noexcept {
  return SIZE_MAX / (2 * sizeof(T));
}

template <typename T>
void vector<T>::reserve(vector<T>::size_type size) noexcept {
  if (size > d_capacity) {
    d_capacity = size;
    VectorPtr a = std::make_unique<T[]>(d_capacity);
    for (size_type i = 0; i < length; i++) {
      a[i] = std::move(v_data[i]);
    }
    v_data = std::move(a);
  }
}

template <typename T>
void vector<T>::shrink_to_fit() noexcept {
  if (length < d_capacity) {
    d_capacity = length;
    VectorPtr a = std::make_unique<T[]>(d_capacity);
    for (size_type i = 0; i < length; i++) {
      a[i] = std::move(v_data[i]);
    }
    v_data = std::move(a);
  }
}

template <typename T>
void vector<T>::clear() noexcept {
  for (size_type i = 0; i < length; ++i) {
    v_data[i].~T();
  }
  length = 0;
}

template <typename T>
typename vector<T>::iterator vector<T>::insert(
    vector<T>::iterator pos, vector<T>::const_reference value) {
  size_type idx = pos.value - v_data.get();
  if (length == d_capacity) resize();
  for (size_t i = length; i > idx; i--) v_data[i] = std::move(v_data[i - 1]);
  v_data[idx] = value;
  length++;
  return vector<T>::iterator(v_data.get() + idx);
}

template <typename T>
void vector<T>::erase(vector<T>::iterator pos) {
  size_type idx = pos.value - v_data.get();
  for (size_t i = idx + 1; i < length; i++) {
    v_data[i - 1] = std::move(v_data[i]);
  }
  length--;
}

template <typename T>
void vector<T>::push_back(vector<T>::const_reference value) noexcept {
  if (length >= d_capacity) resize();
  v_data[length++] = value;
}

template <typename T>
void vector<T>::pop_back() noexcept {
  if (length == 0) return;
  length--;
}

template <typename T>
void vector<T>::swap(vector &other) {
  std::swap(d_capacity, other.d_capacity);
  std::swap(length, other.length);
  std::swap(v_data, other.v_data);
}

template <typename T>
template <typename... Args>
typename vector<T>::iterator vector<T>::insert_many(const_iterator pos,
                                                    Args &&...args) {
  size_t index = pos.value - v_data.get(), res_idx = index;

  (
      [&] {
        auto st = v_data.get();
        insert(iterator(st + index), args);
        index++;
      }(),
      ...);
  iterator res(v_data.get() + res_idx);
  return res;
}

template <typename T>
template <typename... Args>
void vector<T>::insert_many_back(Args &&...args) {
  (push_back(args), ...);
}

}  // namespace my

#endif
