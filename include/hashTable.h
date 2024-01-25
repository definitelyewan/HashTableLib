/**
 * @file hashTable.h
 * @author Ewan Jones
 * @brief function declarations for a dynamic hash table
 * @details based off of w3s implementation, found here : https://www.w3resource.com/c-programming-exercises/hash/c-hash-exercises-4.php
 * @version 0.1
 * @date 2024-01-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */

#ifndef HASH_TABLE_LIB
#define HASH_TABLE_LIB

#ifdef __cplusplus
extern "C"{
#endif

#include <stddef.h>
#include <stdint.h>

#define LOAD_FACTOR_THRESHOLD 0.7
#define HASH_TABLE_MAX_KEY_SIZE 25


/**
 * @brief A key, value pair for indexing in a hash table
 * 
 */
typedef struct _HashTablePair{

    //! Key
    char key[HASH_TABLE_MAX_KEY_SIZE];

    //! Pair
    void *value;

    //! Next node
    struct _HashTablePair *next;

}HashTablePair;

/**
 * @brief Hash table obj
 * 
 */
typedef struct _HashTable {
    
    //! Size of the table
    size_t size;
    
    //! Items in the table
    size_t itemCount;

    //! Key, value pairs
    HashTablePair **table;

    //! User provided free function
    void (*deleteData)(void *toBeDeleted);
    
    //! User provided print function
    char *(*printData)(const void *toBePrinted);
    
    //! User provided copy function
    void *(*copyData)(const void *toBeCopied);

}HashTable;

//mem

HashTable *hashTableCreate(size_t size, void (*deleteData)(void *toBeDeleted), char *(*printData)(const void *toBePrinted), void *(*copyData)(const void *toBeCopied));
HashTablePair *hashTablePairCreate(const char *key, void *value);


void hashTableInsert(HashTable *hashTable, const char *key, void *value);
void *hashTableRetrieve(HashTable *hashTable, const char *key);


void hashTableFree(HashTable *hashTable);
void hashTableResize(HashTable *hashTable, size_t newSize);


void hashTablePrintf(HashTable *hashTable);

#ifdef __cplusplus
} //extern c end
#endif

#endif
