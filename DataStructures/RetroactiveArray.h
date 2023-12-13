#pragma once

#include <vector>

#include "Utils/Segment.h"

template <typename T>
class RetroactiveArray {
 public:
  virtual ~RetroactiveArray() = default;
  virtual void AssignAtTime(const Segment& segment, T value, size_t time) = 0;
  virtual void DeleteOperations(size_t time) = 0;
  virtual T GetSum(const Segment& segment, size_t time) const = 0;
  [[nodiscard]] virtual size_t Size() const = 0;
};