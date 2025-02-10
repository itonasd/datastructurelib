#ifndef LINKEDLIST
#define LINKEDLIST

#include <stdlib.h>

/*  single linked list
    // initialization
    linkedlist_t *name = 0x0; 
    // and you are ready to go.
*/

typedef struct node {
    int *data;
    int length;
    struct node *nxt;
} linkedlist_t;

// O(size) time complexity.
void linkedlist_push(linkedlist_t **node, int size, int input[]) {
    linkedlist_t *a = (linkedlist_t *) malloc(sizeof(linkedlist_t));
    a->data = (int *) malloc(size * sizeof(int));
    a->length = size;

    for (int i = 0; i < size; i++) a->data[i] = input[i];
    a->nxt = *node;
    *node = a;
}

// O(1) time complexity.
void linkedlist_pop(linkedlist_t **node) {
    if (*node == 0x0) return;

    linkedlist_t *tmp = *node; 
    // hold the linked list during moving argument pointer to points into the last node to not cause memory leak
    *node = (*node)->nxt;
    if (tmp->data != 0x0) free(tmp->data);
    free(tmp);
}

// O(nodes) time complexity.
void linkedlist_free(linkedlist_t *node) {
    if (!node) return;

    linkedlist_t *tmp;
    while (node != 0x0) {
        tmp = node;
        node = node->nxt;
        if (tmp->data != 0x0) free(tmp->data);
        free(tmp);
    }
}

// O(1) time complexity.
void linkedlist_nodecreate(linkedlist_t **node) {
    linkedlist_t *a = (linkedlist_t *) malloc(sizeof(linkedlist_t));
    a->data = 0x0;
    a->length = 0x0;

    a->nxt = *node;
    *node = a;
}

// O(index + (end_location - start_location)) time complexity.
void linkedlist_data_bulkwrite(linkedlist_t *node, int index, int start_location, int end_location, int *input) {
    if (start_location > end_location || !node || !input) return;
    int count = 0;

    while (node != 0x0) {
        if (count == index) {
            if (start_location >= node->length || end_location >= node->length) return;
            for (int i = start_location; i <= end_location; i++) node->data[i] = input[i - start_location];
            return;
        }
        node = node->nxt;
        count++;
    }
    return;
}

// O(index) time complexity.
void linkedlist_data_write(linkedlist_t *node, int index, int location, int input) {
    if (!node || !input) return;
    int count = 0;

    while (node != 0x0) {
        if (count == index) {
            if (location >= node->length) return;
            node->data[location] = input;
            return;
        }
        node = node->nxt;
        count++;
    }
    return;
}

// O(index + size) time complexity.
void linkedlist_data_push(linkedlist_t *node, int index, int size, int *input) {
    if (!node || !input || size <= 0) return;
    int count = 0;

    while (node != 0x0) {
        if (count == index) {
            if (node->data != 0x0) free(node->data);
            node->data = (int *) malloc(size * sizeof(int));
            node->length = size;
            for (int i = 0; i < size; i++) node->data[i] = input[i];
            return;
        }
        node = node->nxt;
        count++;
    }
    return;
}

// O(index + size) time complexity (memory intensive)
void linkedlist_data_realloc(linkedlist_t *node, int index, int size) {
    if (!node || size <= 0) return;
    int count = 0;

    while (node != 0x0) {
        if (count == index) {
            int *temp = (int *) realloc(node->data, size * sizeof(int));
            node->data = temp;  
            node->length = size;  
            return;
        }
        node = node->nxt;
        count++;
    }
    return;
}

// O(index) time complexity.
void linkedlist_data_alloc(linkedlist_t *node, int index, int size) {
    if (!node || size <= 0) return;
    int count = 0;

    while (node != 0x0) {
        if (count == index) {
            if (node != 0x0) return;
            node->data = (int *) malloc(size * sizeof(int));
            node->length = size;
            return;
        }
        node = node->nxt;
        count++;
    }
    return;
}

// soon 
void linkedlist_data_sort(linkedlist_t *node, int index);

void linkedlist_data_retrieve(linkedlist_t *node, int index, int location);

void linkedlist_data_concat(linkedlist_t *node, int index, int size, int *input);

void linkedlist_concat(linkedlist_t **node, int index, int node_to_be_merged);


// O(index) time complexity.
void linkedlist_data_free(linkedlist_t *node, int index) {
    if (!node) return;
    int count = 0;

    while (node != 0x0) {
        if (count == index) {
            free(node->data);
            node->data = 0x0;
            node->length = 0x0;
            return;
        }
        node = node->nxt;
        count++;
    }
    return;
}


// O(index) time complexity.
linkedlist_t *linkedlist_get_node(linkedlist_t *node, int index) {
    if (!node) return 0x0;
    int count = 0;

    while (node != 0x0) {
        if (count == index) return node;
        node = node->nxt;
        count++;
    }
    return 0x0;
}

// O(index) time complexity.
int *linkedlist_get_data(linkedlist_t *node, int index) {
    if (!node) return 0x0;
    int count = 0;

    while (node != 0x0) {
        if (count == index) return node->data;
        node = node->nxt;
        count++;
    }
    return 0x0;
}

// O(index) time complexity.
int linkedlist_get_length(linkedlist_t *node, int index) {
    if (!node) return 0x0;
    int count = 0;

    while (node != 0x0) {
        if (count == index) return node->length;
        node = node->nxt;
        count++;
    }
    return 0x0;
}

// O(nodes) time complexity. (not recommended)
linkedlist_t *linkedlist_get_lastnode(linkedlist_t *node) {
    if (node == 0x0) return 0x0;
    linkedlist_t *current = node;

    while (current->nxt != 0x0) current = current->nxt;
    return current;
}

// O(nodes) time complexity. (not recommended)
int *linkedlist_get_lastdata(linkedlist_t *node) {
    if (node == 0x0) return 0x0;
    linkedlist_t *current = node;

    while (current->nxt != 0x0) current = current->nxt;
    return current->data;
}

// O(nodes) time complexity. (not recommended)
int linkedlist_get_lastlength(linkedlist_t *node) {
    if (node == 0x0) return 0x0;
    linkedlist_t *current = node;

    while (current->nxt != 0x0) current = current->nxt;
    return current->length;
}

////////////////////////////////////////////////////////

/*  double linked list
    // initialization
    dlinkedlist_ptr *name = dlinkedlist_init();
    // and you are ready to go.
*/

typedef struct node {
    int *data;
    struct node *nxt;
    struct node *prev;
} dlinkedlist_t;

typedef struct {
    dlinkedlist_t *first; //head
    dlinkedlist_t *last; //tail
    int lenght;
} dlinkedlist_ptr;

typedef struct {
    dlinkedlist_t *points;
} dlinkedlist_iterator_t;

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

    a->data = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) a->data[i] = input[i];
    a->nxt = list->first;
    a->prev = 0x0;

    // list->first not empty, reference to previous node's prev pointer and make it points to a
    // list->first is empty, (last node) then, connect tail into it
    if (list->first != 0x0) list->first->prev = a; else list->last = a;

    list->first = a; // update first pointer to points to newly created node
    list->lenght++;
}

// O(1) time complexity. (input array must remains dataid till program exits)
void dlinkedlist_push_o1(dlinkedlist_ptr *list, int *input) {
    if (!list) return;
    dlinkedlist_t *a = (dlinkedlist_t *) malloc(sizeof(dlinkedlist_t));

    a->data = input;
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

    free(tmp->data);
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
        free(tmp->data);
        free(tmp);
    }
}

// O(index + size) time complexity.
// 0 1 2... move from first node.
// -1 2 3... move from last node.
void dlinkedlist_bulkwrite(dlinkedlist_ptr *list, int index, int size, int* input) {
    if (!list) return;
    dlinkedlist_t *current = (index >= 0) ? list->first : list->last;
    int count = (index >= 0) ? 0 : -1 ;
    while (current != 0x0) {
        if (count == index) for (int i = 0; i < size; i++) current->data[i] = input[i];
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return;
}

// O(index) time complexity.
// 0 1 2... move from first node.
// -1 2 3... move from last node.
void dlinkedlist_write(dlinkedlist_ptr *list, int index, int location,int input) {
    if (!list) return;
    dlinkedlist_t *current = (index >= 0) ? list->first : list->last;
    int count = (index >= 0) ? 0 : -1 ;
    while (current != 0x0) {
        if (count == index) current->data[location] = input;
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return;
}

// O(index) time complexity.
// 0 1 2... move from first node.
// -1 2 3... move from last node.
int *dlinkedlist_getdata(dlinkedlist_ptr *list, int index) {
    if (!list) return 0x0;
    dlinkedlist_t *current = (index >= 0) ? list->first : list->last;
    int count = (index >= 0) ? 0 : -1 ;
    while (current != 0x0) {
        if (count == index) return current->data;
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
// when the selected node is deleted while selector still points into it, the selector becomes indataid.
dlinkedlist_iterator_t *dlinkedlist_iterator_init(dlinkedlist_ptr *list, int index) {
    if (!list) return 0x0;

    dlinkedlist_t *current = (index >= 0) ? list->first : list->last;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) {
            dlinkedlist_iterator_t *a = (dlinkedlist_iterator_t *) malloc(sizeof(dlinkedlist_iterator_t));
            a->points = current;
            return a;
        }
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return 0x0;
}

// O(size) time complexity.
// insert into head of currently selected node.
void dlinkedlist_iterator_insert(dlinkedlist_ptr *origin, dlinkedlist_iterator_t *list, int size, int *input) {
    if (!list || !list->points || !origin) return;

    dlinkedlist_t *a = (dlinkedlist_t *) malloc(sizeof(dlinkedlist_t));
    a->data = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) a->data[i] = input[i];

    a->nxt = list->points;
    a->prev = list->points->prev;
    
    if (list->points->prev == 0x0) origin->first = a; else list->points->prev->nxt = a;
    list->points->prev = a;
    origin->lenght++;

    // insert at tail is not supported. selector requires selected node to insert into. if there is not, the selector becomes indataid. free(<dlinkedlist_iterator_t>); to exit.
}

// O(size) time complexity. (input array must remains dataid till program exits)
// insert into head of currently selected node.
void dlinkedlist_iterator_insert_o1(dlinkedlist_ptr *origin, dlinkedlist_iterator_t *list, int *input) {
    if (!list || !list->points || !origin) return;

    dlinkedlist_t *a = (dlinkedlist_t *) malloc(sizeof(dlinkedlist_t));
    a->data = input;

    a->nxt = list->points;
    a->prev = list->points->prev;
    
    if (list->points->prev == 0x0) origin->first = a; else list->points->prev->nxt = a;
    list->points->prev = a;
    origin->lenght++;

    // insert at tail is not supported. selector requires selected node to insert into. if there is not, the selector becomes indataid. free(<dlinkedlist_iterator_t>); to exit.
}

// O(1) time complexity.
// delete head of currently selected node.
void dlinkedlist_iterator_delete(dlinkedlist_ptr *origin, dlinkedlist_iterator_t *list) {
    if (!list || !list->points || !origin || list->points->prev == 0x0) return;

    dlinkedlist_t *tmp = list->points->prev; // hold

    if (tmp->prev) {
        tmp->prev->nxt = list->points;
        list->points->prev = tmp->prev;

    } else { // tmp is the head node
        list->points->prev = 0x0;
        origin->first = list->points;
    }

    free(tmp->data);
    free(tmp);
    origin->lenght--;

    // why dont you delete itself and move selector into nxt of current deleted node?
    // well, to sync with insert function. anything can be done freely, nothing is automatic.
}

// O(index) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
void dlinkedlist_iterator_move(dlinkedlist_iterator_t *list, int index) {
    if (!list || !list->points) return;

    dlinkedlist_t *current = list->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) {
            list->points = current;
            return;
        }
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

// O(index + size) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
void dlinkedlist_iterator_bulkwrite(dlinkedlist_iterator_t *list, int index, int size, int *input) {
    if (!list || !list->points) return;
    
    dlinkedlist_t *current = list->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) for (int i = 0; i < size; i++) current->data[i] = input[i];

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return;
}

// O(index) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
void dlinkedlist_iterator_write(dlinkedlist_iterator_t *list, int index, int location, int input) {
    if (!list || !list->points) return;
    
    dlinkedlist_t *current = list->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) current->data[location] = input;

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return;
}

// O(index) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
int *dlinkedlist_iterator_getdata(dlinkedlist_iterator_t *list, int index) {
    if (!list || !list->points) return 0x0;
    
    dlinkedlist_t *current = list->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) return current->data;
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return 0x0;
}

// O(index) time complexity.
// 0 1 2... move forward.
// -1 2 3... move backward.
dlinkedlist_t *dlinkedlist_iterator_getnode(dlinkedlist_iterator_t *list, int index) {
    if (!list || !list->points) return 0x0;

    dlinkedlist_t *current = list->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) return current;
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
    return 0x0;
}

#endif
