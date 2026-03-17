#include <set>

#include "tests.h"

TEST(SetTest, DefaultConstructor) {
  my::set<int> s;
  EXPECT_TRUE(s.empty());
  EXPECT_EQ(s.size(), 0);
}

TEST(SetTest, InitializerListConstructor) {
  my::set<int> s{1, 2, 3, 4};
  EXPECT_EQ(s.size(), 4);
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(4));
}

TEST(SetTest, CopyConstructor) {
  my::set<int> a{1, 2, 3};
  my::set<int> b(a);

  EXPECT_EQ(a.size(), b.size());
  EXPECT_TRUE(b.contains(1));
  EXPECT_TRUE(b.contains(2));
  EXPECT_TRUE(b.contains(3));
}

TEST(SetTest, MoveConstructor) {
  my::set<int> a{1, 2, 3};
  my::set<int> b(std::move(a));

  EXPECT_EQ(b.size(), 3);
  EXPECT_TRUE(b.contains(1));
  EXPECT_TRUE(b.contains(2));
  EXPECT_TRUE(b.contains(3));
}

TEST(SetTest, InsertUnique) {
  my::set<int> s;
  auto res1 = s.insert(10);
  auto res2 = s.insert(10);

  EXPECT_TRUE(res1.second);
  EXPECT_FALSE(res2.second);
  EXPECT_EQ(s.size(), 1);
}

TEST(SetTest, InsertMultiple) {
  my::set<int> s;
  s.insert(3);
  s.insert(1);
  s.insert(2);

  EXPECT_EQ(s.size(), 3);
  EXPECT_TRUE(s.contains(1));
  EXPECT_TRUE(s.contains(2));
  EXPECT_TRUE(s.contains(3));
}

TEST(SetTest, FindExisting) {
  my::set<int> s{1, 2, 3};
  auto it = s.find(2);

  EXPECT_NE(it, s.end());
  EXPECT_EQ(*it, 2);
}

TEST(SetTest, FindMissing) {
  my::set<int> s{1, 2, 3};
  auto it = s.find(5);

  EXPECT_EQ(it, s.end());
}

TEST(SetTest, IteratorTraversal) {
  my::set<int> s{3, 1, 4, 2};

  int expected = 1;
  for (auto it = s.begin(); it != s.end(); ++it) {
    EXPECT_EQ(*it, expected);
    expected++;
  }
}

TEST(SetTest, IteratorIncrementDecrement) {
  my::set<int> s{1, 2, 3};

  auto it = s.begin();
  EXPECT_EQ(*it, 1);

  ++it;
  EXPECT_EQ(*it, 2);

  ++it;
  EXPECT_EQ(*it, 3);

  --it;
  EXPECT_EQ(*it, 2);
}

TEST(SetTest, Swap) {
  my::set<int> a{1, 2};
  my::set<int> b{3, 4};

  a.swap(b);

  EXPECT_TRUE(a.contains(3));
  EXPECT_TRUE(a.contains(4));
  EXPECT_TRUE(b.contains(1));
  EXPECT_TRUE(b.contains(2));
}

TEST(SetTest, Merge) {
  my::set<int> a{1, 2};
  my::set<int> b{2, 3};

  a.merge(b);

  EXPECT_EQ(a.size(), 3);
  EXPECT_TRUE(a.contains(1));
  EXPECT_TRUE(a.contains(2));
  EXPECT_TRUE(a.contains(3));
}

TEST(SetTest, Clear) {
  my::set<int> s{1, 2, 3};
  s.clear();

  EXPECT_TRUE(s.empty());
}

TEST(SetErase, EraseElement) {
  my::set<int> s{1, 2, 3};
  auto it = s.find(2);
  s.erase(it);
  EXPECT_EQ(s.size(), 2);
  EXPECT_FALSE(s.contains(2));
}

TEST(SetEdgeCases, InsertSingleEraseRoot) {
  my::set<int> s;
  s.insert(10);

  auto it = s.begin();
  s.erase(it);

  EXPECT_TRUE(s.empty());
}

TEST(SetEdgeCases, EraseLeaf) {
  my::set<int> s{10, 5, 15};
  s.erase(s.find(5));

  EXPECT_FALSE(s.contains(5));
  EXPECT_EQ(s.size(), 2);
}

TEST(SetEdgeCases, EraseNodeWithOneChild) {
  my::set<int> s{10, 5, 15, 12};
  s.erase(s.find(15));

  EXPECT_FALSE(s.contains(15));
  EXPECT_TRUE(s.contains(12));
}

TEST(SetEdgeCases, EraseNodeWithTwoChildren) {
  my::set<int> s{10, 5, 15, 3, 7, 12, 18};
  s.erase(s.find(10));

  EXPECT_FALSE(s.contains(10));
  EXPECT_EQ(s.size(), 6);
}

TEST(SetErase, EraseAllOneByOne) {
  my::set<int> s{1, 2, 3, 4, 5};

  while (!s.empty()) {
    s.erase(s.begin());
  }

  EXPECT_TRUE(s.empty());
}

TEST(SetErase, EraseMiddleSequence) {
  my::set<int> s{1, 2, 3, 4, 5, 6, 7};

  s.erase(s.find(4));
  s.erase(s.find(2));
  s.erase(s.find(6));

  EXPECT_FALSE(s.contains(4));
  EXPECT_FALSE(s.contains(2));
  EXPECT_FALSE(s.contains(6));
  EXPECT_EQ(s.size(), 4);
}

TEST(SetIterator, IncrementUntilEnd) {
  my::set<int> s{1, 2, 3};

  auto it = s.begin();
  ++it;
  ++it;
  ++it;

  EXPECT_EQ(it, s.end());
}

TEST(SetIterator, DecrementFromEnd) {
  my::set<int> s{1, 2, 3};

  auto it = s.end();
  --it;

  EXPECT_EQ(*it, 3);
}

TEST(SetIterator, FullForwardBackward) {
  my::set<int> s{1, 2, 3, 4};

  auto it = s.begin();
  EXPECT_EQ(*it, 1);
  ++it;
  EXPECT_EQ(*it, 2);
  ++it;
  EXPECT_EQ(*it, 3);
  --it;
  EXPECT_EQ(*it, 2);
}

TEST(SetMerge, MergeIntoEmpty) {
  my::set<int> s1;
  my::set<int> s2{1, 2, 3};

  s1.merge(s2);

  EXPECT_EQ(s1.size(), 3);
  EXPECT_TRUE(s2.empty());
}

TEST(SetMerge, MergeEmpty) {
  my::set<int> s1{1, 2, 3};
  my::set<int> s2;

  s1.merge(s2);

  EXPECT_EQ(s1.size(), 3);
}

TEST(SetStress, InsertEraseStress) {
  my::set<int> s;

  for (int i = 0; i < 1000; ++i) s.insert(i);

  for (int i = 0; i < 1000; i += 2) s.erase(s.find(i));

  EXPECT_EQ(s.size(), 500);

  for (int i = 1; i < 1000; i += 2) EXPECT_TRUE(s.contains(i));
}

TEST(SetOrder, AlwaysSorted) {
  my::set<int> s{5, 1, 8, 3, 2, 7};

  int prev = *s.begin();
  for (auto it = s.begin(); it != s.end(); ++it) {
    EXPECT_GE(*it, prev);
    prev = *it;
  }
}

TEST(SetMaxSize, EmptySet) {
  my::set<int> s;
  EXPECT_GT(s.max_size(), 0u);
}

TEST(SetMaxSize, GreaterThanSize) {
  my::set<int> s{1, 2, 3};
  EXPECT_GE(s.max_size(), s.size());
}

TEST(mySetTest, InsertAndSize) {
  my::set<int> my_set;
  std::set<int> std_set;

  EXPECT_TRUE(my_set.empty());
  EXPECT_TRUE(std_set.empty());

  for (int i = 0; i < 10; ++i) {
    my_set.insert(i);
    std_set.insert(i);
  }

  EXPECT_EQ(my_set.size(), std_set.size());
}

TEST(mySetTest, InsertDuplicates) {
  my::set<int> my_set;
  std::set<int> std_set;

  my_set.insert(5);
  my_set.insert(5);

  std_set.insert(5);
  std_set.insert(5);

  EXPECT_EQ(my_set.size(), std_set.size());
}

TEST(mySetTest, IterationOrder) {
  my::set<int> my_set{5, 1, 3, 2, 4};
  std::set<int> std_set{5, 1, 3, 2, 4};

  auto it21 = my_set.begin();
  auto it_std = std_set.begin();

  for (; it21 != my_set.end() && it_std != std_set.end(); ++it21, ++it_std) {
    EXPECT_EQ(*it21, *it_std);
  }

  EXPECT_TRUE(it21 == my_set.end());
  EXPECT_TRUE(it_std == std_set.end());
}

TEST(mySetTest, FindAndContains) {
  my::set<int> my_set{1, 2, 3};
  std::set<int> std_set{1, 2, 3};

  EXPECT_EQ(my_set.contains(2), std_set.count(2));
  EXPECT_EQ(my_set.contains(10), std_set.count(10));

  auto it21 = my_set.find(3);
  auto it_std = std_set.find(3);

  EXPECT_EQ(*it21, *it_std);
}

TEST(mySetTest, Erase) {
  my::set<int> my_set{1, 2, 3, 4};
  std::set<int> std_set{1, 2, 3, 4};

  auto it21 = my_set.find(3);
  auto it_std = std_set.find(3);

  my_set.erase(it21);
  std_set.erase(it_std);

  EXPECT_EQ(my_set.size(), std_set.size());

  auto it21_res = my_set.begin();
  auto it_std_res = std_set.begin();

  for (; it21_res != my_set.end(); ++it21_res, ++it_std_res) {
    EXPECT_EQ(*it21_res, *it_std_res);
  }
}

TEST(mySetTest, Swap) {
  my::set<int> my_a{1, 2};
  my::set<int> my_b{3, 4};

  std::set<int> std_a{1, 2};
  std::set<int> std_b{3, 4};

  my_a.swap(my_b);
  std::swap(std_a, std_b);

  EXPECT_EQ(my_a.size(), std_a.size());
  EXPECT_EQ(my_b.size(), std_b.size());

  auto it21 = my_a.begin();
  auto it_std = std_a.begin();
  for (; it21 != my_a.end(); ++it21, ++it_std) {
    EXPECT_EQ(*it21, *it_std);
  }
}

TEST(mySetTest, Merge) {
  my::set<int> my_a{1, 2, 3};
  my::set<int> my_b{3, 4, 5};

  std::set<int> std_a{1, 2, 3};
  std::set<int> std_b{3, 4, 5};

  my_a.merge(my_b);
  std_a.merge(std_b);

  EXPECT_EQ(my_a.size(), std_a.size());
  EXPECT_TRUE(my_b.empty());

  auto it21 = my_a.begin();
  auto it_std = std_a.begin();

  for (; it21 != my_a.end(); ++it21, ++it_std) {
    EXPECT_EQ(*it21, *it_std);
  }
}

TEST(mySetTest, InsertManyEdgeCases) {
  my::set<int> s;

  auto results = s.insert_many(5, 3, 7);
  EXPECT_EQ(s.size(), 3);
  EXPECT_TRUE(s.contains(3));
  EXPECT_TRUE(s.contains(5));
  EXPECT_TRUE(s.contains(7));

  EXPECT_TRUE(results[0].second);
  EXPECT_EQ(*results[0].first, 5);

  auto r2 = s.insert_many(3, 7);
  EXPECT_FALSE(r2[0].second);
  EXPECT_FALSE(r2[1].second);
  EXPECT_EQ(s.size(), 3);

  auto r3 = s.insert_many(10);
  EXPECT_TRUE(r3[0].second);
  EXPECT_EQ(*r3[0].first, 10);
  EXPECT_EQ(s.size(), 4);
}
