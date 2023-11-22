#include "gtest/gtest.h"

#include "DataStructures/SimpleRetroactiveArray.h"
#include "DataStructures/SimpleRetroactiveArray2.h"
#include "utils.h"
#include "Utils/random_utils.h"

#include <random>



TEST(SimpleRetroactiveArray2, Test1) {
  std::vector<int64_t> numbers = {1, 2, 3, 4, 5};
  std::shared_ptr<RetroactiveArray<int64_t>>
      expected_array = std::make_shared<SimpleRetroactiveArray<>>(numbers);
  std::shared_ptr<RetroactiveArray<int64_t>>
      actual_array = std::make_shared<SimpleRetroactiveArray2<>>(numbers);
  utils::CompareArrays(actual_array, expected_array);

  expected_array->AssignAtTime(0, 2, 0, 10);
  actual_array->AssignAtTime(0, 2, 0, 10);
  utils::CompareArrays(actual_array, expected_array);

  expected_array->AssignAtTime(0, 1, 1, 1);
  actual_array->AssignAtTime(0, 1, 1, 1);
  utils::CompareArrays(actual_array, expected_array);
}

TEST(SimpleRetroactiveArray2, Random) {
  std::vector<int64_t> numbers = {1, 2, 3, 4, 5, 6};
  std::shared_ptr<RetroactiveArray<int64_t>>
      expected_array = std::make_shared<SimpleRetroactiveArray<>>(numbers);
  std::shared_ptr<RetroactiveArray<int64_t>>
      actual_array = std::make_shared<SimpleRetroactiveArray2<>>(numbers);
  utils::CompareArrays(actual_array, expected_array);

  size_t tests_count = 100;
  for (size_t test = 0; test < tests_count; ++test) {
    auto operation =  random_utils::GetRandomOperation<int64_t>(expected_array->Size());
    PerformOperation(actual_array, operation);
    PerformOperation(expected_array, operation);
    utils::CompareArrays(actual_array, expected_array);
  }
}