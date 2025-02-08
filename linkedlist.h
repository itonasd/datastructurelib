#ifndef LINKEDLIST
#define LINKEDLIST

#include <stdlib.h>

/*  single linked list
    // initialization
    linkedlist_t *name = 0x0; 
    // and you are ready to go.
*/

typedef struct node {
    int *val;
    struct node *nxt;
} linkedlist_t;

// O(size) time complexity.
void linkedlist_push(linkedlist_t **node, int size, int input[]) {
    linkedlist_t *a = (linkedlist_t *) malloc(sizeof(linkedlist_t));
    a->val = (int *) malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) a->val[i] = input[i];
    a->nxt = *node;
    *node = a;
}

// O(1) time complexity. (input array must remains valid till program exits)
void linkedlist_push_o1(linkedlist_t **node, int *input) {
    linkedlist_t *a = (linkedlist_t *)malloc(sizeof(linkedlist_t));
    a->val = input;
    a->nxt = *node;
    *node = a;
}

// O(1) time complexity.
void linkedlist_pop(linkedlist_t **node) {
    if (*node == 0x0) return;

    linkedlist_t *tmp = *node; 
    // hold the linked list during moving argument pointer to points into the last node to not cause memory leak
    *node = (*node)->nxt;
    free(tmp->val);
    free(tmp);
}

// uncompatiable to free, getnode, getval functions. 
// compatiable to pop, push functions.
// O(nodes) time complexity.
void linkedlist_connect(linkedlist_t *node) {
    if (node == 0x0) return;
    
    linkedlist_t *current = node;
    while (current->nxt != 0x0) {
        current = current->nxt;
    }
    
    current->nxt = node; // write last node value with the currently main pointing node (first)
}

// O(nodes) time complexity.
void linkedlist_free(linkedlist_t *node) {
    linkedlist_t *tmp;
    while (node != 0x0) {
        tmp = node;
        node = node->nxt;
        free(tmp->val);
        free(tmp);
    }
}

// O(index) time complexity.
linkedlist_t *linkedlist_getnode(linkedlist_t *node, int index) {
    int count = 0;

    while (node != 0x0) {
        if (count == index) return node;
        node = node->nxt;
        count++;
    }
    return 0x0;
}

// O(index) time complexity.
int *linkedlist_getval(linkedlist_t *node, int index) {
    int count = 0;

    while (node != 0x0) {
        if (count == index) return node->val;
        node = node->nxt;
        count++;
    }
    return 0x0;
}

// O(nodes) time complexity. (not recommended)
linkedlist_t *linkedlist_getlastnode(linkedlist_t *node) {
    if (node == 0x0) return 0x0;
    linkedlist_t *current = node;

    while (current->nxt != 0x0) current = current->nxt;
    return current;
}

// O(nodes) time complexity. (not recommended)
int *linkedlist_getlastval(linkedlist_t *node) {
    if (node == 0x0) return 0x0;
    linkedlist_t *current = node;

    while (current->nxt != 0x0) current = current->nxt;
    return current->val;
}

////////////////////////////////////////////////////////

/*  double linked list
    // initialization
    dlinkedlist_ptr *name = dlinkedlists_init();
    // and you are ready to go.
*/

typedef struct node {
    int *val;
    struct node *nxt;
    struct node *prev;
} dlinkedlist_t;

typedef struct {
    dlinkedlist_t *first; //head
    dlinkedlist_t *last; //tail
    int lenght;
} dlinkedlist_ptr;

// O(1) time complexity.
dlinkedlist_ptr *dlinkedlist_init() {
    dlinkedlist_ptr *a = (dlinkedlist_ptr *) malloc(sizeof(dlinkedlist_ptr));
    a->first = NULL;
    a->last = NULL;
    a->lenght = 0;

    return a;
}

// O(size) time complexity.
void dlinkedlist_push(dlinkedlist_ptr *list, int size ,int *input) {
    dlinkedlist_t *a = (dlinkedlist_t *) malloc(sizeof(dlinkedlist_t));

    a->val = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) a->val[i] = input[i];
    a->nxt = list->first;
    a->prev = 0x0;

    // list->first not empty, reference to previous node's prev pointer and make it points to a
    // list->first is empty, (last node) then, connect last into it
    if (list->first != 0x0) list->first->prev = a; else list->last = a;

    list->first = a; // update first pointer to points to newly created node
    list->lenght++;
}

// O(1) time complexity.
void dlinkedlist_pop(dlinkedlist_ptr *list) {
    if (list->first == 0x0) return;

    dlinkedlist_t *tmp = list->first; // temporary hold the node
    list->first = list->first->nxt; // update head pointer to point into the next node

    // list->first not empty, reference to previous node's prev pointer and set it to NULL
    // list->first is empty, (last node) set tail to NULL
    if (list->first != 0x0) list->first->prev == 0x0; else list->last = 0x0;

    free(tmp->val);
    free(tmp);
    list->lenght--;
}

// O(index) time complexity.
int *dlinkedlist_getvalfromfirstnode(dlinkedlist_ptr *list, int index) {
    dlinkedlist_t *current = list->first;
    int count = 0;
    while (current != 0x0) {
        if (count == index) return current->val;
        current = current->nxt;
        count++;
    }
    return 0x0;
}

// O(index) time complexity.
int *dlinkedlist_getvalfromlastnode(dlinkedlist_ptr *list, int index) {
    dlinkedlist_t *current = list->last;
    int count = 0;
    while (current != 0x0) {
        if (count == index) return current->val;
        current = current->prev;
        count++;
    }
    return 0x0;
}

// O(index) time complexity.
dlinkedlist_t *dlinkedlist_getnodefromfirstnode(dlinkedlist_ptr *list, int index) {
    dlinkedlist_t *current = list->first;
    int count = 0;
    while (current != 0x0) {
        if (count == index) return current;
        current = current->nxt;
        count++;
    }
    return 0x0;
}

// O(index) time complexity.
dlinkedlist_t *dlinkedlist_getnodefromlastnode(dlinkedlist_ptr *list, int index) {
    dlinkedlist_t *current = list->last;
    int count = 0;
    while (current != 0x0) {
        if (count == index) return current;
        current = current->prev;
        count++;
    }
    return 0x0;
}

#endif