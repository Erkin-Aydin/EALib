#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <libgen.h>

#include "mm.h"
#include "mst.h"

double get_time_in_ms(struct timespec start, struct timespec end) {
    return (end.tv_sec - start.tv_sec) * 1000.0 + (end.tv_nsec - start.tv_nsec) / 1000000.0;
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <matrix_market_file.mtx>\n", argv[0]);
        return 1;
    }

    /* Extract clean filename (e.g. "bus.mtx" from "data/bus.mtx") */
    char *path_copy = strdup(argv[1]);
    char *mtx_filename = basename(path_copy);

    struct mmdata mm;
    struct csrdata csr;

    if (initialize_mm(argv[1], &mm) != 0) {
        printf("ERROR: Failed to load matrix market file.\n");
        free(path_copy);
        return 1;
    }

    convert_to_csr(&mm, &csr);
    freemm(&mm);

    const int WARMUP_RUNS = 1; 
    const int MEASURE_RUNS = 3;
    const int TOTAL_RUNS = WARMUP_RUNS + MEASURE_RUNS;

    double w_kruskal, w_prim, w_sollin, w_sollin_omp;
    double time_kruskal = 0, time_prim = 0, time_sollin = 0, time_sollin_omp = 0;
    struct timespec start, end;

    // Kruskal
    for (int i = 0; i < TOTAL_RUNS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        w_kruskal = mst_kruskal(&csr, mtx_filename);
        clock_gettime(CLOCK_MONOTONIC, &end);
        if (i >= WARMUP_RUNS) time_kruskal += get_time_in_ms(start, end);
    }

    // Prim
    for (int i = 0; i < TOTAL_RUNS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        w_prim = mst_prim(&csr, mtx_filename);
        clock_gettime(CLOCK_MONOTONIC, &end);
        if (i >= WARMUP_RUNS) time_prim += get_time_in_ms(start, end);
    }

    // Sollin Seq
    for (int i = 0; i < TOTAL_RUNS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        w_sollin = mst_sollin(&csr, mtx_filename);
        clock_gettime(CLOCK_MONOTONIC, &end);
        if (i >= WARMUP_RUNS) time_sollin += get_time_in_ms(start, end);
    }

    // Sollin OMP
    for (int i = 0; i < TOTAL_RUNS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &start);
        w_sollin_omp = mst_sollin_omp(&csr, 8, mtx_filename);
        clock_gettime(CLOCK_MONOTONIC, &end);
        if (i >= WARMUP_RUNS) time_sollin_omp += get_time_in_ms(start, end);
    }

    /* Summary Print */
    printf("\nFinished. Output files generated for %s\n", mtx_filename);
    printf("Kruskal: %.2f ms\nPrim: %.2f ms\nSollin: %.2f ms\nSollin OMP: %.2f ms\n", 
           time_kruskal/MEASURE_RUNS, time_prim/MEASURE_RUNS, 
           time_sollin/MEASURE_RUNS, time_sollin_omp/MEASURE_RUNS);

    if (csr.xadj) free(csr.xadj);
    if (csr.adj) free(csr.adj);
    if (csr.values) free(csr.values);
    free(path_copy);

    return 0;
}
