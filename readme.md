# HashTableLib

HashTableLib is a library containing a resizable hash table data structure and makes use of the dbj2 algorithm. The library can be compiled using gcc or clang but your welcome to try other compilers like MSVC and is coded using the c99 standard.

The official repo for this library is located at https://github.com/definitelyewan/HashTableLib

## Features

HashTableLib provides an easy way to interact with and create a hash table data structure.The structure itself 
is completely user-programmable and provides functionality for performing a deep copy, deleting data, 
comparing data, and printing data to a console.

### The HashTableLib provides:

 - Adding data to a hash table
 - Data retrival
 - User programmable free, print and copy functionality
 - License: MIT

### Building
There are a couple of scripts located in the scripts folder that can be used to build this project.
##### Building the project
```bash
python3 scripts/build.py
```
##### Building documentation
```bash
python3 scripts/docs.py
```

##### Building examples
```bash
python3 scripts/docs.py
```

### Examples
```c
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
```

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashTable.h"

#define MAX 10

struct date{
    int year;
    int month;
    int day;
};

void *copyData(const void *toBeCopied){
    
    struct date *copy = (struct date *) toBeCopied;
    struct date *d = malloc(sizeof(struct date));

    d->year = copy->year;
    d->month = copy->month;
    d->day = copy->day;

    return d;
}

char *printData(const void *toBePrinted){
    
    struct date *d = (struct date *) toBePrinted;

    char *str = malloc((sizeof(char) * (strlen("[day: , month: , year: ]") + 1)) + (sizeof(int) * 3));

    sprintf(str, "[day: %d, month: %d, year: %d]",d->day, d->month, d->year);

    return str;
}

int main(){

    // create a table
    HashTable *table = hashTableCreate(MAX, free, printData, copyData);

    // Insert into the table
    struct date d = {2024, 1, 24};
    hashTableInsert(table, "jan", &d);

    d.year = 2018;
    d.month = 12;
    d.day = 25;
    hashTableInsert(table, "dec", &d);

    d.year = 2001;
    d.month = 4;
    d.day = 14;
    hashTableInsert(table, "may", &d);

    // display the table
    hashTablePrintf(table);

    // free the table
    hashTableFree(table);
}
```
Tested on mac os 14.2.1 (23C71)