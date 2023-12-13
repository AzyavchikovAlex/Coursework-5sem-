#pragma once

#include <optional>
#include <memory>
#include <cassert>
#include <map>

#include "RetroactiveArray.h"

// Insert = O(n * log(t))
// Get = O(n * log(t))

template<typename T=int64_t>
class RetroactiveArray2_0 : public RetroactiveArray<T> {
 public:
  explicit RetroactiveArray2_0(const std::vector<T>& init_values)
      : init_values_(init_values), modifications(init_values.size()) {
  }

  virtual ~RetroactiveArray2_0() = default;

  virtual void AssignAtTime(const Segment& segment, T value, size_t time) {
    for (size_t i = segment.l; i < segment.r; ++i) {
      modifications[i][time] = value;
    }
  }
  virtual void DeleteOperations(size_t time) {
    for (size_t i = 0; i < Size(); ++i) {
      modifications[i].erase(time);
    }
  }

  virtual T GetSum(const Segment& segment, size_t time) const {
    T sum = 0;
    for (size_t i = segment.l; i < segment.r; ++i) {
      auto next_modification = modifications[i].upper_bound(time);
      if (next_modification == modifications[i].begin()) {
        sum += init_values_[i];
      } else {
        --next_modification;
        sum += next_modification->second;
      }
    }
    return sum;
  }

  [[nodiscard]] virtual size_t Size() const {
    return init_values_.size();
  }

 private:
  std::vector<std::map<size_t, T>> modifications;
  std::vector<T> init_values_;
};