#pragma once

#include <vector>
#include <memory>
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
  ~PersistentSegmentTree() = default;

  [[nodiscard]] PersistentSegmentTree Assign(const Segment& segment,
                                             T value) const {
    return {Assign(segment, value, root_, {0, tree_size_}), tree_size_};
  }

  [[nodiscard]] T Sum(const Segment& segment) const {
    return Sum(segment, root_, {0, tree_size_});
  }

  size_t Size() const {
    return array_size_;
  }

 private:
  PersistentSegmentTree(std::shared_ptr<const Node>&& root, size_t size)
      : tree_size_(
      size), root_(std::move(root)) {

  }
  static size_t ProperSize(size_t size) {
    size_t proper_size = 1;
    while (proper_size < size) {
      proper_size <<= 1;
    }
    return proper_size;
  }

  static std::shared_ptr<const Node> Propagate(const std::shared_ptr<const Node>& node,
                                               const Segment& segment) {
    assert(node != nullptr);
    if (!node->has_operation) {
      return node;
    }
    if (segment.GetR() - segment.GetL() == 1) {
      return std::make_shared<const Node>(false, 0, node->modify, nullptr,
                                          nullptr);
    }
    size_t mid = segment.GetMid();
    auto left_son = std::make_shared<const Node>(true,
                                                 node->modify,
                                                 node->modify
                                                     * static_cast<T>(
                                                         mid - segment.GetL()),
                                                 node->left_son->left_son,
                                                 node->left_son->right_son);
    auto right_son = std::make_shared<const Node>(true,
                                                  node->modify,
                                                  node->modify
                                                      * static_cast<T>(
                                                          segment.GetR() - mid),
                                                  node->right_son->left_son,
                                                  node->right_son->right_son);
    return std::make_shared<const Node>(false,
                                        0,
                                        left_son->sum + right_son->sum,
                                        left_son,
                                        right_son);
  }

  static std::shared_ptr<const Node> Assign(const Segment& searching_segment,
                                            T value,
                                            const std::shared_ptr<const Node>& node,
                                            const Segment& segment) {
    assert(node != nullptr);
    assert(searching_segment.GetL() >= segment.GetL()
               && searching_segment.GetR() <= segment.GetR());
    if (searching_segment == segment) {
      return std::make_shared<const Node>(true,
                                          value,
                                          value * (segment.GetR()
                                              - segment.GetL()),
                                          node->left_son,
                                          node->right_son);
    }
    auto new_node = Propagate(node, segment);
    auto new_left_son = new_node->left_son;
    auto new_right_son = new_node->right_son;
    size_t m = segment.GetMid();
    if (searching_segment.GetR() <= m) {
      new_left_son = Assign(searching_segment,
                            value,
                            new_node->left_son,
                            {segment.GetL(), m});
    } else if (searching_segment.GetL() >= m) {
      new_right_son = Assign(searching_segment,
                             value,
                             new_node->right_son,
                             {m, segment.GetR()});
    } else {
      new_left_son = Assign({searching_segment.GetL(), m},
                            value,
                            new_node->left_son,
                            {segment.GetL(), m});
      new_right_son = Assign({m, searching_segment.GetR()},
                             value,
                             new_node->right_son,
                             {m, segment.GetR()});
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
    assert(node != nullptr);
    assert(searching_segment.GetL() >= segment.GetL()
               && searching_segment.GetR() <= segment.GetR());
    if (node->has_operation) {
      return node->modify * static_cast<T >(searching_segment.GetR()
          - searching_segment.GetL());
    }
    if (searching_segment == segment) {
      return node->sum;
    }
    size_t mid = segment.GetMid();
    if (searching_segment.GetR() <= mid) {
      return Sum(searching_segment, node->left_son, {segment.GetL(), mid});
    }
    if (searching_segment.GetL() >= mid) {
      return Sum(searching_segment, node->right_son, {mid, segment.GetR()});
    }
    return Sum({searching_segment.GetL(), mid},
               node->left_son,
               {segment.GetL(), mid})
        + Sum({mid, searching_segment.GetR()},
              node->right_son,
              {mid, segment.GetR()});
  }

  static std::shared_ptr<const Node> BuildTree(const std::vector<T>& values,
                                               const Segment& segment) {
    if (segment.GetR() - segment.GetL() == 1) {
      if (segment.GetL() < values.size()) {
        return std::make_shared<const Node>(false, 0, values[segment.GetL()],
                                            nullptr, nullptr);
      }
      return std::make_shared<const Node>(false, 0, 0, nullptr, nullptr);
    }
    size_t mid = (segment.GetR() + segment.GetL()) / 2;
    auto left_son = BuildTree(values, {segment.GetL(), mid});
    auto right_son = BuildTree(values, {mid, segment.GetR()});
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