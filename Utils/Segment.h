#pragma once

#include <cassert>

class Segment {
 public:
  Segment() = delete;
  Segment(size_t l, size_t r) : l_(l), r_(r) {
    assert(l < r);
  }

  bool operator==(const Segment& segment) const {
    assert(!IsEmpty());
    return l_ == segment.l_ && r_ == segment.r_;
  }

  [[nodiscard]] size_t Size() const {
    return r_ - l_;
  }
  [[nodiscard]] bool IsEmpty() const {
    return l_ == r_;
  }

  [[nodiscard]] size_t GetL() const {
    return l_;
  }
  [[nodiscard]] size_t GetR() const {
    return r_;
  }
  [[nodiscard]] size_t GetMid() const {
    return (r_ + l_) / 2;
  }

 private:
  size_t l_, r_;
};