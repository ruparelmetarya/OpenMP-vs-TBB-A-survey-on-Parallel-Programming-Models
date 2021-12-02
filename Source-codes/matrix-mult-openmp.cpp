/*
Matrix multiplication implementation in OpenMP. Takes no of threads as first argument and array input size as second arg.
Trivial initialisation of matrices, NxN. Gives time for both sequential and parallel runs.
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
    
    int **a = new int*[N];
    int **b = new int*[N];
    int **c = new int*[N];
    int **seq = new int*[N];

    for(int i = 0; i < N; ++i) {
        a[i] = new int[N];
        b[i] = new int[N];
        c[i] = new int[N];
        seq[i] = new int[N];
    }

    double start1; 
    double end1; 
    start1 = omp_get_wtime(); 

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            a[i][j] = i + j;
            b[i][j] = i - j;
            c[i][j] = 0;
            seq[i][j] = 0;
        }
    }
    end1 = omp_get_wtime(); 
    printf("Work took seq %f seconds\n\n", end1 - start1);

    double start; 
    double end; 
    start = omp_get_wtime(); 
    #pragma omp parallel for num_threads(no_threads)
    for (int i = 0; i < N; i++){
      for (int j = 0; j < N; j++){
          for (int k = 0; k < N; k++){
            c[i][j] = c[i][j] + a[i][k] * b[k][j];
          }
        }
    }

    end = omp_get_wtime(); 
    printf("Work took %f seconds\n\n", end - start);
    return 0;
}
