// A C++ implementation of the worst-case linear time algorithm from CLSR for finding the
//  kth order statistic in an array. I found this code on Geeks For Geeks, and I thought
//  reproducing it would be good C practice.

#include<iostream>
//#include<climits> //for INT_MAX

int partition(int arr[], int l, int r, int k);

void insertionSort(int *l, int *r);

// Finds the median of the length n subarray pointed to by arr. Only called for array of 
//  size 5 or less.
int findMedian(int arr[], int n)
{
    insertionSort(arr, arr+n);     
    return arr[n/2];
}

int select(int arr[], int l, int r, int k)
{
    // if k doesn't exceed the number of elements in arr[]
    if (k > 0 && k <= r-l+1)
    {
        int n = r-l+1;

        // Divide the array into groups of 5, and store each of the medians in median[].
        // If the array doesn't divide evenly, we'll have to treat the last group separately.
        int i, median[(n+4)/5]; // equivalent to ceil(n/5)
        for (i=0; i < n/5; i++)
        {
            median[i] = findMedian(arr+l+i*5, 5);
        }
        // If 5 times the number of groups you just went through is still less than
        //  the total number of elements in arr[]
        if (i*5 < n)
        {
            median[i] = findMedian(arr+l+i*5, n%5);
            i++;
        }
        // Call select recursively to find the median of medians. If median[] is a unit array,
        //  (i.e. if i=1) just return median[0];
        int medOfMed = (i==1) ? median[i-1] : select(median,0, i-1, i/2);
        
        // Partition arr[] around the median of medians and get the index of the median.
        int q = partition(arr, l, r, medOfMed); 

        // Compute the length of the low side of the partition, including the pivot
        i = q-l+1;

        // If k equals this length, then the pivot must be the kth order statistic
        if (k==i) return medOfMed;

        // If k is less, then the low side must contain the kth o.s.
        if (k<i) return select(arr, l, q-1, k);

        // If k is greater, then the high side must contain the kth o.s.
        else return select(arr, q+1, r, k-i);
    }

    // If k exceeds the number of elements in arr[], return infinity.
    else return INT_MAX;
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Searches for the pivot x in arr[] (a linear time operation), swaps it with the last element,
//  and then partitions arr around the last element like the standard partition algorithm.
// Runtime: O(n)
int partition(int arr[], int l, int r, int x)
{
    // search for the pivot x and swap it with the last element
    for (int i=l; i<r; i++) //if arr[r] IS the pivot we don't need to swap
    {
        if (arr[i] == x)
        {
            swap(arr+i, arr+r);
            break;
        }
    }
    // standard partition algorithm from here; uses the last element as the pivot.
    int i = l-1;
    for (int j=l; j<r; j++)
    {
        if (arr[j] < arr[r])
        {
            i++;
            swap(arr+j, arr+i);
        }
    }
    swap(arr+i+1, arr+r);
    return i+1;
}

// Sorts the subarray marked by [l,r)
void insertionSort(int *l, int *r)
{
    //int key, j, i;
    for (int j=1; j < r-l; j++)
    {
        int key = *(l+j); 
        int i = j-1;
        while (i >= 0 && *(l+i) > key)
        {
            *(l+i+1) = *(l+i);
            i--;
        }
        *(l+i+1) = key;
    }
}

int main(int argc, char **argv)
{
    using namespace std;
    int n = argc-2;
    int *A = new int[n]; 
    for (int i=0; i<n; i++)
        A[i] = stoi(argv[i+1]);
    int k = stoi(argv[n+1]);
    cout << select(A, 0, n-1, k) << endl;
    delete[] A;
    return 0;
}
