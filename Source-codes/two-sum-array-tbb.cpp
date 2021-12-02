/*
Two sum array implementation in TBB. Takes no of threads as first argument and array input size as second arg.
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
using namespace tbb;
using namespace std;

class TwoArraySum {
  int *p_a;
  int *p_b;
  int *p_c;
public:									
  TwoArraySum(int * a, int * b, int * c) : p_a(a), p_b(b), p_c(c) {}										

  void operator() ( const blocked_range<int>& r ) const {	
    for ( int i = r.begin(); i != r.end(); i++ ) { 
      p_c[i] = p_a[i] + p_b[i];
    }								
  }

};				

int main(int argc, char *argv[]) {
		
    if (argc < 3){
        cout<<"Enter as arg1: thread count, arg2: array size"<<endl;
        exit(0);
    }    
    int *a;
    int *b;
    int *c;

    int no_threads = atoi(argv[1]); 
    int N = atoi(argv[2]);

    a = new int[N];
    b = new int[N];
    c = new int[N];


    for (int i=0; i < N; i++){
        a[i] = i; 
        b[i] = i+1;
        c[i] = 0;
    }

    task_scheduler_init init(no_threads);	
    tick_count t_start = tick_count::now();

    parallel_for(blocked_range<int>(0, N),TwoArraySum(a,b,c));	 
    
    tick_count t_end = tick_count::now();
    cout <<"Work took: "<<(t_end - t_start).seconds()<<" seconds"<< endl;
    
    cout<<endl;
    return 0;
}
