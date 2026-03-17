#include <map>
#include <string>

#include "tests.h"

TEST(MapBasic, DefaultConstructor) {
  my::map<int, int> m;
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

TEST(MapBasic, InitializerListConstructor) {
  my::map<int, int> m{{1, 10}, {2, 20}, {3, 30}};
  EXPECT_EQ(m.size(), 3);
  EXPECT_TRUE(m.contains(1));
  EXPECT_TRUE(m.contains(2));
  EXPECT_TRUE(m.contains(3));
}

TEST(MapBasic, CopyConstructor) {
  my::map<int, int> m1{{1, 10}, {2, 20}};
  my::map<int, int> m2(m1);
  EXPECT_EQ(m2.size(), 2);
  EXPECT_EQ(m2.at(1), 10);
  EXPECT_EQ(m2.at(2), 20);
}

TEST(MapBasic, MoveConstructor) {
  my::map<int, int> m1{{1, 10}, {2, 20}};
  my::map<int, int> m2(std::move(m1));
  EXPECT_EQ(m2.size(), 2);
  EXPECT_TRUE(m2.contains(1));
  EXPECT_TRUE(m2.contains(2));
}

TEST(MapElementAccess, At) {
  my::map<int, int> m{{1, 10}};
  EXPECT_EQ(m.at(1), 10);
  ASSERT_ANY_THROW(m.at(4));
}

TEST(MapElementAccess, OperatorBrackets) {
  my::map<int, int> m{{1, 10}};
  EXPECT_EQ(m[1], 10);
}

TEST(MapInsert, InsertPair) {
  my::map<int, int> m;
  auto res = m.insert({1, 10});
  EXPECT_TRUE(res.second);
  EXPECT_EQ(m.size(), 1);
  EXPECT_EQ((*res.first).first, 1);
  EXPECT_EQ((*res.first).second, 10);
}

TEST(MapInsert, InsertDuplicateKey) {
  my::map<int, int> m{{1, 10}};
  auto res = m.insert({1, 20});
  EXPECT_FALSE(res.second);
  EXPECT_EQ(m.size(), 1);
  EXPECT_EQ(m.at(1), 10);
}

TEST(MapInsert, InsertKeyValue) {
  my::map<int, int> m;
  auto res = m.insert(2, 20);
  EXPECT_TRUE(res.second);
  EXPECT_EQ(m.at(2), 20);
}

TEST(MapInsert, InsertOrAssignInsert) {
  my::map<int, int> m;
  auto res = m.insert_or_assign(1, 10);
  EXPECT_TRUE(res.second);
  EXPECT_EQ(m.at(1), 10);
}

TEST(MapInsert, InsertOrAssignAssign) {
  my::map<int, int> m{{1, 10}};
  auto res = m.insert_or_assign(1, 20);
  EXPECT_FALSE(res.second);
  EXPECT_EQ(m.at(1), 20);
}

TEST(MapIterators, BeginEndTraversal) {
  my::map<int, int> m{{2, 20}, {1, 10}, {3, 30}};
  int keys[] = {1, 2, 3};
  int i = 0;
  for (auto it = m.begin(); it != m.end(); ++it) {
    EXPECT_EQ((*it).first, keys[i]);
    ++i;
  }
}

TEST(MapErase, EraseLeaf) {
  my::map<int, int> m{{1, 10}, {2, 20}, {3, 30}};
  auto it = m.find(3);
  m.erase(it);
  EXPECT_EQ(m.size(), 2);
  EXPECT_FALSE(m.contains(3));
}

TEST(MapErase, EraseNodeWithOneChild) {
  my::map<int, int> m{{1, 10}, {2, 20}};
  auto it = m.find(1);
  m.erase(it);
  EXPECT_EQ(m.size(), 1);
  EXPECT_FALSE(m.contains(1));
  EXPECT_TRUE(m.contains(2));
}

TEST(MapErase, EraseRoot) {
  my::map<int, int> m{{10, 1}, {5, 2}, {15, 3}};
  auto it = m.find(10);
  m.erase(it);
  EXPECT_EQ(m.size(), 2);
  EXPECT_FALSE(m.contains(10));
  EXPECT_TRUE(m.contains(5));
  EXPECT_TRUE(m.contains(15));
}

TEST(MapClear, ClearMap) {
  my::map<int, int> m{{1, 10}, {2, 20}};
  m.clear();
  EXPECT_TRUE(m.empty());
  EXPECT_EQ(m.size(), 0);
}

TEST(MapSwap, SwapMaps) {
  my::map<int, int> m1{{1, 10}};
  my::map<int, int> m2{{2, 20}};
  m1.swap(m2);
  EXPECT_TRUE(m1.contains(2));
  EXPECT_TRUE(m2.contains(1));
}

TEST(MapMerge, MergeMaps) {
  my::map<int, int> m1{{1, 10}, {2, 20}};
  my::map<int, int> m2{{3, 30}, {2, 200}};
  m1.merge(m2);
  EXPECT_EQ(m1.size(), 3);
  EXPECT_TRUE(m1.contains(1));
  EXPECT_TRUE(m1.contains(2));
  EXPECT_TRUE(m1.contains(3));
}

TEST(MapContains, ContainsKey) {
  my::map<int, int> m{{1, 10}};
  EXPECT_TRUE(m.contains(1));
  EXPECT_FALSE(m.contains(2));
}

TEST(MapEraseStress, DeleteRedLeaf) {
  my::map<int, int> m{{10, 1}, {5, 2}, {15, 3}, {3, 4}};
  auto it = m.find(3);
  m.erase(it);
  EXPECT_FALSE(m.contains(3));
  EXPECT_EQ(m.size(), 3);
}

TEST(MapEraseStress, DeleteBlackLeafCase1) {
  my::map<int, int> m{{10, 1}, {5, 2}, {15, 3}, {1, 4}, {7, 5}};
  auto it = m.find(1);
  m.erase(it);
  EXPECT_FALSE(m.contains(1));
  EXPECT_EQ(m.size(), 4);
}

TEST(MapEraseStress, DeleteBlackLeafCase2) {
  my::map<int, int> m{{10, 1}, {5, 2}, {15, 3}, {12, 4}, {18, 5}};
  auto it = m.find(12);
  m.erase(it);
  EXPECT_FALSE(m.contains(12));
  EXPECT_EQ(m.size(), 4);
}

TEST(MapEraseStress, DeleteNodeWithOneChild) {
  my::map<int, int> m{{10, 1}, {5, 2}, {15, 3}, {12, 4}};
  auto it = m.find(15);
  m.erase(it);
  EXPECT_FALSE(m.contains(15));
  EXPECT_TRUE(m.contains(12));
  EXPECT_EQ(m.size(), 3);
}

TEST(MapEraseStress, DeleteNodeWithTwoChildren) {
  my::map<int, int> m{{20, 1}, {10, 2}, {30, 3}, {5, 4},
                      {15, 5}, {25, 6}, {35, 7}};
  auto it = m.find(20);
  m.erase(it);
  EXPECT_FALSE(m.contains(20));
  EXPECT_EQ(m.size(), 6);
}

TEST(MapEraseStress, CascadingFixupToRoot) {
  my::map<int, int> m;
  for (int i = 1; i <= 50; ++i) m.insert({i, i});

  for (int i = 1; i <= 50; ++i) {
    auto it = m.find(i);
    m.erase(it);
    EXPECT_FALSE(m.contains(i));
  }
  EXPECT_TRUE(m.empty());
}

TEST(myMapTest, InsertAndSize) {
  my::map<int, std::string> my_map;
  std::map<int, std::string> std_map;

  EXPECT_TRUE(my_map.empty());
  EXPECT_TRUE(std_map.empty());

  for (int i = 0; i < 5; ++i) {
    my_map.insert(i, "val" + std::to_string(i));
    std_map.insert({i, "val" + std::to_string(i)});
  }

  EXPECT_EQ(my_map.size(), std_map.size());
}

TEST(myMapTest, InsertDuplicates) {
  my::map<int, std::string> my_map;
  std::map<int, std::string> std_map;

  my_map.insert(1, "one");
  my_map.insert(1, "uno");  // не должно увеличить размер
  std_map.insert({1, "one"});
  std_map.insert({1, "uno"});

  EXPECT_EQ(my_map.size(), std_map.size());
}

TEST(myMapTest, AtAndOperatorSquareBrackets) {
  my::map<int, std::string> my_map;
  std::map<int, std::string> std_map;

  my_map[10] = "ten";
  std_map[10] = "ten";

  EXPECT_EQ(my_map[10], std_map[10]);
  EXPECT_EQ(my_map.at(10), std_map.at(10));
}

TEST(myMapTest, FindAndContains) {
  my::map<int, std::string> my_map{{1, "one"}, {2, "two"}};
  std::map<int, std::string> std_map{{1, "one"}, {2, "two"}};

  EXPECT_TRUE(my_map.contains(1));
  EXPECT_FALSE(my_map.contains(3));

  auto it21 = my_map.find(2);
  auto it_std = std_map.find(2);

  EXPECT_EQ((*it21).second, it_std->second);
}

TEST(myMapTest, Erase) {
  my::map<int, std::string> my_map{{1, "one"}, {2, "two"}, {3, "three"}};
  std::map<int, std::string> std_map{{1, "one"}, {2, "two"}, {3, "three"}};

  auto it21 = my_map.find(2);
  auto it_std = std_map.find(2);

  my_map.erase(it21);
  std_map.erase(it_std);

  EXPECT_EQ(my_map.size(), std_map.size());

  auto it21_res = my_map.begin();
  auto it_std_res = std_map.begin();

  for (; it21_res != my_map.end(); ++it21_res, ++it_std_res) {
    EXPECT_EQ((*it21_res).first, it_std_res->first);
    EXPECT_EQ((*it21_res).second, it_std_res->second);
  }
}

TEST(myMapTest, Swap) {
  my::map<int, std::string> my_a{{1, "a"}, {2, "b"}};
  my::map<int, std::string> my_b{{3, "c"}, {4, "d"}};

  std::map<int, std::string> std_a{{1, "a"}, {2, "b"}};
  std::map<int, std::string> std_b{{3, "c"}, {4, "d"}};

  my_a.swap(my_b);
  std::swap(std_a, std_b);

  EXPECT_EQ(my_a.size(), std_a.size());
  EXPECT_EQ(my_b.size(), std_b.size());

  auto it21 = my_a.begin();
  auto it_std = std_a.begin();
  for (; it21 != my_a.end(); ++it21, ++it_std) {
    EXPECT_EQ((*it21).first, it_std->first);
    EXPECT_EQ((*it21).second, it_std->second);
  }
}

TEST(myMapTest, Merge) {
  my::map<int, std::string> my_a{{1, "one"}, {2, "two"}};
  my::map<int, std::string> my_b{{2, "dos"}, {3, "three"}};

  std::map<int, std::string> std_a{{1, "one"}, {2, "two"}};
  std::map<int, std::string> std_b{{2, "dos"}, {3, "three"}};

  my_a.merge(my_b);
  std_a.merge(std_b);

  EXPECT_EQ(my_a.size(), std_a.size());

  auto it21 = my_a.begin();
  auto it_std = std_a.begin();

  for (; it21 != my_a.end(); ++it21, ++it_std) {
    EXPECT_EQ((*it21).first, it_std->first);
    EXPECT_EQ((*it21).second, it_std->second);
  }
}

TEST(MapInsertMany, SingleElement) {
  my::map<int, std::string> m;
  auto res = m.insert_many(std::make_pair(1, "one"));
  EXPECT_EQ(res.size(), 1);
  EXPECT_TRUE(res[0].second);
  EXPECT_EQ((*(res[0].first)).second, "one");
}

TEST(MapInsertMany, MultipleElements) {
  my::map<int, std::string> m;
  auto res = m.insert_many(std::make_pair(1, "one"), std::make_pair(2, "two"),
                           std::make_pair(3, "three"));
  EXPECT_EQ(res.size(), 3);
  for (auto &p : res) EXPECT_TRUE(p.second);
  EXPECT_EQ(m.size(), 3);
  EXPECT_EQ(m[2], "two");
}

TEST(MapInsertMany, DuplicateKeys) {
  my::map<int, std::string> m;
  m.insert({1, "one"});
  auto res = m.insert_many(std::make_pair(1, "uno"), std::make_pair(2, "two"));
  EXPECT_EQ(res.size(), 2);
  EXPECT_FALSE(res[0].second);
  EXPECT_TRUE(res[1].second);
  EXPECT_EQ(m[1], "one");
  EXPECT_EQ(m[2], "two");
}

TEST(MapInsertMany, EdgeValues) {
  my::map<int, int> m;
  auto res = m.insert_many(std::make_pair(INT_MIN, -1), std::make_pair(0, 0),
                           std::make_pair(INT_MAX, 1));
  EXPECT_EQ(res.size(), 3);
  for (auto &p : res) EXPECT_TRUE(p.second);
  EXPECT_EQ(m[INT_MIN], -1);
  EXPECT_EQ(m[0], 0);
  EXPECT_EQ(m[INT_MAX], 1);
}