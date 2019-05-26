#include <iostream>
using namespace std;

/* In the code below we use pointer arithmetic to compute the size of an array */
/* Note that a is a pointer to the first element of the array a[], whereas &a  */
/* is a pointer to the array itself. a + 1 points to the integer after a[0],   */
/* whereas (&a + 1) points to the next array of length 3 after a[].*/
/* The expression (int*)(&a + 1) casts (&a + 1), a pointer to an array, to a   */
/* pointer to an integer, i.e. the very next integer after the last element of */
/* a. Thus the difference (int*)(&a + 1) - a gives us the size of the array a[]*/
/* If we omit int in the cast and just write *(&a + 1) - a, (&a + 1) is cast   */
/* implicitly to the same kind of pointer as a, which is (int*). Thus we don't */
/* need to know the type of the contents of an array to compute it's length.   */

int main() {
    int a[] = {1,2,3};
    cout <<  (int*)(&a + 1) - a << "\n";
}
