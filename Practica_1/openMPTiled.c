#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <omp.h>

#define NUM 1024
#define MIN(a,b) (((a)<(b))?(a):(b))
#define NUM_THREADS 2
#define tile 32

struct timeval tv; 
double get_clock() {
   struct timeval tv; int ok;
   ok = gettimeofday(&tv, (void *) 0);
   return (tv.tv_sec * 1.0 + tv.tv_usec * 1.0E-6); 
}

void check_mul (double *A, double *B, double *C, int n) {
    int i, j, k, sum;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
           sum = 0;
           for (k = 0; k < n; k++) {
               sum += A[i * n + k] * B[k * n + j];
           }
           if (sum != C[i * n + j]) {
               printf("Error multiplying matrices\n");
               exit(1);
           }
        }
    }
    printf("Multiplication ok\n");
}

int main(int argc, char *argv[]) {

	int i,j,k,ii,jj,kk,sum;
	double start,end;
	int numthreads,tid;

    double *A = (double *)malloc(NUM * NUM * sizeof(double));
    double *B = (double *)malloc(NUM * NUM * sizeof(double));
    double *C = (double *)malloc(NUM * NUM * sizeof(double));

    // Matrix A and B have random values
    for (int i = 0; i < NUM; i++) {
        for (int j = 0; j < NUM; j++) {
            *(C + i * NUM + j) = 0;
            *(A + i * NUM + j) = (float)rand()/(float)(RAND_MAX);
            *(B + i * NUM + j) = (float)rand()/(float)(RAND_MAX);
        }
    }

    omp_set_num_threads(NUM_THREADS);

    #pragma omp parallel
    {
        if(omp_get_thread_num()==0)
                printf("Running with %d threads\n",omp_get_num_threads());
    }

	start = get_clock();
	
	#pragma omp parallel for private(jj,kk,i,j,k,sum) 
	for (ii = 0; ii<NUM; ii+=tile) {
        for (jj = 0; jj<NUM; jj+=tile) {
	        for(kk = 0; kk<NUM; kk+=tile) {
                for (i = ii; i < MIN(ii+tile,NUM); i++) {
                    int tmp = i * NUM;
                    for (j = jj; j < MIN(jj+tile,NUM); j++) {
		                sum = 0;
                        for (k = kk; k < MIN(kk+tile,NUM); k++) {
                            sum += (*(A + tmp + k)) * (*(B + k * NUM + j));
                        }
		                *(C + tmp + j)+= sum;
	                }
                }
            }
        }
    }
	
	end = get_clock();
	printf("Time: %lf\n",(end-start));

    check_mul(A,B,C,NUM);

    free(A);
    free(B);
    free(C);
	return 0;
}