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
    void collisionKey(HashType& hash);
    Status searchHash(KeyType key, HashType& hash, int& collision);
    void resizeHash();

private:
    Record** records;
    int capacity;
    int count;
    int* tags;

};

#endif