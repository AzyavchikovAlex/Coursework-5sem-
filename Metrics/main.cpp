#include <iostream>
#include <chrono>
#include <iomanip>
#include <random>
#include <stack>
#include <functional>

#include "Utils/RandomUtils.h"
#include "DataStructures/RetroactiveArray.h"
#include "DataStructures/RetroactiveArray1_0.h"
#include "DataStructures/SegmentTree.h"

template<typename T>
random_utils::Metrics MeasureInsertOperations(const std::shared_ptr<
    RetroactiveArray<T>>& array,
                                              size_t q) {
  assert(q > 0);
  random_utils::Metrics metrics;
  std::vector<random_utils::Operation<T>> operations;
  for (size_t operation = 0; operation < q; ++operation) {
    operations.push_back(random_utils::GetRandomOperation<T>(array->Size()));
    operations.back().time = q + 1 - operation;
  }
  random_utils::Timer::Start();
  for (size_t operation_index = 0; operation_index < operations.size(); ++operation_index) {
    random_utils::Timer::Start();
    array->AssignAtTime({operations[operation_index].l,
                         operations[operation_index].r},
                        operations[operation_index].value,
                        operations[operation_index].time);
    metrics.time_per_operation += random_utils::Timer::Stop();
  }
  metrics.time_per_operation /= q;
  metrics.total_time = random_utils::Timer::Stop();
  return metrics;
}

template<typename T>
random_utils::Metrics MeasureDeleteOperations(
    const std::shared_ptr<RetroactiveArray<T>>& array,
    size_t q) {
  assert(q > 0);
  size_t operations_count = 50;
  random_utils::Metrics metrics;
  std::vector<random_utils::Operation<T>> operations;
  for (size_t operation = 0; operation < q; ++operation) {
    operations.push_back(random_utils::GetRandomOperation<T>(array->Size()));
    operations.back().time = operation + 1;
  }
  for (const random_utils::Operation<T>& operation : operations) {
    array->AssignAtTime({operation.l,
                         operation.r},
                        operation.value,
                        operation.time);
  }
  for (size_t operation_index = 0; operation_index < operations.size()
      && operation_index < operations_count; ++operation_index) {
    random_utils::Timer::Start();
    array->DeleteOperations(operations[operation_index].time);
    metrics.time_per_operation += random_utils::Timer::Stop();
  }
  metrics.time_per_operation /= std::min(operations.size(), operations_count);
  metrics.total_time = metrics.time_per_operation * q;
  return metrics;
}

template<typename T>
random_utils::Metrics MeasureSumOperations(
    const std::shared_ptr<RetroactiveArray<T>>& array,
    size_t q) {
  assert(q > 0);
  size_t operations_count = 1000;
  random_utils::Metrics metrics;

  std::vector<random_utils::Operation<T>> operations;
  for (size_t operation = 0; operation < q; ++operation) {
    operations.push_back(random_utils::GetRandomOperation<T>(array->Size()));
    operations.back().time = operation + 1;
  }
  for (const random_utils::Operation<T>& operation : operations) {
    array->AssignAtTime({operation.l,
                         operation.r},
                        operation.value,
                        operation.time);
  }

  std::shuffle(operations.begin(),
               operations.end(),
               random_utils::GetRandomGenerator());
  for (size_t operation_index = 0; operation_index < operations.size()
      && operation_index < operations_count; ++operation_index) {
    random_utils::Timer::Start();
    array->GetSum({operations[operation_index].l,
                   operations[operation_index].r},
                  operations[operation_index].time);
    metrics.time_per_operation += random_utils::Timer::Stop();
  }
  metrics.time_per_operation /= std::min(operations.size(), operations_count);
  metrics.total_time = metrics.time_per_operation * q;
  return metrics;
}

template<typename K, typename T>
void MeasureOperation(const std::vector<size_t>& n_values,
                      const std::vector<size_t>& q_values,
                      auto measuring_function,
                      std::ostream& stream) {
  size_t operations_count = 50;

  stream << std::setw(10) << std::setprecision(7)
         << "N\\Q";
  stream << std::setw(3) << " ";
  for (auto q : q_values) {
    stream << std::setw(15) << std::setprecision(7)
           << q;
    stream << std::setw(3) << " ";
  }
  stream << "\n";
  for (auto n : n_values) {
    stream << std::setw(10) << std::setprecision(7)
           << std::to_string(n) + ":";
    stream << std::setw(3) << " ";
    for (auto q : q_values) {
      std::shared_ptr<RetroactiveArray<T>>
          array = std::make_shared<K>(std::vector<T>(n, 0));
      random_utils::Metrics
          metrics = measuring_function(array, q);
      stream << std::setw(15) << std::setprecision(7) << std::fixed
             << metrics.time_per_operation;
      stream << std::setw(3) << " ";
    }
    stream << "\n";
  }
}

int main() {
  std::vector<size_t> ns = {1000, 10'000, 100'000};
  std::vector<size_t> qs = {100, 200, 400, 800};
  // MeasureOperation<RetroactiveArray1_0<int64_t>, int64_t>(ns,
  //                                                         qs,
  //                                                         MeasureInsertOperations<
  //                                                             int64_t>,
  //                                                         std::cout);
  //
  // MeasureOperation<RetroactiveArray1_0<int64_t>, int64_t>(ns,
  //                                                         qs,
  //                                                         MeasureDeleteOperations<
  //                                                             int64_t>,
  //                                                         std::cout);
  MeasureOperation<RetroactiveArray1_0<int64_t>, int64_t>({10'000, 100'000, 1'000'000},
                                                          {1000, 10'000, 100'000},
                                                          MeasureSumOperations<
                                                              int64_t>,
                                                          std::cout);

  return 0;
}
