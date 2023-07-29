#include <cassert>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

#ifndef _LRUCAHE_H_
#define _LRUCAHE_H_

using namespace std;

struct LRUHandle {
  int key;
  int value;
  uint32_t refs;
  LRUHandle *prev;
  LRUHandle *next;

  LRUHandle() : key(0), value(0), refs(1), prev(nullptr), next(nullptr) {}
  LRUHandle(int key, int value)
      : key(key), value(value), refs(1), prev(nullptr), next(nullptr) {}
};

class LRUCache {
public:
  LRUCache(uint32_t capacity);
  ~LRUCache();
  LRUHandle *insert(int key, int value);
  LRUHandle *look_up(int key);
  void erase(int key);
  void prune();
  void release(LRUHandle *handle);
  uint32_t get_usage() const;

private:
  void lru_remove(LRUHandle *handle);
  void lru_append(LRUHandle *list, LRUHandle *handle);
  void ref(LRUHandle *handle);
  void unref(LRUHandle *handle);
  void finish_erase(LRUHandle *handle);

private:
  uint32_t usage_;
  uint32_t capacity_;
  LRUHandle lru_;
  LRUHandle in_use_;
  unordered_map<int, LRUHandle *> cache_;
  mutable mutex mutex_;
};

LRUCache::LRUCache(uint32_t capacity) : usage_(0), capacity_(capacity) {
  lru_.prev = &lru_;
  lru_.next = &lru_;
  in_use_.prev = &in_use_;
  in_use_.next = &in_use_;
}

LRUCache::~LRUCache() {
  assert(in_use_.next == &in_use_);
  LRUHandle *handle = lru_.next;
  while (handle != &lru_) {
    LRUHandle *next = handle->next;
    assert(1 == handle->refs);
    delete handle;
    unref(handle);
    handle = next;
  }
}

LRUHandle *LRUCache::insert(int key, int value) {
  unique_lock<mutex> lock(mutex_);
  LRUHandle *newhandle = new LRUHandle(key, value);
  ++newhandle->refs;
  lru_append(&in_use_, newhandle);
  auto iter = cache_.find(key);
  if (cache_.end() != iter) {
    finish_erase(iter->second);
  }
  cache_[key] = newhandle;
  if (++usage_ > capacity_) {
    LRUHandle *victim = lru_.next;
    assert(1 == victim->refs);
    assert(cache_.erase(key) == 1);
    finish_erase(victim);
  }
  return newhandle;
}

LRUHandle *LRUCache::look_up(int key) {
  unique_lock<mutex> lock(mutex_);
  auto iter = cache_.find(key);
  if (cache_.end() != iter) {
    ref(iter->second);
  }
  return iter->second;
}

uint32_t LRUCache::get_usage() const {
  unique_lock<mutex> lock(mutex_);
  return usage_;
}

void LRUCache::release(LRUHandle *handle) {
  unique_lock<mutex> lock(mutex_);
  unref(handle);
}

void LRUCache::erase(int key) {
  unique_lock<mutex> lock(mutex_);
  auto iter = cache_.find(key);
  if (cache_.end() != iter) {
    cache_.erase(key);
    finish_erase(iter->second);
  }
}

void LRUCache::prune() {
  unique_lock<mutex> lock(mutex_);
  LRUHandle *handle = lru_.next;
  while (handle != &lru_) {
    auto iter = cache_.find(handle->key);
    assert(1 == handle->refs);
    assert(cache_.end() != iter);
    LRUHandle *next = handle->next;
    cache_.erase(handle->key);
    finish_erase(handle);
    handle = next;
  }
}

void LRUCache::lru_remove(LRUHandle *handle) {
  handle->prev = handle->next;
  handle->next = handle->prev;
}

void LRUCache::lru_append(LRUHandle *list, LRUHandle *handle) {
  list->prev->next = handle;
  handle->prev = list->prev;
  handle->next = list;
  list->prev = handle;
}

void LRUCache::ref(LRUHandle *handle) {
  if (handle->refs > 1) {
    lru_remove(handle);
    lru_append(&in_use_, handle);
  }
  ++handle->refs;
}

void LRUCache::unref(LRUHandle *handle) {
  assert(handle->refs > 0);
  if (0 == --handle->refs) {
    delete handle;
    handle = nullptr;
  } else if (1 == --handle->refs) {
    lru_remove(handle);
    lru_append(&lru_, handle);
  }
}

void LRUCache::finish_erase(LRUHandle *handle) {
  lru_remove(handle);
  --usage_;
  unref(handle);
}

#endif