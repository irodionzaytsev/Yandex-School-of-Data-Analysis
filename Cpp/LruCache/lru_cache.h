#pragma once

#include <string>
#include <list>
#include <unordered_map>
#include <set>
#include <iostream>

class LruCache {
public:
    LruCache(size_t max_size) {
        max_size_ = max_size;
    }

    void Set(const std::string& key, const std::string& value) {
        hashtable_[key] = value;
        usage_.push_back(key);
        if (hashtable_.size() > max_size_) {
            lru_();
        }
    }

    bool Get(const std::string& key, std::string* value) {
        if (hashtable_.find(key) == hashtable_.end()) {
            return false;
        } else {
            usage_.push_back(key);
            *value = hashtable_[key];
            return true;
        }
    }

private:
    int max_size_;
    std::unordered_map<std::string, std::string> hashtable_;
    std::list<std::string> usage_;

    void lru_() {
        std::set<std::string> usage_set;
        std::string old_string;
        for (auto it = --usage_.end(); ;--it) {
            usage_set.insert(*it);
            if (usage_set.size() > max_size_) {
                old_string = *it;
                break;
            }
        }
        hashtable_.erase(old_string);
        usage_.remove(old_string);
    };
};
