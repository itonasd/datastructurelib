#ifndef array_h
#define array_h

#include <stdlib.h>
#include <string.h>
#include <malloc.h>

typedef struct array {
    unsigned char *data;
    unsigned int length;
    unsigned char size;
} array;

array *array_init(unsigned char size) {
    if (size >= 128) return 0x0;

    array *li = (array *) malloc(sizeof(array));
    li->data = 0x0;
    li->length = 0;
    li->size = size;

    return li;
}

// extended erase function
// - supports multiple ranges, sources.
// - supports forward, backward search.
// - supports array pre allocation.
// - array placeholder option. 
void write_s (
    array *li, 
    ssize_t *starts, 
    ssize_t *ends, 
    size_t preAllocate, 
    unsigned char _0xfill, 
    size_t sources,
    void **input
) {
    if (!li || !input || !starts || !ends) return;
    
    unsigned char swaps[sources];
    size_t highest_ends = 0;

    for (size_t i = 0; i < sources; i++) {
        if (starts[i] == -1) starts[i] = li->length - 1;
        if (ends[i] == -1) ends[i] = li->length - 1;
        if (starts[i] < 0 || ends[i] < 0) return;

        swaps[i] = 0;
        if (starts[i] > ends[i]) {
            size_t temp = starts[i];
            starts[i] = ends[i];
            ends[i] = temp;
            swaps[i] = 1;
        }

        highest_ends = (ends[i] > highest_ends) ? ends[i] : highest_ends;
    }

    unsigned char size = li->size;

    if (li->data == 0x0) {

        li->data = (unsigned char *) _aligned_malloc(((highest_ends + 1) + preAllocate) * size, 32);
        if (!li->data) return;

        memset(li->data, _0xfill, ((highest_ends + 1) + preAllocate) * size);
        li->length = (highest_ends + 1) + preAllocate;

    } else if (highest_ends >= li->length) {

        unsigned char *new_array = (unsigned char *) _aligned_realloc(li->data, ((highest_ends + 1) + preAllocate) * size, 32);
        if (!new_array) return;
        li->data = new_array;

        memset(li->data + (li->length * size), _0xfill, (((highest_ends + 1) + preAllocate) - li->length) * size);
        li->length = (highest_ends + 1) + preAllocate;
    }

    for (size_t i = 0; i < sources; i++) {
        if (swaps[i]) {
            for (size_t i = 0; i <= (ends[i] - starts[i]); i++) {
                memcpy(
                    li->data + (starts[i] + i) * size,
                    (unsigned char *)input[i] + (ends[i] - starts[i] - i) * size,
                    size
                );
            }
        } else {
            memcpy(
                li->data + starts[i] * size, 
                (unsigned char *)input[i],
                (ends[i] - starts[i] + 1) * size
            );
        }
    }

    return;
}

// extended erase function
// - supports multiple ranges.
// - array shrink option.
// - array placeholder option. 
void erase_s(
    array *li, 
    ssize_t *starts, 
    ssize_t *ends, 
    unsigned char shrink,
    unsigned char _0xfill,
    size_t sources
) {
    if (!li || !starts || !ends) return;
    unsigned char size = li->size;
    size_t new_length = li->length;

    for (size_t i = 0; i < sources; i++) {
        if (starts[i] == -1) starts[i] = li->length - 1;
        if (ends[i] == -1) ends[i] = li->length - 1;
        if (starts[i] < 0 || ends[i] < 0) return;
        if (starts[i] > li->length - 1 || ends[i] > li->length - 1) return;

        if (starts[i] > ends[i]) {
            size_t temp = starts[i];
            starts[i] = ends[i];
            ends[i] = temp;
        }
        new_length -= 1 + (ends[i] - starts[i]);
        memset(li->data + (starts[i] * size), _0xfill, (ends[i] - starts[i] + 1) * size);
    }

    unsigned char compare[size];
    memset(compare, _0xfill, size);

    size_t valid_indx = 0;
    for (size_t i = 0; i < li->length; i++) {
        if (memcmp(li->data + (i * size), compare, size) != 0) {
            if (i != valid_indx) {
                memmove(li->data + (valid_indx * size), li->data + (i * size), size);
                memset(li->data + (i * size), _0xfill, size);
            }
            valid_indx++;
        }
    }

    if (shrink) {
        unsigned char *new_array = (unsigned char *) _aligned_realloc(li->data, new_length * size, 32);
        if (!new_array) return;

        li->data = new_array;
        li->length = new_length;
    }
}

// extended retrieve function
// - supports multiple ranges.
array *retrieve_s(
    array *li, 
    ssize_t *starts, 
    ssize_t *ends, 
    size_t sources
) {
    if (!li || !starts || !ends) return 0x0;
    size_t copies = 0;

    for (size_t i = 0; i < sources; i++) {
        if (starts[i] == -1) starts[i] = li->length - 1;
        if (ends[i] == -1) ends[i] = li->length - 1;
        if (starts[i] < 0 || ends[i] < 0) return 0x0;
        if (starts[i] > li->length - 1 || ends[i] > li->length - 1) return 0x0;
        if (starts)

        if (starts[i] > ends[i]) {
            size_t temp = starts[i];
            starts[i] = ends[i];
            ends[i] = temp;
        }

        copies += (ends[i] - starts[i]) + 1;
    }

    array *new_array = array_init(li->size);
    if (!new_array) return 0x0;

    unsigned char size = li->size;
    new_array->data = (unsigned char *) _aligned_malloc(copies * size, 32);
    if (!new_array->data) return 0x0;

    new_array->size = size;
    new_array->length = copies;

    size_t prev_location = 0;

    for (size_t i = 0; i < sources; i++) {
        memcpy(new_array->data + (prev_location * size), li->data + (starts[i] * size),
        (ends[i] - starts[i] + 1) * size);

        prev_location += ends[i] - starts[i] + 1;
    }

    return new_array;
}

// extended search function
// - supports multiple ranges and value.
// - supports forward, backward search.
// - index / boolean return type option.
ssize_t search_s(
    array *li, 
    ssize_t *starts, 
    ssize_t *ends,
    size_t sources_length,
    size_t sources,
    size_t type,
    void *input
) {
    if (!li || !starts || !ends || !sources_length || !input) return (type) ? 0 : -1;
    for (size_t i = 0; i < sources; i++) {
        if (starts[i] == -1) starts[i] = li->length - 1;
        if (ends[i] == -1) ends[i] = li->length - 1;
        if (starts[i] < 0 || ends[i] < 0) return (type) ? 0 : -1;
        if (starts[i] > li->length - 1 || ends[i] > li->length - 1) return (type) ? 0 : -1;
    }

    size_t size = li->size;
    size_t result = -1;

    for (size_t i = 0; i < sources; i++) {
        ssize_t step = (starts[i] <= ends[i]) ? 1 : -1;
        for (ssize_t j = starts[i]; (step == 1) ? j <= ends[i] : j >= ends[i]; j += step) {
            for (size_t k = 0; k < sources_length; k++) {
                unsigned char *dest = li->data + (j * size);
                unsigned char *src = (unsigned char *) input + (k * size);

                if (memcmp(dest, src, size) == 0) {
                    result = j;
                    goto finish;
                }

            }
        }
    }
    finish:

    return (type) ? (result == -1) ? 0 : 1 : result;
}

ssize_t *range(ssize_t st, ssize_t en) {
    ssize_t* tmp = (ssize_t *) malloc(sizeof(ssize_t) * 2);
    tmp[0] = st; tmp[1] = en;
    return tmp;
}

// extended method: write_s
void write(array *dest, ssize_t *range, void* src) {
    void *srcs[1] = {src};
    ssize_t st[1] = {range[0]};
    ssize_t en[1] = {range[1]};
    write_s(dest, st, en, 0, 0xFF, 1, srcs);
    free(range);
}

// extended method: erase_s
void erase(array *dest, ssize_t *range) {
    ssize_t st[1] = {range[0]};
    ssize_t en[1] = {range[1]};
    erase_s(dest, st, en, 1, 0xFF, 1);
    free(range);
}

// extended method: retrieve_s
array *retrieve(array *dest, ssize_t *range) {
    ssize_t st[1] = {range[0]};
    ssize_t en[1] = {range[1]};
    return retrieve_s(dest, st, en, 1);
    free(range);
}

// extended method: search_s
ssize_t search(array *dest, ssize_t *range, ssize_t src) {
    ssize_t srcs[1] = {src};
    ssize_t st[1] = {range[0]};
    ssize_t en[1] = {range[1]};
    return search_s(dest, st, en, 1, 1, 0, (void *) srcs);
    free(range);
}

/*
void array_log(array *data) {
    unsigned char *byte = (unsigned char *)data->data;
    int *_int = (int *)data->data;
    
    for (int i = 0; i < data->length; i++) {
        printf("    cell %2d: [ ", i);
        for (int j = 0; j < data->size; j++) {
            printf("0x%02x ", byte[i * data->size + j]);
        }
        printf("]  %d\n", _int[i]);
    }
    printf("    size: %d, length: %d\n\n", data->size, data->length);
}


DOES SUPPORT POINTER TO ANOTHER ARRAY STRUCTURE (AKA MULTI DIMENSIONAL ARRAY)
USAGE EXAMPLE:

    array *b = array_init(sizeof(int));

    int source1[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    write(b, range(0, 9), source1);

    // array a that holds another array pointer
    array *a = array_init(sizeof(array *));

    // write b into a
    array *source2[1] = {b};
    write(a, range(0, 0), source2);

    // retrieve b from a
    array *retrieved_b = retrieve(a, range(0, 0));
    array *ret = ((array **)retrieved_b->data)[0];
*/

#endif
