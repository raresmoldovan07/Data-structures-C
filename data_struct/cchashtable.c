#include "cchashtable.h"
#define HASH_SIZE 666013

static unsigned long Hash(char *Str)
{
    unsigned long hash = 5381;
    int c;
    while (c = *Str++)
    {
        hash = ((hash << 5) + hash) ^ c;
    }
    return hash % HASH_SIZE;
}

static int CompareStrings(const char *S1, const char *S2)
{
    const unsigned char *s1 = (const unsigned char *)S1;
    const unsigned char *s2 = (const unsigned char *)S2;
    unsigned char c1, c2;
    do
    {
        c1 = (unsigned char)*s1++;
        c2 = (unsigned char)*s2++;
        if (c1 == '\0')
            return c1 - c2;
    } while (c1 == c2);
    return c1 - c2;
}

static char *CopyString(char *Destination, const char* Source)
{
    char *Save = Destination;
    while (*Destination++ = *Source++);
    return Save;
}

static void DeleteTableList(NODE *node)
{
    if (node == NULL)
    {
        return;
    }
    if (node->Next != NULL)
    {
        DeleteTableList(node->Next);
        node->Next = NULL;
    }
    free(node->Key);
    free(node);
    node = NULL;
    return;
}

static NODE *NewPair(char *Key, int Value)
{
    NODE *newPair;

    newPair = (NODE*)malloc(sizeof(NODE));
    if (newPair == NULL)
    {
        return NULL;
    }
    newPair->Value = Value;
    int keyLength;
    for (keyLength = 0; Key[keyLength] != '\0'; ++keyLength);
    newPair->Key = (char*)malloc(sizeof(char) * (keyLength + 1));
    if (newPair->Key == NULL)
    {
        return NULL;
    }
    newPair->Key = CopyString(newPair->Key, Key);
    newPair->Next = NULL;

    return newPair;
}

int HtCreate(CC_HASH_TABLE** HashTable)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    *HashTable = (CC_HASH_TABLE*)malloc(sizeof(CC_HASH_TABLE));
    if (*HashTable == NULL)
    {
        return -1;
    }
    (*HashTable)->Table = (NODE**)malloc(sizeof(NODE*) * HASH_SIZE);
    if ((*HashTable)->Table == NULL)
    {
        return -1;
    }
    (*HashTable)->Size = HASH_SIZE;
    (*HashTable)->KeyCount = 0;
    int i;
    for (i = 0; i < (*HashTable)->Size; ++i)
    {
        (*HashTable)->Table[i] = NULL;
    }
    return 0;
}

int HtDestroy(CC_HASH_TABLE** HashTable)
{
    if ((*HashTable) == NULL)
    {
        return -1;
    }
    int i;
    for (i = 0; i < (*HashTable)->Size; ++i)
    {
        free((*HashTable)->Table[i]);
        (*HashTable)->Table[i] = NULL;
    }
    free((*HashTable));
    *HashTable = NULL;
    return 0;

}

int HtSetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int Value)
{
    if (HashTable == NULL || Key == NULL)
    {
        return -1;
    }

    NODE *actualNode = NULL;
    NODE *lastNode = NULL;
    NODE *newNode = NULL;

    int hashIndex = Hash(Key);

    actualNode = HashTable->Table[hashIndex];

    while (actualNode != NULL && actualNode->Key != NULL && CompareStrings(Key, actualNode->Key) > 0)
    {
        lastNode = actualNode;
        actualNode = actualNode->Next;
    }

    if (actualNode != NULL && actualNode->Key != NULL && CompareStrings(Key, actualNode->Key) == 0)
    {
        actualNode->Value = Value;
    }
    else
    {
        newNode = NewPair(Key, Value);
        if (newNode == NULL)
        {
            return -1;
        }
        HashTable->KeyCount++;
        if (actualNode == HashTable->Table[hashIndex])
        {
            newNode->Next = actualNode;
            HashTable->Table[hashIndex] = newNode;
        }
        else
        {
            newNode->Next = actualNode;
            lastNode->Next = newNode;
        }
    }
    return 0;
}

int HtGetKeyValue(CC_HASH_TABLE* HashTable, char* Key, int *Value)
{
    if (HashTable == NULL || Key == NULL || Value == NULL)
    {
        return -1;
    }

    NODE *actualNode = NULL;
    NODE *lastNode = NULL;

    int hashIndex = Hash(Key);

    actualNode = HashTable->Table[hashIndex];

    while (actualNode != NULL && actualNode->Key != NULL && CompareStrings(Key, actualNode->Key) > 0)
    {
        lastNode = actualNode;
        actualNode = actualNode->Next;
    }
    if (actualNode != NULL && actualNode->Key != NULL && CompareStrings(Key, actualNode->Key) == 0)
    {
        *Value = actualNode->Value;
        return 0;
    }
    return 0;
}

int HtRemoveKey(CC_HASH_TABLE* HashTable, char* Key)
{
    if (HashTable == NULL || Key == NULL)
    {
        return -1;
    }

    NODE *actualNode = NULL;
    NODE *lastNode = NULL;

    int hashIndex = Hash(Key);

    actualNode = HashTable->Table[hashIndex];

    while (actualNode != NULL && actualNode->Key != NULL && CompareStrings(Key, actualNode->Key) > 0)
    {
        lastNode = actualNode;
        actualNode = actualNode->Next;
    }
    if (actualNode != NULL && actualNode->Key != NULL && CompareStrings(Key, actualNode->Key) == 0)
    {
        if (lastNode == NULL)
        {
            HashTable->Table[hashIndex] = actualNode->Next;
            free(actualNode);
        }
        else
        {
            lastNode->Next = actualNode->Next;
            free(actualNode);
        }
        HashTable->KeyCount--;
        return 0;
    }
    return -1;
}

int HtHasKey(CC_HASH_TABLE* HashTable, char* Key)
{
    if (HashTable == NULL || Key == NULL)
    {
        return -1;
    }

    NODE *actualNode = NULL;
    NODE *lastNode = NULL;

    int hashIndex = Hash(Key);

    actualNode = HashTable->Table[hashIndex];

    while (actualNode != NULL && actualNode->Key != NULL && CompareStrings(Key, actualNode->Key) > 0)
    {
        lastNode = actualNode;
        actualNode = actualNode->Next;
    }
    if (actualNode != NULL && actualNode->Key != NULL && CompareStrings(Key, actualNode->Key) == 0)
    {
        return 1;
    }
    return 0;
}

int HtGetNthKey(CC_HASH_TABLE* HashTable, int Index, char** Key)
{
    if (HashTable == NULL || (*Key) == NULL)
    {
        return -1;
    }
    if (Index >= HashTable->KeyCount || Index < 0)
    {
        return -1;
    }
    int myIndex = -1, i;
    for (i = 0; i < HashTable->Size; ++i)
    {
        NODE *node = HashTable->Table[i];
        while (node != NULL && myIndex < Index)
        {
            myIndex++;
            if (myIndex == Index)
            {
                *Key = node->Key;
                return 0;
            }
            node = node->Next;
        }
    }
    return -1;
}

int HtClear(CC_HASH_TABLE* HashTable)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    int i;
    for (i = 0; i < HashTable->Size; ++i)
    {
        NODE *node = HashTable->Table[i];
        if (node != NULL)
        {
            DeleteTableList(node);
        }
        HashTable->Table[i] = NULL;
    }
    HashTable->KeyCount = 0;
    return 0;
}

int HtGetKeyCount(CC_HASH_TABLE* HashTable)
{
    if (HashTable == NULL)
    {
        return -1;
    }
    return HashTable->KeyCount;
}
