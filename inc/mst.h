#ifndef MST_H
#define MST_H

#include "mm.h"
#include "sort.h"
#include "heap.h"
#include "disjoint_set_forest.h"

/* ========================================================================= *
 * MINIMUM SPANNING TREE (MST) ALGORITHMS                                    *
 * ========================================================================= */

/* * All functions take a populated CSR graph structure and return the 
 * total weight of the Minimum Spanning Tree. 
 * Returns -1.0 if the graph is disconnected or invalid.
 */

double mst_kruskal(struct csrdata* csr, const char* mtxname);
double mst_prim(struct csrdata* csr, const char* mtxname);
double mst_sollin(struct csrdata* csr, const char* mtxname);

/* NEW: Parallel Shared-Memory Version */
double mst_sollin_omp(struct csrdata* csr, int num_threads, const char* mtxname);

#endif /* MST_H */