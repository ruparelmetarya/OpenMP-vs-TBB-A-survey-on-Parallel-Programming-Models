/*
Two sum array implementation in OpenMP. Takes no of threads as first argument and array input size as second arg, chunk size as third.
Trivial initialisation of arrays
Run as:
g++ -fopenmp -Wall name.cpp
./a.out t N 
Use gcc-9.2
*/
#include <iostream>
#include <iomanip>
#include <omp.h>
using namespace std;

int main(int argc, char const *argv[])
{
    if(argc < 4)
    {
        cout<<"Enter as arg1: thread count, arg2: array size, arg3: chunksize"<<endl;
        exit(0);
    }  
    int no_threads = atoi(argv[1]);
    int N = atoi(argv[2]);
    int chunksize = atoi(argv[3]);
    int a[N], b[N], c[N];

    for (int i=0; i < N; i++){
        a[i] = i; 
        b[i] = i+1;
        c[i] = 0;
    }
    double start1; 
    double end1; 
    start1 = omp_get_wtime(); 
    for(int i=0;i<N;i++){
        c[i] = a[i] + b[i];
    }

    end1 = omp_get_wtime();
    for(int i=0;i<N;i++){
        c[i] = 0;
    }		 
    printf("Work took seq %f seconds\n\n", end1 - start1);
    double ts = end1 - start1; 
    double start; 
    double end; 
    start = omp_get_wtime(); 
    #pragma omp parallel for num_threads(no_threads) schedule(static,chunksize)
    for(int i=0;i<N;i++){
        c[i] = a[i] + b[i];
    }
    end = omp_get_wtime(); 
    /* change schedule with
    #pragma omp parallel for num_threads(no_threads) schedule(static) or schedule(dynamic)
    for(int i=0;i<N;i++){
        c[i] = a[i] + b[i];
    }
    */
    printf("Work took %f seconds\n\n", end - start);
    double tp = end - start;
    
    double overhead = (tp - ts);
    printf("Overhead took %f seconds\n\n", overhead);
    
    return 0;
}
