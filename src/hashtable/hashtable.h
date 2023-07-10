#include <climits>
#include <iostream>

#ifndef HASHTABLE_H_
#define HASHTABLE_H_

using namespace std;

#define KEY_EXISTS 0
#define KEY_NOT_EXISTS 1
#define SUCCESS 2
#define DUPLICATED_KEY 3
#define OVERFLOW -1

typedef int Status;
typedef int KeyType;
typedef int HashType;
typedef struct {
  KeyType key;
} Record;

class HashTable {
public:
  ~HashTable();
  void initHashtable(int cap);
  Status insertHash(KeyType key);
  Status deleteHash(KeyType key);
  Status findHash(KeyType key);
  void printHash();
  int getTableCapacity();

private:
  int hashFunction(KeyType key);
  void collisionKey(HashType &hash);
  Status searchHash(KeyType key, HashType &hash, int &collision);
  void resizeHash();

private:
  Record **records;
  int capacity;
  int count;
  int *tags;
};

HashTable::~HashTable() {
  for (int i = 0; i < capacity; ++i) {
    Record *record = records[i];
    if (record != NULL) {
      free(record);
    }
  }
  free(records);
  free(tags);
}

void HashTable::initHashtable(int cap) {
  count = 0;
  capacity = cap;
  records = (Record **)calloc(capacity, sizeof(Record *));
  tags = (int *)malloc(capacity * sizeof(int));
  for (int i = 0; i < capacity; ++i) {
    records[i] = (Record *)malloc(sizeof(Record));
    records[i]->key = INT_MAX;
    tags[i] = 0;
  }
}

int HashTable::hashFunction(KeyType key) { return (3 * key) % capacity; }

void HashTable::collisionKey(HashType &hash) { hash = (hash + 1) % capacity; }

Status HashTable::searchHash(KeyType key, HashType &hash, int &collision) {
  hash = hashFunction(key);
  while ((tags[hash] == 1 && records[hash]->key != key) || tags[hash] == -1) {
    collisionKey(hash);
    collision++;
  }
  if (tags[hash] == 1 && records[hash]->key == key) {
    return KEY_EXISTS;
  } else {
    return KEY_NOT_EXISTS;
  }
}

Status HashTable::insertHash(KeyType key) {
  HashType hash;
  int collision = 0;
  if (searchHash(key, hash, collision) == KEY_NOT_EXISTS) {
    records[hash]->key = key;
    tags[hash] = 1;
    count++;
    if (collision * 1.0 / capacity > 0.2 || count * 1.0 / capacity > 0.8) {
      resizeHash();
    }
    return SUCCESS;
  }
  return OVERFLOW;
}

Status HashTable::deleteHash(KeyType key) {
  HashType hash;
  int collision = 0;
  if (searchHash(key, hash, collision) == KEY_EXISTS) {
    tags[hash] = -1;
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
  Record **oldRecords = records;
  int *oldTags = tags;
  int oldCapacity = capacity;
  int newCapacity = 2 * capacity - 1;
  initHashtable(newCapacity);
  for (int i = 0; i < oldCapacity; ++i) {
    if (oldTags[i] == 1) {
      insertHash(oldRecords[i]->key);
      free(oldRecords[i]);
    }
  }
  free(oldRecords);
  free(oldTags);
}

void HashTable::printHash() {
  cout << "Current Hash Table: ";
  for (int i = 0; i < capacity; ++i) {
    if (tags[i] == 1) {
      cout << records[i] << " ";
    }
  }
  cout << endl;
}

int HashTable::getTableCapacity() { return capacity; }

#endif