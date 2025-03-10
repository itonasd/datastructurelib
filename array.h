#ifndef array_h
#define array_h

#include <stdlib.h>
#include <string.h>

// 40ULL
typedef struct array {
    unsigned char *data;
    size_t length;
    unsigned short size;

    struct config {
        unsigned char default_cell_value; // uninitialized cell value (default: 0xFF)
        unsigned char write_preference_mode; // 0: overwrite | 1: insertion (default: 0)
        unsigned char erase_preference_mode; // 0: leave as default value | 1: realign + shrink (default: 1)
        unsigned char search_return_as; // 0: return as index | 1: return as boolean (default: 0)
        unsigned char memory_alignment_factor; // memory alignment factor (default: 32)
        ssize_t pre_allocation_factor; // memory pre-allocation on expand (default: 0)
    } config;

} array; 

array *array_init(unsigned short size) {

    array *li = (array *) malloc(sizeof(array));
    li->data = 0x0;
    li->length = 0;
    li->size = size;

    // configurations
    li->config.default_cell_value = 0xFF;
    li->config.write_preference_mode = 0;
    li->config.erase_preference_mode = 1;
    li->config.search_return_as = 0;
    li->config.memory_alignment_factor = 32;
    li->config.pre_allocation_factor = 0;

    return li;
}

// extended write function
// - supports multiple ranges, sources.
// - supports forward, backward write.
// - supports array pre allocation.
// - supports insertion.
// - array placeholder option. 
void write_beta (
    array *li, 
    ssize_t *starts, 
    ssize_t *ends, 
    size_t preAllocate, 
    unsigned char _0xfill, 
    unsigned char __BETA_INSERTION_MODE,
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
    
    unsigned short size = li->size;
    if (li->data == 0x0 && __BETA_INSERTION_MODE) __BETA_INSERTION_MODE = 0;
    
    if (li->data == 0x0) {
        
        li->data = (unsigned char *) malloc(((highest_ends + 1) + preAllocate) * size);
        if (!li->data) return;
        
        memset(li->data, _0xfill, ((highest_ends + 1) + preAllocate) * size);
        li->length = (highest_ends + 1) + preAllocate;
        
    } else if (highest_ends >= li->length && !__BETA_INSERTION_MODE) {
        
        unsigned char *new_array = (unsigned char *) realloc(li->data, ((highest_ends + 1) + preAllocate) * size);
        if (!new_array) return;
        li->data = new_array;
        
        memset(li->data + (li->length * size), _0xfill, (((highest_ends + 1) + preAllocate) - li->length) * size);
        li->length = (highest_ends + 1) + preAllocate;
    }

    if (__BETA_INSERTION_MODE) {
        size_t required_space[sources];
        size_t total_req_space = 0;
        size_t preAllocatedSpace = 0;
        unsigned char compare[size];
        memset(compare, _0xfill, size);

        for (size_t i = 0; i < sources; i++) {
            required_space[i] = 0;
            unsigned char *data_ptr = li->data + (starts[i] * size);
            for (size_t j = starts[i]; j <= ends[i]; j++, data_ptr += size) if (memcmp(data_ptr, compare, size) != 0) {
                required_space[i]++;
                total_req_space++;
            }
        }

        
        for (size_t i = li->length; i > 0; i--) {
            if (memcmp(li->data + ((i - 1) * size), compare, size) != 0) break;
            preAllocatedSpace++;
        }
        
        size_t old_length = 0;
        size_t new_length = 0;
        size_t old_size = 0;
        size_t new_size = 0;
        
        
        if ((ssize_t) total_req_space - (ssize_t) preAllocatedSpace > 0) {
            old_length = li->length;
            new_length = li->length + total_req_space + preAllocate - preAllocatedSpace;
            old_size = old_length * size;
            new_size = new_length * size;
            
            unsigned char* new_array = (unsigned char *) realloc(li->data, new_size);
            if (!new_array) return;
            
            li->data = new_array;
            li->length = new_length;
            
            memset(li->data + old_size, _0xfill, new_size - old_size);
        }
        
        if (total_req_space) { 
            size_t iterator_high = ((li->length - 1) - preAllocatedSpace) + required_space[0];
            size_t origin_iterator_high = iterator_high;
            size_t iterator_low = (li->length - 1) - preAllocatedSpace;
            
            if ((ssize_t) total_req_space - (ssize_t) preAllocatedSpace > 0) {
                iterator_high = ((new_length - 1) - preAllocate) - (total_req_space - required_space[0]);
                origin_iterator_high = iterator_high;
                iterator_low = old_length - 1;
            }
            
            for (size_t i = 0; i < sources;) {
                if (iterator_low <= ends[i]) if (memcmp(li->data + (iterator_low * size), compare, size) == 0) {
                    if (iterator_low == starts[i]) goto here;
                    iterator_low--;
                    continue;
                }
                
                memmove(li->data + (iterator_high * size), li->data + (iterator_low * size), size);
                iterator_high--;
                
                if (iterator_low == starts[i]) {
                    here:
                    i++;
                    if ((ssize_t) total_req_space - (ssize_t) preAllocatedSpace > 0) {
                        iterator_high = origin_iterator_high + required_space[i];
                        iterator_low = origin_iterator_high + 1;
                        origin_iterator_high = iterator_high;
                    } else {
                        iterator_high = origin_iterator_high + required_space[i];
                        iterator_low = origin_iterator_high + 1;
                        origin_iterator_high = iterator_high;
                    }
                };
                iterator_low--;
            }
        }
    }
    
    for (size_t i = 0; i < sources; i++) {
        if (swaps[i]) {
            for (size_t j = 0; j < (ends[i] - starts[i] + 1); j++) {
                memcpy(
                    li->data + (starts[i] + j) * size,
                    (unsigned char *)input[i] + (ends[i] - starts[i] - j) * size,
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
}


void align_s(array *dest, unsigned char _0xfill) {
    if (!dest) return;

    unsigned short size = dest->size;
    unsigned char compare[size];
    memset(compare, _0xfill, size);

    size_t valid_indx = 0;
    for (size_t i = 0; i < dest->length; i++) {
        if (memcmp(dest->data + (i * size), compare, size) != 0) {
            if (i != valid_indx) {
                memmove(dest->data + (valid_indx * size), dest->data + (i * size), size);
                memset(dest->data + (i * size), _0xfill, size);
            }
            valid_indx++;
        }
    }
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
    unsigned short size = li->size;
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

    align_s(li, _0xfill);

    if (shrink) {
        unsigned char *new_array = (unsigned char *) realloc(li->data, new_length * size);
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

    unsigned short size = li->size;
    new_array->data = (unsigned char *) malloc(copies * size);
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

void align(array *dest) {
    align_s(dest, dest->config.default_cell_value);
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
    write_beta (
        dest, st, en, 
        dest->config.pre_allocation_factor, 
        dest->config.default_cell_value, 
        dest->config.write_preference_mode, 
        1, srcs
    );
    free(range);
}

// extended method: erase_s
void erase(array *dest, ssize_t *range) {
    ssize_t st[1] = {range[0]};
    ssize_t en[1] = {range[1]};
    erase_s (
        dest, st, en, 
        dest->config.erase_preference_mode, 
        dest->config.default_cell_value, 
        1
    );
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
    return search_s(dest, st, en, 1, 1, dest->config.search_return_as, (void *) srcs);
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
*/

#endif
