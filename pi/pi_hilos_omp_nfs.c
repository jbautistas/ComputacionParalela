#include <omp.h>
#include <math.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

 static long num_steps = 2e09;
 double step;
 #define NUM_THREADS 8
 #define PAD 8
 void main ()
 {
  int i, nthreads; double pi, sum[NUM_THREADS][PAD];
  struct timeval tval_before, tval_after, tval_result;
  gettimeofday(&tval_before, NULL);

 step = 1.0/(double) num_steps;
 omp_set_num_threads(NUM_THREADS);
 #pragma omp parallel
 {
 int i, id,nthrds;
 double x;
 id = omp_get_thread_num();
 nthrds = omp_get_num_threads();
 if (id == 0) nthreads = nthrds;
 for (i=id, sum[id][0]=0.0;i< num_steps; i=i+nthrds) {
 x = (i+0.5) * step;
 sum[id][0] += 4.0/(1.0+x*x);
 }
 }
 for(i=0, pi=0.0;i<nthreads;i++)pi += sum[i][0] * step;

 printf("pi: %.16lf\n", pi);
  gettimeofday(&tval_after, NULL);
  timersub(&tval_after, &tval_before, &tval_result);
  printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
 }