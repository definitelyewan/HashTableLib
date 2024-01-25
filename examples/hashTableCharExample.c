/**
 * @file hashTableCharExample.c
 * @author Ewan Jones
 * @brief An example on how to use the library with a string
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

void *copyData(const void *toBeCopied){
    
    size_t len = strlen((char *)toBeCopied);
    char *copy = malloc(sizeof(char) * (len + 1));
    strcpy(copy, (char *)toBeCopied);

    return (void *)copy;
}

char *printData(const void *toBeCopied){
    
    size_t len = strlen((char *)toBeCopied);
    char *copy = malloc(sizeof(char) * (len + 1));
    strcpy(copy, (char *)toBeCopied);

    return copy;
}

int main() {
    // Example usage
    HashTable *hashTable = hashTableCreate(10, free, printData, copyData);

    // Insert key-value pairs
    hashTableInsert(hashTable, "Red", "#ff0000");
    hashTableInsert(hashTable, "Green", "#008000");
    hashTableInsert(hashTable, "Blue", "#0000FF");
    hashTableInsert(hashTable, "Yellow", "#FFFF00");
    hashTableInsert(hashTable, "Orange", "#FFA500");
    hashTableInsert(hashTable, "Orange2", "#ad");
    hashTableInsert(hashTable, "smell", "#adasd");
    hashTableInsert(hashTable, "tests", "#FFA500");
    
    printf("%s\n",(char *)hashTableRetrieve(hashTable, "Red"));

    hashTablePrintf(hashTable);

    // Free allocated memory
    hashTableFree(hashTable);

    return 0;
}