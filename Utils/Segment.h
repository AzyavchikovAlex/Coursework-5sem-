#pragma once

#include <cassert>

struct Segment {
  Segment() = default;
  Segment(size_t l_value, size_t r_value) : l(l_value), r(r_value) {
    if (l_value >= r_value) {
      throw std::runtime_error("l >= r");
    }
  }

  bool operator==(const Segment& segment) const {
    if (IsEmpty()) {
      throw std::runtime_error("Empty");
    }
    return l == segment.l && r == segment.r;
  }

  [[nodiscard]] size_t Size() const {
    return r - l;
  }
  [[nodiscard]] bool IsEmpty() const {
    return l == r;
  }

  [[nodiscard]] size_t GetMid() const {
    return (r + l) >> 1;
  }

  size_t l, r;
};