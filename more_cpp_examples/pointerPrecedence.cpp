/* practice with pointer precedence */
#include <iostream>
using namespace std;

void increment_all(int *start, int *end)
{
    while (start < end)
        ++*start++;
}

void decrement_all(int *start, int*end)
{
    while (start < end)
        --*start++;
}

int main()
{
    // Note end points to the space in memory immediately after the last element of arr.
    // This is typical for functions that take a start and end pointer as arguments, e.g. sort().
    int arr[] {1,2,3,4,5}, *start = arr, *end = arr + 5, *p;

    increment_all(start, end);
    p = start;
    while (p < end)
        cout << *p++;
    cout << endl;

    decrement_all(start, end);
    p = start;
    while (p < end)
        cout << *p++;
    cout << endl;

    return 0;
}
