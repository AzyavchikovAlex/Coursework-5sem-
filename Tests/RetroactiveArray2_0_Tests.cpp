#include "gtest/gtest.h"

#include "DataStructures/SimpleRetroactiveArray.h"
#include "DataStructures/RetroactiveArray2_0.h"
#include "TestingFunctions.h"
#include "Utils/RandomUtils.h"

TEST(RetroactiveArray2_0, Random) {
  size_t size = 30;
  std::vector<int64_t> numbers(size, 0);
  std::shared_ptr<RetroactiveArray<int64_t>>
      expected_array = std::make_shared<SimpleRetroactiveArray<>>(numbers);
  std::shared_ptr<RetroactiveArray<int64_t>>
      actual_array = std::make_shared<RetroactiveArray2_0<>>(numbers);
  testing_functions::CompareArrays(actual_array, expected_array);

  size_t tests_count = 5000;
  int64_t max_value = 100;
  size_t max_time = 100;
  std::set<size_t> using_times;
  for (size_t test = 0; test < tests_count; ++test) {
    if (using_times.empty() || random_utils::GetRandomGenerator()() % 10 >= 6) {
      // 60 percent chance
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