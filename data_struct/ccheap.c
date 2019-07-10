#include "ccheap.h"

inline void Swap(int *A, int *B)
{
    int aux;
    aux = *A;
    *A = *B;
    *B = aux;
}

static void UpInHeap(CC_HEAP *Heap, int Position)
{
    //Pune nodul de pe pozitia Position pe pozitia corespunzatoare
    if (Heap == NULL)
    {
        return;
    }
    if (Heap->Type == 0)
    {
        //MIN-HEAP
        while (Position > 0 && Heap->Data[Position] < Heap->Data[(Position - 1) >> 1])
        {
            //Parintele nodului are indicele ( Position - 1 ) / 2
            Swap(&Heap->Data[Position], &Heap->Data[(Position - 1) >> 1]);
            Position = (Position - 1) >> 1;
        }
    }
    else
    {
        //MAX-HEAP
        while (Position > 0 && Heap->Data[Position] > Heap->Data[(Position - 1) >> 1])
        {
            //Parintele nodului are indicele ( Position - 1 ) / 2
            Swap(&Heap->Data[Position], &Heap->Data[(Position - 1) >> 1]);
            Position = (Position - 1) >> 1;
        }
    }
}

static void DownInHeap(CC_HEAP *Heap, int Position)
{
    if (Heap == NULL)
    {
        return;
    }
    int son = 0;
    if (Heap->Type == 0)
    {
        //MIN-HEAP
        while (son != -1)
        {
            son = -1;
            if ((Position << 1) + 1 < Heap->Size)
            {
                //Alegem initial fiul stang fara sa stim daca este mai mic decat nodul parinte
                son = (Position << 1) + 1;
                if (Heap->Data[(Position << 1) + 2] < Heap->Data[son] && (Position << 1) + 2 < Heap->Size)
                {
                    //Fiul drept este mai mic decat cel stang
                    son = (Position << 1) + 2;
                }
                if (Heap->Data[son] > Heap->Data[Position])
                {
                    //Nodul parinte este mai mic deja ca ambii fii
                    son = -1;
                }
            }
            if (son != -1)
            {
                Swap(&Heap->Data[son], &Heap->Data[Position]);
                Position = son;
            }
        }
    }
    else
    {
        //MAX-HEAP
        while (son != -1)
        {
            son = -1;
            if ((Position << 1) + 1 < Heap->Size)
            {
                //Alegem initial fiul stang fara sa stim daca este mai mic decat nodul parinte
                son = (Position << 1) + 1;
                if (Heap->Data[(Position << 1) + 2] > Heap->Data[son] && (Position << 1) + 2 < Heap->Size)
                {
                    //Fiul drept este mai mare decat cel stang
                    son = (Position << 1) + 2;
                }
                if (Heap->Data[son] < Heap->Data[Position])
                {
                    //Nodul parinte este mai mare deja ca ambii fii
                    son = -1;
                }
            }
            if (son != -1)
            {
                Swap(&Heap->Data[son], &Heap->Data[Position]);
                Position = son;
            }
        }
    }
}

int HpCreateMaxHeap(CC_HEAP **MaxHeap, CC_VECTOR* InitialElements)
{
    if (MaxHeap == NULL)
    {
        return -1;
    }
    *MaxHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
    if (*MaxHeap == NULL)
    {
        //malloc failed
        return -1;
    }
    (*MaxHeap)->Type = 1;
    (*MaxHeap)->Data = (int *)malloc(sizeof(int));
    (*MaxHeap)->Size = 0;
    if (InitialElements != NULL)
    {
        int i;
        for (i = 0; i < InitialElements->Size; ++i)
        {
            HpInsert(*MaxHeap, InitialElements->Data[i]);
        }
    }
    return 0;
}

int HpCreateMinHeap(CC_HEAP **MinHeap, CC_VECTOR* InitialElements)
{
    if (MinHeap == NULL)
    {
        return -1;
    }
    *MinHeap = (CC_HEAP*)malloc(sizeof(CC_HEAP));
    if (*MinHeap == NULL)
    {
        //malloc failed
        return -1;
    }
    (*MinHeap)->Type = 0;
    (*MinHeap)->Data = (int *)malloc(sizeof(int));
    (*MinHeap)->Size = 0;
    if (InitialElements != NULL)
    {
        int i;
        for (i = 0; i < InitialElements->Size; ++i)
        {
            HpInsert(*MinHeap, InitialElements->Data[i]);
        }
    }
    return 0;
}

int HpDestroy(CC_HEAP **Heap)
{
    if (*Heap == NULL)
    {
        return -1;
    }
    free((*Heap)->Data);
    free(*Heap);
    *Heap = NULL;
    return 0;
}

int HpInsert(CC_HEAP *Heap, int Value)
{
    if (Heap == NULL)
    {
        return -1;
    }
    if (Heap->Data == NULL)
    {
        return -1;
    }
    int *newData = (int*)malloc((Heap->Size + 2) * sizeof(int));
    int i;
    for (i = 0; i < Heap->Size; ++i)
    {
        newData[i] = Heap->Data[i];
    }
    free(Heap->Data);
    Heap->Data = newData;
    Heap->Data[Heap->Size++] = Value;
    UpInHeap(Heap, Heap->Size - 1);
    return 0;
}

int HpRemove(CC_HEAP *Heap, int Value)
{
    if (Heap == NULL)
    {
        return -1;
    }
    int valuesLeft = 1; //presupunem ca au mai ramas elemente de sters
    while (valuesLeft)
    {
        valuesLeft = 0;
        int i;
        for (i = 0; i < Heap->Size; ++i)
        {
            if (Heap->Data[i] == Value)
            {
                valuesLeft = 1;
                Swap(&Heap->Data[i], &Heap->Data[--Heap->Size]);//interschimbam cu ultimul nod
                //apoi il coboram sau urcam in heap
                UpInHeap(Heap, i);
                DownInHeap(Heap, i);
                break;//nu stergem mai multe elemente cat timp iteram - comportament nedefinit
            }
        }
    }
    return 0;
}

int HpGetExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    if (Heap == NULL || ExtremeValue == NULL || Heap->Size == 0)
    {
        return -1;
    }
    *ExtremeValue = Heap->Data[0];
    return 0;
}

int HpPopExtreme(CC_HEAP *Heap, int* ExtremeValue)
{
    if (Heap == NULL || ExtremeValue == NULL || Heap->Size == 0)
    {
        return -1;
    }
    *ExtremeValue = Heap->Data[0];
    Swap(&Heap->Data[0], &Heap->Data[--Heap->Size]);//schimbam nodul din radacina cu ultima frunza
    DownInHeap(Heap, 0); //apoi coboram radacina pana in pozitia corespunzatoare
    return 0;
}

int HpGetElementCount(CC_HEAP *Heap)
{
    if (Heap == NULL)
    {
        return -1;
    }
    return Heap->Size;
}

int HpSortToVector(CC_HEAP *Heap, CC_VECTOR* SortedVector)
{
    if (Heap == NULL || SortedVector == NULL)
    {
        return -1;
    }
    if (Heap->Size == 0)
    {
        return 0;
    }
    if (Heap->Type == 0)
    {
        //MIN-HEAP
        while (HpGetElementCount(Heap) != 0)
        {
            int popValue, retVal;
            retVal = HpPopExtreme(Heap, &popValue);
            if (retVal == -1)
            {
                return -1;
            }
            VecInsertTail(SortedVector, popValue);
        }
    }
    else
    {
        //MAX-HEAP
        while (HpGetElementCount(Heap) != 0)
        {
            int popValue, retVal;
            retVal = HpPopExtreme(Heap, &popValue);
            if (retVal == -1)
            {
                return -1;
            }
            VecInsertHead(SortedVector, popValue);
        }      
    }
    int i;
    for (i = 0; i < SortedVector->Size; ++i)
    {
        HpInsert(Heap, SortedVector->Data[i]);
    }
    return 0;

}
