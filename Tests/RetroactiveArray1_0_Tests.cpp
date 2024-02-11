#include "gtest/gtest.h"

#include "DataStructures/SimpleRetroactiveArray.h"
#include "DataStructures/RetroactiveArray1_0.h"
#include "DataStructures/PersistentSegmetTree.h"
#include "DataStructures/SegmentTree.h"
#include "TestingFunctions.h"
#include "Utils/RandomUtils.h"

TEST(PersistentSegmentTree, Test1) {
  std::vector<int64_t> values(4, 0);
  std::shared_ptr<PersistentSegmentTree<int64_t>>
      tree = std::make_shared<PersistentSegmentTree<int64_t>>(values);
  Tree<int64_t> correct_tree(values);
  tree = tree->Assign({0, 2}, 0);
  correct_tree.Assign(0, 2, 0);
  tree = tree->Assign({0, 4}, 2);
  correct_tree.Assign(0, 4, 2);
  tree = tree->Assign({1, 4}, 8);
  correct_tree.Assign(1, 4, 8);

  ASSERT_EQ(tree->Sum({0, values.size()}),
            correct_tree.Sum(0, values.size()));
}

TEST(PersistentSegmentTree, Random) {
  {
    size_t n = 100'000;
    PersistentSegmentTree<int64_t> tree(std::vector<int64_t>(n, 0));
    random_utils::Timer::Start();
    tree.Assign({0, 1}, 1);
    std::cerr << random_utils::Timer::Stop() << "\n";
    Tree tree_best(std::vector<int64_t>(n, 0));
    random_utils::Timer::Start();
    tree_best.Assign(0, 1, 1);
    std::cerr << random_utils::Timer::Stop() << "\n";
  }

  std::vector<int64_t> values(2000, 0);
  std::shared_ptr<PersistentSegmentTree<int64_t>>
      tree = std::make_shared<PersistentSegmentTree<int64_t>>(values);
  Tree<int64_t> correct_tree(values);
  for (int tests = 0; tests < 50000; ++tests) {
    auto operation = random_utils::GetRandomOperation<int64_t>(values.size());
    tree = tree->Assign({operation.l, operation.r}, operation.value);
    correct_tree.Assign(operation.l, operation.r, operation.value);
    ASSERT_EQ(tree->Sum({0, values.size()}),
              correct_tree.Sum(0, values.size()));
    operation = random_utils::GetRandomOperation<int64_t>(values.size());
    ASSERT_EQ(tree->Sum({operation.l, operation.r}),
              correct_tree.Sum(operation.l, operation.r));
  }
}

TEST(RetroactiveArray1_0, Random) {
  size_t size = 50;
  std::vector<int64_t> numbers(size, 0);
  std::shared_ptr<RetroactiveArray<int64_t>>
      expected_array = std::make_shared<SimpleRetroactiveArray<>>(numbers);
  std::shared_ptr<RetroactiveArray<int64_t>>
      actual_array = std::make_shared<RetroactiveArray1_0<int64_t>>(numbers);
  testing_functions::CompareArrays(actual_array, expected_array);

  size_t tests_count = 5000;
  int64_t max_value = 100;
  size_t max_time = 100;
  std::set<size_t> using_times;
  for (size_t test = 0; test < tests_count; ++test) {
    if (using_times.empty() || random_utils::GetRandomGenerator()() % 10 >= 6) {
      // 60 persent chanse
      auto operation =
          random_utils::GetRandomOperation<int64_t>(expected_array->Size(),
                                                    max_value,
                                                    max_time);
      using_times.insert(operation.time);
      // std::cerr << operation.l << " " << operation.r << " " << operation.value << " " <<operation.time << "\n";
      random_utils::PerformAssignOperation<int64_t>(actual_array, operation);
      random_utils::PerformAssignOperation<int64_t>(expected_array, operation);
    } else {
      auto time_point = *std::next(using_times.begin(),
                                   static_cast<size_t>(
                                       random_utils::GetRandomGenerator()()
                                           % using_times.size()));
      random_utils::PerformDeleteOperation(actual_array, time_point);
      random_utils::PerformDeleteOperation(expected_array, time_point);
      using_times.erase(time_point);
    }
    testing_functions::CompareArrays<int64_t>(actual_array,
                                              expected_array,
                                              max_time);
  }
}