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

  virtual void AssignAtTime(const Segment& segment, T value, size_t time) {
    if (segment.GetR() > init_values_.size()) {
      throw std::runtime_error("Out of bound indexes");
    }
    Operation new_operation(segment, time, value);
    operations_.push_back(new_operation);
    for (size_t i = operations_.size() - 1; i > 0; --i) {
      if (operations_[i - 1].time > operations_[i].time) {
        std::swap(operations_[i], operations_[i - 1]);
      } else {
        break;
      }
    }
  }
  virtual T GetSum(const Segment& segment, size_t time) const {
    std::vector<T> new_values = init_values_;
    for (const Operation& operation : operations_) {
      if (operation.time > time) {
        break;
      }
      for (size_t i = operation.segment.GetL(); i < operation.segment.GetR();
           ++i) {
        new_values[i] = operation.value;
      }
    }
    T sum = 0;
    for (size_t i = segment.GetL(); i < segment.GetR(); ++i) {
      sum += new_values[i];
    }
    return sum;
  }

  [[nodiscard]] virtual size_t Size() const {
    return init_values_.size();
  }

 private:
  struct Operation {
    Segment segment;
    size_t time;
    T value;
  };
  std::vector<T> init_values_;
  std::vector<Operation> operations_{};
};
