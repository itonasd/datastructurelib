#ifndef list
#define list

#include <stdlib.h>

typedef struct node {
    int *data;
    struct node *nxt;
    struct node *prev;
} list_t;

typedef struct {
    list_t *tail; // list[-1]: last node
    list_t *head; // list[0]: first node
    int length;
} list_ptr;

typedef struct {
    list_t *points;
} iterator_ptr;

/*
 * Description:
 *    - Initialize double linked list
 *    - list_ptr *<variable_name> = list_init();
 *
 * Return: pointer to the list_ptr
 * Time Complexity: O(1)
 */
list_ptr *list_init() {
    list_ptr *a = (list_ptr *) malloc(sizeof(list_ptr));
    a->tail = 0x0;
    a->head = 0x0;
    a->length = 0;

    return a;
}

#define FRONT 0x2000
#define BACK 0x1000


/*
 * Adds a new node to the list with the given data.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - location => FRONT: push to the head | BACK: push to the tail
 *    - size => input size
 *    - input => pointer to the input data
 *
 * Time Complexity: O(data_size)
 */
void list_push(list_ptr *li, int location, int size ,int *input) {
    if (!li || size <= 0 || !input || (location != FRONT && location != BACK)) return;
    list_t *a = (list_t *) malloc(sizeof(list_t));

    a->data = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) a->data[i] = input[i];


    if (location == FRONT) {
        a->nxt = 0x0;
        a->prev = li->head;

        if (li->head != 0x0) li->head->nxt = a; else li->tail = a;
        li->head = a;
    }

    if (location == BACK) {
        a->nxt = li->tail;
        a->prev = 0x0;

        if (li->tail != 0x0) li->tail->prev = a; else li->head = a;
        li->tail = a;
    }

    li->length++;
}

/*
 * Removes a node from the list.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - location => FRONT: pop from the head | BACK: pop from the tail
 *
 * Time Complexity: O(1)
 */
void list_pop(list_ptr *li, int location) {
    if (li->tail == 0x0 || !li || (location != FRONT && location != BACK)) return;

    if (location == BACK) {
        list_t *tmp = li->tail;
        li->tail = li->tail->nxt;
    
        if (li->tail != 0x0) li->tail->prev = 0x0; else li->head = 0x0;
        free(tmp->data);
        free(tmp);
        li->length--;

        return;
    }

    if (location == FRONT) {
        list_t *tmp = li->head;
        li->head = li->head->prev;
    
        if (li->head != 0x0) li->head->nxt = 0x0; else li->tail = 0x0;
        free(tmp->data);
        free(tmp);
        li->length--;

        return;
    }
}

/*
 * Frees the whole list.
 * Parameters:
 *    - list_ptr => pointer to the list
 * 
 * Time Complexity: O(node)
 */
void list_free(list_ptr *li) {
    if (!li) return;
    list_t *current = li->tail;
    list_t *tmp;

    while (current != 0x0) {
        tmp = current;
        current = current->nxt;
        free(tmp->data);
        free(tmp);
    }
}

/*
 * Writes the input data to a specific range in a specific list.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *    - start_location, end_location => write range
 *    - input => pointer to the input data
 * 
 * Time Complexity: O(index + data_size)
 */
void list_write(list_ptr *li, int index, int start_location, int end_location, int *input) {
    if (!li || !input || start_location > end_location) return;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (start_location < 0 || end_location >= (sizeof(current->data) / sizeof(int))) return;

            for (int i = start_location; i <= end_location; i++) {
                current->data[i] = input[i - start_location];
            }
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Retrieves the data from a specific list.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *
 * Return: pointer to the data
 * Time Complexity: O(index)
 */
int *list_retrieve(list_ptr *li, int index) {
    if (!li) return 0x0;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1 ;

    while (current != 0x0) {
        if (count == index) return current->data;

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return 0x0;
}

/*
 * Description:
 *    - Initialize double linked list iterator
 *    - iterator_ptr *<variable_name> = iterator_init(<list_ptr>, index);
 *    - At least one node must be present for the iterator to point to
 *    - If the selected node is deleted, the iterator becomes invalid
 *    - Iterator's insert and delete function does not move iterator itself
 *    - Iterator's move function is the only way to move iterator
 * 
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *
 * Return: pointer to the iterator_ptr
 * Time Complexity: O(index)
 */
iterator_ptr *iterator_init(list_ptr *li, int index) {
    if (!li) return 0x0;

    list_t *current = (index >= 0) ? li->tail : li->head;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) {
            iterator_ptr *a = (iterator_ptr *) malloc(sizeof(iterator_ptr));
            a->points = current;
            return a;
        }
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return 0x0;
}

/*
 * Adds a new node to the back of selected list with the given data.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - iterator_ptr => pointer to the iterator
 *    - size => input size
 *    - input => pointer to the input data
 *
 * Time Complexity: O(data_size)
 */
void iterator_insert(list_ptr *origin, iterator_ptr *li, int size, int *input) {
    if (!li || !li->points || !origin) return;

    list_t *a = (list_t *) malloc(sizeof(list_t));
    a->data = (int *) malloc(size * sizeof(int));
    for (int i = 0; i < size; i++) a->data[i] = input[i];

    a->nxt = li->points;
    a->prev = li->points->prev;
    
    if (li->points->prev == 0x0) origin->tail = a; else li->points->prev->nxt = a;
    li->points->prev = a;
    origin->length++;

    // insert at head is not supported. selector requires selected node1 to insert into. if there is not, the selector becomes indataid. free(<iterator_ptr>); to exit.
}

/*
 * Deletes a node from the back of selected list.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - iterator_ptr => pointer to the iterator
 *
 * Time Complexity: O(1)
 */
void iterator_delete(list_ptr *origin, iterator_ptr *li) {
    if (!li || !li->points || !origin || li->points->prev == 0x0) return;

    list_t *tmp = li->points->prev; // hold

    if (tmp->prev) {
        tmp->prev->nxt = li->points;
        li->points->prev = tmp->prev;

    } else { // tmp is the tail node1
        li->points->prev = 0x0;
        origin->tail = li->points;
    }

    free(tmp->data);
    free(tmp);
    origin->length--;
}

/*
 * Traverse throught the list
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *
 * Time Complexity: O(index)
 */
void iterator_move(iterator_ptr *li, int index) {
    if (!li || !li->points) return;

    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) {
            li->points = current;
            return;
        }
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Writes the input data to a specific range in a specific list (from iterator).
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *    - start_location, end_location => write range
 *    - input => pointer to the input data
 * 
 * Time Complexity: O(index + data_size)
 */
void iterator_write(iterator_ptr *li, int index, int start_location, int end_location, int *input) {
    if (!li || !li->points) return;
    
    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) for (int i = start_location; i <= end_location; i++) current->data[i] = input[i - start_location];

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return;
}

/*
 * Retrieves the data from a specific list (from iterator).
 * Parameters:
 *    - iterator_ptr => pointer to the iterator 
 *    - index => 0: forward | -1: backward (start from current node)
 *
 * Return: pointer to the data
 * Time Complexity: O(index)
 */
int *iterator_retrieve(iterator_ptr *li, int index) {
    if (!li || !li->points) return 0x0;
    
    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) return current->data;
        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return 0x0;
}

#endif
