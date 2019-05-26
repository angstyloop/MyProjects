/* Practice with std::fill and std::copy, and an example of a simple array-printing function. */

#include <iostream>
using namespace std;

template <class T>
void print(const T *start, const T *end)
{
    while (start < end)
        cout << *start++;
    cout << endl;
}

int main()
{
    int n = 5;
    int *arr1 = new int[n], *arr2 = new int[n];

    fill(arr1, arr1+n, 0);
    fill(arr2, arr2+n, 1);

    print(arr1, arr1+n);
    print(arr2, arr2+n);

    copy(arr1, arr1+n, arr2);

    print(arr1, arr1+n);
    print(arr2, arr2+n);

    delete[] arr1;
    delete[] arr2;

    return 0;

}
