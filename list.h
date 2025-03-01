#ifndef list
#define list

#include <stdlib.h>

typedef struct node {
    int *data;
    int size;
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

void integrated_mergesort_component_1(int arr[], int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    int leftArr[n1], rightArr[n2];

    for (int i = 0; i < n1; i++) {
        leftArr[i] = arr[left + i];
    }
    for (int j = 0; j < n2; j++) {
        rightArr[j] = arr[mid + 1 + j];
    }

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (leftArr[i] <= rightArr[j]) {
            arr[k] = leftArr[i];
            i++;
        } else {
            arr[k] = rightArr[j];
            j++;
        }
        k++;
    }

    while (i < n1) {
        arr[k] = leftArr[i];
        i++;
        k++;
    }

    while (j < n2) {
        arr[k] = rightArr[j];
        j++;
        k++;
    }
}


void integrated_mergesort_component_2(int arr[], int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;

        integrated_mergesort_component_2(arr, left, mid); // left side

        integrated_mergesort_component_2(arr, mid + 1, right); // right side

        integrated_mergesort_component_1(arr, left, mid, right);
    }
}

/*
 * Description:
 *    - As linked list node stored in a non-contiguous memory locations. It might cause cache miss. Frequently traversing is not recommended.
 *    - Inside node, a contiguous memory block. is provided (array) with useful built in features. data management and automatic memory allocation, pre-allocation. sorting algorithium included. You might utilize that instead.
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
    a->size = size;
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
 * Description:
 *    - Writes the input data to a specific range in a specific list.
 *    - To access the last element, set start and end to -1.
 *    - Automatically resizes the array if end_location exceeds its current size, with extra memory reserved (optional).
 *
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *    - start, end => write range
 *    - input => pointer to the input data
 *    - reserved => number of extra elements to pre-allocate on resize
 *
 * Time Complexity: O(index + input_size)
 */
void list_write(list_ptr *li, int index, int start, int end, int *input, int reserved) {
    if (!li || !input || start > end) return;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (start == -1 && end == -1) {
                current->data[current->size - 1] = input[0];
                return;
            }

            if (start < 0) return;

            if (end >= current->size) {
                if (reserved < 0) return;

                int *new_data = (int*) realloc(current->data, ((end + 1) + reserved) * sizeof(int));
                current->data = new_data;

                for (int i = 0; i < reserved; i++) current->data[(end + 1) + i] = 0xFFFFFFFF;

                current->size = (end + 1) + reserved;
            }

            for (int i = start; i <= end; i++) current->data[i] = input[i - start];
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Returns size of an array in a specified list. 
 * Recommended to use in list_write start, end argument to write to the back
 * Eg. list_write(pointer, 0, list_size(pointer, 0) - 1, list_size(pointer, 0) + 9, input, 0);
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 * 
 * Time Complexity: O(index)
 */
int list_size(list_ptr *li, int index) {
    if (!li) return;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) return current->size;

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
 *    - O(index + data_size) in the worst case
 *    - O(index + 1) in the best case
 */
void list_erase(list_ptr *li, int index, int start, int end, int free_memory) {
    if (!li || start > end) return;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (start == -1 && end == -1) {
                current->data[current->size - 1] = 0xFFFFFFFF;

                if (free_memory) {
                    int *new_data = (int*) realloc(current->data, current->size - 1 * sizeof(int));
                    current->data = new_data;
                    current->size--;
                }
                return;
            }

            if (start < 0) return;

            for (int i = end + 1; i < current->size; i++) {
                current->data[i - (end - start + 1)] = current->data[i];
                current->data[i] = 0xFFFFFFFF;
            }

            if (free_memory) {
                int *new_data = (int*) realloc(current->data, (current->size - (end - start + 1)) * sizeof(int));
                current->data = new_data;
                current->size -= (end - start + 1);
            }
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Simple built in, in-place sorting algorithium.
 * Recommended for small datasets. 
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *
 * Time Complexity: 
 *    - O(index + data_size ^2) in the worst case
 *    - O(index + data_size) in the best case
 */
void list_bubblesort(list_ptr *li, int index) {
    if (!li) return;
    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            int *data = current->data;
            for (int i = 0; i < current->size - 1; i++) {
                int swap = 0;

                for (int j = 0; j < current->size - 1 - i; j++) {
                    if (data[j] > data[j + 1]) {
                        int temp = data[j];
                        data[j] = data[j + 1];
                        data[j + 1] = temp;

                        swap = 1;
                    }
                }

                if (!swap) break;
            }
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Advanced built in, memory intensive sorting algorithium.
 * Recommended for large datasets.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *
 * Time Complexity: 
 *    - O(index + data_size log data_size) in all cases
 * 
 * Space Complexity: O(data_size) 
 */
void list_mergesort(list_ptr *li, int index) {
    if (!li) return;
    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            integrated_mergesort_component_2(current->data, 0, current->size - 1);
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Retrieves the data from a specific list. Must free manually afterward.
 * To access the last element, set start and end to -1.
 * Parameters:
 *    - list_ptr => pointer to the list
 *    - index => 0: forward | -1: backward
 *    - start, end => retrieve range
 *
 * Return: copy of pointer to the data
 * Time Complexity: O(index + retrieve_size)
 */
int *list_retrieve(list_ptr *li, int index, int start, int end) {
    if (!li || start > end) return 0x0;

    list_t *current = (index >= 0) ? li->head : li->tail;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (end >= current->size) return 0x0;

            if (start == -1 && end == -1) {
                int *scopy = (int *) malloc(sizeof(int));
                scopy[0] = current->data[current->size - 1];
                return scopy;
            }

            if (start < 0 || end < 0) return 0x0;

            int *copy = (int *) malloc((end - start + 1) * sizeof(int));
            for (int i = start; i <= end; i++) copy[i - start] = current->data[i];
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
    a->size = size;
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
 * Description:
 *    - Writes the input data to a specific range in a specific list (from iterator).
 *    - To access the last element, set start and end to -1.
 *    - Automatically resizes the array if end_location exceeds its current size, with extra memory reserved (optional).
 *
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *    - start, end => write range
 *    - input => pointer to the input data
 *    - reserved => number of extra elements to pre-allocate on resize
 *
 * Time Complexity: O(index + input_size)
 */
void iterator_write(iterator_ptr *li, int index, int start, int end, int *input, int reserved) {
    if (!li || !li->points || start > end) return;
    
    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;
    while (current != 0x0) {
        if (count == index) {
            if (start == -1 && end == -1) {
                current->data[current->size - 1] = input[0];
                return;
            }

            if (start < 0) return;

            if (end >= current->size) {
                if (reserved < 0) return;

                int *new_data = (int*) realloc(current->data, ((end + 1) + reserved) * sizeof(int));
                current->data = new_data;

                for (int i = 0; i < reserved; i++) current->data[(end + 1) + i] = 0xFFFFFFFF;

                current->size = (end + 1) + reserved;
            }

            for (int i = start; i <= end; i++) current->data[i] = input[i - start];
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Returns size of an array in a specified list (from iterator).
 * Recommended to use in iterator_write start, end argument to write to the back
 * Eg. iterator_write(pointer, 0, iterator_size(pointer, 0) - 1, iterator_size(pointer, 0) + 9, input, 0);
 * Parameters:
 *    - iterator_ptr => pointer to the iterator 
 *    - index => 0: forward | -1: backward (start from current node)
 * 
 * Time Complexity: O(index)
 */
int iterator_size(iterator_ptr *li, int index) {
    if (!li) return;

    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) return current->size;

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
 *    - O(index + data_size) in the worst case
 *    - O(index + 1) in the best case
 */
void iterator_erase(iterator_ptr *li, int index, int start, int end, int free_memory) {
    if (!li || start > end) return;

    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (start == -1 && end == -1) {
                current->data[current->size - 1] = 0xFFFFFFFF;

                if (free_memory) {
                    int *new_data = (int*) realloc(current->data, current->size - 1 * sizeof(int));
                    current->data = new_data;
                    current->size--;
                }
                return;
            }

            if (start < 0) return;

            for (int i = end + 1; i < current->size; i++) {
                current->data[i - (end - start + 1)] = current->data[i];
                current->data[i] = 0xFFFFFFFF;
            }

            if (free_memory) {
                int *new_data = (int*) realloc(current->data, (current->size - (end - start + 1)) * sizeof(int));
                current->data = new_data;
                current->size -= (end - start + 1);
            }
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Simple built in, in-place sorting algorithium.
 * Recommended for small datasets. 
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *
 * Time Complexity: 
 *    - O(index + data_size ^2) in the worst case
 *    - O(index + data_size) in the best case
 */
void iterator_bubblesort(iterator_ptr *li, int index) {
    if (!li) return;
    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            int *data = current->data;
            for (int i = 0; i < current->size - 1; i++) {
                int swap = 0;

                for (int j = 0; j < current->size - 1 - i; j++) {
                    if (data[j] > data[j + 1]) {
                        int temp = data[j];
                        data[j] = data[j + 1];
                        data[j + 1] = temp;

                        swap = 1;
                    }
                }

                if (!swap) break;
            }
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Advanced built in, memory intensive sorting algorithium.
 * Recommended for large datasets.
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *
 * Time Complexity: 
 *    - O(index + data_size log data_size) in all cases
 * 
 * Space Complexity: O(data_size) 
 */
void iterator_mergesort(iterator_ptr *li, int index) {
    if (!li) return;
    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            integrated_mergesort_component_2(current->data, 0, current->size - 1);
            return;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }
}

/*
 * Retrieves the data from a specific list (from iterator). Must free manually afterward.
 * To access the last element, set start and end to -1.
 * Parameters:
 *    - iterator_ptr => pointer to the iterator
 *    - index => 0: forward | -1: backward (start from current node)
 *    - start, end => retrieve range
 *
 * Return: copy of pointer to the data
 * Time Complexity: O(index + retrieve_size)
 */
int *iterator_retrieve(iterator_ptr *li, int index, int start, int end) {
    if (!li || start > end) return 0x0;

    list_t *current = li->points;
    int count = (index >= 0) ? 0 : -1;

    while (current != 0x0) {
        if (count == index) {
            if (end >= current->size) return 0x0;

            if (start == -1 && end == -1) {
                int *scopy = (int *) malloc(sizeof(int));
                scopy[0] = current->data[current->size - 1];
                return scopy;
            }

            if (start < 0 || end < 0) return 0x0;

            int *copy = (int *) malloc((end - start + 1) * sizeof(int));
            for (int i = start; i <= end; i++) copy[i - start] = current->data[i];
            return copy;
        }

        current = (index >= 0) ? current->nxt : current->prev;
        count += (index >= 0) ? 1 : -1;
    }

    return 0x0;
}

#endif
