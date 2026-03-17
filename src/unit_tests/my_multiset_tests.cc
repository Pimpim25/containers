#include <set>

#include "tests.h"

TEST(MultisetBasic, DefaultConstructor) {
  my::multiset<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(MultisetBasic, InitList) {
  my::multiset<int> s{1, 2, 2, 3, 3, 3};
  EXPECT_EQ(s.size(), 6);
  EXPECT_EQ(s.count(1), 1);
  EXPECT_EQ(s.count(2), 2);
  EXPECT_EQ(s.count(3), 3);
}

TEST(MultisetBasic, InsertDuplicates) {
  my::multiset<int> s;
  s.insert(5);
  s.insert(5);
  s.insert(5);
  EXPECT_EQ(s.size(), 3);
  EXPECT_EQ(s.count(5), 3);
}

TEST(MultisetBasic, FindAndContains) {
  my::multiset<int> s{1, 2, 2, 3};
  EXPECT_TRUE(s.contains(2));
  EXPECT_FALSE(s.contains(4));
  auto it = s.find(2);
  EXPECT_EQ(*it, 2);
}

TEST(MultisetIterators, OrderTraversal) {
  my::multiset<int> s{3, 1, 2, 2};
  auto it = s.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
}

TEST(MultisetBounds, LowerUpperBound) {
  my::multiset<int> s{1, 2, 2, 2, 3, 4};
  auto lb = s.lower_bound(2);
  auto ub = s.upper_bound(2);
  EXPECT_EQ(*lb, 2);
  int count = 0;
  for (auto it = lb; it != ub; ++it) count++;
  EXPECT_EQ(count, 3);
}

TEST(MultisetBounds, EqualRange) {
  my::multiset<int> s{1, 2, 2, 2, 3};
  auto range = s.equal_range(2);
  int count = 0;
  for (auto it = range.first; it != range.second; ++it) count++;
  EXPECT_EQ(count, 3);
}

TEST(MultisetErase, EraseSingleDuplicate) {
  my::multiset<int> s{1, 2, 2, 2, 3};
  auto it = s.find(2);
  s.erase(it);
  EXPECT_EQ(s.size(), 4);
  EXPECT_EQ(s.count(2), 2);
}

TEST(MultisetErase, EraseAllDuplicatesOneByOne) {
  my::multiset<int> s{5, 5, 5};
  while (s.contains(5)) {
    s.erase(s.find(5));
  }
  EXPECT_EQ(s.size(), 0);
  EXPECT_FALSE(s.contains(5));
}

TEST(MultisetErase, EraseRoot) {
  my::multiset<int> s{10, 5, 15};
  auto it = s.find(10);
  s.erase(it);
  EXPECT_FALSE(s.contains(10));
  EXPECT_EQ(s.size(), 2);
}

TEST(MultisetEraseStress, DeleteRedLeaf) {
  my::multiset<int> s{10, 5, 15, 3};
  s.erase(s.find(3));
  EXPECT_FALSE(s.contains(3));
  EXPECT_EQ(s.size(), 3);
}

TEST(MultisetEraseStress, DeleteBlackLeafCase1) {
  my::multiset<int> s{10, 5, 15, 1, 7};
  s.erase(s.find(1));
  EXPECT_FALSE(s.contains(1));
  EXPECT_EQ(s.size(), 4);
}

TEST(MultisetEraseStress, DeleteBlackLeafCase2) {
  my::multiset<int> s{10, 5, 15, 12, 18};
  s.erase(s.find(12));
  EXPECT_FALSE(s.contains(12));
  EXPECT_EQ(s.size(), 4);
}

TEST(MultisetEraseStress, DeleteNodeWithOneChild) {
  my::multiset<int> s{10, 5, 15, 12};
  s.erase(s.find(15));
  EXPECT_FALSE(s.contains(15));
  EXPECT_TRUE(s.contains(12));
  EXPECT_EQ(s.size(), 3);
}

TEST(MultisetEraseStress, DeleteNodeWithTwoChildren) {
  my::multiset<int> s{20, 10, 30, 5, 15, 25, 35};
  s.erase(s.find(20));
  EXPECT_FALSE(s.contains(20));
  EXPECT_EQ(s.size(), 6);
}

TEST(MultisetEraseStress, CascadingFixupToRoot) {
  my::multiset<int> s;
  for (int i = 1; i <= 50; ++i) s.insert(i);

  for (int i = 1; i <= 50; ++i) {
    s.erase(s.find(i));
    EXPECT_FALSE(s.contains(i));
  }
  EXPECT_TRUE(s.empty());
}

TEST(MultisetMerge, MergeWithDuplicates) {
  my::multiset<int> a{1, 2, 2};
  my::multiset<int> b{2, 3, 3};
  a.merge(b);
  EXPECT_EQ(a.size(), 6);
  EXPECT_EQ(a.count(2), 3);
  EXPECT_EQ(a.count(3), 2);
  EXPECT_TRUE(b.empty());
}

TEST(MultisetSwap, SwapContent) {
  my::multiset<int> a{1, 1};
  my::multiset<int> b{2, 2, 2};
  a.swap(b);
  EXPECT_EQ(a.count(2), 3);
  EXPECT_EQ(b.count(1), 2);
}

TEST(myMultisetTest, InsertAndSize) {
  my::multiset<int> my_ms;
  std::multiset<int> std_ms;

  EXPECT_TRUE(my_ms.empty());
  EXPECT_TRUE(std_ms.empty());

  for (int i = 0; i < 5; ++i) {
    my_ms.insert(i);
    std_ms.insert(i);
  }

  EXPECT_EQ(my_ms.size(), std_ms.size());
}

TEST(myMultisetTest, InsertDuplicates) {
  my::multiset<int> my_ms;
  std::multiset<int> std_ms;

  my_ms.insert(1);
  my_ms.insert(1);
  my_ms.insert(2);

  std_ms.insert(1);
  std_ms.insert(1);
  std_ms.insert(2);

  EXPECT_EQ(my_ms.size(), std_ms.size());
  EXPECT_EQ(my_ms.count(1), std_ms.count(1));
  EXPECT_EQ(my_ms.count(2), std_ms.count(2));
}

TEST(myMultisetTest, FindAndContains) {
  my::multiset<int> my_ms{1, 2, 2, 3};
  std::multiset<int> std_ms{1, 2, 2, 3};

  EXPECT_TRUE(my_ms.contains(2));
  EXPECT_FALSE(my_ms.contains(5));

  auto it21 = my_ms.find(3);
  auto it_std = std_ms.find(3);

  EXPECT_EQ(*it21, *it_std);
}

TEST(myMultisetTest, Erase) {
  my::multiset<int> my_ms{1, 2, 2, 3};
  std::multiset<int> std_ms{1, 2, 2, 3};

  auto it21 = my_ms.find(2);
  auto it_std = std_ms.find(2);

  my_ms.erase(it21);
  std_ms.erase(it_std);

  EXPECT_EQ(my_ms.size(), std_ms.size());
  EXPECT_EQ(my_ms.count(2), std_ms.count(2));
}

TEST(myMultisetTest, Swap) {
  my::multiset<int> my_a{1, 2};
  my::multiset<int> my_b{2, 3};

  std::multiset<int> std_a{1, 2};
  std::multiset<int> std_b{2, 3};

  my_a.swap(my_b);
  std::swap(std_a, std_b);

  EXPECT_EQ(my_a.size(), std_a.size());
  EXPECT_EQ(my_b.size(), std_b.size());
}

TEST(myMultisetTest, Merge) {
  my::multiset<int> my_a{1, 2};
  my::multiset<int> my_b{2, 3};

  std::multiset<int> std_a{1, 2};
  std::multiset<int> std_b{2, 3};

  my_a.merge(my_b);
  std_a.merge(std_b);

  EXPECT_EQ(my_a.size(), std_a.size());
  EXPECT_TRUE(my_b.empty());

  for (int x = 1; x <= 3; ++x) {
    EXPECT_EQ(my_a.count(x), std_a.count(x));
  }
}

TEST(myMultisetTest, BoundsAndEqualRange) {
  my::multiset<int> my_ms{1, 2, 2, 3, 4};
  std::multiset<int> std_ms{1, 2, 2, 3, 4};

  auto lb21 = my_ms.lower_bound(2);
  auto ub21 = my_ms.upper_bound(2);

  auto lb_std = std_ms.lower_bound(2);
  auto ub_std = std_ms.upper_bound(2);

  EXPECT_EQ(*lb21, *lb_std);
  EXPECT_EQ(*ub21, *ub_std);

  auto eq21 = my_ms.equal_range(2);
  auto eq_std = std_ms.equal_range(2);

  EXPECT_EQ(*eq21.first, *eq_std.first);
  EXPECT_EQ(*eq21.second, *eq_std.second);
}

TEST(MultisetInsertMany, SingleElement) {
  my::multiset<int> ms;
  auto res = ms.insert_many(1);
  EXPECT_EQ(res.size(), 1);
  EXPECT_TRUE(res[0].second);
  EXPECT_EQ(*res[0].first, 1);
}

TEST(MultisetInsertMany, MultipleElements) {
  my::multiset<int> ms;
  auto res = ms.insert_many(1, 2, 3);
  EXPECT_EQ(res.size(), 3);
  for (auto &p : res) EXPECT_TRUE(p.second);
  EXPECT_EQ(ms.count(1), 1);
  EXPECT_EQ(ms.count(2), 1);
  EXPECT_EQ(ms.count(3), 1);
}

TEST(MultisetInsertMany, DuplicateElements) {
  my::multiset<int> ms;
  ms.insert(1);
  auto res = ms.insert_many(1, 2, 2);
  EXPECT_EQ(res.size(), 3);
  for (auto &p : res) EXPECT_TRUE(p.second);  // multiset всегда возвращает true
  EXPECT_EQ(ms.count(1), 2);
  EXPECT_EQ(ms.count(2), 2);
}

TEST(MultisetInsertMany, EdgeValues) {
  my::multiset<int> ms;
  auto res = ms.insert_many(INT_MIN, 0, INT_MAX);
  EXPECT_EQ(res.size(), 3);
  for (auto &p : res) EXPECT_TRUE(p.second);
  EXPECT_EQ(ms.count(INT_MIN), 1);
  EXPECT_EQ(ms.count(0), 1);
  EXPECT_EQ(ms.count(INT_MAX), 1);
}