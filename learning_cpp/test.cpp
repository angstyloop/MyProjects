
#include<iostream> 
using namespace std; 

template <class T>
class MyClass {
    string value;
  public:
    //MyClass() {}
    MyClass(T s): value{s} {}
};

MyClass<string> myFun() {
    return MyClass<string>{"foo"};
}
  
int main() 
{ 
    return 0;
} 
