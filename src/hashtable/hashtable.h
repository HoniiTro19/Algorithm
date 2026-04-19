#include <iostream>
#include <vector>
#include <limits>

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

using Status = int;
using KeyType = int;
using HashType = int;

static constexpr Status KEY_EXISTS = 0;
static constexpr Status KEY_NOT_EXISTS = 1;
static constexpr Status SUCCESS = 2;
// Avoid name OVERFLOW: macOS <math.h> defines OVERFLOW as a macro.
static constexpr Status KEY_DUPLICATE = -1;

class HashTable {
public:
  void initHashtable(int cap);
  Status insertHash(KeyType key);
  Status deleteHash(KeyType key);
  Status findHash(KeyType key);
  void printHash();
  int getTableCapacity();

private:
  enum class SlotState { Empty = 0, Occupied = 1, Deleted = -1 };

  struct Slot {
    KeyType key{std::numeric_limits<KeyType>::max()};
    SlotState state{SlotState::Empty};
  };

  int hashFunction(KeyType key) const { return (3 * key) % capacity; }
  void collisionKey(HashType &hash) const { hash = (hash + 1) % capacity; }
  Status searchHash(KeyType key, HashType &hash, int &collision);
  void resizeHash();

private:
  std::vector<Slot> table;
  int capacity{0};
  int count{0};
};

void HashTable::initHashtable(int cap) {
  count = 0;
  capacity = cap;
  table.assign(capacity,
               Slot{std::numeric_limits<KeyType>::max(), SlotState::Empty});
}

Status HashTable::searchHash(KeyType key, HashType &hash, int &collision) {
  hash = hashFunction(key);
  collision = 0;
  // Keep probing while:
  // - slot is occupied but key differs
  // - slot is marked as deleted (to match your original behavior)
  while ((table[hash].state == SlotState::Occupied && table[hash].key != key) ||
         table[hash].state == SlotState::Deleted) {
    collisionKey(hash);
    collision++;
  }
  if (table[hash].state == SlotState::Occupied && table[hash].key == key) {
    return KEY_EXISTS;
  } else {
    return KEY_NOT_EXISTS;
  }
}

Status HashTable::insertHash(KeyType key) {
  HashType hash;
  int collision = 0;
  if (searchHash(key, hash, collision) == KEY_NOT_EXISTS) {
    table[hash].key = key;
    table[hash].state = SlotState::Occupied;
    count++;
    if (collision * 1.0 / capacity > 0.2 || count * 1.0 / capacity > 0.8) {
      resizeHash();
    }
    return SUCCESS;
  }
  return KEY_DUPLICATE;
}

Status HashTable::deleteHash(KeyType key) {
  HashType hash;
  int collision = 0;
  if (searchHash(key, hash, collision) == KEY_EXISTS) {
    table[hash].state = SlotState::Deleted;
    count--;
    return SUCCESS;
  }
  return KEY_NOT_EXISTS;
}

Status HashTable::findHash(KeyType key) {
  HashType hash;
  int collision = 0;
  return searchHash(key, hash, collision);
}

void HashTable::resizeHash() {
  int newCapacity = 2 * capacity - 1;
  std::vector<Slot> oldTable = std::move(table);
  int oldCapacity = capacity;

  initHashtable(newCapacity);
  for (int i = 0; i < oldCapacity; ++i) {
    if (oldTable[i].state == SlotState::Occupied) {
      insertHash(oldTable[i].key);
    }
  }
}

void HashTable::printHash() {
  std::cout << "Current Hash Table: ";
  for (int i = 0; i < capacity; ++i) {
    if (table[i].state == SlotState::Occupied) {
      std::cout << table[i].key << " ";
    }
  }
  std::cout << std::endl;
}

int HashTable::getTableCapacity() { return capacity; }

#endif