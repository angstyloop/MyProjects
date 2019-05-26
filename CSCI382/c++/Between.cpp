#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

int Between(vector<int> &A, int x, int y)
{
    int i=0;
    int j=A.size()-1;
    while (i<=j)
    {
        if (A[i] <= x) i++;
        else if (A[j] >= y) j--;
        else break;
    }
    return j-i+1;
}

int main(int argc, char **argv)
{
    if (argc != 3)
    {
        cout << "Please provide exactly two arguments." << endl;
        return -1;
    }
    vector <int> A {1,2,3,4,8,9,10};
    int x = atoi(argv[1]);
    int y = atoi(argv[2]);
    cout << "Test Array: ";
    for (auto x: A) cout << x << " ";
    cout << endl;
    cout << "There are " << Between(A,x,y) << " numbers between " << x << " and " << y << " in the array." << endl;
}
