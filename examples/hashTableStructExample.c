/**
 * @file hashTableStructExample.c
 * @author Ewan Jones
 * @brief An example of how to use the library ith a struct
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

    HashTable *table = hashTableCreate(MAX, free, printData, copyData);

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

    hashTablePrintf(table);

    hashTableFree(table);
}