/*
Two sum array implementation in OpenMP. Takes no of threads as first argument and array input size as second arg.
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
    if (argc < 3){
        cout<<"Enter as arg1: thread count, arg2: array size"<<endl;
        exit(0);
    }
    int no_threads = atoi(argv[1]); 
    int N = atoi(argv[2]);
    int a[N], b[N], c[N];

    for (int i=0; i < N; i++){
        a[i] = i; 
        b[i] = i+1;
        c[i] = 0;
    }
    double start; 
    double end; 
    start = omp_get_wtime(); 
    #pragma omp parallel for num_threads(no_threads)
    for(int i=0;i<N;i++){
        c[i] = a[i] + b[i];
    }
    end = omp_get_wtime(); 
    printf("Work took %f seconds\n\n", end - start);
   
    cout<<endl;
    return 0;
}