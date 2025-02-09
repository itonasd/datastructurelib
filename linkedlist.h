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
    dlinkedlist_ptr *name = dlinkedlist_init();
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

typedef struct {
    dlinkedlist_t *pointing;
} dlinkedlist_selector_t;

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
    if (!list) return;
    dlinkedlist_t *a = (dlinkedlist_t *) malloc(sizeof(dlinkedlist_t));

    a->val = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) a->val[i] = input[i];
    a->nxt = list->first;
    a->prev = 0x0;

    // list->first not empty, reference to previous node's prev pointer and make it points to a
    // list->first is empty, (last node) then, connect tail into it
    if (list->first != 0x0) list->first->prev = a; else list->last = a;

    list->first = a; // update first pointer to points to newly created node
    list->lenght++;
}

// O(1) time complexity. (input array must remains valid till program exits)
void dlinkedlist_push_o1(dlinkedlist_ptr *list, int *input) {
    if (!list) return;
    dlinkedlist_t *a = (dlinkedlist_t *) malloc(sizeof(dlinkedlist_t));

    a->val = input;
    a->nxt = list->first;
    a->prev = 0x0;

    if (list->first != 0x0) list->first->prev = a; else list->last = a;

    list->first = a;
    list->lenght++;
}

// O(1) time complexity.
void dlinkedlist_pop(dlinkedlist_ptr *list) {
    if (list->first == 0x0 || !list) return;

    dlinkedlist_t *tmp = list->first; // temporary hold the node
    list->first = list->first->nxt; // update head pointer to point into the next node

    // list->first not empty, reference to previous node's prev pointer and set it to NULL
    // list->first is empty, (last node) set tail to NULL
    if (list->first != 0x0) list->first->prev = 0x0; else list->last = 0x0;

    free(tmp->val);
    free(tmp);
    list->lenght--;
}

// O(nodes) time complexity.
void dlinkedlist_free(dlinkedlist_ptr *list) {
    if (!list) return;
    dlinkedlist_t *current = list->first;
    dlinkedlist_t *tmp;

    while (current != 0x0) {
        tmp = current;
        current = current->nxt;
        free(tmp->val);
        free(tmp);
    }
}

// O(index) time complexity.
// 0 1 2... move from first node.
// -1 2 3... move from last node.
int *dlinkedlist_getval(dlinkedlist_ptr *list, int index) {
    if (!list) return 0x0;
    dlinkedlist_t *current = (index >= 0) ? list->first : list->last;
    int count = (index >= 0) ? 0 : -1 ;
    while (current != 0x0) {
        if (count == index) return current->val;
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return 0x0;
}

// O(index) time complexity.
// 0 1 2... move from first node.
// -1 2 3... move from last node.
dlinkedlist_t *dlinkedlist_getnode(dlinkedlist_ptr *list, int index) {
    if (!list) return 0x0;
    dlinkedlist_t *current = (index >= 0) ? list->first : list->last;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) return current;
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return 0x0;
}

// O(index) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
// there must be atleast one node for selector to points to. else the function returns null.
// when the selected node is deleted while selector still pointing into it, the selector becomes invalid.
dlinkedlist_selector_t *dlinkedlist_selector_init(dlinkedlist_ptr *list, int index) {
    if (!list) return 0x0;

    dlinkedlist_t *current = (index >= 0) ? list->first : list->last;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) {
            dlinkedlist_selector_t *a = (dlinkedlist_selector_t *) malloc(sizeof(dlinkedlist_selector_t));
            a->pointing = current;
            return a;
        }
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return 0x0;
}

// O(size) time complexity.
// insert into head of currently selected node.
void dlinkedlist_selector_insert(dlinkedlist_ptr *origin, dlinkedlist_selector_t *list, int size, int *input) {
    if (!list || !list->pointing || !origin) return;

    dlinkedlist_t *a = (dlinkedlist_t *) malloc(sizeof(dlinkedlist_t));
    a->val = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) a->val[i] = input[i];

    a->nxt = list->pointing;
    a->prev = list->pointing->prev;
    
    if (list->pointing->prev == 0x0) origin->first = a; else list->pointing->prev->nxt = a;
    list->pointing->prev = a;
    origin->lenght++;

    // insert at tail is not supported. selector requires selected node to insert into. if there is not, the selector becomes invalid. free(<dlinkedlist_selector_t>); to exit.
}

// O(1) time complexity.
// delete head of currently selected node.
void dlinkedlist_selector_delete(dlinkedlist_ptr *origin, dlinkedlist_selector_t *list) {
    if (!list || !list->pointing || !origin || list->pointing->prev == 0x0) return;

    dlinkedlist_t *tmp = list->pointing->prev; // hold

    if (tmp->prev) {
        tmp->prev->nxt = list->pointing;
        list->pointing->prev = tmp->prev;

    } else { // tmp is the head node
        list->pointing->prev = 0x0;
        origin->first = list->pointing;
    }

    free(tmp->val);
    free(tmp);
    origin->lenght--;

    // why dont you delete itself and move selector into nxt of current deleted node?
    // well, to sync with insert function. anything can be done freely, nothing is automatic.
}

// O(index) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
void dlinkedlist_selector_mov(dlinkedlist_selector_t *list, int index) {
    if (!list || !list->pointing) return;

    dlinkedlist_t *current = list->pointing;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) {
            list->pointing = current;
            return;
        }
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

// O(index) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
int *dlinkedlist_selector_getval(dlinkedlist_selector_t *list, int index) {
    if (!list || !list->pointing) return 0x0;
    
    dlinkedlist_t *current = list->pointing;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) return current->val;
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return 0x0;
}

// O(index) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
dlinkedlist_t *dlinkedlist_selector_getnode(dlinkedlist_selector_t *list, int index) {
    if (!list || !list->pointing) return 0x0;

    dlinkedlist_t *current = list->pointing;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) return current;
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return 0x0;
}

#endif
