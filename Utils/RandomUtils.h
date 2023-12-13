#pragma once

#include "DataStructures/SimpleRetroactiveArray.h"

#include <random>
#include <cassert>
#include <memory>
#include <chrono>
#include <stack>

namespace random_utils {


class Timer {
 public:
  static void Start() {
    // Timer::time_point = std::chrono::high_resolution_clock::now();
    Timer::time_points.push(std::chrono::high_resolution_clock::now());
  }

  static auto Stop() {
    auto finish = std::chrono::high_resolution_clock::now();
    auto total_work_time =
        std::chrono::duration<double>(finish - Timer::time_points.top()).count()
            * 1000;
    Timer::time_points.pop();
    return total_work_time;
  }

 private:
  static std::stack<std::chrono::time_point<std::chrono::high_resolution_clock>>
      time_points;
};

struct Metrics {
  long double total_time{0};
  long double time_per_operation{0};
};

template<typename T>
struct Operation {
  size_t l, r;
  T value;
  size_t time;
};

std::mt19937_64& GetRandomGenerator();

size_t GetRandomTime(size_t max_time);

template<typename T>
Operation<T> GetRandomOperation(size_t size,
                                int64_t max_value = 100,
                                size_t max_time = 100) {
  assert (size >= 2);
  std::uniform_int_distribution<size_t> l_distribution{0, size - 2};
  size_t l = l_distribution(GetRandomGenerator());
  std::uniform_int_distribution<size_t> r_distribution{l + 1, size};
  size_t r = r_distribution(GetRandomGenerator());
  std::uniform_int_distribution<int64_t> value_distribution{0, max_value};
  T value = value_distribution(GetRandomGenerator());
  return {l, r, value, GetRandomTime(max_time)};
}

template<typename T>
void PerformAssignOperation(
    const std::shared_ptr<RetroactiveArray<T>>& array,
    const Operation<T>& operation) {
  array->AssignAtTime({operation.l,
                       operation.r},
                      operation.value,
                      operation.time);
}

template<typename T>
void PerformDeleteOperation(
    const std::shared_ptr<RetroactiveArray<T>>& array,
    size_t time) {
  array->DeleteOperations(time);
}

} // namespace random_utils
