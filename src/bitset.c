#include "bitset.h"
#include <sys/mman.h>
#include <stddef.h>
#include <stdint.h>
//#include <math.h>
#include <stdlib.h>
#include <string.h>

int init_bitset(struct bitset **set, size_t univ_size)
{
    if(!set)
    {
        return BITSET_INPUT_ERR;
    }
    (*set) = (struct bitset *)malloc((size_t)sizeof(struct bitset));
    if(!(*set))
    {
        return BITSET_ALLOC_ERR;
    }
    (*set)->univ_size = univ_size;
    (*set)->arr_size = (univ_size + 63) >> 6; 

    size_t byte_size = (*set)->arr_size * sizeof(uint64_t);
    // This is done to actually allocate the memory in the RAM and prevent page faults. Please do NOT replace this with calloc or malloc
    (*set)->arr = mmap(NULL, byte_size, PROT_READ | PROT_WRITE, 
            MAP_PRIVATE | MAP_ANONYMOUS | MAP_POPULATE, -1, 0);
    if ((*set)->arr == MAP_FAILED)
    {
        free(*set);
        *set = NULL;
        return BITSET_ALLOC_ERR;
    }

    /*    
          (*set)->arr = (uint64_t *) calloc( (*set)->arr_size, sizeof(uint64_t));

          if(!((*set)->arr))
          {
          free(*set);
     *set = NULL;
     return BITSET_ALLOC_ERR;
     }
     */
    return BITSET_SUCCESS;
}

int remove_element(struct bitset *set, size_t element_idx)
{
    if (!set || !set->arr)
    {
        return BITSET_INPUT_ERR;
    }

    if (element_idx >= set->univ_size)
    {
        return BITSET_BOUNDS_ERR; 
    }

    set->arr[element_idx >> 6] &= ~(1ULL << (element_idx & 63));

    return BITSET_SUCCESS;
}

int add_element(struct bitset *set, size_t element_idx)
{
    if (!set || !set->arr)
    {
        return BITSET_INPUT_ERR;
    }

    if (element_idx >= set->univ_size)
    {
        return BITSET_BOUNDS_ERR; 
    }

    set->arr[element_idx >> 6] |= (1ULL << (element_idx & 63));

    return BITSET_SUCCESS;
}

int is_element_in(struct bitset *set, size_t element_idx, int *is_in_return)
{
    if (!set || !set->arr || !is_in_return)
    {
        return BITSET_INPUT_ERR;
    }

    if (element_idx >= set->univ_size)
    {
        return BITSET_BOUNDS_ERR; 
    }
    *is_in_return = (set->arr[element_idx >> 6] & (1ULL << (element_idx & 63))) != 0;
    return BITSET_SUCCESS;
}

int clear_all(struct bitset *set)
{
    if (!set || !set->arr)
    {
        return BITSET_INPUT_ERR;
    }
    memset(set->arr, 0, set->arr_size * sizeof(uint64_t));
    return BITSET_SUCCESS;
}

int get_population(struct bitset *set, size_t *population_return)
{
    if (!set || !set->arr || !population_return)
    {
        return BITSET_INPUT_ERR;
    }

    size_t count = 0;

#pragma GCC unroll 8 
    for (size_t i = 0; i < set->arr_size; ++i)
    {
        count += (size_t)__builtin_popcountll(set->arr[i]);
    }

    *population_return = count;

    return BITSET_SUCCESS;
}

int bitset_intersect(struct bitset *dest, struct bitset *src1, struct bitset *src2)
{
    if (!dest || !dest->arr || !src1 || !src1->arr || !src2 || !src2->arr)
        return BITSET_INPUT_ERR;

    if (dest->univ_size != src1->univ_size || src1->univ_size != src2->univ_size)
        return BITSET_BOUNDS_ERR;

#pragma GCC unroll 8
    for (size_t i = 0; i < dest->arr_size; ++i)
    {
        dest->arr[i] = src1->arr[i] & src2->arr[i];
    }

    return BITSET_SUCCESS;
}

int bitset_union(struct bitset *dest, struct bitset *src1, struct bitset *src2)
{
    if (!dest || !dest->arr || !src1 || !src1->arr || !src2 || !src2->arr)
        return BITSET_INPUT_ERR;

    if (dest->univ_size != src1->univ_size || src1->univ_size != src2->univ_size)
        return BITSET_BOUNDS_ERR;

#pragma GCC unroll 8
    for (size_t i = 0; i < dest->arr_size; ++i)
    {
        dest->arr[i] = src1->arr[i] | src2->arr[i];
    }

    return BITSET_SUCCESS;
}

int bitset_difference(struct bitset *dest, struct bitset *src1, struct bitset *src2)
{
    if (!dest || !dest->arr || !src1 || !src1->arr || !src2 || !src2->arr)
        return BITSET_INPUT_ERR;

    if (dest->univ_size != src1->univ_size || src1->univ_size != src2->univ_size)
        return BITSET_BOUNDS_ERR;

#pragma GCC unroll 8
    for (size_t i = 0; i < dest->arr_size; ++i)
    {
        dest->arr[i] = src1->arr[i] & ~(src2->arr[i]);
    }

    return BITSET_SUCCESS;
}

int free_bitset(struct bitset **set)
{
    if(!(set) || !(*set))
    {
        return BITSET_INPUT_ERR;
    }
    if(!((*set)->arr))
    {
        free((*set)->arr);
    }
    free(*set);
    return BITSET_SUCCESS;
}

