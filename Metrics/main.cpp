#include <iostream>
#include <chrono>
#include <iomanip>

#include "Utils/RandomUtils.h"
#include "DataStructures/RetroactiveArray.h"

class Timer {
 public:
  static void Start() {
    Timer::time_point = std::chrono::high_resolution_clock::now();
  }

  static auto Stop() {
    auto finish = std::chrono::high_resolution_clock::now();
    auto total_work_time =
        std::chrono::duration<double>(finish - time_point).count()
            * 1000;
    return total_work_time;
  }

 private:
  static std::chrono::time_point<std::chrono::high_resolution_clock> time_point;
};
std::chrono::time_point<std::chrono::high_resolution_clock>
    Timer::time_point = std::chrono::high_resolution_clock::now();

template<typename T>
double PerformInsertOperations(const std::shared_ptr<RetroactiveArray<T>>& array,
                               size_t operations_count = 100) {
  assert(operations_count > 0);
  std::vector<random_utils::Operation<T>> operations;
  for (size_t operation = 0; operation < operations_count; ++operation) {
    operations.push_back(random_utils::GetRandomOperation<T>(array->Size()));
  }
  Timer::Start();
  for (const random_utils::Operation<T>& operation : operations) {
    array->AssignAtTime({operation.l,
                        operation.r},
                        operation.value,
                        operation.time);
  }
  return Timer::Stop();
}

template<typename T>
double PerformInsertAndSumOperations(const std::shared_ptr<RetroactiveArray<T>>& array,
                                     size_t operations_count = 100) {
  assert(operations_count > 0);
  std::vector<random_utils::Operation<T>> operations;
  for (size_t operation = 0; operation < operations_count; ++operation) {
    operations.push_back(random_utils::GetRandomOperation<T>(array->Size()));
  }
  Timer::Start();
  for (const random_utils::Operation<T>& operation : operations) {
    array->AssignAtTime({operation.l,
                        operation.r},
                        operation.value,
                        operation.time);
    auto value = array->GetSum({operation.l, operation.r}, std::numeric_limits<size_t>::max());
  }
  return Timer::Stop();
}

template<typename K, typename T>
void Measure(size_t size, size_t operations_count = 1000) {
  std::shared_ptr<RetroactiveArray<T>>
      array =
      std::make_shared<K>(std::vector<T>(size, 0));
  auto insert_metrics = PerformInsertOperations(array, operations_count);
  auto insert_and_sum_metrics =
      PerformInsertAndSumOperations(array, operations_count);
  std::cout << "Size " << std::setw(10) << size
            << "\t\t Insert " << std::setw(20)
            << insert_metrics / operations_count << "ms\t"
            << "Sum " << std::setw(20)
            << insert_and_sum_metrics / operations_count << "ms\n";
}

int main() {
  std::cout << "1000 operations test\n";
  // {
  //   std::cout << "SimpleRetroactiveArray2 \n";
  //   Measure<SimpleRetroactiveArray2<int64_t>, int64_t>(100);
  //   Measure<SimpleRetroactiveArray2<int64_t>, int64_t>(1000);
  //   Measure<SimpleRetroactiveArray2<int64_t>, int64_t>(10000);
  //   Measure<SimpleRetroactiveArray2<int64_t>, int64_t>(100000);
  //   Measure<SimpleRetroactiveArray2<int64_t>, int64_t>(1000000);
  // }
  {
    std::cout << "SimpleRetroactiveArray \n";
    Measure<SimpleRetroactiveArray<int64_t>, int64_t>(100);
    Measure<SimpleRetroactiveArray<int64_t>, int64_t>(1000);
    Measure<SimpleRetroactiveArray<int64_t>, int64_t>(10000);
    Measure<SimpleRetroactiveArray<int64_t>, int64_t>(20000);
  }
  return 0;
}
