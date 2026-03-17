
#ifndef my_SET
#define my_SET
#include <memory>

#include "my_rbt.h"
#include "my_vector.h"

namespace my {
// класс set

template <typename Key>
class set {
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  struct identity {
    const Key& operator()(const Key& k) const noexcept { return k; }
  };

  using rbt_tree = RBT<Key, Key, identity, 0>;
  using iterator = typename rbt_tree::Iterator;
  using const_iterator = typename rbt_tree::ConstIterator;

  rbt_tree tree;
  size_type s_size{};

 public:
  set() noexcept;
  set(std::initializer_list<value_type> const& items) noexcept;
  set(const set& s) noexcept;
  set(set&& s) noexcept;
  ~set();
  set& operator=(const set& s) noexcept;
  set& operator=(set&& s) noexcept;

  iterator begin() noexcept;
  iterator end() noexcept;
  const_iterator begin() const noexcept;
  const_iterator end() const noexcept;
  bool empty() const noexcept;
  size_type size() const noexcept;
  size_type max_size() const noexcept;
  void clear() noexcept;
  std::pair<iterator, bool> insert(const value_type& value);
  void erase(iterator pos) noexcept;
  void swap(set& other) noexcept;
  void merge(set& other) noexcept;
  iterator find(const Key& key) noexcept;
  bool contains(const Key& key) noexcept;
  void s_order() const noexcept;

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args);
};

// конструкторы set

template <typename Key>
set<Key>::set() noexcept : tree() {}

template <typename Key>
set<Key>::set(std::initializer_list<value_type> const& items) noexcept : set() {
  for (const auto& item : items) {
    if ((tree.insert_node(item)).second) s_size++;
  }
}

template <typename Key>
set<Key>::set(const set<Key>& s) noexcept : set() {
  for (auto it = s.begin(); it != s.end(); ++it) {
    insert(*it);
  }
}

template <typename Key>
set<Key>::set(set<Key>&& s) noexcept : set() {
  std::swap(tree.root, s.tree.root);
  std::swap(tree.nil, s.tree.nil);
  std::swap(s_size, s.s_size);
}

template <typename Key>
set<Key>::~set() = default;

template <typename Key>
set<Key>& set<Key>::operator=(const set& s) noexcept {
  if (this != &s) {
    clear();
    for (const auto& item : s) {
      insert(item);
    }
  }
  return *this;
}

template <typename Key>
set<Key>& set<Key>::operator=(set&& s) noexcept {
  if (this != &s) {
    std::swap(tree.root, s.tree.root);
    std::swap(tree.nil, s.tree.nil);
    std::swap(s_size, s.s_size);
  }
  return *this;
}

// методы set

template <typename Key>
typename set<Key>::iterator set<Key>::begin() noexcept {
  return iterator(tree.minimum(), &tree);
}

template <typename Key>
typename set<Key>::const_iterator set<Key>::begin() const noexcept {
  return const_iterator(tree.minimum(), &tree);
}

template <typename Key>
typename set<Key>::iterator set<Key>::end() noexcept {
  return iterator(tree.nil, &tree);
}

template <typename Key>
typename set<Key>::const_iterator set<Key>::end() const noexcept {
  return const_iterator(tree.nil, &tree);
}

template <typename Key>
bool set<Key>::empty() const noexcept {
  return (s_size == 0);
}

template <typename Key>
typename set<Key>::size_type set<Key>::size() const noexcept {
  return s_size;
}

template <typename Key>
typename set<Key>::size_type set<Key>::max_size() const noexcept {
  return tree.max_size();
}

template <typename Key>
void set<Key>::clear() noexcept {
  tree.clear(tree.root);
  tree.root = tree.nil;
  s_size = 0;
}

template <typename Key>
std::pair<typename set<Key>::iterator, bool> set<Key>::insert(
    const value_type& value) {
  auto res = tree.insert_node(value);
  if (res.second) s_size++;
  return {iterator(res.first, &tree), res.second};
}

template <typename Key>
void set<Key>::erase(iterator pos) noexcept {
  if (tree.erase_node(pos.value)) --s_size;
}

template <typename Key>
void set<Key>::swap(set& other) noexcept {
  std::swap(tree.root, other.tree.root);
  std::swap(tree.nil, other.tree.nil);
  std::swap(s_size, other.s_size);
}

template <typename Key>
void set<Key>::merge(set& other) noexcept {
  for (const auto& item : other) {
    if (tree.insert_node(item).second) s_size++;
  }
  other.clear();
}

template <typename Key>
typename set<Key>::iterator set<Key>::find(const Key& key) noexcept {
  return iterator(tree.find_node(key), &tree);
}

template <typename Key>
bool set<Key>::contains(const Key& key) noexcept {
  return (tree.find_node(key) != tree.nil);
}
template <typename Key>
void set<Key>::s_order() const noexcept {
  tree.order(tree.root);
}

template <typename Key>
template <typename... Args>
vector<std::pair<typename set<Key>::iterator, bool>> set<Key>::insert_many(
    Args&&... args) {
  vector<std::pair<iterator, bool>> res;

  (res.push_back(insert(args)), ...);

  return res;
}

}  // namespace my

#endif