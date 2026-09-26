#ifndef _BITSET_H_
#define _BITSET_H_

#include <stddef.h>
#include <stdint.h>

#define BITSET_SUCCESS 0
#define BITSET_ALLOC_ERR 1
#define BITSET_INPUT_ERR 2
#define BITSET_BOUNDS_ERR 3

struct bitset {
    size_t univ_size;
    size_t arr_size;
    uint64_t *arr;
};

int init_bitset(struct bitset **set, size_t univ_size);

int remove_element(struct bitset *set, size_t element_idx);

int add_element(struct bitset *set, size_t element_idx);

int is_element_in(struct bitset *set, size_t element_idx, int *is_in_return);

int clear_all(struct bitset *set);

int get_population(struct bitset *set, size_t *population_return);

int bitset_intersect(struct bitset *dest, struct bitset *src1, struct bitset *src2);

int bitset_union(struct bitset *dest, struct bitset *src1, struct bitset *src2);

int bitset_difference(struct bitset *dest, struct bitset *src1, struct bitset *src2);

int free_bitset(struct bitset **set);

#endif
