#include "cctree.h"

static void UpdateHeights(TREE_NODE *Node)
{
    if (Node == NULL)
    {
        return;
    }
    UpdateHeights(Node->Left);
    UpdateHeights(Node->Right);
    if (Node->Left == NULL && Node->Right == NULL)
    {
        Node->Height = 1;
    }
    else if (Node->Left == NULL)
    {
        Node->Height = Node->Right->Height + 1;
    }
    else if (Node->Right == NULL)
    {
        Node->Height = Node->Left->Height + 1;
    }
    else
    {
        if (Node->Left->Height > Node->Right->Height)
        {
            Node->Height = Node->Left->Height + 1;
        }
        else
        {
            Node->Height = Node->Right->Height + 1;
        }
    }

}

static TREE_NODE *RotateLeft(TREE_NODE *Node)
{
    TREE_NODE *leftNode = Node->Left;
    Node->Left = leftNode->Right;
    leftNode->Right = Node;
    UpdateHeights(Node);
    return leftNode;
}

static TREE_NODE *RotateRight(TREE_NODE *Node)
{
    TREE_NODE *rightNode = Node->Right;
    Node->Right = rightNode->Left;
    rightNode->Left = Node;
    UpdateHeights(Node);
    return rightNode;
}

static TREE_NODE* BalanceTree(TREE_NODE *Node)
{
    if (Node == NULL)
    {
        return NULL;
    }
    int leftHeight = 0, rightHeight = 0;

    UpdateHeights(Node);

    if (Node->Right == NULL && Node->Left == NULL)
    {
        return Node;
    }
    else if (Node->Right == NULL)
    {
        //nu avem fiu in dreapta
        leftHeight = Node->Left->Height;
    }
    else if (Node->Left == NULL)
    {
        //nu avem fiu in stanga

        rightHeight = Node->Right->Height;
    }
    else
    {
        leftHeight = Node->Left->Height;
        rightHeight = Node->Right->Height;
    }
    if (leftHeight > rightHeight + 1)
    {
        //subarborele stang este mai lung
        if (Node->Left->Right && Node->Left->Left)
            if (Node->Left->Right->Height > Node->Left->Left->Height)
            {
                Node->Left = RotateRight(Node->Left);
            }
        Node = RotateLeft(Node);
    }
    else if (rightHeight > leftHeight + 1)
    {
        if (Node->Right->Left != NULL && Node->Right->Right)
            if (Node->Right->Left->Height > Node->Right->Right->Height)
            {
                Node->Right = RotateLeft(Node->Right);
            }
        Node = RotateRight(Node);
    }

    return Node;
}

static TREE_NODE* FindMin(TREE_NODE *Node)
{
    //minimul se afla in nodul cel mai din stanga
    while (Node->Left != NULL)
    {
        Node = Node->Left;
    }
    return Node;
}

static TREE_NODE* InsertNode(TREE_NODE* Node, int Value)
{
    // If the tree is empty, return a new, single node  
    if (Node == NULL)
    {
        TREE_NODE *newNode = (TREE_NODE*)malloc(sizeof(TREE_NODE));
        if (newNode == NULL)
        {
            //malloc failed
            return NULL;
        }
        newNode->Left = NULL;
        newNode->Right = NULL;
        newNode->Key = Value;
        newNode->Height = 1;
        return newNode;
    }
    else
    {
        if (Value <= Node->Key)
        {
            Node->Left = InsertNode(Node->Left, Value);
        }
        else
        {
            Node->Right = InsertNode(Node->Right, Value);
        }
        return BalanceTree(Node);

    }
}

static TREE_NODE* IterativeSearch(TREE_NODE* Node, int Value)
{
    while (Node != NULL && Node->Key != Value)
    {
        if (Node->Key > Value)
        {
            Node = Node->Left;
        }
        else
        {
            Node = Node->Right;
        }
    }
    return Node;
}

static TREE_NODE* DeleteNode(TREE_NODE* Node, int Value)
{
    TREE_NODE *auxNode;
    if (Node == NULL)
    {
        return NULL;
    }
    else if (Value < Node->Key)
    {
        Node->Left = DeleteNode(Node->Left, Value);
    }
    else if (Value > Node->Key)
    {
        Node->Right = DeleteNode(Node->Right, Value);
    }
    else
    {
        if (Node->Right && Node->Left)
        {
            auxNode = FindMin(Node->Right);
            Node->Key = auxNode->Key;
            Node->Right = DeleteNode(Node->Right, auxNode->Key);
        }
        else
        {
            auxNode = Node;
            if (Node->Left == NULL)
            {
                Node = Node->Right;
            }
            else if (Node->Right == NULL)
            {
                Node = Node->Left;
            }
            free(auxNode);
        }
    }
    return BalanceTree(Node);
}

static void RecursiveClear(TREE_NODE *Node)
{
    if (Node == NULL)
    {
        return;
    }
    RecursiveClear(Node->Left);
    RecursiveClear(Node->Right);
    free(Node);
    Node = NULL;
}

static void PreOrder(TREE_NODE *Node, int Index, int *Value, int *IndexInOrder)
{
    if (Node == NULL)
    {
        return;
    }
    *IndexInOrder = *IndexInOrder + 1;
    if (*IndexInOrder == Index)
    {
        *Value = Node->Key;
        return;
    }

    PreOrder(Node->Left, Index, Value, IndexInOrder);
    PreOrder(Node->Right, Index, Value, IndexInOrder);
    return;

}

static void InOrder(TREE_NODE *Node, int Index, int *Value, int *IndexInOrder)
{
    if (Node == NULL)
    {
        return;
    }
    InOrder(Node->Left, Index, Value, IndexInOrder);
    *IndexInOrder = *IndexInOrder + 1;
    if (*IndexInOrder == Index)
    {
        *Value = Node->Key;
        return;
    }
    InOrder(Node->Right, Index, Value, IndexInOrder);
    return;

}

static void PostOrder(TREE_NODE *Node, int Index, int *Value, int *IndexInOrder)
{
    if (Node == NULL)
    {
        return;
    }
    PostOrder(Node->Left, Index, Value, IndexInOrder);
    PostOrder(Node->Right, Index, Value, IndexInOrder);
    *IndexInOrder = *IndexInOrder + 1;
    if (*IndexInOrder == Index)
    {
        *Value = Node->Key;
        return;
    }
    return;
}

int TreeCreate(CC_TREE **Tree)
{
    if (Tree == NULL)
    {
        return -1;
    }
    *Tree = (CC_TREE*)malloc(sizeof(CC_TREE));
    if ((*Tree) == NULL)
    {
        //malloc failed
        return -1;
    }
    (*Tree)->Root = NULL;
    (*Tree)->Size = 0;
    return 0;
}

int TreeDestroy(CC_TREE **Tree)
{
    if (*Tree == NULL)
    {
        return -1;
    }
    TreeClear(*Tree);
    free((*Tree)->Root);
    free(*Tree);
    *Tree = NULL;
    return 0;
}

int TreeInsert(CC_TREE *Tree, int Value)
{
    if (Tree == NULL)
    {
        return -1;
    }
    Tree->Root = InsertNode(Tree->Root, Value);
    if (Tree->Root == NULL)
    {
        //insert failed
        return -1;
    }
    Tree->Size++;
    return 0;
}

int TreeRemove(CC_TREE *Tree, int Value)
{
    if (Tree == NULL || Tree->Size == 0)
    {
        return -1;
    }

    while (TreeContains(Tree, Value))
    {
        Tree->Root = DeleteNode(Tree->Root, Value);
    }
    Tree->Size--;
    return 0;
}

int TreeContains(CC_TREE *Tree, int Value)
{
    if (Tree == NULL)
    {
        return -1;
    }
    if (IterativeSearch(Tree->Root, Value))
    {
        return 1;
    }
    return 0;
}

int TreeGetCount(CC_TREE *Tree)
{
    if (Tree == NULL)
    {
        return -1;
    }
    return Tree->Size;
}

int TreeGetHeight(CC_TREE *Tree)
{
    if (Tree == NULL || Tree->Root == NULL)
    {
        return -1;
    };
    UpdateHeights(Tree->Root);
    return Tree->Root->Height - 1;
}

int TreeClear(CC_TREE *Tree)
{
    if (Tree == NULL)
    {
        return -1;
    }
    if (Tree->Root == NULL)
    {
        return -1;
    }
    RecursiveClear(Tree->Root);
    Tree->Size = 0;
    Tree->Root = NULL;
    return 0;
}

int TreeGetNthPreorder(CC_TREE *Tree, int Index, int *Value)
{
    if (Tree == NULL || Value == NULL)
    {
        return -1;
    }
    if (Index > Tree->Size - 1)
    {
        return -1;
    }
    int IndexInOrder = -1;
    PreOrder(Tree->Root, Index, Value, &IndexInOrder);
    return 0;
}

int TreeGetNthInorder(CC_TREE *Tree, int Index, int *Value)
{
    if (Tree == NULL || Value == NULL)
    {
        return -1;
    }
    if (Index > Tree->Size - 1)
    {
        return -1;
    }
    int IndexInOrder = -1;
    InOrder(Tree->Root, Index, Value, &IndexInOrder);
    return 0;
}

int TreeGetNthPostorder(CC_TREE *Tree, int Index, int *Value)
{
    if (Tree == NULL || Value == NULL)
    {
        return -1;
    }
    if (Index > Tree->Size - 1)
    {
        return -1;
    }
    int IndexInOrder = -1;
    PostOrder(Tree->Root, Index, Value, &IndexInOrder);
    return 0;
}

