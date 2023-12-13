#include "RandomUtils.h"
std::mt19937_64& random_utils::GetRandomGenerator() {
  // static std::mt19937_64 rand_generator(42);
  static std::mt19937_64 rand_generator(43);
  // static std::mt19937_64 rand_generator(time(nullptr));
  return rand_generator;
}
size_t random_utils::GetRandomTime(size_t max_time) {
  std::uniform_int_distribution<size_t> time_distribution{0, max_time};
  return time_distribution(GetRandomGenerator());
}
std::stack<std::chrono::time_point<std::chrono::high_resolution_clock>>
    random_utils::Timer::time_points =
    std::stack<std::chrono::time_point<std::chrono::high_resolution_clock>>();
