#include <algorithm>
#include <cassert>
#include <climits>
#include <gtest/gtest.h>
#include <iostream>
#include <mutex>
#include <numeric>
#include <queue>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>

struct CacheHandle {
    int key{0};
    int value{0};
    CacheHandle *next{nullptr};
    CacheHandle *prev{nullptr};
};

class LRUCache {
private:
    int count_;
    int capacity_;
    std::unordered_map<int, CacheHandle*> map_;
    CacheHandle head_;
    CacheHandle tail_;
private:
    void lru_remove(CacheHandle *handle) {
        int key = handle->key;
        CacheHandle *prev = handle->prev;
        CacheHandle *next = handle->next;
        prev->next = next;
        next->prev = prev;
        map_.erase(key);
        count_ -= 1;
    }

    void lru_append_head(CacheHandle *handle) {
        int key = handle->key;;
        CacheHandle *next = head_.next;
        head_.next = handle;
        handle->prev = &head_;
        handle->next = next;
        next->prev = handle;
        map_[key] = handle;
        count_ += 1;
    }

public:
    LRUCache(int capacity) {
        count_ = 0;
        capacity_ = capacity;
        head_.next = &tail_;
        tail_.prev = &head_;
    }

    ~LRUCache() {
        CacheHandle *handle = head_.next;
        while (handle != &tail_) {
            CacheHandle *next = handle->next;
            delete handle;
            handle = next;
        }
    }

    void put(int key, int value) {
        if (!map_.contains(key)) {
            CacheHandle *new_handle = new CacheHandle();
            new_handle->key = key;
            new_handle->value = value;
            lru_append_head(new_handle);
        } else {
            CacheHandle *handle = map_[key];
            handle->value = value;
            lru_remove(handle);
            lru_append_head(handle);
        }
        if (count_ > capacity_) {
            CacheHandle *eviction = tail_.prev;
            lru_remove(eviction);
            delete eviction;
        }
    }

    int get(int key) {
        if (map_.contains(key)) {
            CacheHandle *handle = map_[key];
            lru_remove(handle);
            lru_append_head(handle);
            return handle->value;
        }
        return -1;
    }
};