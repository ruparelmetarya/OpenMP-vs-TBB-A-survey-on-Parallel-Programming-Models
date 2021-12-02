/*
Sum of array implementation in TBB. Takes no of threads as first argument and array input size as second arg.
Trivial initialisation of arrays 
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
#include <tbb/parallel_reduce.h>

using namespace tbb;
using namespace std;



class SumClass {
    int* my_a;
public:
    int my_sum;
    void operator()( const blocked_range<size_t>& r ) {
        int *a = my_a;
        int sum = my_sum;
        size_t end = r.end();
        for( size_t i=r.begin(); i!=end; ++i )
            sum += a[i];
        my_sum = sum;
    }
    SumClass( SumClass& x, split ) : my_a(x.my_a), my_sum(0) {}

    void join( const SumClass& y ) {my_sum+=y.my_sum;}

    SumClass(int *a) :
        my_a(a), my_sum(0)
    {}
};


int main(int argc, char const *argv[])
{
    int *a;
    if (argc < 3){
        cout<<"Enter as arg1: thread count, arg2: array size"<<endl;
        exit(0);
    }   
    int no_threads = atoi(argv[1]);
    int N = atoi(argv[2]);
    a = new int[N];

    for (int i=0; i < N; i++){
        a[i] = i; 
    }
    SumClass sf(a);
    task_scheduler_init init(no_threads);	
    tick_count t_start = tick_count::now();
    parallel_reduce( blocked_range<size_t>(0,N),sf);
    tick_count t_end = tick_count::now();
    cout<<"Work took"<<(t_end - t_start).seconds()<<endl;
    cout<<sf.my_sum<<endl;


    return 0;
}
