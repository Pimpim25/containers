#ifndef my_RBT
#define my_RBT

#include <memory>
#include <utility>

namespace my {
template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
class RBT {
  struct Node {
    enum Color { red, black };
    Value value;
    Color color;
    Node* parent;
    Node* left;
    Node* right;
  };
  Node* root;
  Node* nil;
  KeyOfValue key_of_value;
  using size_type = size_t;

  template <typename K>
  friend class set;
  template <typename K>
  friend class multiset;
  template <typename K, typename T>
  friend class map;

 public:
  RBT() noexcept;
  ~RBT() noexcept;
  Node* CreateNode(const Value& v, typename Node::Color c = Node::red) noexcept;
  std::pair<Node*, bool> insert_node(const Value& v) noexcept;
  void balance_tree(Node* z) noexcept;
  void switch_color(Node* z) noexcept;
  void right_turn(Node* z) noexcept;
  void left_turn(Node* z) noexcept;
  Node* find_node(const Key& key) const noexcept;
  Node* minimum() const noexcept;
  Node* maximum() const noexcept;
  void clear(Node* z) noexcept;
  size_type node_count(const Key& key) const noexcept;
  bool erase_node(Node* z) noexcept;
  void transplant(Node* u, Node* v) noexcept;
  void delete_fixup(Node* x) noexcept;
  size_type max_size() const noexcept;

  class Iterator {
    friend class RBT<Value, Key, KeyOfValue, AllowDuplicates>;

    Node* value;
    RBT* tree;

    template <typename K>
    friend class set;
    template <typename K>
    friend class multiset;
    template <typename K, typename T>
    friend class map;
    explicit Iterator(Node* v, RBT* t) noexcept;

   public:
    Iterator() noexcept;
    Iterator(const Iterator& it) noexcept;
    Iterator& operator=(const Iterator& it) noexcept;
    bool operator==(const Iterator& it) const noexcept;
    bool operator!=(const Iterator& it) const noexcept;
    Iterator& operator--() noexcept;
    Iterator& operator++() noexcept;
    Value operator*() const noexcept;
  };

  class ConstIterator {
    friend class RBT<Value, Key, KeyOfValue, AllowDuplicates>;

    const Node* value;
    const RBT* tree;

    template <typename K>
    friend class set;
    template <typename K>
    friend class multiset;
    template <typename K, typename T>
    friend class map;
    explicit ConstIterator(const Node* v, const RBT* t) noexcept;

   public:
    ConstIterator() noexcept;
    ConstIterator(const ConstIterator& it) noexcept;
    ConstIterator& operator=(const ConstIterator& it) noexcept;
    bool operator==(const ConstIterator& it) const noexcept;
    bool operator!=(const ConstIterator& it) const noexcept;
    ConstIterator& operator--() noexcept;
    ConstIterator& operator++() noexcept;
    const Value& operator*() const noexcept;
  };

  Iterator lower_bound(const Key& key) noexcept;
  Iterator upper_bound(const Key& key) noexcept;
  ConstIterator lower_bound(const Key& key) const noexcept;
  ConstIterator upper_bound(const Key& key) const noexcept;
};

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
RBT<Value, Key, KeyOfValue, AllowDuplicates>::RBT() noexcept {
  nil = new Node();
  nil->color = Node::black;
  nil->parent = nil->right = nil->left = nil;
  root = nil;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
RBT<Value, Key, KeyOfValue, AllowDuplicates>::~RBT() noexcept {
  clear(root);
  delete nil;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Node*
RBT<Value, Key, KeyOfValue, AllowDuplicates>::CreateNode(
    const Value& v, typename Node::Color c) noexcept {
  Node* a = new Node;
  a->value = v;
  a->parent = a->right = a->left = nil;
  a->color = c;
  return a;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
std::pair<typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Node*, bool>
RBT<Value, Key, KeyOfValue, AllowDuplicates>::insert_node(
    const Value& v) noexcept {
  Node* y = nil;
  Node* x = root;
  const Key& key = key_of_value(v);
  while (x != nil) {
    y = x;
    const Key& node_key = key_of_value(x->value);
    if (!AllowDuplicates && key_of_value(x->value) == key_of_value(v))
      return {x, false};
    else if (key < node_key)
      x = x->left;
    else
      x = x->right;
  }

  Node* z = CreateNode(v);
  z->parent = y;
  if (y == nil)
    root = z;
  else if (key < key_of_value(y->value))
    y->left = z;
  else
    y->right = z;

  balance_tree(z);
  return {z, true};
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
void RBT<Value, Key, KeyOfValue, AllowDuplicates>::balance_tree(
    Node* z) noexcept {
  while (z != root && z->parent->color == Node::red) {
    Node* p = z->parent;
    Node* g = p->parent;
    Node* u = (p == g->left) ? g->right : g->left;
    if (u != nil && u->color == Node::red) {
      switch_color(g);
      z = g;
    } else {
      if (p == g->left) {
        if (z == p->right) {
          left_turn(p);
          z = p;
          p = z->parent;
        }
        right_turn(g);
      } else {
        if (z == p->left) {
          right_turn(p);
          z = p;
          p = z->parent;
        }
        left_turn(g);
      }
    }
  }
  root->color = Node::black;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
void RBT<Value, Key, KeyOfValue, AllowDuplicates>::switch_color(
    Node* z) noexcept {
  z->left->color = z->right->color = Node::black;
  z->color = Node::red;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
void RBT<Value, Key, KeyOfValue, AllowDuplicates>::right_turn(
    Node* z) noexcept {
  Node* g = z->parent;
  Node* x = z->left;

  z->left = x->right;
  if (x->right != nil) x->right->parent = z;
  x->parent = g;

  if (g == nil)
    root = x;
  else if (g->right == z)
    g->right = x;
  else
    g->left = x;

  x->right = z;
  z->parent = x;
  z->color = Node::red;
  x->color = Node::black;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
void RBT<Value, Key, KeyOfValue, AllowDuplicates>::left_turn(Node* z) noexcept {
  Node* g = z->parent;
  Node* x = z->right;

  z->right = x->left;
  if (x->left != nil) x->left->parent = z;
  x->parent = g;

  if (g == nil)
    root = x;
  else if (g->left == z)
    g->left = x;
  else
    g->right = x;

  x->left = z;
  z->parent = x;
  z->color = Node::red;
  x->color = Node::black;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Node*
RBT<Value, Key, KeyOfValue, AllowDuplicates>::find_node(
    const Key& key) const noexcept {
  Node* x = root;
  while (x != nil) {
    const Key& node_key = key_of_value(x->value);

    if (key < node_key)
      x = x->left;
    else if (node_key < key)
      x = x->right;
    else
      return x;
  }
  return nil;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Node*
RBT<Value, Key, KeyOfValue, AllowDuplicates>::minimum() const noexcept {
  Node* x = root;
  while (x != nil && x->left != nil) {
    x = x->left;
  }
  return x;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Node*
RBT<Value, Key, KeyOfValue, AllowDuplicates>::maximum() const noexcept {
  Node* x = root;
  while (x != nil && x->right != nil) {
    x = x->right;
  }
  return x;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
void RBT<Value, Key, KeyOfValue, AllowDuplicates>::clear(Node* z) noexcept {
  if (z == nil) return;
  clear(z->left);
  clear(z->right);
  delete z;
}

// Iterator

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::Iterator(
    RBT<Value, Key, KeyOfValue, AllowDuplicates>::Node* v,
    RBT<Value, Key, KeyOfValue, AllowDuplicates>* t) noexcept
    : value(v), tree(t) {}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::Iterator() noexcept
    : value(nullptr), tree(nullptr) {}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::Iterator(
    const Iterator& it) noexcept
    : value(it.value), tree(it.tree) {}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator&
RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::operator=(
    const Iterator& it) noexcept {
  if (this != &it) {
    value = it.value;
    tree = it.tree;
  }
  return *this;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
bool RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::operator==(
    const Iterator& it) const noexcept {
  return (value == it.value);
}
template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
bool RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::operator!=(
    const Iterator& it) const noexcept {
  return (value != it.value);
}
template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator&
RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::operator--() noexcept {
  if (value == tree->nil) {
    value = tree->maximum();
  } else if (value->left != tree->nil) {
    value = value->left;
    while (value->right != tree->nil) value = value->right;
  } else {
    Node* p = value->parent;
    while (p != tree->nil && value == p->left) {
      value = p;
      p = p->parent;
    }
    value = p;
  }
  return *this;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator&
RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::operator++() noexcept {
  if (value->right != tree->nil) {
    value = value->right;
    while (value->left != tree->nil) value = value->left;
  } else {
    Node* p = value->parent;
    while (p != tree->nil && value == p->right) {
      value = p;
      p = p->parent;
    }
    value = p;
  }
  return *this;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
Value RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator::operator*()
    const noexcept {
  return value->value;
}
// ConstIterator

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator::ConstIterator(
    const typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Node* v,
    const RBT<Value, Key, KeyOfValue, AllowDuplicates>* t) noexcept
    : value(v), tree(t) {}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
RBT<Value, Key, KeyOfValue,
    AllowDuplicates>::ConstIterator::ConstIterator() noexcept
    : value(nullptr), tree(nullptr) {}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator::ConstIterator(
    const ConstIterator& it) noexcept
    : value(it.value), tree(it.tree) {}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator&
RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator::operator=(
    const ConstIterator& it) noexcept {
  if (this != &it) {
    value = it.value;
    tree = it.tree;
  }
  return *this;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
bool RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator::operator==(
    const ConstIterator& it) const noexcept {
  return (value == it.value);
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
bool RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator::operator!=(
    const ConstIterator& it) const noexcept {
  return (value != it.value);
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator&
RBT<Value, Key, KeyOfValue,
    AllowDuplicates>::ConstIterator::operator--() noexcept {
  if (value == tree->nil) {
    value = tree->maximum(tree->root);
  } else if (value->left != tree->nil) {
    value = value->left;
    while (value->right != tree->nil) value = value->right;
  } else {
    Node* p = value->parent;
    while (p != tree->nil && value == p->left) {
      value = p;
      p = p->parent;
    }
    value = p;
  }
  return *this;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator&
RBT<Value, Key, KeyOfValue,
    AllowDuplicates>::ConstIterator::operator++() noexcept {
  if (value->right != tree->nil) {
    value = value->right;
    while (value->left != tree->nil) value = value->left;
  } else {
    Node* p = value->parent;
    while (p != tree->nil && value == p->right) {
      value = p;
      p = p->parent;
    }
    value = p;
  }
  return *this;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
const Value& RBT<Value, Key, KeyOfValue,
                 AllowDuplicates>::ConstIterator::operator*() const noexcept {
  return value->value;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator
RBT<Value, Key, KeyOfValue, AllowDuplicates>::lower_bound(
    const Key& key) noexcept {
  Node* x = root;
  Node* res = nil;

  while (x != nil) {
    const Key& node_key = key_of_value(x->value);
    if (key > node_key)
      x = x->right;
    else {
      res = x;
      x = x->left;
    }
  }
  return Iterator(res, this);
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::Iterator
RBT<Value, Key, KeyOfValue, AllowDuplicates>::upper_bound(
    const Key& key) noexcept {
  Node* x = root;
  Node* res = nil;

  while (x != nil) {
    const Key& node_key = key_of_value(x->value);
    if (key < node_key) {
      res = x;
      x = x->left;
    } else
      x = x->right;
  }
  return Iterator(res, this);
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator
RBT<Value, Key, KeyOfValue, AllowDuplicates>::lower_bound(
    const Key& key) const noexcept {
  Node* x = root;
  Node* res = nil;

  while (x != nil) {
    const Key& node_key = key_of_value(x->value);
    if (key > node_key)
      x = x->right;
    else {
      res = x;
      x = x->left;
    }
  }
  return ConstIterator(res, this);
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::ConstIterator
RBT<Value, Key, KeyOfValue, AllowDuplicates>::upper_bound(
    const Key& key) const noexcept {
  Node* x = root;
  Node* res = nil;

  while (x != nil) {
    const Key& node_key = key_of_value(x->value);
    if (key < node_key) {
      res = x;
      x = x->left;
    } else
      x = x->right;
  }
  return ConstIterator(res, this);
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::size_type
RBT<Value, Key, KeyOfValue, AllowDuplicates>::node_count(
    const Key& key) const noexcept {
  size_type count{};
  for (auto it = lower_bound(key); it != upper_bound(key); ++it) {
    ++count;
  }
  return count;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
bool RBT<Value, Key, KeyOfValue, AllowDuplicates>::erase_node(
    Node* z) noexcept {
  if (z == nil) return false;
  Node* y = z;
  typename Node::Color y_original_color = y->color;
  Node* x = nil;
  if (z->left == nil) {
    x = z->right;
    transplant(z, z->right);
  } else if (z->right == nil) {
    x = z->left;
    transplant(z, z->left);
  } else {
    y = z->right;
    while (y->left != nil) {
      y = y->left;
    }
    y_original_color = y->color;
    x = y->right;
    if (y->parent == z) {
      if (x != nil) x->parent = y;
    } else {
      transplant(y, y->right);
      y->right = z->right;
      y->right->parent = y;
    }
    transplant(z, y);
    y->left = z->left;
    y->left->parent = y;
    y->color = z->color;
  }
  if (y_original_color == Node::black) delete_fixup(x);
  delete z;
  return true;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
void RBT<Value, Key, KeyOfValue, AllowDuplicates>::transplant(
    Node* u, Node* v) noexcept {
  if (u->parent == nil)
    root = v;
  else if (u == u->parent->left)
    u->parent->left = v;
  else
    u->parent->right = v;

  if (v != nil) v->parent = u->parent;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
void RBT<Value, Key, KeyOfValue, AllowDuplicates>::delete_fixup(
    Node* x) noexcept {
  while (x != root && x->color == Node::black) {
    if (x->parent->left == x) {
      Node* w = x->parent->right;
      if (w->color == Node::red) {
        x->parent->color = Node::red;
        w->color = Node::black;
        left_turn(x->parent);
        w = x->parent->right;
      }
      if (w->left->color == Node::black && w->right->color == Node::black) {
        w->color = Node::red;
        x = x->parent;
      } else {
        if (w->right->color == Node::black) {
          w->left->color = Node::black;
          w->color = Node::red;
          right_turn(w);
          w = x->parent->right;
        } else {
          w->color = x->parent->color;
          x->parent->color = Node::black;
          w->right->color = Node::black;
          left_turn(x->parent);
          x = root;
        }
      }
    } else {
      Node* w = x->parent->left;
      if (w->color == Node::red) {
        x->parent->color = Node::red;
        w->color = Node::black;
        right_turn(x->parent);
        w = x->parent->left;
      }
      if (w->left->color == Node::black && w->right->color == Node::black) {
        w->color = Node::red;
        x = x->parent;
      } else {
        if (w->left->color == Node::black) {
          w->right->color = Node::black;
          w->color = Node::red;
          left_turn(w);
          w = x->parent->left;
        } else {
          w->color = x->parent->color;
          x->parent->color = Node::black;
          w->left->color = Node::black;
          right_turn(x->parent);
          x = root;
        }
      }
    }
  }
  x->color = Node::black;
}

template <typename Value, typename Key, typename KeyOfValue,
          bool AllowDuplicates>
typename RBT<Value, Key, KeyOfValue, AllowDuplicates>::size_type
RBT<Value, Key, KeyOfValue, AllowDuplicates>::max_size() const noexcept {
  return std::numeric_limits<size_type>::max() / sizeof(Node);
}

};  // namespace my

#endif