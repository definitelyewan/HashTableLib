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
    
    HashTablePair *newPair = malloc(sizeof(HashTablePair));
    
    if(newPair != NULL){
        strcpy(newPair->key, key);
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
 * @param hashTable 
 * @param key 
 * @param value 
 */
void hashTableInsert(HashTable *hashTable, const char *key, void *value){
    
    unsigned long index = hashTableHashFunction(key) % hashTable->size;
    HashTablePair *newPair = hashTablePairCreate(key, hashTable->copyData(value));

    // Insert the new pair at the beginning of the linked list
    newPair->next = hashTable->table[index];
    hashTable->table[index] = newPair;

    // Update the item count
    hashTable->itemCount++;

    // Check if resizing is needed
    if ((double)hashTable->itemCount / hashTable->size > LOAD_FACTOR_THRESHOLD) {
        hashTableResize(hashTable, hashTable->size * 2); // Resize to double the current size
    }
}

/**
 * @brief Returns a referance to the value stored at the provided keys hash 
 * 
 * @param hashTable 
 * @param key 
 * @return void* 
 */
void *hashTableRetrieve(HashTable *hashTable, const char *key){
    
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
    for(int i = 0; i < hashTable->size; i++){
        printf("[%d] -> ", i);

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
