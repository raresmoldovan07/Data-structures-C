#include "ccvector.h"

void mergeSort(int *AuxVector, int *Vector, int Left, int Right);
static int VecResizeToHalf(CC_VECTOR *Vector);
static int VecResize(CC_VECTOR *Vector);

int VecCreate(CC_VECTOR **Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    *Vector = (CC_VECTOR*)malloc(sizeof(CC_VECTOR));
    if (*Vector == NULL)
    {
        return -1;
    }
    (*Vector)->Capacity = 1;
    (*Vector)->Size = 0;
    (*Vector)->Data = (int *)malloc((*Vector)->Capacity * sizeof(int));
    if ((*Vector)->Data == NULL)
    {
        return -1;
    }
    return 0;
}

int VecDestroy(CC_VECTOR **Vector)
{
    if (*Vector == NULL)
    {
        return -1;
    }
    free((*Vector)->Data);
    free(*Vector);
    *Vector = NULL;
    return 0;
}

int VecInsertTail(CC_VECTOR *Vector, int Value)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Vector->Size >= Vector->Capacity)
    {
        int resizeResult = VecResize(Vector);
        if (resizeResult == -1)
        {
            //resize failed
            return -1;
        }
    }
    Vector->Data[Vector->Size++] = Value;
    return 0;
}

int VecInsertHead(CC_VECTOR *Vector, int Value)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Vector->Size >= Vector->Capacity)
    {
        int resizeResult = VecResize(Vector);
        if (resizeResult == -1)
        {
            //resize failed
            return -1;
        }
    }
    if (Vector->Size == 0)
    {
        Vector->Data[0] = Value;
        Vector->Size++;
        return 0;
    }
    Vector->Size++;
    int i;
    for (i = Vector->Size; i >= 1; --i)
    {
        Vector->Data[i] = Vector->Data[i - 1];
    }
    Vector->Data[0] = Value;
    return 0;
}

int VecInsertAfterIndex(CC_VECTOR *Vector, int Index, int Value)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Index < 0 || Index >= Vector->Size)
    {
        return -1;
    }
    if (Vector->Size >= Vector->Capacity)
    {
        int resizeResult = VecResize(Vector);
        if (resizeResult == -1)
        {
            return -1;
        }
    }
    Vector->Size++;
    int i;
    for (i = Vector->Size; i >= Index + 1; --i)
    {
        Vector->Data[i] = Vector->Data[i - 1];
    }
    Vector->Data[Index + 1] = Value;
    return 0;
}

int VecRemoveByIndex(CC_VECTOR *Vector, int Index)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Index < 0 || Index >= Vector->Size)
    {
        return -1;
    }
    int i;
    for (i = Index; i < Vector->Size; ++i)
    {
        Vector->Data[i] = Vector->Data[i + 1];
    }
    Vector->Size--;
    if (Vector->Size < Vector->Capacity / 2)
        VecResizeToHalf(Vector);
    return 0;
}

int VecGetValueByIndex(CC_VECTOR *Vector, int Index, int *Value)
{
    if (Vector == NULL || Value == NULL)
    {
        return -1;
    }
    if (Index < 0 || Index >= Vector->Size)
    {
        return -1;
    }
    *Value = Vector->Data[Index];
    return 0;
}

int VecGetCount(CC_VECTOR *Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    return Vector->Size;
}

int VecClear(CC_VECTOR *Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    if (Vector->Data != NULL)
    {
        free(Vector->Data);
        Vector->Data = (int *)malloc(sizeof(int));
        Vector->Capacity = 1;
        Vector->Size = 0;
    }
    return 0;
}

int VecSort(CC_VECTOR *Vector)
{
    if (Vector == NULL )
    {
        return -1;
    }
    if (Vector->Size == 0)
    {
        return 0;
    }
    int *auxVector = (int *)malloc(sizeof(int) * (Vector->Size));
    if (auxVector == NULL)
    {
        return -1;
    }
    mergeSort(auxVector, Vector->Data, 0, Vector->Size - 1);
    return 0;
}

static int VecResize(CC_VECTOR *Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    int *newVector = (int *)malloc(sizeof(int) * (1 + 2 * Vector->Capacity));
    if (newVector == NULL)
    {
        return -1;
    }
    int i;
    for (i = 0; i < Vector->Size; ++i)
        newVector[i] = Vector->Data[i];
    if (Vector->Data != NULL)
    {
        free(Vector->Data);
    }
    Vector->Data = newVector;
    Vector->Capacity *= 2;

    return 0;
}

static int VecResizeToHalf(CC_VECTOR *Vector)
{
    if (Vector == NULL)
    {
        return -1;
    }
    int *newVector = (int *)malloc(sizeof(int) * (1 + Vector->Capacity / 2));
    if (newVector == NULL)
    {
        return -1;
    }
    int i;
    for (i = 0; i < Vector->Size; ++i)
        newVector[i] = Vector->Data[i];
    if (Vector->Data != NULL)
    {
        free(Vector->Data);
    }
    Vector->Data = newVector;
    Vector->Capacity /= 2;

    return 0;
}

void mergeSort(int *AuxVector, int *Vector, int Left, int Right)
{
    int middle = (Left + Right) >> 1, i, j, k;
    if (Left == Right)
        return;

    mergeSort(AuxVector, Vector, Left, middle);
    mergeSort(AuxVector, Vector, middle + 1, Right);

    for (i = Left, j = middle + 1, k = Left; i <= middle || j <= Right; )
    {
        if (j > Right || (Vector[i] < Vector[j] && i <= middle))
        {
            AuxVector[k++] = Vector[i++];
        }
        else
        {
            AuxVector[k++] = Vector[j++];
        }
    }

    for (k = Left; k <= Right; ++k)
        Vector[k] = AuxVector[k];
}