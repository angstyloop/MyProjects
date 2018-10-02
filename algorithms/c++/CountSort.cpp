/* An implementation of the CountSort algorithm that sorts an array of ASCII characters */
/* This is a cool example of how you can use char variables as array indices. */
/* Without explicit casting to int the compiler gives a warning. */
/* Since the final loop runs backward, this sort is stable.*/
// Runtime O(n).
// keywords: strlen(), memset(), memcpy(), stable, character indices, null-terminated character strings (NTCS)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define RANGE 256

void CountSort(char arr[])
{
    // allocate memory for temp array
    int n = strlen(arr), j;
    char *temp = new char[n];
    
    // initialize count array
    int count[RANGE];
    memset(count, 0, RANGE*sizeof(int));

    // count occurences of each character in arr[]
    for (j=0; arr[j]; ++j)
        ++count[arr[j]];

    // make count[j] the number of elements <= arr[j] 
    for (j=1; j<RANGE; ++j)
        count[j] += count[j-1];

    // place the characters at the appropriate index in temp and decrease count.
    // go backwards to preserve order of identical letters.
    for (j=n-1; j>=0; --j)
    {
        temp[count[arr[j]]-1] = arr[j];
        --count[arr[j]];
    }

    // copy temp to arr[] and clean up
    memcpy(arr, temp, n);
    delete[] temp;
}

// Driver program. Sorts the first string (no spaces) provided on command line.
// Note we can write to the strings in argv. I didn't know that :)
int main(int argc, char **argv)
{
    if (argc==0)
        exit(0);
    //int n = strlen(argv[1]);
    //char *arr = new char[n];
    //memcpy(arr, argv[1], n);
    CountSort(argv[1]);
    puts(argv[1]);
}
