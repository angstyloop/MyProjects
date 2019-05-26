#include <iostream>
using namespace std;

// The largest product in the multiplication table is HEIGHT*WIDTH.
#define HEIGHT 20
#define WIDTH 10

// Count the number of digits in an integer. Need this for indentation.
int digits(int n)
{
    int i = 0;
    while (n > 0)
    {
        n /= 10;
        ++i;
    }
    return i;
}

// Print a multiplication table on the command line with dimensions HEIGHT x WIDTH.
int main()
{
    int entry;
    int maxDigits = digits((HEIGHT+1)*(WIDTH+1)) + 5;
    //int multTable[HEIGHT][WIDTH];
    int multTable[HEIGHT*WIDTH];

    for (int i=0; i<HEIGHT; ++i)
    {
        for (int j=0; j<WIDTH; ++j)
        {
            entry = (i+1)*(j+1);   
            multTable[i*WIDTH+j] = entry;
            cout << multTable[i*WIDTH+j] << string(maxDigits - digits(entry), ' ');
        }    
        cout << endl;
    } 
}
