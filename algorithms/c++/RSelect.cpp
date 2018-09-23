/* Uses a random partitioning algorithm RPartition to select the ith order statistic from
 * an array in expected linear time, with worst-case quadratic runtime.
 */

#include <random>
#include <iostream>
#include <cstdlib>
#include <vector>
using namespace std;

// We need a swap function
void Swap(vector<int> &A, int i, int j)
{
    int c = A[i];
    A[i] = A[j];
    A[j] = c;
}

// Partition using the last element as a pivot.
// Runtime: O(n)
int Partition(vector<int> &A, int p, int r)
{
    int i = p-1;
    for (int j=p; j<r; j++ )
    {
        if (A[j] < A[r])
        {
            i++;
            Swap(A, i, j);
        }
    }
    Swap(A, i+1, r);
    return i+1;
}

// Partition with a random pivot.
// Runtime: O(n)
int RPartition(vector<int> &A, int p, int r)
{
    random_device rd; // Should really seed once and then feed it to RPartition and RSelect
                      //    as an extra argument. That would probably save some time.
    mt19937 gen(rd());
    uniform_int_distribution<int> dis(p,r);
    Swap(A, r, dis(gen));
    return Partition(A, p, r);
}

// Now use RPartition to get the ith order statistic of the subarray A[p..r].
// Expected runtime: <O(n)>; Worst case: <O(n^2)>
int RSelect(vector<int> &A, int p, int r, int i)
{
    if (p==r) return A[p];
    int q = RPartition(A, p, r);
    int k = q-p+1;
    if (k==i) return A[q];
    else if (i<k) return RSelect(A, p, q-1, i);
    else return RSelect(A, q+1, r, i-k);
}

// now let's see if it works
int main(int argc, char **argv)
{
    vector<int> A(argc-2); // make room for all args except first and last
    for (int i=0; i<argc-2; i++) A[i] = atoi(argv[i+1]); // get all args except first and last
    int i = atoi(argv[argc-1]); // the last arg is the order 
    cout << RSelect(A, 0, A.size()-1, i) << endl;
    return 0;
}
