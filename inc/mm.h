#ifndef _MM_H_
#define _MM_H_

#define MM_MAXLINE 1000

struct mmdata {

	long int N, M, NNZ;
	int *x;
	int *y;
	double *v;

	int symmetricity;
	int binary;
	int ndiagonal;
	long int realnnz;
};

struct point 
{

	int x, y;
};

struct csrdata {
	long int xadj_size;
    int *xadj;
    int *adj;
    double *values;  // NULL if binary
};
int initialize_mm_omp(char *file, struct mmdata *mm);
int initialize_mm(char *file, struct mmdata *mm);
void convert_to_csr(struct mmdata *mm, struct csrdata *csr);
void printmm(struct mmdata *mm, char *file);
void freemm(struct mmdata *mm);

#endif 
