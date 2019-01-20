#include <iostream>
using namespace std;

class foo {
    public:
        const int bar;
        foo (int b) : bar(b) {}
};

int main() {
    foo f(5);
    cout << f.bar << endl;

    f.bar = 2;
    
}
