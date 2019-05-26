#include <iostream>
using namespace std;

void insertion_sort(int A[], int n){
    int i;
    int key;
    for (int j=0; j<n; j++) {
        key=A[j];
        i=j-1;
        while (i>=0 && A[i]>key) {
            A[i+1]=A[i];
            i--;
        }
        A[i+1]=key;
    }
}

int main() {
    int A[] = {2,1,4,3,7,5,9,10,8,6};
    cout << A[0] << "\n";
    insertion_sort(A,2);
    cout << A[0] << "\n";
}
