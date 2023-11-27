#include "gtest/gtest.h"

#include "DataStructures/SimpleRetroactiveArray.h"
#include "DataStructures/RetroactiveArray1_0.h"
#include "DataStructures/PersistentSegmetTree.h"
#include "DataStructures/SegmentTree.h"
#include "TestingFunctions.h"
#include "Utils/RandomUtils.h"

TEST(PersistentSegmentTree, Random) {
  std::vector<int64_t> values(1000, 0);
  PersistentSegmentTree<int64_t> tree(values);
  Tree<int64_t> correct_tree(values);
  for (int tests = 0; tests < 50000; ++tests) {
    auto operation = random_utils::GetRandomOperation<int64_t>(values.size());
    tree = tree.Assign({operation.l, operation.r}, operation.value);
    correct_tree.Assign(operation.l, operation.r, operation.value);
    ASSERT_EQ(tree.Sum({0, values.size()}), correct_tree.Sum(0, values.size()));
    operation = random_utils::GetRandomOperation<int64_t>(values.size());
    ASSERT_EQ(tree.Sum({operation.l, operation.r}), correct_tree.Sum(operation.l, operation.r));
  }
}

TEST(RetroactiveArray1_0, Random) {
  std::vector<int64_t> numbers(30, 0);
  std::shared_ptr<RetroactiveArray<int64_t>>
      expected_array = std::make_shared<SimpleRetroactiveArray<>>(numbers);
  std::shared_ptr<RetroactiveArray<int64_t>>
      actual_array = std::make_shared<RetroactiveArray1_0<int64_t>>(numbers);
  testing_functions::CompareArrays(actual_array, expected_array);

  size_t tests_count = 500;
  for (size_t test = 0; test < tests_count; ++test) {
    auto operation =  random_utils::GetRandomOperation<int64_t>(expected_array->Size(), 100, 100);
    PerformOperation(actual_array, operation);
    PerformOperation(expected_array, operation);
    testing_functions::CompareArrays(actual_array, expected_array);
  }
}