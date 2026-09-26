#include "mst.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <omp.h>

typedef struct {
    int u;
    int v;
    double weight;
} Edge;

/* Helper to write MST to file: <algoname>_<mtxname>_mst.txt */
void write_mst_to_file(const char* algoname, const char* mtxname, double total_weight, Edge* mst_edges, int count) {
    char filename[512];
    snprintf(filename, sizeof(filename), "%s_%s_mst.txt", algoname, mtxname);
    FILE* f = fopen(filename, "w");
    if (!f) {
        perror("Failed to open output file");
        return;
    }

    fprintf(f, "%.6f\n", total_weight);
    for (int i = 0; i < count; i++) {
        if (mst_edges[i].u < mst_edges[i].v)
            fprintf(f, "%d, %d\n", mst_edges[i].u, mst_edges[i].v);
        else
            fprintf(f, "%d, %d\n", mst_edges[i].v, mst_edges[i].u);
    }
    fclose(f);
}

double mst_kruskal(struct csrdata* csr, const char* mtxname) {
    if (!csr || csr->xadj_size <= 0 || !csr->values) return -1.0;
    int V = csr->xadj_size;
    int max_edges = csr->xadj[V];
    
    Edge* edges = (Edge*)malloc(max_edges * sizeof(Edge));
    struct RecF64* sort_recs = (struct RecF64*)malloc(max_edges * sizeof(struct RecF64));
    Edge* final_mst = (Edge*)malloc((V - 1) * sizeof(Edge));

    int E = 0;
    for (int u = 0; u < V; u++) {
        for (int j = csr->xadj[u]; j < csr->xadj[u+1]; j++) {
            int v = csr->adj[j];
            if (u < v) {
                edges[E].u = u;
                edges[E].v = v;
                edges[E].weight = csr->values[j];
                sort_recs[E].key = edges[E].weight;
                sort_recs[E].data = (void*)&edges[E];
                E++;
            }
        }
    }

    universal_sort_recf64(sort_recs, E);
    DisjointSetForest* dsf = disjoint_set_forest_init((size_t)V);
    
    double mst_weight = 0.0;
    int edges_added = 0;

    for (int i = 0; i < E; i++) {
        Edge* current_edge = (Edge*)sort_recs[i].data;
        if (disjoint_set_forest_union(dsf, (size_t)current_edge->u, (size_t)current_edge->v)) {
            final_mst[edges_added++] = *current_edge;
            mst_weight += current_edge->weight;
            if (edges_added == V - 1) break;
        }
    }

    if (edges_added == V - 1) 
        write_mst_to_file("kruskal", mtxname, mst_weight, final_mst, edges_added);

    disjoint_set_forest_free(dsf);
    free(edges); free(sort_recs); free(final_mst);
    return (edges_added == V - 1) ? mst_weight : -1.0;
}

double mst_prim(struct csrdata* csr, const char* mtxname) {
    if (!csr || csr->xadj_size <= 0 || !csr->values) return -1.0;
    int V = csr->xadj_size;
    int* in_mst = (int*)calloc(V, sizeof(int));
    Edge* final_mst = (Edge*)malloc((V - 1) * sizeof(Edge));
    HeapRecF64* min_heap = heap_recf64_init((size_t)csr->xadj[V], 1);

    double mst_weight = 0.0;
    int edges_added = 0;
    in_mst[0] = 1;
    
    for (int j = csr->xadj[0]; j < csr->xadj[1]; j++) {
        struct RecF64 item;
        item.key = csr->values[j];
        // Encoding source and dest in data pointer for Prim
        uint64_t packed = ((uint64_t)0 << 32) | (uint32_t)csr->adj[j];
        item.data = (void*)(uintptr_t)packed;
        heap_recf64_insert(min_heap, item);
    }

    struct RecF64 min_edge;
    while (heap_recf64_extract(min_heap, &min_edge) && edges_added < V - 1) {
        uint64_t packed = (uint64_t)(uintptr_t)min_edge.data;
        int u_src = (int)(packed >> 32);
        int v_dst = (int)(packed & 0xFFFFFFFF);

        if (in_mst[v_dst]) continue;

        in_mst[v_dst] = 1;
        final_mst[edges_added].u = u_src;
        final_mst[edges_added].v = v_dst;
        final_mst[edges_added].weight = min_edge.key;
        mst_weight += min_edge.key;
        edges_added++;

        for (int j = csr->xadj[v_dst]; j < csr->xadj[v_dst+1]; j++) {
            int next_v = csr->adj[j];
            if (!in_mst[next_v]) {
                struct RecF64 next_e;
                next_e.key = csr->values[j];
                uint64_t next_packed = ((uint64_t)v_dst << 32) | (uint32_t)next_v;
                next_e.data = (void*)(uintptr_t)next_packed;
                heap_recf64_insert(min_heap, next_e);
            }
        }
    }

    if (edges_added == V - 1)
        write_mst_to_file("prim", mtxname, mst_weight, final_mst, edges_added);

    heap_recf64_free(min_heap);
    free(in_mst); free(final_mst);
    return (edges_added == V - 1) ? mst_weight : -1.0;
}

double mst_sollin(struct csrdata* csr, const char* mtxname) {
    if (!csr || csr->xadj_size <= 0 || !csr->values) return -1.0;
    int V = csr->xadj_size;
    DisjointSetForest* dsf = disjoint_set_forest_init((size_t)V);
    Edge* cheapest = (Edge*)malloc(V * sizeof(Edge));
    Edge* final_mst = (Edge*)malloc((V - 1) * sizeof(Edge));

    double mst_weight = 0.0;
    int num_components = V;
    int total_edges_added = 0;

    while (num_components > 1) {
        int merges_in_phase = 0;
        for (int i = 0; i < V; i++) cheapest[i].weight = -1.0;

        for (int u = 0; u < V; u++) {
            size_t set_u = dsf->parent[u];
            while (set_u != dsf->parent[set_u]) set_u = dsf->parent[set_u];

            for (int j = csr->xadj[u]; j < csr->xadj[u+1]; j++) {
                int v = csr->adj[j];
                size_t set_v = dsf->parent[v];
                while (set_v != dsf->parent[set_v]) set_v = dsf->parent[set_v];

                if (set_u == set_v) continue;

                if (cheapest[set_u].weight == -1.0 || csr->values[j] < cheapest[set_u].weight) {
                    cheapest[set_u].u = u;
                    cheapest[set_u].v = v;
                    cheapest[set_u].weight = csr->values[j];
                }
            }
        }

        for (int i = 0; i < V; i++) {
            if (cheapest[i].weight != -1.0) {
                if (disjoint_set_forest_union(dsf, (size_t)cheapest[i].u, (size_t)cheapest[i].v)) {
                    final_mst[total_edges_added++] = cheapest[i];
                    mst_weight += cheapest[i].weight;
                    num_components--;
                    merges_in_phase++;
                }
            }
        }
        if (merges_in_phase == 0) break;
        for (int i = 0; i < V; i++) {
            size_t root = i;
            while (root != dsf->parent[root]) root = dsf->parent[root];
            dsf->parent[i] = root; 
        }
    }

    if (total_edges_added == V - 1)
        write_mst_to_file("sollin", mtxname, mst_weight, final_mst, total_edges_added);

    free(cheapest); free(final_mst);
    disjoint_set_forest_free(dsf);
    return (num_components == 1) ? mst_weight : -1.0;
}

double mst_sollin_omp(struct csrdata* csr, int num_threads, const char* mtxname) {
    if (!csr || csr->xadj_size <= 0 || !csr->values) return -1.0;
    if (num_threads > 0) omp_set_num_threads(num_threads);

    int V = csr->xadj_size;
    DisjointSetForest* dsf = disjoint_set_forest_init((size_t)V);
    Edge* cheapest = (Edge*)malloc(V * sizeof(Edge));
    Edge* final_mst = (Edge*)malloc((V - 1) * sizeof(Edge));
    omp_lock_t* locks = (omp_lock_t*)malloc(V * sizeof(omp_lock_t));
    for (int i = 0; i < V; i++) omp_init_lock(&locks[i]);

    double mst_weight = 0.0;
    int num_components = V;
    int total_edges_added = 0;

    while (num_components > 1) {
        int merges_in_phase = 0;
        #pragma omp parallel for
        for (int i = 0; i < V; i++) cheapest[i].weight = -1.0;

        #pragma omp parallel for schedule(guided)
        for (int u = 0; u < V; u++) {
            size_t set_u = dsf->parent[u];
            while (set_u != dsf->parent[set_u]) set_u = dsf->parent[set_u];

            for (int j = csr->xadj[u]; j < csr->xadj[u+1]; j++) {
                int v = csr->adj[j];
                double weight = csr->values[j];
                size_t set_v = dsf->parent[v];
                while (set_v != dsf->parent[set_v]) set_v = dsf->parent[set_v];

                if (set_u == set_v) continue;

                if (cheapest[set_u].weight == -1.0 || weight < cheapest[set_u].weight) {
                    omp_set_lock(&locks[set_u]);
                    if (cheapest[set_u].weight == -1.0 || weight < cheapest[set_u].weight) {
                        cheapest[set_u].u = u;
                        cheapest[set_u].v = v;
                        cheapest[set_u].weight = weight;
                    }
                    omp_unset_lock(&locks[set_u]);
                }
            }
        }

        for (int i = 0; i < V; i++) {
            if (cheapest[i].weight != -1.0) {
                if (disjoint_set_forest_union(dsf, (size_t)cheapest[i].u, (size_t)cheapest[i].v)) {
                    final_mst[total_edges_added++] = cheapest[i];
                    mst_weight += cheapest[i].weight;
                    num_components--;
                    merges_in_phase++;
                }
            }
        }
        if (merges_in_phase == 0) break;
        #pragma omp parallel for
        for (int i = 0; i < V; i++) {
            size_t root = i;
            while (root != dsf->parent[root]) root = dsf->parent[root];
            dsf->parent[i] = root; 
        }
    }

    if (total_edges_added == V - 1)
        write_mst_to_file("sollin_omp", mtxname, mst_weight, final_mst, total_edges_added);

    for (int i = 0; i < V; i++) omp_destroy_lock(&locks[i]);
    free(locks); free(cheapest); free(final_mst);
    disjoint_set_forest_free(dsf);
    return (num_components == 1) ? mst_weight : -1.0;
}
