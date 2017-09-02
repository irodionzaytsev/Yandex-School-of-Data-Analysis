#pragma once

#include <vector>
#include <string>
#include <utility>
#include <algorithm>
#include <iterator>

class StaticMap {
 public:
    explicit StaticMap(const std::vector<
                    std::pair<std::string,
                    std::string>>& items);

    bool Find(const std::string& key, std::string* value) const;

 private:
    std::vector<std::pair<std::string, std::string>> items_;
};

StaticMap::StaticMap(const std::vector<
                std::pair<std::string,
                std::string>> &items) {
    items_ = items;
    std::sort(items_.begin(), items_.end());
}

bool StaticMap::Find(const std::string &key, std::string *value) const {
    auto it = std::lower_bound(items_.begin(),
                               items_.end(),
                               std::make_pair(key, std::string("")));
    if (it != items_.end() && (*it).first == key) {
        (*value) = (*it).second;
        return true;
    }
    return false;
}
