#ifndef DISJOINT_SET_FOREST_H
#define DISJOINT_SET_FOREST_H

#include <stddef.h>
#include <stdlib.h>

/* ========================================================================= *
 * DISJOINT SET FOREST DATA STRUCTURE                                        *
 * ========================================================================= */

typedef struct 
{
    size_t* parent;
    size_t* rank;
    size_t size;
} DisjointSetForest;

/* ========================================================================= *
 * EXPLICITLY DECLARED OPERATIONS                                            *
 * ========================================================================= */

DisjointSetForest* disjoint_set_forest_init(size_t size);
void disjoint_set_forest_free(DisjointSetForest* ds);

/* Returns the root representative of element i (with path compression) */
size_t disjoint_set_forest_find(DisjointSetForest* ds, size_t i);

/* * Connects the sets containing i and j (union by rank). 
 * Returns 1 if a merge happened, 0 if they were already in the same set. 
 */
int disjoint_set_forest_union(DisjointSetForest* ds, size_t i, size_t j);

#endif /* DISJOINT_SET_H */