#include "disjoint_set_forest.h"

/* ========================================================================= *
 * DISJOINT SET IMPLEMENTATION                                               *
 * ========================================================================= */

DisjointSetForest* disjoint_set_forest_init(size_t size) 
{
    DisjointSetForest* ds = (DisjointSetForest*)malloc(sizeof(DisjointSetForest));
    if (!ds) return NULL;
    
    ds->parent = (size_t*)malloc(size * sizeof(size_t));
    ds->rank = (size_t*)malloc(size * sizeof(size_t));
    
    if (!ds->parent || !ds->rank) 
    {
        if (ds->parent) free(ds->parent);
        if (ds->rank) free(ds->rank);
        free(ds);
        return NULL;
    }
    
    ds->size = size;
    
    /* Initially, every element is its own parent (its own independent set) */
    for (size_t i = 0; i < size; i++) 
    {
        ds->parent[i] = i;
        ds->rank[i] = 0;
    }
    
    return ds;
}

void disjoint_set_forest_free(DisjointSetForest* ds) 
{
    if (ds) 
    {
        if (ds->parent) free(ds->parent);
        if (ds->rank) free(ds->rank);
        free(ds);
    }
}

size_t disjoint_set_forest_find(DisjointSetForest* ds, size_t i) 
{
    /* If i is not its own parent, it is not the root of the tree */
    if (ds->parent[i] != i) 
    {
        /* PATH COMPRESSION: recursively find the root and attach i directly to it */
        ds->parent[i] = disjoint_set_forest_find(ds, ds->parent[i]);
    }
    
    return ds->parent[i];
}

int disjoint_set_forest_union(DisjointSetForest* ds, size_t i, size_t j) 
{
    size_t root_i = disjoint_set_forest_find(ds, i);
    size_t root_j = disjoint_set_forest_find(ds, j);
    
    /* If the roots are the same, they are already part of the same connected component */
    if (root_i == root_j) 
    {
        return 0; 
    }
    
    /* UNION BY RANK: Attach the shorter tree under the root of the taller tree */
    if (ds->rank[root_i] < ds->rank[root_j]) 
    {
        ds->parent[root_i] = root_j;
    } 
    else if (ds->rank[root_i] > ds->rank[root_j]) 
    {
        ds->parent[root_j] = root_i;
    } 
    else 
    {
        /* If ranks are equal, pick one as the new root and increment its rank */
        ds->parent[root_j] = root_i;
        ds->rank[root_i]++;
    }
    
    return 1;
}