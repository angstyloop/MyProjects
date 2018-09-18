#include <iostream>
#include <vector>
#include <cstdlib>
using namespace std;

struct HeapElement
{
    int key;
    void * handle;
};
int main()
{
    int *p = new int;
    int *q = p;
    *p = 5;
    *q = 4;
    delete p;
    //delete q;
}
