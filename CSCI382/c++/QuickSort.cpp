/* An implementation of QuickSort. The first version uses the last element of A as the pivot. The second version
 * uses a random pivot to improve expected runtime.
 */
#include <iostream>
#include <vector>
#include <random>
using namespace std;

int Partition(vector<int> &A, int p, int r);

void Swap(vector<int> &A, int i, int j);

// Uses Partition, which uses the last element as the pivot.
// Worst case runtime: O(n^2) when partition does an n-1 to 1 split every time.
// Best case runtime: O(nlogn) when partition does the same constant ratio split every time.
void QuickSort(vector<int> &A, int p, int r)
{
    if (p<r)
    {
        int q = Partition(A, p, r);
        QuickSort(A, p, q-1);
        QuickSort(A, q+1, r);
    }
}

// Uses the last element as the pivot.
// Runtime O(n).
int Partition(vector<int> &A, int p, int r)
{
    int i = p-1;
    for (int j=p; j<r; j++)
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

// Swaps two elements in the vector A.
void Swap(vector<int> &A, int i, int j)
{
    int c = A[i];
    A[i] = A[j];
    A[j] = c;
}

// We can rewrite partition to use a random pivot, which then gives quicksort an expected runtime <O(nlogn)>.
// Runtime: O(n)
int RPartition(vector<int> &A, int p, int r)
{
    // this is best way I've seen so far to generate a random number in [p..r] (includes endpoints!)
    random_device rd; //the seed
    mt19937 gen(rd()); // the generator
    uniform_int_distribution<int> dis(p,r); // the distribution
    int i = dis(gen); // finally, the random index
    Swap(A,i,r);
    return Partition(A,p,r); // we just call the old Partition on the adjusted A.
}

// Now we can rewrite quicksort using RPartition.
// Expected runtime: <O(nlogn)>
void RQuickSort(vector<int> &A, int p, int r)
{
    if (p<r)
    {
        int q = RPartition(A,p,r);
        RQuickSort(A,p,q-1);
        RQuickSort(A, q+1, r);
    }
}

// Now let's test everything out on the command line.
int main(int argc, char **argv)
{
    vector<int> A(argc-1);
    for (int i=0; i<argc-1; i++)
    {
        A[i] = atoi(argv[i+1]);
    }
    RQuickSort(A,0,A.size()-1);
    for (auto x: A) cout << x << " ";
    cout << endl;
    return 0;
}

// Seems to work fine!
