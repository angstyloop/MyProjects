#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <algorithm>

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::copy;
using std::advance;
using std::pair;
using std::unique_ptr;
using std::swap;
using std::size_t;

//change to use T* instead
template<class T>
class Property {
  T value;
 public:
  ~Property(); //dtr
  Property(); //ctr
  Property(Property&&); // move ctr
  Property& operator=(Property&&); // move assign
  Property(Property&); // copy ctr
  Property& operator=(Property); // copy assign
  Property(T); // ctr from string
  Property& operator=(T); // assign from string
  T operator()() const; // obj is a functor
  Property& test();
};
