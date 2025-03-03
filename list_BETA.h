#ifndef list_BETA
#define list_BETA

#include <stdlib.h>
#include <string.h>

typedef struct node {
    void *array;
    unsigned int length;
    unsigned int size;
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

#define FRONT 0x2000
#define BACK 0x1000
#define TRUE 1
#define FALSE 0


void mergesortcmp_1(list_t *arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;
    unsigned int size = arr->size;

    int *leftArr = (int *) malloc(n1 * size);
    int *rightArr = (int *) (n2 * size);

    for (int i = 0; i < n1; i++) {
        memcpy(leftArr + (i * size), arr->array + ((left + i) * size), size);
    }
    for (int j = 0; j < n2; j++) {
        memcpy(rightArr + (j * size), arr->array + ((mid + 1 + j) * size), size);
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (*(int *)(leftArr + (i * size)) <= *(int *)(rightArr + (j * size))) {
            memcpy(arr->array + (k * size), leftArr + (i * size), size);
            i++;
        } else {
            memcpy(arr->array + (k * size), rightArr + (j * size), size);
            j++;
        }
        k++;
    }

    while (i < n1) {
        memcpy(arr->array + (k * size), leftArr + (i * size), size);
        i++;
        k++;
    }

    while (j < n2) {
        memcpy(arr->array + (k * size), rightArr + (j * size), size);
        j++;
        k++;
    }

    free(leftArr); free(rightArr);
}


void mergesortcmp_2(list_t *arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        mergesortcmp_2(arr, left, mid); // left side

        mergesortcmp_2(arr, mid + 1, right); // right side

        mergesortcmp_1(arr, left, mid, right);
    }
}

void bubblesortcmp_1(list_t *current) {
    void *array = current->array;
    for (int i = 0; i < current->length - 1; i++) {

        int swap = 0;

        for (int j = 0; j < current->length - 1 - i; j++) {
            if (*(int*)current->array + (i * current->size) > *(int*)current->array + ((j + 1) * current->size)) {
                int temp = *(int*)current->array + (j * current->size);
                memcpy(current->array + (j * current->size), current->array + ((j + 1) * current->size), current->size);
                memset(current->array + ((j+ 1) * current->size), temp, current->size);

                swap = 1;
            }
        }

        if (!swap) break;
    }
}

int linearsearchcmp_1(list_t *current, int start, int end, int *target, int target_length) {
    if (start < 0 || end >= current->length) return 0xFFFFFFFF;

    int step = (start <= end) ? 1 : -1;
    for (int i = start; (step == 1) ? i <= end : i >= end; i += step) {
        for (int j = 0; j < target_length; j++) if (*(int*)current->array + (i * current->size) == target[j]) return i;
    }

    return 0xFFFFFFFF;
}


int binarysearchcmp_1(list_t *current, int start, int end, int *target, int target_length) {
    if (end >= current->length || start < 0 || start > end) return 0xFFFFFFFF;

    int left = start;
    int right = end;

    while (left <= right) {
        int mid = (left + right) / 2;
        for (int i = 0; i < target_length; i++) {
            if (*(int*)current->array + (mid * current->size) == target[i]) {
                return mid;
            }
        }

        if (*(int*)current->array + (mid * current->size) < target[0]) {
            left = mid + 1;
        } else {
            right = mid - 1;
        }
    }

    return 0xFFFFFFFF;
}



/*
 * Description:
 *    - As linked list node stored in a non-contiguous memory locations. It might cause cache miss. Frequently traversing is not recommended.
 *    - Inside node, a contiguous memory block. is provided (array) with useful built in features. You might utilize that instead.
 *    - 9 Avaliable built in features: push, pop, free, write, erase, retrieve, sort, find, length
 *    - Initialize double linked list
 *    - list_ptr *<variable_name> = list_init();
 *
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

/*
 * Adds a new node to the list with the given array.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - location => FRONT: push to the head | BACK: push to the tail
 *    - length => input length
 *    - size => allocation size (byte), best practice to use (sizeof(T))
 *    - input => pointer to the input array
 *
 * Time Complexity: O(array_length)
 */
void list_push(list_ptr *li, unsigned int location, unsigned int length, unsigned char size ,void *input) {
    if (!li || !input || (location != FRONT && location != BACK)) return;
    list_t *a = (list_t *) malloc(sizeof(list_t));

    a->array = (void *) malloc(length * size);
    a->length = length;
    a->size = size;
    memcpy(a->array, input, length * size);


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
        free(tmp->array);
        free(tmp);
        li->length--;

        return;
    }

    if (location == FRONT) {
        list_t *tmp = li->head;
        li->head = li->head->prev;
    
        if (li->head != 0x0) li->head->nxt = 0x0; else li->tail = 0x0;
        free(tmp->array);
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
        free(tmp->array);
        free(tmp);
    }
}

/*
 * Description:
 *    - Writes the input array to a specific range in a specific list.
 *    - To access the last element, set start and end to -1.
 *    - Automatically relengths the array if end_location exceeds its current length, with extra memory reserved (optional).
 *
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *    - start, end => write range
 *    - input => pointer to the input array
 *    - reserved => number of extra elements to pre-allocate on relength
 *
 * Time Complexity: O(index + input_length)
 */
void list_write(list_ptr *li, int index, int start, int end, void *input, unsigned int reserved) {
    if (!li || !input || start > end) return;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (start == -1 && end == -1) {
                memcpy(current->array + (current->length - 1) * current->size, input, current->size);
                return;
            }

            if (start < 0) return;

            if (end >= current->length) {
                if (reserved < 0) return;

                void *new_array = (void *) realloc(current->array, ((end + 1) + reserved) * sizeof(current->size));
                current->array = new_array;

                memset(current->array + ((end + 1) * current->size), 0xFFFFFFFF, reserved * current->size);

                current->length = (end + 1) + reserved;
            }

            for (int i = start; i <= end; i++) memcpy(current->array + i * current->size,
                (void *)input + (i - start) * current->size,
                current->size);

            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Returns length of an array in a specified list. 
 * Recommended to use in list_write start, end argument to write to the back
 * Eg. list_write(pointer, 0, list_length(pointer, 0) - 1, list_length(pointer, 0) + 9, input, 0);
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 * 
 * Time Complexity: O(index)
 */
int list_length(list_ptr *li, int index) {
    if (!li) return;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) return current->length;

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Description:
 *    - Deletes element in a specific range, list. 
 *    - To access the last element, set start and end to -1.
 *    - Deleting from the end is recommended as it avoids shifting elements. 
 *    - Deleting from the start requires moving other elements to fill the gap.
 *
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *    - start, end => erase range
 *    - free_memory => FALSE | TRUE
 *
 * Time Complexity: 
 *    - O(index + array_length) in the worst case
 *    - O(index + 1) in the best case
 */
void list_erase(list_ptr *li, int index, int start, int end, unsigned int free_memory) {
    if (!li || start > end) return;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (start == -1 && end == -1) {
                memset(current->array + (current->length - 1) * current->size, 0xFFFFFFFF, current->size);

                if (free_memory) {
                    void *new_array = (void*) realloc(current->array, current->length - 1 * sizeof(current->size));
                    current->array = new_array;
                    current->length--;
                }
                return;
            }

            if (start < 0) return;

            for (int i = end + 1; i < current->length; i++) {
                memcpy(current->array + ((i - (end - start + 1)) * current->size), current->array + i, current->size);
                memset(current->array + (i * current->size), 0xFFFFFFFF, current->size);
            }

            if (free_memory) {
                void *new_array = (void*) realloc(current->array, (current->length - (end - start + 1)) * sizeof(current->size));
                current->array = new_array;
                current->length -= (end - start + 1);
            }
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Sorting Algorithiums. (ONLY 4BYTE INTEGER TYPE SUPPORTED)
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *    - option => 0: bubble | 1: merge (recommended)
 *
 * Time Complexity:
 *    - bubble: O(array_length) best case | O(n^array_length) worst case
 *    - merge: O(array_length log array_length) all cases
 */
void list_sort(list_ptr *li, int index, unsigned int option) {
    if (!li) return;
    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            switch (option) {
                case 0:
                    bubblesortcmp_1(current);
                    return;
                case 1:
                    mergesortcmp_2(current, 0, current->length - 1);
                    return;
                default:
                    return;
            }
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Searching Algorithiums. (ONLY 4BYTE INTEGER TYPE SUPPORTED)
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *    - start, end => search range (linear search supports backward searching)
 *    - target => pointer to group of target number
 *    - target_length => group of target number's length
 *    - option => 0: linear search | 1: binary search (sorted array)
 *    - returnType => 0: index | 1: boolean
 * 
 * Return: (index | 0xFFFFFFFF) | (TRUE | FALSE) | -1 (Invalid)
 * Time Complexity:
 *    - linear search: O(index) best case | O(index + search_range * target_length) worst case
 *    - binary search: O(index + log search_range * target_length) all cases
 */
int list_find(list_ptr *li, int index, unsigned int start, unsigned int end, int *target, unsigned int target_length, unsigned int option, unsigned int returnType) {
    if (!li) return;
    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            switch (option) {
                case 0:
                    int result = linearsearchcmp_1(current, start, end, target, target_length);
                    return (returnType) ? (result == 0xFFFFFFFF) ? FALSE : TRUE : result;
                case 1:
                    int result = binarysearchcmp_1(current, start, end, target, target_length);
                    return (returnType) ? (result == 0xFFFFFFFF) ? FALSE : TRUE : result;
                default:
                    return -1;
            }
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return -1;
}

/*
 * Retrieves the array from a specific list. Must free manually afterward.
 * To access the last element, set start and end to -1.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *    - start, end => retrieve range
 *
 * Return: copy of pointer to the array
 * Time Complexity: O(index + retrieve_length)
 */
void *list_retrieve(list_ptr *li, int index, int start, int end) {
    if (!li || start > end) return 0x0;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (end >= current->length) return 0x0;

            if (start == -1 && end == -1) {
                void *scopy = (void *) malloc(sizeof(void));
                memcpy(scopy, current->array + ((current->length - 1) * current->size), current->size);
                return scopy;
            }

            if (start < 0 || end < 0) return 0x0;

            void *copy = (void *) malloc((end - start + 1) * sizeof(void));
            for (int i = start; i <= end; i++)
            memcpy(copy + ((i - start) * current->size), current->array + (i * current->size), current->size);
            return copy;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return 0x0;
}

/*
 * Description:
 *    - Initialize double linked list iterator.
 *    - iterator_ptr *<variable_name> = iterator_init(<list_ptr>, index);
 *    - At least one node must be present for the iterator to point to.
 *    - If the selected node is deleted, the iterator becomes invalid.
 *    - Iterator's insert and delete function does not move iterator itself.
 *    - Iterator's move function is the only way to move iterator.
 *    - 9 Avaliable built in features: insert, delete, move, write, erase, retrieve, sort, find, length
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
 * Adds a new node to the back of selected list with the given array.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - iterator_ptr => pointer to the iterator
 *    - length => input length
 *    - size => allocation size (byte), best practice to use (sizeof(T))
 *    - input => pointer to the input array
 *
 * Time Complexity: O(array_length)
 */
void iterator_insert(list_ptr *origin, iterator_ptr *li, unsigned int length, unsigned int size, void *input) {
    if (!li || !li->points || !origin) return;
    list_t *a = (list_t *) malloc(sizeof(list_t));

    a->array = (void *) malloc(length * size);
    a->length = length;
    a->size = size;
    memcpy(a->array, input, length * size);

    a->nxt = li->points;
    a->prev = li->points->prev;
    
    if (li->points->prev == 0x0) origin->tail = a; else li->points->prev->nxt = a;
    li->points->prev = a;
    origin->length++;

    // insert at head is not supported. selector requires selected node1 to insert into. if there is not, the selector becomes inarrayid. free(<iterator_ptr>); to exit.
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

    free(tmp->array);
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
 * Description:
 *    - Writes the input array to a specific range in a specific list (from iterator).
 *    - To access the last element, set start and end to -1.
 *    - Automatically relengths the array if end_location exceeds its current length, with extra memory reserved (optional).
 *
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *    - start, end => write range
 *    - input => pointer to the input array
 *    - reserved => number of extra elements to pre-allocate on relength
 *
 * Time Complexity: O(index + input_length)
 */
void iterator_write(iterator_ptr *li, int index, int start, int end, int *input, unsigned int reserved) {
    if (!li || !li->points || start > end) return;
    
    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) {
            if (start == -1 && end == -1) {
                memcpy(current->array + (current->length - 1) * current->size, input, current->size);
                return;
            }

            if (start < 0) return;

            if (end >= current->length) {
                if (reserved < 0) return;

                void *new_array = (void *) realloc(current->array, ((end + 1) + reserved) * sizeof(current->size));
                current->array = new_array;

                memset(current->array + ((end + 1) * current->size), 0xFFFFFFFF, reserved * current->size);

                current->length = (end + 1) + reserved;
            }

            for (int i = start; i <= end; i++) memcpy(current->array + i * current->size,
                (void *)input + (i - start) * current->size,
                current->size);

            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Returns length of an array in a specified list (from iterator).
 * Recommended to use in iterator_write start, end argument to write to the back
 * Eg. iterator_write(pointer, 0, iterator_length(pointer, 0) - 1, iterator_length(pointer, 0) + 9, input, 0);
 * Parameters:
 *    - iterator_ptr => pointer to the iterator 
 *    - index => 0: forward | -1: backward (start from current node)
 * 
 * Time Complexity: O(index)
 */
int iterator_length(iterator_ptr *li, int index) {
    if (!li) return;

    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) return current->length;

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Description:
 *    - Deletes element in a specific range, list (from iterator). 
 *    - To access the last element, set start and end to -1.
 *    - Deleting from the end is recommended as it avoids shifting elements. 
 *    - Deleting from the start requires moving other elements to fill the gap.
 *
 * Parameters:
 *    - iterator_ptr => pointer to the itertor 
 *    - index => 0: forward | -1: backward (start from current node)
 *    - start, end => erase range
 *    - free_memory => FALSE | TRUE
 *
 * Time Complexity: 
 *    - O(index + array_length) in the worst case
 *    - O(index + 1) in the best case
 */
void iterator_erase(iterator_ptr *li, int index, int start, int end, unsigned int free_memory) {
    if (!li || start > end) return;

    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (start == -1 && end == -1) {
                memset(current->array + (current->length - 1) * current->size, 0xFFFFFFFF, current->size);

                if (free_memory) {
                    void *new_array = (void*) realloc(current->array, current->length - 1 * sizeof(current->size));
                    current->array = new_array;
                    current->length--;
                }
                return;
            }

            if (start < 0) return;

            for (int i = end + 1; i < current->length; i++) {
                memcpy(current->array + ((i - (end - start + 1)) * current->size), current->array + i, current->size);
                memset(current->array + (i * current->size), 0xFFFFFFFF, current->size);
            }

            if (free_memory) {
                void *new_array = (void*) realloc(current->array, (current->length - (end - start + 1)) * sizeof(current->size));
                current->array = new_array;
                current->length -= (end - start + 1);
            }
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Sorting Algorithiums. (ONLY 4BYTE INTEGER TYPE SUPPORTED)
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *    - option => 0: bubble | 1: merge (recommended)
 *
 * Time Complexity:
 *    - bubble: O(1) best case | O(array_length^2) worst case
 *    - merge: O(array_length log array_length) all cases
 */
void iterator_sort(iterator_ptr *li, int index, int option) {
    if (!li) return;
    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            switch (option) {
                case 0:
                    bubblesortcmp_1(current);
                    return;
                case 1:
                    mergesortcmp_2(current, 0, current->length - 1);
                    return;
                default:
                    return;
            }
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Searching Algorithiums. (ONLY 4BYTE INTEGER TYPE SUPPORTED)
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *    - start, end => search range (linear search supports backward searching)
 *    - target => pointer to group of target number
 *    - target_length => group of target number's length
 *    - option => 0: linear search | 1: binary search (sorted array)
 *    - returnType => 0: index | 1: boolean
 * 
 * Return: (index | 0xFFFFFFFF) | (TRUE | FALSE) | -1 (Invalid)
 * Time Complexity:
 *    - linear search: O(index) best case | O(index + search_range * target_length) worst case
 *    - binary search: O(index + log search_range * target_length) all cases
 */
int iterator_find(iterator_ptr *li, int index, int start, int end, int *target, int target_length, int option, int returnType) {
    if (!li) return;
    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            switch (option) {
                case 0:
                    int result = linearsearchcmp_1(current, start, end, target, target_length);
                    return (returnType) ? (result == 0xFFFFFFFF) ? FALSE : TRUE : result;
                case 1:
                    int result = binarysearchcmp_1(current, start, end, target, target_length);
                    return (returnType) ? (result == 0xFFFFFFFF) ? FALSE : TRUE : result;
                default:
                    return -1;
            }
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return -1;
}

/*
 * Retrieves the array from a specific list (from iterator). Must free manually afterward.
 * To access the last element, set start and end to -1.
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *    - start, end => retrieve range
 *
 * Return: copy of pointer to the array
 * Time Complexity: O(index + retrieve_length)
 */
void *iterator_retrieve(iterator_ptr *li, int index, int start, int end) {
    if (!li || start > end) return 0x0;

    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (end >= current->length) return 0x0;

            if (start == -1 && end == -1) {
                void *scopy = (void *) malloc(sizeof(void));
                memcpy(scopy, current->array + ((current->length - 1) * current->size), current->size);
                return scopy;
            }

            if (start < 0 || end < 0) return 0x0;

            void *copy = (void *) malloc((end - start + 1) * sizeof(void));
            for (int i = start; i <= end; i++)
            memcpy(copy + ((i - start) * current->size), current->array + (i * current->size), current->size);
            return copy;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return 0x0;
}

#endif
