#include <iostream>
#include <vector>
using namespace std;

class A
{
    private:
        int a = 1;
    public:
        void showa();
};

void A::showa() {cout << a << endl;}

int main()
{
    class A a;
    a.showa();
    return 0;
}
