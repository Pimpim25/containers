#ifndef my_MAP
#define my_MAP
#include <utility>

#include "my_rbt.h"
#include "my_vector.h"

namespace my {
template <typename Key, typename T>
class map {
  using key_type = Key;
  using mapped_type = T;
  using value_type = std::pair<const key_type, mapped_type>;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  struct identity {
    const Key& operator()(const std::pair<Key, T>& p) const noexcept {
      return p.first;
    }
  };

  using rbt_tree = RBT<std::pair<Key, T>, Key, identity, 0>;
  using iterator = typename rbt_tree::Iterator;
  using const_iterator = typename rbt_tree::ConstIterator;

  size_type s_size{};
  rbt_tree tree;

 public:
  map() noexcept;
  map(std::initializer_list<value_type> const& items) noexcept;
  map(const map& m) noexcept;
  map(map&& m) noexcept;
  ~map() noexcept;
  map& operator=(map&& m) noexcept;
  map& operator=(map& m) noexcept;

  T& at(const Key& key);
  const T& at(const Key& key) const;
  T& operator[](const Key& key) noexcept;

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;

  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;

  void clear() noexcept;
  std::pair<iterator, bool> insert(const value_type& value) noexcept;
  std::pair<iterator, bool> insert(const Key& key, const T& obj) noexcept;
  std::pair<iterator, bool> insert_or_assign(const Key& key,
                                             const T& obj) noexcept;
  void erase(iterator pos) noexcept;
  void swap(map& other) noexcept;
  void merge(map& other) noexcept;
  iterator find(const Key& key) noexcept;
  bool contains(const Key& key) const noexcept;

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args);
};

// конструкторы map

template <typename Key, typename T>
map<Key, T>::map() noexcept : tree() {}

template <typename Key, typename T>
map<Key, T>::map(std::initializer_list<value_type> const& items) noexcept {
  for (const auto& item : items) {
    insert(item);
  }
}

template <typename Key, typename T>
map<Key, T>::map(const map& m) noexcept {
  for (auto it = m.begin(); it != m.end(); ++it) {
    insert(*it);
  }
}

template <typename Key, typename T>
map<Key, T>::map(map&& m) noexcept {
  std::swap(s_size, m.s_size);
  std::swap(tree.root, m.tree.root);
  std::swap(tree.nil, m.tree.nil);
}

template <typename Key, typename T>
map<Key, T>::~map() noexcept {
  clear();
}

template <typename Key, typename T>
map<Key, T>& map<Key, T>::operator=(map&& m) noexcept {
  if (this != &m) {
    std::swap(tree.root, m.tree.root);
    std::swap(tree.nil, m.tree.nil);
    std::swap(s_size, m.s_size);
  }
  return *this;
}

template <typename Key, typename T>
map<Key, T>& map<Key, T>::operator=(map& m) noexcept {
  if (this != &m) {
    clear();
    for (const auto& item : m) {
      insert(item);
    }
  }
  return *this;
}

template <typename Key, typename T>
T& map<Key, T>::at(const Key& key) {
  auto node = tree.find_node(key);
  if (node == tree.nil)
    throw std::out_of_range("map::at: specified element is out of the map");
  return node->value.second;
}

template <typename Key, typename T>
const T& map<Key, T>::at(const Key& key) const {
  auto node = tree.find_node(key);
  if (node == tree.nil)
    throw std::out_of_range("map::at: specified element is out of the map");
  return node->value.second;
}

template <typename Key, typename T>
T& map<Key, T>::operator[](const Key& key) noexcept {
  auto node = tree.find_node(key);
  if (node == tree.nil) {
    auto it = tree.insert_node({key, T()}).first;
    return it->value.second;
  }
  return node->value.second;
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::begin() noexcept {
  return iterator(tree.minimum(), &tree);
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::end() noexcept {
  return iterator(tree.nil, &tree);
}

template <typename Key, typename T>
typename map<Key, T>::const_iterator map<Key, T>::begin() const noexcept {
  return const_iterator(tree.minimum(), &tree);
}

template <typename Key, typename T>
typename map<Key, T>::const_iterator map<Key, T>::end() const noexcept {
  return const_iterator(tree.nil, &tree);
}

template <typename Key, typename T>
bool map<Key, T>::empty() const noexcept {
  return (s_size == 0);
}

template <typename Key, typename T>
typename map<Key, T>::size_type map<Key, T>::size() const noexcept {
  return s_size;
}

template <typename Key, typename T>
typename map<Key, T>::size_type map<Key, T>::max_size() const noexcept {
  return tree.max_size();
}

template <typename Key, typename T>
void map<Key, T>::clear() noexcept {
  tree.clear(tree.root);
  tree.root = tree.nil;
  s_size = 0;
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const value_type& value) noexcept {
  auto it = tree.insert_node({value.first, value.second});
  if (it.second) s_size++;
  return {iterator(it.first, &tree), it.second};
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert(
    const Key& key, const T& obj) noexcept {
  return insert(value_type(key, obj));
}

template <typename Key, typename T>
std::pair<typename map<Key, T>::iterator, bool> map<Key, T>::insert_or_assign(
    const Key& key, const T& obj) noexcept {
  auto it = tree.insert_node(value_type(key, obj));
  if (!it.second)
    it.first->value.second = obj;
  else
    s_size++;
  return {iterator(it.first, &tree), it.second};
}

template <typename Key, typename T>
void map<Key, T>::erase(iterator pos) noexcept {
  if (tree.erase_node(pos.value)) --s_size;
}

template <typename Key, typename T>
void map<Key, T>::swap(map& other) noexcept {
  std::swap(s_size, other.s_size);
  std::swap(tree.root, other.tree.root);
  std::swap(tree.nil, other.tree.nil);
}

template <typename Key, typename T>
void map<Key, T>::merge(map& other) noexcept {
  for (auto it = other.begin(); it != other.end(); ++it) {
    insert(*it);
  }
}

template <typename Key, typename T>
typename map<Key, T>::iterator map<Key, T>::find(const Key& key) noexcept {
  return iterator(tree.find_node(key), &tree);
}

template <typename Key, typename T>
bool map<Key, T>::contains(const Key& key) const noexcept {
  return tree.find_node(key) != tree.nil;
}

template <typename Key, typename T>
template <typename... Args>
vector<std::pair<typename map<Key, T>::iterator, bool>>
map<Key, T>::insert_many(Args&&... args) {
  vector<std::pair<iterator, bool>> res;

  (res.push_back(insert(args)), ...);

  return res;
}

}  // namespace my

#endif