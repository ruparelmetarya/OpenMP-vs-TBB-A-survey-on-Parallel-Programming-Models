/*
Merge Sort implementation in OpenMP using tasks. Takes no of threads as first argument and array input size as second arg.
Randomn initialisation of arrays
Run as:
g++ -fopenmp -Wall name.cpp
./a.out t N 
Use gcc-9.2
*/
#include <iostream>
#include <iomanip>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <omp.h>

using namespace std;

unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;
    while (r >= int(limit))
    {
        r = rand();
    } 
    return min + (r / buckets);
}

void fillupRandomly (int *m, int size, unsigned int min, unsigned int max){
    for (int i = 0; i < size; i++)
    m[i] = rand_interval(min, max);
} 

void mergeSortAux(int *X, int n, int *tmp) {
   int i = 0;
   int j = n/2;
   int ti = 0;

   while (i<n/2 && j<n) {
      if (X[i] < X[j]) {
         tmp[ti] = X[i];
         ti++; i++;
      } else {
         tmp[ti] = X[j];
         ti++; j++;
      }
   }
   while (i<n/2) { 
      tmp[ti] = X[i];
      ti++; i++;
   }
   while (j<n) { 
      tmp[ti] = X[j];
      ti++; j++;
   }
   memcpy(X, tmp, n*sizeof(int));
} 

void mergeSort(int *X, int n, int *tmp)
{
   if (n < 2) return;

   #pragma omp task shared(X)
   mergeSort(X, n/2, tmp);

   #pragma omp task shared(X)
   mergeSort(X+(n/2), n-(n/2), tmp + n/2);

   #pragma omp taskwait
   mergeSortAux(X, n, tmp);
}


int isSorted(int *a, int size){
   for(int i = 0; i < size - 1; i++)
      if(a[i] > a[i + 1])
        return 0;
   return 1;
}

int main(int argc, char *argv[]) {
        srand(123456);
        
        if (argc < 3){
            cout<<"Enter as arg1: thread count, arg2: array size"<<endl;
            exit(0);
        }    

        int no_threads = atoi(argv[1]);
        int N  = atoi(argv[2]);
        
        int *X = new int[N];
        int *tmp = new int[N];

        fillupRandomly(X,N,0,5);

        double begin = omp_get_wtime();
        #pragma omp parallel num_threads(no_threads)
        {
            #pragma omp single
            mergeSort(X, N, tmp);
        }   
        double end = omp_get_wtime();
        printf("Work took: %f seconds\n",end-begin);
    
        assert(1 == isSorted(X, N));
        return 0;
}
