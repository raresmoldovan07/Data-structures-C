#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct _CC_STACK{
    int *Data;
    int Top;
    int Capacity;
} CC_STACK; 
  
int StCreate(CC_STACK **Stack);
int StDestroy(CC_STACK **Stack);
int StPush(CC_STACK *Stack, int Value);
int StPop(CC_STACK *Stack, int *Value);
int StPeek(CC_STACK *Stack, int *Value); // gets top of stack without popping the value
int StIsEmpty(CC_STACK *Stack); // returns 1 if stack is empty and 0 otherwise
int StGetCount(CC_STACK *Stack); // returns the number of elements in the stack
int StClear(CC_STACK *Stack); // removes all elements from the stack
int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush);
