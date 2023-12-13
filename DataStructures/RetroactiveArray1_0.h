#pragma once

#include <optional>

#include "RetroactiveArray.h"
#include "PersistentSegmetTree.h"
#include "Utils/RandomUtils.h"

template<typename T=int64_t>
class RetroactiveArray1_0 : public RetroactiveArray<T> {
 public:
  explicit RetroactiveArray1_0(const std::vector<T>& init_values)
      : versions_() {
    versions_.push_back({std::make_shared<PersistentSegmentTree<T>>(init_values),
                         {0, init_values.size()},
                         0});
  }

  virtual ~RetroactiveArray1_0() = default;

  virtual void AssignAtTime(const Segment& segment, T value, size_t time) {
    Version new_version{nullptr, segment, value, time};
    size_t new_version_index = versions_.size();
    versions_.push_back(new_version);
    for (size_t i = versions_.size() - 1; i > 0; --i) {
      if (versions_[i].time >= versions_[i - 1].time) {
        break;
      }
      std::swap(versions_[i], versions_[i - 1]);
      --new_version_index;
    }
    for (size_t i = new_version_index; i < versions_.size(); ++i) {
      versions_[i].tree = versions_[i - 1].tree->Assign(versions_[i].segment,
                                                        versions_[i].value);
    }
  }
  virtual void DeleteOperations(size_t time) {
    auto prev_version = std::lower_bound(versions_.begin(),
                                         versions_.end(),
                                         time,
                                         [](const Version& version,
                                            size_t time) {
                                           return version.time < time;
                                         });
    if (prev_version == versions_.end() || prev_version->time != time) {
      return;
    }
    size_t insert_pos =
        std::max((size_t) std::distance(versions_.begin(), prev_version),
                 (size_t) 1);
    for (size_t i = insert_pos; i < versions_.size(); ++i) {
      if (versions_[i].time != time) {
        versions_[insert_pos] = versions_[i];
        versions_[insert_pos].tree = versions_[insert_pos - 1].tree->Assign(versions_[insert_pos].segment, versions_[insert_pos].value);
        ++insert_pos;
      }
    }
    versions_.resize(insert_pos);
  }

  virtual T GetSum(const Segment& segment, size_t time) const {
    auto version = std::lower_bound(versions_.begin(),
                                    versions_.end(),
                                    time,
                                    [](const Version& version, size_t time) {
                                      return version.time <= time;
                                    });
    --version;
    assert((*version).time <= time);
    if (version == versions_.end()) {
      return versions_.back().tree->Sum(segment);
    } else {
      return (*version).tree->Sum(segment);
    }
  }

  [[nodiscard]] virtual size_t Size() const {
    return versions_.front().tree->Size();
  }

 private:
  struct Version {
    std::shared_ptr<PersistentSegmentTree<T>> tree;
    Segment segment;
    T value;
    size_t time;
  };
  std::vector<Version> versions_{};
};