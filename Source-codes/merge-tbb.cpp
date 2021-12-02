/*
Merge Sort implementation in OpenMP using task groups. Takes no of threads as first argument and array input size as second arg.
Randomn initialisation of arrays
Run as:
g++ name.cpp -ltbb 
./a.out t N 
Use gcc-9.2
*/
#include <iostream>
#include <assert.h>
#include <iostream>
#include <random>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <ctime>
#include <cstdlib>
#include "tbb/parallel_for.h"
#include "tbb/blocked_range.h"
#include "tbb/task_scheduler_init.h"
#include "tbb/tick_count.h"
#include <tbb/task_group.h>

using namespace std;
using namespace tbb;

int binarySearch(int x, int T[], int p, int r) {
	int low = p;
	int high = max(p, r + 1);
	while (low < high) {
		int mid = (low + high) / 2;
		if (x <= T[mid]) {
			high = mid;
		} else {
			low = mid + 1;
		}
	}
	return high;
}

void parallelMerge(int T[], int p1, int r1, int p2, int r2, int A[], int p3) {
	int n1 = r1 - p1 + 1;
	int n2 = r2 - p2 + 1;
	if (n1 < n2) {
		swap(p1, p2);
		swap(r1, r2);
		swap(n1, n2);
	}
	if (n1 == 0) {
		return;
	} else {
		int q1 = (p1 + r1) / 2;
		int q2 = binarySearch(T[q1], T, p2, r2);
		int q3 = p3 + (q1 - p1) + (q2 - p2);
		A[q3] = T[q1];
		task_group tg;
		tg.run([=] { parallelMerge(T, p1, q1 - 1, p2, q2 - 1, A, p3); });
		tg.run([=] { parallelMerge(T, q1 + 1, r1, q2, r2, A, q3 + 1); });
		tg.wait();
	}
}

void parallelMergeSort(int A[], int p, int r, int B[], int s) {
	int n = r - p + 1;
	if (n == 1) {
		B[s] = A[p];
	} else {
		int *T = new int[n];
		T--;
		int q = (p + r) / 2;
		int qp = q - p + 1;
		task_group tg;
		tg.run([=] { parallelMergeSort(A, p, q, T, 1); });
		tg.run([=] { parallelMergeSort(A, q + 1, r, T, qp + 1); });
		tg.wait();
		parallelMerge(T, 1, qp, qp + 1, n, B, s);
		T++;
		delete[] T;
	}
}

void MergeSort(int in[], int out[], int n) {
	parallelMergeSort(in - 1, 1, n, out - 1, 1);
}


unsigned int rand_interval(unsigned int min, unsigned int max)
{
    int r;
    const unsigned int range = 1 + max - min;
    const unsigned int buckets = RAND_MAX / range;
    const unsigned int limit = buckets * range;

    while (r >= limit)
    {
        r = rand();
    };

    return min + (r / buckets);
}

void fillupRandomly (int *m, int size, unsigned int min, unsigned int max){
    for (int i = 0; i < size; i++)
    m[i] = rand_interval(min, max);
} 

int isSorted(int *a, int size){
   for(int i = 0; i < size - 1; i++)
      if(a[i] > a[i + 1])
        return 0;
   return 1;
}

int main(int argc, char const *argv[])
{
    srand(123456);
    if(argc < 3)
    {
        cout<<"Enter as arg1: thread count, arg2: array size"<<endl;
        exit(0);
    }    
    int no_threads = atoi(argv[1]);
    int N = atoi(argv[2]);
    int *a = new int[N];
	int *res = new int[N];
    fillupRandomly(a,N,0,40);
    task_scheduler_init init(no_threads);
	tick_count t0 = tick_count::now();

	MergeSort(a,res,N);

	tick_count t1 = tick_count::now();

	double time = (t1 - t0).seconds();
    cout<<"Work took "<<time<<" sec"<<endl;
   
    assert(1 == isSorted(res, N));

    return 0;
}

