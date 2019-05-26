/* An implementation of the Kadane maximum subarray algorithm */

#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

// If we just want the sum, the algorithm is pretty short.
// Runtime: O(n)
int MaxSubArraySum(vector<int> &A)
{
    int maxEndsHere = A[0];
    int maxSoFar = A[0];
    for (int i=1; i < A.size(); i++){
        maxEndsHere = max(maxEndsHere + A[i], A[i]);
        maxSoFar = max(maxSoFar, maxEndsHere);
    }
    return maxSoFar;
}

// If we also want the indices of the max subarray, we have to note when maxSoFar changes. I use nested if statements.
//  instead of the max function.
// Runtime: O(n)
int *MaxSubArray(vector<int> &A)
{
    int maxEndsHere, maxSoFar;
    maxEndsHere = maxSoFar = A[0];
    int l, r;
    l = r = 0;
    static int res[3];
    for (int i=1; i < A.size(); i++){
        if (maxEndsHere >= 0){
            maxEndsHere += A[i];
            if (maxEndsHere > maxSoFar){
                maxSoFar = maxEndsHere;
                r = i;
            }
        }
        else {
            maxEndsHere = A[i];
            if (maxEndsHere > maxSoFar) {
                maxSoFar = maxEndsHere;
                l = i;
                r = i;
            }
        }
    }
    res[0]=maxSoFar;
    res[1]=l;
    res[2]=r;
    return res;
}


// Takes an series of integers from the command line, separated by spaces, and prints the indices of the
//  max subarray and its sum to standard output.
int main(int argc, char **argv) 
{
    vector<int> argNums(argc-1);
    for (int i=0; i < argc-1; i++) argNums[i] = atoi(argv[i+1]);
    int *res = MaxSubArray(argNums);
    cout << "The maximum subarray has indices (" << res[1] << ", " << res[2] << ") and sum " << res[0] << ".";
    cout << endl;
    return 0;
}
