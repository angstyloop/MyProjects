#include <iostream>
using namespace std;

int addition(int m, int n)
{
    return m+n;
}

int subtraction(int m, int n)
{
    return m-n;
}

int operation(int m, int n, int (*f)(int, int))
{
    return (*f)(m, n);
}

int main()
{
    int m, n;
    int (*minus)(int, int) = subtraction;
    m = operation(2, 3, addition);
    n = operation(m, 2, minus);
    cout << n << endl;
    return 0;
}
