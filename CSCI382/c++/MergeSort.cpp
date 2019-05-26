/* MergeSort uses Merge to recursively sort a subarray of array A marked by the indices p and r.
 * MergeSort has runtime theta(n*logn), where n is the size of the initial subarray.
 * Merge combines two sorted subarrays of A marked by the indices p, q, and r into a single
 * sorted subarray of A. Merge has runtime theta(n).*/

#include <iostream>
#include <vector>
using namespace std;

const double INFINITY = 10e10; 

void Merge(vector<double> &A, int p, int q, int r){
    int n1 = q - p + 1;
    int n2 = r - q;
    double L[n1+1];
    double R[n2+1];

    for (int i=0; i<n1; i++){
        L[i] = A[p+i];
    }
    L[n1] = INFINITY;//constant = infinity

    for (int i=0; i<n2; i++){
        R[i] = A[q+1+i];
    }
    R[n2] = INFINITY;

    int i=0;
    int j=0;
    for (int k=p; k<=r; k++) {
        if (L[i] <= R[j]) {
            A[k] = L[i];
            i++;
        }
        else {
            A[k] = R[j];
            j++;
        }
    }
}

void MergeSort(vector<double> &A, int p, int r) {
    if (p < r) {
        int q = (p + r)/2;   // integer division implicity truncates the decimal
        MergeSort(A, p, q);
        MergeSort(A, q+1, r);
        Merge(A, p, q, r);
    }
}

int main() {
    vector<double> v {4e9,2,1e9,3,5,1,2};
    cout << "unsorted: ";
    for (auto x: v) cout << x << ' ';
    cout << endl;
    cout << "sorted: ";
    MergeSort(v, 0, v.size()-1);
    for (auto x: v) cout << x << ' ';
    cout << endl;
}
