#include <iostream>
using namespace std;

int main()
{
    // Pointer initialization to null
    int *p = NULL;
   
    // Request memory using the new operator.
    p = new int;
    if (!p)
        cout << "exception type: bad_alloc" << endl;
    else
    {
        // Assign a value to the memory at p.
        *p = 29;
        cout << "the value "
             << *p
             << " is stored at p."
             << endl;
    }

    // Request memory using the new operator.
    float *r = new float(75.25);
    cout << "the value of r is: " << *r << endl;


    // Request a block of memory of size n
    int n = 5;
    int *q = new int[n];
    if (!q)
        cout << "exception type: bad_alloc" << endl;
    else
    {
        // put the integers 1..5 int q[]
        for (int i=0; i<n; i++)
            q[i] = i+1;

        for (int i=0; i<n; i++)
            cout << q[i] << " ";
        cout << endl;
    }
    
    // free the allocated memory
    delete p;
    delete r;
    
    // free the block of allocated memory
    delete[] q;

    return 0;
}
