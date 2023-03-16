#include <math.h>
#include <omp.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>


#define MAX_TERMS 2000000000

int main(int argc, char** argv) {
  if (argc != 2) {
    printf("usage: ./pi thread_count");
    return 0;
  }

  struct timeval tval_before, tval_after, tval_result;
  gettimeofday(&tval_before, NULL);

  int thread_count = atoi(argv[1]);
  double result = 0;
  #pragma omp parallel for num_threads(thread_count) reduction(+:result)
  for(int i = 0; i < MAX_TERMS; i++) {
    result += (i % 2 == 0 ? 1: -1) / (2.0 * i + 1);
  }

  printf("Result: %.16lf\n", (4.0 * result));
  gettimeofday(&tval_after, NULL);
  timersub(&tval_after, &tval_before, &tval_result);
  printf("Time elapsed: %ld.%06ld\n", (long int)tval_result.tv_sec, (long int)tval_result.tv_usec);
}