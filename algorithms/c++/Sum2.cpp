/* Sum2(int x, vector<int> &A) returns true if A contains two distinct elements that sum
 * to x, and false otherwise. Runtime is O(n*logn)
 */

#include <iostream>
#include <vector>
using namespace std;

//LSum(x,n,A,s,e) returns true if the subarray A[s:e+1] contains an element other than A[n]
// that sums with A[n] to equal x, and false otherwise. Runtime is O(n).

bool LSum(int x, int n, vector<int> &A, int s, int e)
{
    for (int i=s; i<=e; i++)
    {
        if (A[i]+A[n]==x && i!=n) return true;    
    }
    return false;
}

//BSum(x,A,s,e) returns true if the subarray A[s:e+1] contains two distinct elements that
// sum to x. Uses LSum. Uses recursion. Runtime is O(n*logn).

bool BSum(int x, vector<int> &A, int s, int e)
{
    if (s<e)
    {
        int m = int((s+e)/2); //explicit floor division for clarity
        return BSum(x,A,s,m) || BSum(x,A,m+1,e);
    }
    return LSum(x,s,A,0,A.size()-1);
}

//Sum2(x,A) just uses BSum as a helper function

bool Sum2(int x, vector<int> &A)
{
    return BSum(x,A,0,A.size()-1);
}

int main(int argc, char **argv)
{
    int x = *argv[1] - '0'; /* gets the first command line argument, assumed to be a number,
                               and converts it from char to the actual int value.*/
    vector<int> A {1,2,3,4,5};
    cout << Sum2(x, A) << endl;
    return 0;
}
