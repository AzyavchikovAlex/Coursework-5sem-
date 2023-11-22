#pragma once

#include <vector>

template <typename T>
class RetroactiveArray {
 public:
  virtual void AssignAtTime(size_t l, size_t r, T value, size_t time) = 0;
  virtual ~RetroactiveArray() = default;
  virtual T GetCurrentSum(size_t l, size_t r) const = 0;
  [[nodiscard]] virtual size_t Size() const = 0;
};