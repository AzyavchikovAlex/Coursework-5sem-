#pragma once

#include <vector>
#include <memory>
#include <exception>
#include "Utils/Segment.h"

template<typename T>
class PersistentSegmentTree {
 private:
  struct Node;
 public:
  PersistentSegmentTree() = default;
  explicit PersistentSegmentTree(const std::vector<T>& values) {
    array_size_ = values.size();
    tree_size_ = ProperSize(values.size());
    root_ = BuildTree(values, {0, tree_size_});
  }
  PersistentSegmentTree(std::shared_ptr<const Node>&& root, size_t size)
      : tree_size_(
      size), root_(std::move(root)) {

  }
  ~PersistentSegmentTree() = default;

  std::shared_ptr<PersistentSegmentTree<T>> Assign(const Segment& segment,
                                                   T value) const {
    return std::make_shared<PersistentSegmentTree<T>>(Assign2(segment,
                                                             value,
                                                             root_,
                                                             {0, tree_size_}),
                                                      tree_size_);
  }

  [[nodiscard]] T Sum(const Segment& segment) const {
    return Sum(segment, root_, {0, tree_size_});
  }

  size_t Size() const {
    return array_size_;
  }

 private:
  static size_t ProperSize(size_t size) {
    size_t proper_size = 1;
    while (proper_size < size) {
      proper_size <<= 1;
    }
    return proper_size;
  }

  static std::shared_ptr<const Node> SetValue(const std::shared_ptr<const Node>& node, Segment
   segment, T value) {
    return std::make_shared<const Node>(segment.Size() > 1,
                                        value,
                                        segment.Size() * value,
                                        node->left_son,
                                        node->right_son);
  }

  static std::shared_ptr<const Node> Assign2(const Segment& searching_segment,
                      T value,
                      const std::shared_ptr<const Node>& node,
                      const Segment& segment,
                      std::optional<T> propagation_value = std::nullopt) {
    if (node == nullptr || !(searching_segment.l >= segment.l && searching_segment.r <= segment.r)) {
      throw std::runtime_error("Bad data");
    }
    if (searching_segment == segment) {
      return std::make_shared<Node>(segment.Size() > 1,
                                    value,
                                    value * segment.Size(),
                                    node->left_son,
                                    node->right_son);
    }
    if (node->has_operation && !propagation_value.has_value()) {
      propagation_value = node->modify;
    }
    auto new_left_son = node->left_son;
    auto new_right_son = node->right_son;
    size_t m = segment.GetMid();
    if (searching_segment.r <= m) {
      new_left_son = Assign2(searching_segment,
                            value,
                            node->left_son,
                            {segment.l, m},
                            propagation_value);
      if (propagation_value.has_value()) {
        new_right_son = SetValue(new_right_son, {m, segment.r}, propagation_value.value());
      }
    } else if (searching_segment.l >= m) {
      new_right_son = Assign2(searching_segment,
                             value,
                             node->right_son,
                             {m, segment.r},
                             propagation_value);
      if (propagation_value.has_value()) {
        new_left_son = SetValue(new_left_son, {segment.l, m}, propagation_value.value());
      }
    } else {
      new_left_son = Assign2({searching_segment.l, m},
                            value,
                            node->left_son,
                            {segment.l, m},
                            propagation_value);
      new_right_son = Assign2({m, searching_segment.r},
                             value,
                             node->right_son,
                             {m, segment.r},
                             propagation_value);
    }
    return std::make_shared<const Node>(0,
                                        0,
                                        new_left_son->sum + new_right_son->sum,
                                        new_left_son,
                                        new_right_son);
  }

  static T Sum(const Segment& searching_segment,
               const std::shared_ptr<const Node>& node,
               const Segment& segment) {

    if (node == nullptr || !(searching_segment.l >= segment.l && searching_segment.r <= segment.r)) {
      throw std::runtime_error("Bad data");
    }
    // assert(node != nullptr);
    // assert(searching_segment.GetL() >= segment.GetL()
    //            && searching_segment.GetR() <= segment.GetR());
    if (node->has_operation) {
      return node->modify * static_cast<T >(searching_segment.Size());
    }
    if (searching_segment == segment) {
      return node->sum;
    }
    size_t mid = segment.GetMid();
    if (searching_segment.r <= mid) {
      return Sum(searching_segment, node->left_son, {segment.l, mid});
    }
    if (searching_segment.l >= mid) {
      return Sum(searching_segment, node->right_son, {mid, segment.r});
    }
    return Sum({searching_segment.l, mid},
               node->left_son,
               {segment.l, mid})
        + Sum({mid, searching_segment.r},
              node->right_son,
              {mid, segment.r});
  }

  static std::shared_ptr<const Node> BuildTree(const std::vector<T>& values,
                                               const Segment& segment) {
    if (segment.Size() == 1) {
      if (segment.l < values.size()) {
        return std::make_shared<const Node>(false, 0, values[segment.l],
                                            nullptr, nullptr);
      }
      return std::make_shared<const Node>(false, 0, 0, nullptr, nullptr);
    }
    size_t mid = segment.GetMid();
    auto left_son = BuildTree(values, {segment.l, mid});
    auto right_son = BuildTree(values, {mid, segment.r});
    return std::make_shared<const Node>(false,
                                        0,
                                        left_son->sum + right_son->sum,
                                        left_son,
                                        right_son);
  }

  size_t array_size_{0};
  size_t tree_size_{0};
  std::shared_ptr<const Node> root_{nullptr};
  struct Node {
    bool has_operation{false};
    T modify{0};
    T sum{0};
    std::shared_ptr<const Node> left_son{nullptr};
    std::shared_ptr<const Node> right_son{nullptr};
  };
};