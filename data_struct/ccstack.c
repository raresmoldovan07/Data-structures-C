#include "ccstack.h"

int StCreate(CC_STACK **Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    *Stack = (CC_STACK *)malloc(sizeof(CC_STACK));
    if (*Stack == NULL)
    {
        //malloc failed.
        return -1;
    }
    (*Stack)->Capacity = 1;
    (*Stack)->Top = -1;
    (*Stack)->Data = (int *)malloc(sizeof(int));
    return 0;
}

int StDestroy(CC_STACK **Stack)
{
    if (*Stack == NULL)
    {
        return -1;
    }
    free((*Stack)->Data);
    free(*Stack);
    *Stack = NULL;
    return 0;
}

int StPush(CC_STACK *Stack, int Value)
{
    if (Stack == NULL)
    {
        return -1;
    }
    if (Stack->Top == Stack->Capacity - 1)
    {
        Stack->Capacity = Stack->Capacity + 1;
        int* newData = (int *)malloc(sizeof(int) * (Stack->Capacity + 1));
        if (newData == NULL)
        {
            return -1;
        }
        int i;
        for (i = 0; i <= Stack->Top; ++i)
        {
            newData[i] = Stack->Data[i];
        }
        free(Stack->Data);
        Stack->Data = newData;
    }
    Stack->Data[++Stack->Top] = Value;
    return 0;
}

int StPop(CC_STACK *Stack, int *Value)
{
    if (Stack == NULL || Value == NULL)
    {
        return -1;
    }
    if (Stack->Top == -1)
    {
        //stiva este goala
        return -1;
    }
    *Value = Stack->Data[Stack->Top--];
    return 0;
}

int StPeek(CC_STACK *Stack, int *Value)
{
    if (Stack == NULL || Value == NULL)
    {
        return -1;
    }
    if (Stack->Top == -1)
    {
        //stiva este goala
        return -1;
    }
    *Value = Stack->Data[Stack->Top];
    return 0;
}

int StIsEmpty(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    return Stack->Top == -1;
}

int StGetCount(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    return Stack->Top + 1;
}

int StClear(CC_STACK *Stack)
{
    if (Stack == NULL)
    {
        return -1;
    }
    free(Stack->Data);
    Stack->Data = (int *)malloc(sizeof(int));
    Stack->Capacity = 1;
    Stack->Top = -1;
    return 0;
}

int StPushStack(CC_STACK *Stack, CC_STACK *StackToPush)
{
    if (Stack == NULL || StackToPush == NULL)
    {
        return -1;
    }
    int *newData = malloc(sizeof(int) * (Stack->Top + StackToPush->Top + 2));
    if (newData == NULL)
    {
        return -1;
    }
    int i;
    for (i = 0; i <= Stack->Top; ++i)
    {
        newData[i] = Stack->Data[i];
    }
    for (i = 0; i <= StackToPush->Top; ++i)
    {
        newData[Stack->Top + i + 1] = StackToPush->Data[i];
    }

    //setam noul varf si noua capacitate inainte de a elibera stiva copiata
    Stack->Capacity = Stack->Top + StackToPush->Top + 2;
    Stack->Top = Stack->Top + StackToPush->Top + 1;

    //eliberam stiva copiataa
    StClear(StackToPush);
    free(Stack->Data);

    Stack->Data = newData;

    return 0;
}
