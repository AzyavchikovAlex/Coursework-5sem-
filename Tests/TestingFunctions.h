#pragma once

#include <gtest/gtest.h>

#include "DataStructures/SimpleRetroactiveArray.h"

#include <random>
#include <cassert>
#include <memory>
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
                   const std::shared_ptr<RetroactiveArray<T>>& expected_array) {
  ASSERT_EQ(actual_array->Size(), expected_array->Size());
  for (size_t l = 0; l < expected_array->Size(); ++l) {
    for (size_t r = l + 1; r <= expected_array->Size(); ++r) {
      ASSERT_EQ(actual_array->GetSum({l, r},
                                     std::numeric_limits<size_t>::max()),
                expected_array->GetSum({l, r},
                                       std::numeric_limits<size_t>::max()));
    }
  }
}
} // namespace testing_functions
