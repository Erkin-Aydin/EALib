#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "mm.h"
#include "mst.h"

/* Helper to calculate elapsed time in milliseconds using POSIX high-res clock */
double get_time_in_ms(struct timespec start, struct timespec end) 
{
    return (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

int main(int argc, char *argv[]) 
{
    if (argc < 2) 
    {
        printf("Usage: %s <matrix_market_file.mtx>\n", argv[0]);
        return 1;
    }

    struct mmdata mm;
    struct csrdata csr;

    printf("Loading Matrix Market file: %s...\n", argv[1]);
    
    /* Parse the .mtx coordinate list */
    if (initialize_mm(argv[1], &mm) != 0) 
    {
        printf("ERROR: Failed to load matrix market file.\n");
        return 1;
    }

    printf("Graph Loaded: %d Vertices, %d Non-Zero Edges (Symmetric: %s)\n", 
            mm.N, mm.NNZ, mm.symmetricity ? "Yes" : "No");

    /* Convert standard coordinates to cache-friendly CSR format */
    convert_to_csr(&mm, &csr);
    printf("Converted to CSR format. Total Directed Edges: %d\n\n", mm.realnnz);

    /* The mmdata is no longer needed after CSR conversion, free it */
    freemm(&mm);

    const int WARMUP_RUNS = 3;
    const int MEASURE_RUNS = 7;
    const int TOTAL_RUNS = WARMUP_RUNS + MEASURE_RUNS;

    double w_kruskal = 0.0, w_prim = 0.0, w_sollin = 0.0;
    double time_kruskal = 0.0, time_prim = 0.0, time_sollin = 0.0;
    struct timespec start, end;

    printf("Benchmarking Kruskal's Algorithm...\n");
    for (int i = 0; i < TOTAL_RUNS; i++) 
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        w_kruskal = mst_kruskal(&csr, "");
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        if (i >= WARMUP_RUNS) 
        {
            time_kruskal += get_time_in_ms(start, end);
        }
    }
    time_kruskal /= MEASURE_RUNS;

    printf("Benchmarking Prim's Algorithm...\n");
    for (int i = 0; i < TOTAL_RUNS; i++) 
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        w_prim = mst_prim(&csr, "");
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        if (i >= WARMUP_RUNS) 
        {
            time_prim += get_time_in_ms(start, end);
        }
    }
    time_prim /= MEASURE_RUNS;

    printf("Benchmarking Sollin's (Boruvka's) Algorithm...\n");
    for (int i = 0; i < TOTAL_RUNS; i++) 
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        w_sollin = mst_sollin(&csr, "");
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        if (i >= WARMUP_RUNS) 
        {
            time_sollin += get_time_in_ms(start, end);
        }
    }
    time_sollin /= MEASURE_RUNS;

    double w_sollin_omp = 0.0, time_sollin_omp = 0.0;
    
    printf("Benchmarking Sollin's (OpenMP) Algorithm...\n");
    for (int i = 0; i < TOTAL_RUNS; i++) 
    {
        clock_gettime(CLOCK_MONOTONIC, &start);
        w_sollin_omp = mst_sollin_omp(&csr, 8, "");
        clock_gettime(CLOCK_MONOTONIC, &end);
        
        if (i >= WARMUP_RUNS) time_sollin_omp += get_time_in_ms(start, end);
    }
    time_sollin_omp /= MEASURE_RUNS;

    /* Print Formatting */
    printf("\n=================================================================\n");
    printf(" MST ALGORITHM BENCHMARK (Average of %d runs)\n", MEASURE_RUNS);
    printf("=================================================================\n");
    printf(" %-15s | %-18s | %-15s \n", "Algorithm", "MST Total Weight", "Time (ms)");
    printf("-----------------------------------------------------------------\n");
    
    if (w_kruskal == -1.0) printf(" %-15s | %-18s | %-15.3f \n", "Kruskal", "Disconnected/Error", time_kruskal);
    else printf(" %-15s | %-18.2f | %-15.3f \n", "Kruskal", w_kruskal, time_kruskal);
    
    if (w_prim == -1.0) printf(" %-15s | %-18s | %-15.3f \n", "Prim", "Disconnected/Error", time_prim);
    else printf(" %-15s | %-18.2f | %-15.3f \n", "Prim", w_prim, time_prim);
    
    if (w_sollin == -1.0) printf(" %-15s | %-18s | %-15.3f \n", "Sollin", "Disconnected/Error", time_sollin);
    else printf(" %-15s | %-18.2f | %-15.3f \n", "Sollin", w_sollin, time_sollin);

    if (w_sollin_omp == -1.0) printf(" %-15s | %-18s | %-15.3f \n", "Sollin (OpenMP)", "Error", time_sollin_omp);
    else printf(" %-15s | %-18.2f | %-15.3f \n", "Sollin (OpenMP)", w_sollin_omp, time_sollin_omp);
    printf("=================================================================\n");

    /* Free the dynamically allocated CSR arrays created by convert_to_csr */
    if (csr.row_ptr) free(csr.row_ptr);
    if (csr.col_idx) free(csr.col_idx);
    if (csr.values) free(csr.values);

    return 0;
}