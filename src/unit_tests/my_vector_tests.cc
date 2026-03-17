#include <vector>

#include "tests.h"

TEST(VectorIteratorTest, Iterators_AllInOne) {
  my::vector<int> v{1, 2, 3};

  auto it = v.begin();
  EXPECT_EQ(*it, 1);

  ++it;
  EXPECT_EQ(*it, 2);

  --it;
  EXPECT_EQ(*it, 1);

  auto it_copy(it);
  EXPECT_EQ(*it_copy, *it);
  EXPECT_TRUE(it_copy == it);
  EXPECT_FALSE(it_copy != it);

  auto it_end = v.end();
  EXPECT_TRUE(it != it_end);

  const my::vector<int> cv{4, 5, 6};

  auto cit = cv.cbegin();
  EXPECT_EQ(*cit, 4);

  ++cit;
  EXPECT_EQ(*cit, 5);

  --cit;
  EXPECT_EQ(*cit, 4);

  auto cit_copy(cit);
  EXPECT_EQ(*cit_copy, *cit);
  EXPECT_TRUE(cit_copy == cit);
  EXPECT_FALSE(cit_copy != cit);

  auto cit_end = cv.cend();
  EXPECT_TRUE(cit != cit_end);
}

TEST(VectorTest, Vector_Member_functions) {
  my::vector<int> test1;
  EXPECT_EQ(test1.capacity(), 0);
  EXPECT_EQ(test1.size(), 0);

  my::vector<int> test2(5);
  EXPECT_EQ(test2.capacity(), 5);
  EXPECT_EQ(test2.size(), 0);

  my::vector<int> test3{1, 2, 3, 4};
  EXPECT_EQ(test3.capacity(), 4);
  EXPECT_EQ(test3.size(), 4);
  int arr[] = {1, 2, 3, 4};
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test3.at(i), arr[i]);
  }
  my::vector<int> test4(test3);
  EXPECT_EQ(test4.capacity(), 4);
  EXPECT_EQ(test4.size(), 4);
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test4.at(i), test3.at(i));
  }

  my::vector<int> test5(std::move(test4));
  EXPECT_EQ(test5.capacity(), 4);
  EXPECT_EQ(test5.size(), 4);
  EXPECT_EQ(test4.capacity(), 0);
  EXPECT_EQ(test4.size(), 0);
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test5.at(i), arr[i]);
  }
  EXPECT_TRUE(test4.data() == nullptr);

  my::vector<int> test6;
  test6 = std::move(test5);
  EXPECT_EQ(test6.capacity(), 4);
  EXPECT_EQ(test6.size(), 4);
  EXPECT_EQ(test5.capacity(), 0);
  EXPECT_EQ(test5.size(), 0);
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test6.at(i), arr[i]);
  }
  EXPECT_TRUE(test5.data() == nullptr);

  my::vector<int> test7;
  test7 = test6;
  EXPECT_EQ(test7.capacity(), 4);
  EXPECT_EQ(test7.size(), 4);
  EXPECT_EQ(test6.capacity(), 4);
  EXPECT_EQ(test6.size(), 4);
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test7.at(i), arr[i]);
    EXPECT_EQ(test6.at(i), arr[i]);
  }
}

TEST(VectorTest, Vector_Element_access) {
  my::vector<int> test{1, 2, 3, 4};
  EXPECT_EQ(test.capacity(), 4);
  EXPECT_EQ(test.size(), 4);
  int arr[] = {1, 2, 3, 4};
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test.at(i), arr[i]);
    EXPECT_EQ(test[i], arr[i]);
  }
  ASSERT_ANY_THROW(test.at(4));
  EXPECT_EQ(test.front(), arr[0]);
  EXPECT_EQ(test.back(), arr[3]);
  EXPECT_TRUE(test.data() != nullptr);
}

TEST(VectorTest, IteratorCorrectness) {
  my::vector<int> v{1, 2, 3, 4};

  EXPECT_EQ(*(++v.begin()), 2);
  EXPECT_EQ(*(--v.end()), 4);
}

TEST(VectorTest, Vector_Capacity) {
  my::vector<int> test2{1, 2, 3, 4}, test1;
  EXPECT_TRUE(test1.empty());
  EXPECT_FALSE(test2.empty());
  EXPECT_EQ(test2.capacity(), 4);
  EXPECT_EQ(test2.size(), 4);

  test2.reserve(3);
  EXPECT_EQ(test2.capacity(), 4);
  test2.reserve(10);
  EXPECT_EQ(test2.capacity(), 10);
  int arr[] = {1, 2, 3, 4};
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test2.at(i), arr[i]);
  }
  test2.shrink_to_fit();
  EXPECT_EQ(test2.capacity(), test2.size());
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test2.at(i), arr[i]);
  }
}

TEST(VectorTest, Vector_Modifiers) {
  my::vector<int> test{1, 2, 3, 4}, test1;

  EXPECT_EQ(test.size(), 4);
  EXPECT_GE(test.capacity(), 4);

  test.push_back(5);
  EXPECT_EQ(test[4], 5);
  EXPECT_EQ(test.size(), 5);

  test.pop_back();
  EXPECT_EQ(test.size(), 4);

  test.erase(test.begin());
  EXPECT_EQ(test[0], 2);
  EXPECT_EQ(test.size(), 3);

  test.insert(test.begin(), 1);
  EXPECT_EQ(test[0], 1);
  EXPECT_EQ(test.size(), 4);

  test.swap(test1);
  int arr[] = {1, 2, 3, 4};
  EXPECT_EQ(test1.size(), 4);
  EXPECT_GE(test1.capacity(), 4);
  EXPECT_EQ(test.size(), 0);
  EXPECT_GE(test.capacity(), 0);
  for (size_t i = 0; i < 4; i++) {
    EXPECT_EQ(test1.at(i), arr[i]);
  }

  test1.clear();
  EXPECT_EQ(test1.size(), 0);
  EXPECT_GE(test1.capacity(), 4);
}

TEST(VectorConstructors, Default) {
  my::vector<int> v;
  EXPECT_TRUE(v.empty());
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorConstructors, SizeConstructor) {
  my::vector<int> v(5);
  EXPECT_EQ(v.capacity(), 5);
  EXPECT_EQ(v.size(), 0);
}

TEST(VectorConstructors, InitList) {
  my::vector<int> v{1, 2, 3};
  EXPECT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
}

TEST(VectorConstructors, CopyConstructor) {
  my::vector<int> a{1, 2, 3};
  my::vector<int> b(a);
  EXPECT_EQ(b.size(), 3);
  EXPECT_EQ(b[1], 2);
}

TEST(VectorConstructors, MoveConstructor) {
  my::vector<int> a{1, 2, 3};
  my::vector<int> b(std::move(a));
  EXPECT_EQ(b.size(), 3);
  EXPECT_TRUE(a.empty());
}

TEST(VectorAssign, CopyAssign) {
  my::vector<int> a{1, 2};
  my::vector<int> b;
  b = a;
  EXPECT_EQ(b.size(), 2);
  EXPECT_EQ(b[0], 1);
}

TEST(VectorAssign, MoveAssign) {
  my::vector<int> a{1, 2};
  my::vector<int> b;
  b = std::move(a);
  EXPECT_EQ(b.size(), 2);
  EXPECT_TRUE(a.empty());
}

TEST(VectorAccess, AtAndIndex) {
  my::vector<int> v{10, 20, 30};
  EXPECT_EQ(v.at(1), 20);
  EXPECT_EQ(v[2], 30);
}

TEST(VectorAccess, FrontBack) {
  my::vector<int> v{5, 6, 7};
  EXPECT_EQ(v.front(), 5);
  EXPECT_EQ(v.back(), 7);
}

TEST(VectorIterators, Iterate) {
  my::vector<int> v{1, 2, 3};
  int sum = 0;
  for (auto it = v.begin(); it != v.end(); ++it) sum += *it;
  EXPECT_EQ(sum, 6);
}

TEST(VectorCapacity, Reserve) {
  my::vector<int> v;
  v.reserve(10);
  EXPECT_GE(v.capacity(), 10);
}

TEST(VectorCapacity, ShrinkToFit) {
  my::vector<int> v{1, 2, 3};
  v.reserve(10);
  v.shrink_to_fit();
  EXPECT_EQ(v.capacity(), v.size());
}

TEST(VectorModifiers, PushBack) {
  my::vector<int> v;
  v.push_back(1);
  v.push_back(2);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.back(), 2);
}

TEST(VectorModifiers, PopBack) {
  my::vector<int> v{1, 2, 3};
  v.pop_back();
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.back(), 2);
}

TEST(VectorModifiers, InsertEmpty) {
  my::vector<int> v;
  v.insert(v.begin(), 1);
  v.insert(++(v.begin()), 2);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v.size(), 2);
}

TEST(VectorModifiers, InsertBegin) {
  my::vector<int> v{2, 3};
  v.insert(v.begin(), 1);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v.size(), 3);
}

TEST(VectorModifiers, InsertMiddle) {
  my::vector<int> v{1, 3};
  auto it = v.begin();
  ++it;
  v.insert(it, 2);
  EXPECT_EQ(v[1], 2);
}

TEST(VectorModifiers, InsertEnd) {
  my::vector<int> v{1, 2};
  v.insert(v.end(), 3);
  EXPECT_EQ(v.back(), 3);
}

TEST(VectorModifiers, EraseBegin) {
  my::vector<int> v{1, 2, 3};
  v.erase(v.begin());
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[0], 2);
}

TEST(VectorModifiers, EraseMiddle) {
  my::vector<int> v{1, 2, 3};
  auto it = v.begin();
  ++it;
  v.erase(it);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v[1], 3);
}

TEST(VectorModifiers, EraseEndMinusOne) {
  my::vector<int> v{1, 2, 3};
  auto it = v.end();
  --it;
  v.erase(it);
  EXPECT_EQ(v.size(), 2);
  EXPECT_EQ(v.back(), 2);
}

TEST(VectorModifiers, Clear) {
  my::vector<int> v{1, 2, 3};
  v.clear();
  EXPECT_TRUE(v.empty());
}

TEST(VectorModifiers, Swap) {
  my::vector<int> a{1, 2};
  my::vector<int> b{3};
  a.swap(b);
  EXPECT_EQ(a.size(), 1);
  EXPECT_EQ(a[0], 3);
  EXPECT_EQ(b.size(), 2);
}

TEST(VectorStress, PushEraseMany) {
  my::vector<int> v;
  for (int i = 0; i < 1000; ++i) v.push_back(i);
  for (int i = 0; i < 500; ++i) v.erase(v.begin());
  EXPECT_EQ(v.size(), 500);
  EXPECT_EQ(v.front(), 500);
}

TEST(VectorTest, MaxSize) {
  my::vector<int> my_vec;
  std::vector<int> std_vec;

  EXPECT_EQ(my_vec.max_size(), std_vec.max_size());

  static_assert(noexcept(my_vec.max_size()), "max_size() should be noexcept");

  my::vector<double> my_dvec;
  std::vector<double> std_dvec;
  EXPECT_EQ(my_dvec.max_size(), std_dvec.max_size());

  my::vector<char> my_cvec;
  std::vector<char> std_cvec;
  EXPECT_EQ(my_cvec.max_size(), std_cvec.max_size());
}

TEST(VectorTest, PushBackAndSize) {
  my::vector<int> my_vec;
  std::vector<int> std_vec;
  for (int i = 0; i < 100; ++i) {
    my_vec.push_back(i);
    std_vec.push_back(i);
  }
  EXPECT_EQ(my_vec.size(), std_vec.size());
  EXPECT_EQ(my_vec.capacity() >= my_vec.size(), true);
}

TEST(VectorTest, AccessOperators) {
  my::vector<int> my_vec{1, 2, 3};
  std::vector<int> std_vec{1, 2, 3};
  for (size_t i = 0; i < my_vec.size(); ++i) {
    EXPECT_EQ(my_vec[i], std_vec[i]);
    EXPECT_EQ(my_vec.at(i), std_vec.at(i));
  }
  EXPECT_EQ(my_vec.front(), std_vec.front());
  EXPECT_EQ(my_vec.back(), std_vec.back());
}

TEST(VectorTest, Iterators) {
  my::vector<int> my_vec{1, 2, 3, 4, 5};
  std::vector<int> std_vec{1, 2, 3, 4, 5};
  auto it_my = my_vec.begin();
  auto it_std = std_vec.begin();
  for (; it_my != my_vec.end() && it_std != std_vec.end(); ++it_my, ++it_std) {
    EXPECT_EQ(*it_my, *it_std);
  }
}

TEST(VectorTest, ClearAndEmpty) {
  my::vector<int> my_vec{1, 2, 3};
  EXPECT_FALSE(my_vec.empty());
  my_vec.clear();
  EXPECT_TRUE(my_vec.empty());
  EXPECT_EQ(my_vec.size(), 0);
}

TEST(VectorTest, Swap) {
  my::vector<int> a{1, 2}, b{3, 4};
  a.swap(b);
  EXPECT_EQ(a[0], 3);
  EXPECT_EQ(a[1], 4);
  EXPECT_EQ(b[0], 1);
  EXPECT_EQ(b[1], 2);
}

TEST(VectorTest, ReserveAndShrink) {
  my::vector<int> my_vec;
  my_vec.reserve(50);
  EXPECT_GE(my_vec.capacity(), 50);
  for (int i = 0; i < 10; ++i) my_vec.push_back(i);
  my_vec.shrink_to_fit();
  EXPECT_EQ(my_vec.capacity(), my_vec.size());
}

TEST(myVectorTest, InsertManyBack) {
  my::vector<int> v{1, 2, 3};

  v.insert_many_back(4, 5, 6);

  ASSERT_EQ(v.size(), 6);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
  EXPECT_EQ(v[5], 6);

  v.insert_many_back(7);
  ASSERT_EQ(v.size(), 7);
  EXPECT_EQ(v[6], 7);

  int x = 8;
  v.insert_many_back(x, 9);
  ASSERT_EQ(v.size(), 9);
  EXPECT_EQ(v[7], 8);
  EXPECT_EQ(v[8], 9);
}

TEST(myVectorTest, InsertManyBasic) {
  my::vector<int> v;
  v.push_back(1);
  v.push_back(4);

  auto pos = v.cbegin();
  ++pos;
  auto it = v.insert_many(pos, 2, 3);

  ASSERT_EQ(v.size(), 4);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(*it, 2);
}

TEST(myVectorTest, InsertManyLvalueRvalue) {
  my::vector<int> v{1, 2, 3};
  int x = 4;
  auto it1 = v.insert_many(v.cend(), x, 5);

  ASSERT_EQ(v.size(), 5);
  EXPECT_EQ(v[3], 4);
  EXPECT_EQ(v[4], 5);
  EXPECT_EQ(*it1, 4);

  auto it2 = v.insert_many(v.cbegin(), 0);
  EXPECT_EQ(v.size(), 6);
  EXPECT_EQ(v[0], 0);
  EXPECT_EQ(*it2, 0);
}

TEST(myVectorTest, InsertManyAtBegin) {
  my::vector<int> v{3, 4, 5};
  auto it = v.insert_many(v.cbegin(), 1, 2);

  ASSERT_EQ(v.size(), 5);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 2);
  EXPECT_EQ(v[2], 3);
  EXPECT_EQ(*it, 1);
}

TEST(myVectorTest, InsertManyEmptyVector) {
  my::vector<int> v;
  auto it = v.insert_many(v.cbegin(), 10, 20, 30);

  ASSERT_EQ(v.size(), 3);
  EXPECT_EQ(v[0], 10);
  EXPECT_EQ(v[1], 20);
  EXPECT_EQ(v[2], 30);
  EXPECT_EQ(*it, 10);
}

TEST(myVectorTest, InsertManySingleElement) {
  my::vector<int> v{1, 2, 3};
  auto i = v.cbegin();
  ++i;
  auto it = v.insert_many(i, 99);

  ASSERT_EQ(v.size(), 4);
  EXPECT_EQ(v[0], 1);
  EXPECT_EQ(v[1], 99);
  EXPECT_EQ(v[2], 2);
  EXPECT_EQ(*it, 99);
}