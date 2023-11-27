#pragma once

#include <optional>

#include "RetroactiveArray.h"
#include "PersistentSegmetTree.h"

// Insert = O(t * log(n))
// Get = O(log(t) * log(n))

template<typename T=int64_t>
class RetroactiveArray1_0 : public RetroactiveArray<T> {
 public:
  explicit RetroactiveArray1_0(const std::vector<T>& init_values) : versions_() {
    versions_.push_back({PersistentSegmentTree(init_values),
                        {0, init_values.size()},
                        0});
  }

  virtual ~RetroactiveArray1_0() = default;

  virtual void AssignAtTime(const Segment& segment, T value, size_t time) {
    auto prev_version = std::lower_bound(versions_.begin(),
                                         versions_.end(),
                                         time,
                                         [](const Version& version,
                                            size_t time) {
                                           return version.time <= time;
                                         });
    Version new_version{{}, segment, value, time};
    if (prev_version == versions_.end()) {
      new_version.tree = versions_.back().tree.Assign(segment, value);
      versions_.push_back(new_version);
      return;
    }
    new_version.tree = (*prev_version).tree.Assign(segment, value);
    size_t new_version_index = versions_.size();
    versions_.push_back(new_version);
    for (size_t i = versions_.size() - 1; i > 0; --i) {
      if (versions_[i].time >= versions_[i - 1].time) {
        break;
      }
      std::swap(versions_[i], versions_[i - 1]);
      --new_version_index;
    }
    for (size_t i = new_version_index + 1; i < versions_.size(); ++i) {
      assert(i > 0);
      versions_[i].tree = versions_[i - 1].tree.Assign(versions_[i].segment, versions_[i].value);
    }
  }
  virtual T GetSum(const Segment& segment, size_t time) const {
    auto version = std::lower_bound(versions_.begin(),
                                    versions_.end(),
                                    time,
                                    [](const Version& version, size_t time) {
                                      return version.time <= time;
                                    });
    if (version == versions_.end()) {
      return versions_.back().tree.Sum(segment);
    } else {
      return (*version).tree.Sum(segment);
    }
  }

  [[nodiscard]] virtual size_t Size() const {
    return versions_.front().tree.Size();
  }

 private:
  struct Version {
    PersistentSegmentTree<T> tree;
    Segment segment;
    T value;
    size_t time;
  };
  std::vector<Version> versions_{};
};