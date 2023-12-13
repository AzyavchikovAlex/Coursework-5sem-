#pragma once

#include <stdexcept>

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

  void AssignAtTime(const Segment& segment, T value, size_t time) override {
    if (segment.r > init_values_.size()) {
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

  void DeleteOperations(size_t time) override {
    size_t insert_pos = 0;
    for (size_t i = 0; i < operations_.size(); ++i) {
      if (operations_[i].time != time) {
        std::swap(operations_[i], operations_[insert_pos]);
        ++insert_pos;
      }
    }
    operations_.resize(insert_pos);
  }
  T GetSum(const Segment& segment, size_t time) const override {
    std::vector<T> new_values = init_values_;
    for (const Operation& operation : operations_) {
      if (operation.time > time) {
        break;
      }
      for (size_t i = operation.segment.l; i < operation.segment.r; ++i) {
        new_values[i] = operation.value;
      }
    }
    T sum = 0;
    for (size_t i = segment.l; i < segment.r; ++i) {
      sum += new_values[i];
    }
    return sum;
  }

  [[nodiscard]] size_t Size() const override {
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
