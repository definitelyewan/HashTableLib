/**
 * @file hashTable.c
 * @author Ewan Jones
 * @brief Function definitions for the hashmap
 * @version 0.1
 * @date 2024-01-24
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"

/**
 * @brief An implementation of the djb2 algorithm
 * 
 * @param str 
 * @return unsigned long 
 */
unsigned long hashTableHashFunction(const char *str){
    
    unsigned long hash = 5381;
    int c;

    while ((c = *str++) != '\0') {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }

    return hash;
}

/**
 * @brief Creates a new key value pair
 * 
 * @param key 
 * @param value 
 * @return HashTablePair* 
 */
HashTablePair *hashTablePairCreate(const char *key, void *value){

    if(key == NULL){
        return NULL;
    }

    size_t len = strlen(key);

    if(len > HASH_TABLE_MAX_KEY_SIZE){
        return NULL;
    }

    HashTablePair *newPair = malloc(sizeof(HashTablePair));

    if(newPair != NULL){
        memcpy(newPair->key, key, len + 1);
        newPair->value = value;
        newPair->next = NULL;
    }

    return newPair;
}

/**
 * @brief Creates a hash table with size initial size and the provided functions
 * 
 * @param size 
 * @return HashTable* 
 */
HashTable *hashTableCreate(size_t size, void (*deleteData)(void *toBeDeleted), char *(*printData)(const void *toBePrinted), void *(*copyData)(const void *toBeCopied)){
    
    HashTable *newTable = malloc(sizeof(HashTable));
    
    if (newTable != NULL){

        newTable->size = size;
        newTable->itemCount = 0;
        newTable->table = (HashTablePair **)calloc(size, sizeof(HashTablePair *));

        newTable->deleteData = deleteData;
        newTable->printData = printData;
        newTable->copyData = copyData;
    }

    return newTable;
}

/**
 * @brief Inserts a value and key into the hash table
 * 
 * @details Keys longer than HASH_TABLE_MAX_KEY_SIZE characters are rejected, not truncated.
 * @param hashTable
 * @param key
 * @param value
 * @return int - 0 on success, 1 if the table or key is NULL, the key is too long or allocation fails
 */
int hashTableInsert(HashTable *hashTable, const char *key, void *value){

    if(hashTable == NULL || key == NULL || hashTable->size == 0){
        return 1;
    }

    // check before copying the value so a rejected key leaks nothing
    if(strlen(key) > HASH_TABLE_MAX_KEY_SIZE){
        return 1;
    }

    unsigned long index = hashTableHashFunction(key) % hashTable->size;
    void *valueCopy = hashTable->copyData(value);
    HashTablePair *newPair = hashTablePairCreate(key, valueCopy);

    if(newPair == NULL){
        hashTable->deleteData(valueCopy);
        return 1;
    }

    // Insert the new pair at the beginning of the linked list
    newPair->next = hashTable->table[index];
    hashTable->table[index] = newPair;

    // Update the item count
    hashTable->itemCount++;

    // Check if resizing is needed
    if ((double)hashTable->itemCount / hashTable->size > LOAD_FACTOR_THRESHOLD) {
        hashTableResize(hashTable, hashTable->size * 2); // Resize to double the current size
    }

    return 0;
}

/**
 * @brief Returns a referance to the value stored at the provided keys hash
 *
 * @param hashTable
 * @param key
 * @return void* - the value, or NULL if not found or the key is longer than HASH_TABLE_MAX_KEY_SIZE
 */
void *hashTableRetrieve(HashTable *hashTable, const char *key){

    if(hashTable == NULL || key == NULL || hashTable->size == 0){
        return NULL;
    }

    if(strlen(key) > HASH_TABLE_MAX_KEY_SIZE){
        return NULL;
    }

    unsigned long index = hashTableHashFunction(key) % hashTable->size;
    HashTablePair *current = hashTable->table[index];

    // Traverse the linked list at the index
    while(current != NULL){
        
        if(strcmp(current->key, key) == 0){
            return current->value; // Key found, return the value
        }
        
        current = current->next;
    }
    
    return NULL; // Key not found
}

/**
 * @brief Prints a hash table to the console
 * 
 * @param hashTable 
 */
void hashTablePrintf(HashTable *hashTable){
    
    if(hashTable == NULL){
        return;
    }
    
    for(size_t i = 0; i < hashTable->size; i++){
        printf("[%ld] -> ", i);

        HashTablePair *current = hashTable->table[i];

        while(current != NULL){

            printf("(%s, ",current->key);
            
            char *str = hashTable->printData(current->value);
            printf("%s) -> ",str);
            free(str);

            current = current->next;
        }

        printf("NULL\n");
    }
}

/**
 * @brief Frees the hash table and all of its elements
 * 
 * @param hashTable 
 */
void hashTableFree(HashTable *hashTable) {
    
    if(hashTable == NULL){
        return;
    }

    for(size_t i = 0; i < hashTable->size; i++){
        
        HashTablePair *current = hashTable->table[i];
        
        while(current != NULL){

            HashTablePair *temp = current;
            current = current->next;

            hashTable->deleteData(temp->value);
            free(temp);
        }
    }

    free(hashTable->table);
    free(hashTable);
}

/**
 * @brief Resizes the hashtable
 * @details makes a new table :(
 * @param hashTable 
 * @param newSize 
 */
void hashTableResize(HashTable *hashTable, size_t newSize) {
    
    if(hashTable == NULL){
        return;
    }

    HashTable *newTable = hashTableCreate(newSize, hashTable->deleteData, hashTable->printData, hashTable->copyData);

    for(size_t i = 0; i < hashTable->size; i++){

        HashTablePair *current = hashTable->table[i];
        
        while(current != NULL){

            hashTableInsert(newTable, current->key, current->value);
            HashTablePair *toFree = current; 
            current = current->next;

            hashTable->deleteData(toFree->value);
            free(toFree);
        }
    }

    // Update the hash table with the new size and table
    hashTable->size = newSize;
    free(hashTable->table);
    hashTable->table = newTable->table;

    // Free the memory allocated for the new table structure
    free(newTable);
}
