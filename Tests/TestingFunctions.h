#pragma once

#include <gtest/gtest.h>

#include "DataStructures/SimpleRetroactiveArray.h"
#include "Utils/RandomUtils.h"

#include <random>
#include <cassert>
#include <memory>
#include <thread>
#include <limits>

namespace testing_functions {

template<typename T>
void CompareArrays(const std::shared_ptr<RetroactiveArray<T>>& array,
                   const std::vector<int64_t>& expected_values) {
  ASSERT_EQ(array->Size(), expected_values.size());
  for (size_t l = 0; l < expected_values.size(); ++l) {
    int64_t sum = 0;
    for (size_t r = l + 1; r <= expected_values.size(); ++r) {
      sum += expected_values[r - 1];
      ASSERT_EQ(array->GetSum({l, r}, std::numeric_limits<size_t>::max()), sum);
    }
  }
}

template<typename T>
void CompareArrays(const std::shared_ptr<RetroactiveArray<T>>& actual_array,
                   const std::shared_ptr<RetroactiveArray<T>>& expected_array,
                   size_t max_time = std::numeric_limits<size_t>::max()) {
  ASSERT_EQ(actual_array->Size(), expected_array->Size());
  for (size_t l = 0; l < expected_array->Size(); ++l) {
    for (size_t r = l + 1; r <= expected_array->Size(); ++r) {
      size_t time = random_utils::GetRandomTime(max_time);
      if (actual_array->GetSum({l, r}, time)
          != expected_array->GetSum({l, r}, time)) {
        std::cerr << l << " " << r <<  " " << time << "\n";
        std::cerr << actual_array->GetSum({l, r}, time) << "\t" << expected_array->GetSum({l, r}, time) << "\n";
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000ms);
        std::abort();
      }
      ASSERT_EQ(actual_array->GetSum({l, r}, time),
                expected_array->GetSum({l, r}, time));
    }
  }
}
} // namespace testing_functions
