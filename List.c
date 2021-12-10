#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "List.h"
#include <assert.h>

typedef struct NodeObj{
    void *data;
    struct NodeObj* next;
} NodeObj;

typedef struct NodeObj* NodePtr;

typedef struct ListObj{
    NodePtr head;
    int (*dataCompare)(void *obj1, void*obj2);
    void (*dataPrinter)(void *data);
    void (*freeData)(void *data);
    int length;
} ListObj;

typedef struct ListObj* ListPtr;



ListPtr list_create(int (*dataCompare)(void *obj1, void *obj2), void (*dataPrinter)(void *data), void (*freeData)(void *data)) {
    ListPtr l = (ListPtr)malloc(sizeof(ListObj));
    l->dataCompare = dataCompare;
    l->dataPrinter = dataPrinter;
    l->freeData = freeData;
    l->length = 0;
    l->head = NULL;
    return l;
}

void list_destroy(ListPtr pL, bool freeData) {
    if (pL != NULL) {
        NodePtr n = pL->head;
        while (n != NULL) {
            NodePtr next = n->next;
            if (n->data != NULL && freeData) {
                pL->freeData(n->data);
            }
            free(n);
            n = next;
        }
        free(pL);
    }
    
}

int list_length(ListPtr L) {
    if (L != NULL) {
        return L->length;
    }
    return -1;
}

int list_find_element(ListPtr L, void *comp) {
    if (L == NULL) {
        return -1;
    }
    NodePtr n = L->head;
    int i = 0;
    while (n != NULL) {
        if (L->dataCompare(n->data, comp) == 0) {
            return i;
        }
        n = n->next;
        i++;
    }
    return -1;
}

void *list_get_index( ListPtr L, int i) {
    if (L == NULL || i < 0) {
        return NULL;
    }
    NodePtr n = L->head;
    while (n != NULL && i > 0) {
        n = n->next;
        i--;
    }
    if (n == NULL) {
        return NULL;
    } else {
        return n->data;
    }
}

void *list_del_index(ListPtr L, int i) {
    if (L == NULL) return NULL;
    NodePtr n = L->head;
    NodePtr prev = NULL;
    if (i < 0 || n == NULL || i >= L->length) {
	    return NULL;
    }
    for (;i > 0;i--) {
	prev = n;
        n = n->next;
    }
    if (prev == NULL) {
        L->head =n->next;
    } else {
        prev->next = n->next;
    }
    void *d = n->data;
    L->length--;
    free(n);
    return d;
}

bool list_append(ListPtr L, void *data) {
    if (L == NULL) return false;
    return list_insert(L, L->length, data);
}

bool list_insert(ListPtr L, int pos, void *data) {
    if ( L == NULL || L->length < pos || pos < 0) {
        return false;
    }
    NodePtr n = L->head;
    NodePtr newNode = (NodePtr)malloc(sizeof(NodeObj));
    newNode->next = NULL;
    newNode->data = data;
    if (pos == 0) {
        newNode->next = L->head;
        L->head = newNode;
    } else {
        for (;pos > 1; pos--) {
            n = n->next;
        }
	    newNode->next = n->next;
        n->next = newNode;
    }
    L->length++;
    return true;
}

void list_print(ListPtr L) {
    assert (L != NULL);
    NodePtr n = L->head;
    while (n != NULL) {
        (*L->dataPrinter)(n->data);
        n = n->next;
    }
}

void list_split(NodePtr source, NodePtr *front, NodePtr *back) {
    //Splits list into 2 equal parts
    NodePtr fast;
    NodePtr slow;
    slow = source;
    fast = source->next;
 
    while (fast != NULL) {
        fast = fast->next;
        if (fast != NULL) {
            slow = slow->next;
            fast = fast->next;
        }
    }
 
    *front = source;
    *back = slow->next;
    slow->next = NULL;
}

NodePtr list_sorted_merge(NodePtr a, NodePtr b, int (*dataCompare)(void *obj1, void *obj2), bool ascending) {
    NodePtr result = NULL;
    
    //Base cases if a or b is null
    if (a == NULL) {
        return (b);
    } else if (b == NULL) {
        return (a);
    }
    int multiplier = ascending ? -1 : 1;
    //Recursively merge a or b and return pointer to first one
    if (multiplier * dataCompare(a->data, b->data) >= 0) {
        result = a;
        result->next = list_sorted_merge(a->next, b, dataCompare, ascending);
    } else {
        result = b;
        result->next = list_sorted_merge(a, b->next, dataCompare, ascending);
    }
    return(result);
}

void list_merge_sort(NodePtr *headRef, int (*dataCompare)(void *obj1, void *obj2), bool ascending) {
    // Merge sorts list
    NodePtr head = *headRef;
    NodePtr a;
    NodePtr b;
 
    //Base case
    if ((head == NULL) || (head->next == NULL)) {
        return;
    } else {
        //Split list in 2
        list_split(head, &a, &b);
    
        //Merge base cases
        list_merge_sort(&a, dataCompare, ascending);
        list_merge_sort(&b, dataCompare, ascending);
        // Set head ref to the correct position
        *headRef = list_sorted_merge(a, b, dataCompare, ascending);
    }
}

void list_sort(ListPtr L, bool ascending) {
    assert(L != NULL);
    list_merge_sort(&(L->head), L->dataCompare, ascending);
}

bool list_swap(ListPtr L, int pos1, int pos2) {
    if (L == NULL || pos1 == pos2 || pos1 < 0 || pos2 < 0 || pos1 >= L->length || pos2 >= L->length) return false;
    int min = (pos1 < pos2) ? pos1 : pos2;
    int max = (pos1 < pos2) ? pos2 : pos1;
    NodePtr n = L->head;
    NodePtr p = NULL;
    NodePtr nodeToSwap = NULL;
    NodePtr prevNodeToSwap = NULL;
    for (int i = 0; i < max; i++) {
        if (i == min) {
            prevNodeToSwap = p;
            nodeToSwap = n;
        }
        p = n;
        n = n->next;
    }

    if (prevNodeToSwap == NULL) {
        L->head = n;
    } else {
        prevNodeToSwap->next = n;
    }

    if (nodeToSwap->next == n) {
        nodeToSwap->next = n->next;
	    n->next = nodeToSwap;
    } else {
        p->next = nodeToSwap;
        NodePtr tmpNext = nodeToSwap->next;
        nodeToSwap->next = n->next;
        n->next = tmpNext;
    }

    return true;
}
