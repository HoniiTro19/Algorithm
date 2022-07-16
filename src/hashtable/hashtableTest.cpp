#include "gtest/gtest.h"
#include "hashtable.h"

TEST(HashTable, insertHashtable) {
    int capacity = 1000;
    HashTable hashtable;
    hashtable.initHashtable(capacity);
    for (int i = 1; i <= 100; ++i) {
        hashtable.insertHash(i);
    }
    for (int i = 1; i <= 100; ++i) {
        ASSERT_EQ(KEY_EXISTS, hashtable.findHash(i));
    }
    for (int i = 101; i <= 1000; ++i) {
        ASSERT_EQ(KEY_NOT_EXISTS, hashtable.findHash(i));
    }
}

TEST(HashTable, deleteHashtable) {
    int capacity = 1000;
    HashTable hashtable;
    hashtable.initHashtable(capacity);
    for (int i = 1; i <= 100; ++i) {
        hashtable.insertHash(i);
    }
    for (int i = 1; i <= 50; ++i) {
        hashtable.deleteHash(i);
        ASSERT_EQ(KEY_NOT_EXISTS, hashtable.findHash(i));
    }
    for (int i = 51; i <= 100; ++i) {
        ASSERT_EQ(KEY_EXISTS, hashtable.findHash(i));
    }
}

TEST(HashTable, resizeHashtable) {
    int capacity = 1000;
    HashTable hashtable;
    hashtable.initHashtable(capacity);
    for (int i = 1; i <= 2000; ++i) {
        hashtable.insertHash(i);
    }
    ASSERT_GT(hashtable.getTableCapacity(), capacity);
    for (int i = 1; i <= 2000; ++i) {
        ASSERT_EQ(KEY_EXISTS, hashtable.findHash(i));
    }
    for (int i = 2001; i <= 3000; ++i) {
        ASSERT_EQ(KEY_NOT_EXISTS, hashtable.findHash(i));
    }
}


TEST(HashTable, highCollision) {
    int capacity = 100;
    int seed = 6;
    HashTable hashtable;
    hashtable.initHashtable(capacity);
    for (int i = 1, val = seed; i <= 500; ++i) {
        hashtable.insertHash(val);
        val += capacity;
    }
    ASSERT_GT(hashtable.getTableCapacity(), capacity);
    for (int i = 1, val = seed; i <= 500; ++i) {
        ASSERT_EQ(KEY_EXISTS, hashtable.findHash(val));
        val += capacity;
    }
}