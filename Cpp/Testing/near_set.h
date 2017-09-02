#pragma once

#include <vector>
#include <cstdint>
#include <set>

class NearSet {
 public:
    void Add(int64_t point) {
        set_.insert(point);
    }

    void Remove(int64_t point) {\
        set_.erase(point);
    }

    // find all points x in set, such that abs(x - point) <= distance
    // returned vector must be sorted
    std::vector<int64_t> FindNear(int64_t point, int64_t distance) {
        std::set<int64_t>::iterator it = set_.lower_bound(point - distance);
        std::vector<int64_t> ans;
        while(it!=set_.end() && std::abs(*it - point) <= distance) {
            ans.push_back(*it);
            ++it;
        }
        return ans;
    }

 private:
    std::set<int64_t> set_;
};
