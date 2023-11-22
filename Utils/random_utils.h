#pragma once

#include "DataStructures/SimpleRetroactiveArray.h"
#include "DataStructures/SimpleRetroactiveArray2.h"

#include <random>
#include <cassert>
#include <memory>

namespace random_utils {

template<typename T>
struct Operation {
  size_t l, r;
  T value;
  size_t time;
};

template<typename T>
Operation<T> GetRandomOperation(size_t size,
                                int64_t max_value = 100,
                                size_t max_time = 100) {
  static std::mt19937_64 rand_generator(42);
  assert (size >= 2);
  std::uniform_int_distribution<size_t> l_distribution{0, size - 2};
  size_t l = l_distribution(rand_generator);
  std::uniform_int_distribution<size_t> r_distribution{l + 1, size};
  size_t r = r_distribution(rand_generator);
  std::uniform_int_distribution<int64_t> value_distribution{0, max_value};
  T value = value_distribution(rand_generator);
  std::uniform_int_distribution<size_t> time_distribution{0, max_time};
  size_t time = value_distribution(rand_generator);
  return {l, r, value, time};
}

template<typename T>
void PerformOperation(
    const std::shared_ptr<RetroactiveArray<T>>& array,
    const Operation<T>& operation) {
  array->AssignAtTime(operation.l,
                      operation.r,
                      operation.value,
                      operation.time);
}

} // namespace random_utils
