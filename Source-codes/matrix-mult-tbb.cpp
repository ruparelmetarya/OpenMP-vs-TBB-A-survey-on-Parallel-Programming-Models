/*
Matrix multiplication implementation in TBB. Takes no of threads as first argument and array input size as second arg.
Trivial initialisation of matrices, NxN. Gives time for both sequential and parallel runs.
Run as:
g++ name.cpp -ltbb 
./a.out t N 
Use gcc-9.2
*/

#include <iostream>
#include <stdlib.h>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"

using namespace tbb;
using namespace std;


class Multiply
{
    int ** p_a;
    int ** p_b;
    int ** p_c;
    int n;

    public:
        Multiply(int ** a, int ** b, int ** c, int N): p_a(a), p_b(b), p_c(c), n(N) {}

        void operator()(blocked_range<int> r) const {
            for (int i = r.begin(); i != r.end(); ++i) {
                for (int j = 0; j < n; ++j) {
                    for (int k = 0; k < n; ++k) {
                        p_c[i][j] += p_a[i][k] * p_b[k][j];
                    }
                }
            }
        }
};

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

    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            a[i][j] = i + j;
            b[i][j] = i - j;
            c[i][j] = 0;
            seq[i][j] = 0;
        }
    }
    
    tick_count t_start1 = tick_count::now();
    for (int i = 0; i < N; i++){
      for (int j = 0; j < N; j++){
          for (int k = 0; k < N; k++){
            seq[i][j] += a[i][k] * b[k][j];
          }
        }
    }
    tick_count t_end1 = tick_count::now();
    
    cout<<"Work took seq: "<<(t_end1 - t_start1).seconds()<<" sec"<<endl;


    task_scheduler_init init(no_threads);	
    tick_count t_start = tick_count::now();

    parallel_for(blocked_range<int>(0,N), Multiply(a,b,c,N));

    tick_count t_end = tick_count::now();
    
    cout<<"Work took: "<<(t_end - t_start).seconds()<<" sec"<<endl;
    
    return 0;

}

