#ifndef my_multiset
#define my_multiset
#include <memory>
#include <utility>

#include "my_rbt.h"
#include "my_vector.h"

namespace my {
template <typename Key>
class multiset {
  using key_type = Key;
  using value_type = Key;
  using reference = value_type&;
  using const_reference = const value_type&;
  using size_type = size_t;

  struct identity {
    const Key& operator()(const Key& k) const noexcept { return k; }
  };

  using rbt_tree = RBT<Key, Key, identity, 1>;
  using iterator = typename rbt_tree::Iterator;
  using const_iterator = typename rbt_tree::ConstIterator;

  rbt_tree tree;
  size_type s_size{};

 public:
  multiset() noexcept;
  multiset(std::initializer_list<value_type> const& items) noexcept;
  multiset(const multiset& s) noexcept;
  multiset(multiset&& s) noexcept;
  ~multiset();
  multiset& operator=(const multiset& s) noexcept;
  multiset& operator=(multiset&& s) noexcept;

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
  void swap(multiset& other) noexcept;
  void merge(multiset& other) noexcept;
  size_type count(const Key& key) const noexcept;
  iterator find(const Key& key) noexcept;
  bool contains(const Key& key) noexcept;
  iterator lower_bound(const Key& key) noexcept;
  iterator upper_bound(const Key& key) noexcept;
  const_iterator lower_bound(const Key& key) const noexcept;
  const_iterator upper_bound(const Key& key) const noexcept;
  std::pair<iterator, iterator> equal_range(const Key& key) noexcept;
  std::pair<const_iterator, const_iterator> equal_range(
      const Key& key) const noexcept;

  template <typename... Args>
  vector<std::pair<iterator, bool>> insert_many(Args&&... args);
};

// конструкторы multiset

template <typename Key>
multiset<Key>::multiset() noexcept : tree() {}

template <typename Key>
multiset<Key>::multiset(std::initializer_list<value_type> const& items) noexcept
    : multiset() {
  for (const auto& item : items) {
    if ((tree.insert_node(item)).second) s_size++;
  }
}

template <typename Key>
multiset<Key>::multiset(const multiset<Key>& s) noexcept : multiset() {
  for (auto it = s.begin(); it != s.end(); ++it) {
    insert(*it);
  }
}

template <typename Key>
multiset<Key>::multiset(multiset<Key>&& s) noexcept : multiset() {
  std::swap(tree.root, s.tree.root);
  std::swap(tree.nil, s.tree.nil);
  std::swap(s_size, s.s_size);
}

template <typename Key>
multiset<Key>::~multiset() = default;

template <typename Key>
multiset<Key>& multiset<Key>::operator=(const multiset& s) noexcept {
  if (this != &s) {
    clear();
    for (const auto& item : s) {
      insert(item);
    }
  }
  return *this;
}

template <typename Key>
multiset<Key>& multiset<Key>::operator=(multiset&& s) noexcept {
  if (this != &s) {
    std::swap(tree.root, s.tree.root);
    std::swap(tree.nil, s.tree.nil);
    std::swap(s_size, s.s_size);
  }
  return *this;
}

// методы multiset

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::begin() noexcept {
  return iterator(tree.minimum(), &tree);
}

template <typename Key>
typename multiset<Key>::const_iterator multiset<Key>::begin() const noexcept {
  return const_iterator(tree.minimum(), &tree);
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::end() noexcept {
  return iterator(tree.nil, &tree);
}

template <typename Key>
typename multiset<Key>::const_iterator multiset<Key>::end() const noexcept {
  return const_iterator(tree.nil, &tree);
}

template <typename Key>
bool multiset<Key>::empty() const noexcept {
  return (s_size == 0);
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::size() const noexcept {
  return s_size;
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::max_size() const noexcept {
  return tree.max_size();
}

template <typename Key>
void multiset<Key>::clear() noexcept {
  tree.clear(tree.root);
  tree.root = tree.nil;
  s_size = 0;
}

template <typename Key>
std::pair<typename multiset<Key>::iterator, bool> multiset<Key>::insert(
    const value_type& value) {
  auto res = tree.insert_node(value);
  if (res.second) s_size++;
  return {iterator(res.first, &tree), res.second};
}

template <typename Key>
void multiset<Key>::erase(iterator pos) noexcept {
  if (tree.erase_node(pos.value)) --s_size;
}

template <typename Key>
void multiset<Key>::swap(multiset& other) noexcept {
  std::swap(tree.root, other.tree.root);
  std::swap(tree.nil, other.tree.nil);
  std::swap(s_size, other.s_size);
}

template <typename Key>
void multiset<Key>::merge(multiset& other) noexcept {
  for (const auto& item : other) {
    if (tree.insert_node(item).second) s_size++;
  }
  other.clear();
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::find(const Key& key) noexcept {
  return iterator(tree.find_node(key), &tree);
}

template <typename Key>
bool multiset<Key>::contains(const Key& key) noexcept {
  return (tree.find_node(key) != tree.nil);
}

template <typename Key>
typename multiset<Key>::size_type multiset<Key>::count(
    const Key& key) const noexcept {
  return tree.node_count(key);
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::lower_bound(
    const Key& key) noexcept {
  return tree.lower_bound(key);
}

template <typename Key>
typename multiset<Key>::iterator multiset<Key>::upper_bound(
    const Key& key) noexcept {
  return tree.upper_bound(key);
}

template <typename Key>
typename multiset<Key>::const_iterator multiset<Key>::lower_bound(
    const Key& key) const noexcept {
  return tree.lower_bound(key);
}

template <typename Key>
typename multiset<Key>::const_iterator multiset<Key>::upper_bound(
    const Key& key) const noexcept {
  return tree.upper_bound(key);
}

template <typename Key>
std::pair<typename multiset<Key>::iterator, typename multiset<Key>::iterator>
multiset<Key>::equal_range(const Key& key) noexcept {
  return {lower_bound(key), upper_bound(key)};
}

template <typename Key>
std::pair<typename multiset<Key>::const_iterator,
          typename multiset<Key>::const_iterator>
multiset<Key>::equal_range(const Key& key) const noexcept {
  return {lower_bound(key), upper_bound(key)};
}

template <typename Key>
template <typename... Args>
vector<std::pair<typename multiset<Key>::iterator, bool>>
multiset<Key>::insert_many(Args&&... args) {
  vector<std::pair<iterator, bool>> res;

  (res.push_back(insert(args)), ...);

  return res;
}

}  // namespace my

#endif