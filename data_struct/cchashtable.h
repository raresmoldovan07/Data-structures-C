#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct _NODE {
    char *Key;
    int Value;
    struct _NODE *Next;
}NODE;

typedef struct _CC_HASH_TABLE {
    int Size;
    int KeyCount;
    NODE **Table;
} CC_HASH_TABLE;

  
int HtCreate(CC_HASH_TABLE** HashTable); 
int HtDestroy(CC_HASH_TABLE** HashTable); 
 
int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value); 
int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int *Value); 
int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key); 
int HtHasKey(CC_HASH_TABLE* HashTable, char* Key); 
int HtGetNthKey(CC_HASH_TABLE* HashTable, int Index, char** Key); 
int HtClear(CC_HASH_TABLE* HashTable); 
int HtGetKeyCount(CC_HASH_TABLE* HashTable);  
