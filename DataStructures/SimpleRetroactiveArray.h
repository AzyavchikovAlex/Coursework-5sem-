#pragma once

#include "RetroactiveArray.h"

// Insert = O(n)
// Get = O(n^2 * t)

template<typename T=int64_t>
class SimpleRetroactiveArray : public RetroactiveArray<T> {
 public:
  explicit SimpleRetroactiveArray(const std::vector<T>& values) : init_values_(
      values) {
  }

  virtual ~SimpleRetroactiveArray() = default;

  virtual void AssignAtTime(size_t l, size_t r, T value, size_t time) {
    if (l > r || r > init_values_.size()) {
      throw std::runtime_error("Out of bound indexes");
    }
    Operation new_operation(time, l, r, value);
    operations_.push_back(new_operation);
    for (size_t i = operations_.size() - 1; i > 0; --i) {
      if (operations_[i - 1].time > operations_[i].time) {
        std::swap(operations_[i], operations_[i - 1]);
      }
    }
  }
  virtual T GetCurrentSum(size_t l, size_t r) const {
    std::vector<T> new_values = init_values_;
    for (const Operation& operation : operations_) {
      for (size_t i = operation.l; i < operation.r; ++i) {
        new_values[i] = operation.value;
      }
    }
    T sum = 0;
    for (size_t i = l; i < r; ++i) {
      sum += new_values[i];
    }
    return sum;
  }

  [[nodiscard]] virtual size_t Size() const {
    return init_values_.size();
  }

 private:
  struct Operation {
    size_t time;
    size_t l, r;
    T value;
  };
  std::vector<T> init_values_;
  std::vector<Operation> operations_{};
};
