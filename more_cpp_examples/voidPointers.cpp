// increaser; a great example of why void pointers are useful
#include <iostream>
using namespace std;

void increase(void *data, int psize)
{
    if (psize == sizeof(char))
        ++*(char*)data; //cast to a char pointer and increment
    else if (psize == sizeof(int))
        ++*(int*)data; // cast to an int pointer and increment
}

int main()
{
    int x = 1;
    char y = 'a';
    increase(&x,sizeof(int));
    cout << x << endl;
    increase (&y, sizeof(char));
    cout << y << endl;
}
