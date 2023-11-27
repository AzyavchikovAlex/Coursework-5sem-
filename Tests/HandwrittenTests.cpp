#include "gtest/gtest.h"

#include "DataStructures/SimpleRetroactiveArray.h"
#include "DataStructures/RetroactiveArray.h"
#include "TestingFunctions.h"

TEST(SimpleRetroactiveArray, Test1) {
  std::vector<int64_t> numbers = {1, 2, 3, 4, 5};
  std::shared_ptr<RetroactiveArray<int64_t>> array = std::make_shared<SimpleRetroactiveArray<>>(numbers);
  testing_functions::CompareArrays(array, numbers);

  array->AssignAtTime({0, 2}, 0, 10);
  testing_functions::CompareArrays(array, {0, 0, 3, 4, 5});

  array->AssignAtTime({0, 1}, 1, 1);
  testing_functions::CompareArrays(array, {0, 0, 3, 4, 5});
}

TEST(SimpleRetroactiveArray, Test2) {
  std::vector<int64_t> numbers = {1, 2, 3, 4};
  std::shared_ptr<RetroactiveArray<int64_t>> array = std::make_shared<SimpleRetroactiveArray<>>(numbers);
  testing_functions::CompareArrays(array, {1, 2, 3, 4});

  array->AssignAtTime({1, 3}, 2, 3);
  testing_functions::CompareArrays(array, {1, 2, 2, 4});

  array->AssignAtTime({2, 4}, 5, 2);
  testing_functions::CompareArrays(array, {1, 2, 2, 5});

  array->AssignAtTime({0, 4}, 0, 1);
  testing_functions::CompareArrays(array, {0, 2, 2, 5});

  array->AssignAtTime({0, 4}, 1, 4);
  testing_functions::CompareArrays(array, {1, 1, 1, 1});
}

TEST(SimpleRetroactiveArray, Throw) {
  std::vector<int64_t> numbers = {0};
  std::shared_ptr<RetroactiveArray<int64_t>> array = std::make_shared<SimpleRetroactiveArray<>>(numbers);
  testing_functions::CompareArrays(array, numbers);

  array->AssignAtTime({0, 1}, 1, 5);
  testing_functions::CompareArrays(array, {1});

  array->AssignAtTime({0, 1}, 10, 2);
  testing_functions::CompareArrays(array, {1});

  EXPECT_THROW(array->AssignAtTime({2, 4}, 42, 6), std::runtime_error);
}
