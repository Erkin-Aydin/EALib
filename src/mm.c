#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../inc/mm.h"
#include <sys/mman.h>
#include <sys/stat.h>
#include <omp.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#define STRINGSIZE 1000

char * dummy;

int initialize_mm_omp(char *file, struct mmdata *mm) {
    char filename[STRINGSIZE];
    sprintf(filename, "%s", file);
    printf("%s\n", file);

    int fd = open(filename, O_RDONLY);
    if (fd == -1) {
        perror("Failed to open file");
        return -1; 
    }

    struct stat sb;
    if (fstat(fd, &sb) == -1) {
        perror("Failed to get file size");
        close(fd);
        return -1;
    }

    const char *file_data = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);
    if (file_data == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return -1;
    }

    const char *p = file_data;
    const char *file_end = file_data + sb.st_size;

    mm->symmetricity = 0;

    char first_line[MM_MAXLINE];
    int iter = 0;
    while(p < file_end && *p != '\n' && iter < MM_MAXLINE - 1) {
        first_line[iter++] = *p++;
    }
    first_line[iter] = '\0';
    if(p < file_end && *p == '\n') p++;

    if(first_line[0] == '%') {
        if (strstr(first_line, "symmetric") != NULL) {
            mm->symmetricity = 1;
        }
        while (p < file_end && *p == '%') {
            const char *nl = memchr(p, '\n', file_end - p);
            p = nl ? nl + 1 : file_end;
        }
    }

    while (p < file_end && (*p == ' ' || *p == '\t')) p++;
    mm->N = 0; while (p < file_end && *p >= '0' && *p <= '9') { mm->N = mm->N * 10 + (*p - '0'); p++; }
    
    while (p < file_end && (*p == ' ' || *p == '\t')) p++;
    mm->M = 0; while (p < file_end && *p >= '0' && *p <= '9') { mm->M = mm->M * 10 + (*p - '0'); p++; }
    
    while (p < file_end && (*p == ' ' || *p == '\t')) p++;
    mm->NNZ = 0; while (p < file_end && *p >= '0' && *p <= '9') { mm->NNZ = mm->NNZ * 10 + (*p - '0'); p++; }

    const char *nl = memchr(p, '\n', file_end - p);
    p = nl ? nl + 1 : file_end;


    mm->x = (int *)malloc((size_t)mm->NNZ * sizeof(int));
    mm->y = (int *)malloc((size_t)mm->NNZ * sizeof(int));
    mm->v = NULL;

    const char *data_start = p;

    const char *peek = data_start;
    while (peek < file_end && (*peek == ' ' || *peek == '\t')) peek++;
    while (peek < file_end && *peek >= '0' && *peek <= '9') peek++; 
    while (peek < file_end && (*peek == ' ' || *peek == '\t')) peek++;
    while (peek < file_end && *peek >= '0' && *peek <= '9') peek++; 
    while (peek < file_end && (*peek == ' ' || *peek == '\t')) peek++;
    
    if (peek < file_end && *peek != '\n' && *peek != '\r') {
        mm->binary = 0;
        mm->v = (double *)malloc((size_t)mm->NNZ * sizeof(double));
    } else {
        mm->binary = 1;
    }

    int num_threads = omp_get_max_threads();
    long *thread_offsets = calloc(num_threads, sizeof(long));
    long *thread_counts = calloc(num_threads, sizeof(long));
    
    mm->ndiagonal = 0;
    int global_diag = 0;
    size_t data_len = file_end - data_start;

    int * restrict local_x = mm->x;
    int * restrict local_y = mm->y;
    double * restrict local_v = mm->v;
    int is_binary = mm->binary;
    int is_symmetric = mm->symmetricity;

    #pragma omp parallel num_threads(num_threads)
    {
        int tid = omp_get_thread_num();
        size_t chunk_size = data_len / num_threads;
        
        const char *my_start = data_start + tid * chunk_size;
        const char *my_end = (tid == num_threads - 1) ? file_end : my_start + chunk_size;

        if (tid > 0) {
            const char *next_nl = memchr(my_start, '\n', file_end - my_start);
            my_start = next_nl ? next_nl + 1 : file_end;
        }
        if (tid < num_threads - 1) {
            const char *next_nl = memchr(my_end, '\n', file_end - my_end);
            my_end = next_nl ? next_nl + 1 : file_end;
        }

        // --- Phase 1: Highly optimized line counting ---
        long my_nnz = 0;
        const char *ptr = my_start;
        while (ptr < my_end) {
            while (ptr < my_end && (*ptr == ' ' || *ptr == '\t')) ptr++;
            if (ptr < my_end && ((*ptr >= '0' && *ptr <= '9') || *ptr == '-')) {
                my_nnz++; 
            }
            const char *line_end = memchr(ptr, '\n', my_end - ptr);
            ptr = line_end ? line_end + 1 : my_end;
        }
        thread_counts[tid] = my_nnz;
        printf("aaa\n");
        #pragma omp barrier

        #pragma omp single
        {
            long current_offset = 0;
            for (int i = 0; i < num_threads; i++) {
                thread_offsets[i] = current_offset;
                current_offset += thread_counts[i];
            }
        }
        // --- Phase 2: Parallel Write to Memory ---
        long idx = thread_offsets[tid];
        ptr = my_start;
        int local_diag = 0;

        while (ptr < my_end) {
            while (ptr < my_end && (*ptr == ' ' || *ptr == '\t')) ptr++;
            if (ptr >= my_end || (!(*ptr >= '0' && *ptr <= '9') && *ptr != '-')) {
                const char *line_end = memchr(ptr, '\n', my_end - ptr);
                ptr = line_end ? line_end + 1 : my_end;
                continue;
            }

            int r = 0, c = 0;
            double v = 0.0;

            while (ptr < my_end && *ptr >= '0' && *ptr <= '9') { r = r * 10 + (*ptr - '0'); ptr++; }
            while (ptr < my_end && (*ptr == ' ' || *ptr == '\t')) ptr++;
            
            while (ptr < my_end && *ptr >= '0' && *ptr <= '9') { c = c * 10 + (*ptr - '0'); ptr++; }
            
            if (!is_binary) {
                while (ptr < my_end && (*ptr == ' ' || *ptr == '\t')) ptr++;
                
                // Inline fast float parser to bypass slow strtod locks
                if (ptr < my_end && *ptr != '\n' && *ptr != '\r') {
                    double sign = 1.0;
                    if (*ptr == '-') { sign = -1.0; ptr++; }
                    
                    while (ptr < my_end && *ptr >= '0' && *ptr <= '9') {
                        v = v * 10.0 + (*ptr - '0');
                        ptr++;
                    }
                    if (*ptr == '.') {
                        ptr++;
                        double frac = 1.0;
                        while (ptr < my_end && *ptr >= '0' && *ptr <= '9') {
                            frac *= 0.1;
                            v += (*ptr - '0') * frac;
                            ptr++;
                        }
                    }
                    // Fallback to strtod only if scientific notation is encountered
                    if (*ptr == 'e' || *ptr == 'E') {
                        v = strtod(ptr, (char**)&ptr);
                    } else {
                        v *= sign;
                    }
                }
            }

            local_x[idx] = r - 1;
            local_y[idx] = c - 1;
            if (!is_binary) {
                local_v[idx] = v;
            }
            
            if (is_symmetric && local_x[idx] == local_y[idx]) {
                local_diag++;
            }
            
            idx++;
            const char *line_end = memchr(ptr, '\n', my_end - ptr);
            ptr = line_end ? line_end + 1 : my_end;
        }
        #pragma omp atomic
        global_diag += local_diag;
    }

    mm->ndiagonal = global_diag;
    mm->realnnz = (!mm->symmetricity) ? mm->NNZ : 2 * mm->NNZ - mm->ndiagonal;
    madvise((void *)file_data, sb.st_size, MADV_DONTNEED);
    free(thread_offsets);
    free(thread_counts);
    munmap((void *)file_data, sb.st_size);
    close(fd);
    return 0;
}

int initialize_mm(char *file, struct mmdata *mm) {

	char filename[STRINGSIZE];
	char line[MM_MAXLINE];
	char *saveptr, *token;
	sprintf(filename, "%s", file);
	
	FILE *f = fopen(filename, "r");
	if (f == NULL) {
		perror("Failed to open file");
		return -1;  // Return an error code or handle it as needed
	}

	mm -> symmetricity = 0;
	
	dummy = fgets(line, MM_MAXLINE, f);
	if(line[0] == '%') {
	
		token = strtok_r(line, " \n", &saveptr);
		while(token != NULL) {
			if(!strcmp(token, "symmetric")) {
				mm -> symmetricity = 1;
				break;
			}
			token = strtok_r(NULL, " \n", &saveptr);
		}

		dummy = fgets(line, MM_MAXLINE, f);	
		while(line[0] == '%') {
			dummy = fgets(line, MM_MAXLINE, f);	
		}
	}
		
	sscanf(line, " %d %d %d", &mm->N, &mm->M, &mm->NNZ);
	
	mm -> ndiagonal = 0;
	mm -> x = (int *)malloc(mm->NNZ * sizeof(int));
	mm -> y = (int *)malloc(mm->NNZ * sizeof(int));
	mm -> v = NULL;	

	dummy = fgets(line, MM_MAXLINE, f);
	double v;
	if(2 == sscanf(line, " %d %d %lf", &mm -> x[0], &mm -> y[0], &v)) 
		mm -> binary = 1;
	else {
		mm->binary = 0;
		mm -> v = (double *)malloc(mm->NNZ * sizeof(double));	
		mm->v[0] = v;
	}
	mm -> x[0] --;
	mm -> y[0] --;
	if(mm->symmetricity && mm -> x[0] == mm -> y[0])
		mm -> ndiagonal ++;
	
	int i;
	for(i=1; i<mm->NNZ; i++) {

		dummy = fgets(line, MM_MAXLINE, f);
		if(! mm -> binary) 
			sscanf(line, " %d %d %lf", &mm -> x[i], &mm -> y[i], &mm -> v[i]);
		else	
			sscanf(line, " %d %d", &mm -> x[i], &mm -> y[i]);

		mm -> x[i] --;
		mm -> y[i] --;

		if(mm->symmetricity && mm -> x[i] == mm -> y[i])
			mm -> ndiagonal ++;
	}

	mm -> realnnz = (!mm -> symmetricity)? mm->NNZ: 2 * mm->NNZ - mm->ndiagonal;
	fclose(f);
	return 0;
}

void convert_to_csr(struct mmdata *mm, struct csrdata *csr) {
    int N = mm->N;
    int *row_counts = (int *)calloc(N + 1, sizeof(int));
    int total_nnz = mm->realnnz;

    // Allocate CSR arrays
    csr->xadj = (int *)malloc((N + 1) * sizeof(int));
    csr->adj = (int *)malloc(total_nnz * sizeof(int));
    csr->values = (mm->binary) ? NULL : (double *)malloc(total_nnz * sizeof(double));
	csr->xadj_size = mm->N;
    // Step 1: Count non-zeros per row
    for (int i = 0; i < mm->NNZ; i++) {
        row_counts[mm->x[i] + 1]++;
        if (mm->symmetricity && mm->x[i] != mm->y[i])  // Symmetric part
            row_counts[mm->y[i] + 1]++;
    }

    // Step 2: Prefix sum for xadj
    csr->xadj[0] = 0;
    for (int i = 1; i <= N; i++)
        csr->xadj[i] = csr->xadj[i - 1] + row_counts[i];

    // Reset row_counts to track current insertion index
    for (int i = 0; i <= N; i++) row_counts[i] = csr->xadj[i];

    // Step 3: Fill adj and values
    for (int i = 0; i < mm->NNZ; i++) {
        int row = mm->x[i];
        int idx = row_counts[row]++;
        csr->adj[idx] = mm->y[i];
        if (!mm->binary) csr->values[idx] = mm->v[i];

        // Add symmetric entry if needed
        if (mm->symmetricity && row != mm->y[i]) {
            int sym_row = mm->y[i];
            int sym_idx = row_counts[sym_row]++;
            csr->adj[sym_idx] = row;
            if (!mm->binary) csr->values[sym_idx] = mm->v[i];
        }
    }

    free(row_counts);
}


void printmm(struct mmdata *mm, char *filename) {

	FILE *f = fopen(filename, "w");
	fprintf(f, "%c%cMatrixMarket matrix coordinate %s symmetric\n", '%', '%', mm->binary? "pattern": "real");
	fprintf(f, "%d %d %d\n", mm->N, mm->M, mm->NNZ);
	int i;
	for(i=0; i<mm->NNZ; i++)
		fprintf(f, "%d %d\n", mm->x[i]+1, mm->y[i]+1);
	fclose(f);
}

void freemm(struct mmdata *mm) {

	if (mm->x) free(mm->x);
    if (mm->y) free(mm->y);
    if (mm->v) free(mm->v);
}
