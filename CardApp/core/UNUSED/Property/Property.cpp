#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <algorithm>
#include "Property.h"

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

template <class T>
Property<T>::~Property() {}

template <class T>
Property<T>::Property() {}

template <class T>
Property<T>::Property(Property<T>&& other) {
    value = other();
}

template <class T>
Property<T>& Property<T>::operator=(Property<T>&& other) {
    value = other();
}


template <class T>
Property<T>::Property(Property<T>& v): value(v()) {}

template <class T>
Property<T>& Property<T>::operator=(Property<T> other) {
  if (this!=&other)
    value = other.value;
  return *this;
}

template <class T>
Property<T>::Property(T v): value(v) {}

template <class T>
Property<T>& Property<T>::operator=(T newValue) {
  value = newValue;
  return *this;
}

template <class T>
T Property<T>::operator()() const {
  return value;
}

template <>
Property<int>& Property<int>::test() {
  Property<int> p1 {1};
  Property<int> p2 {2};
  Property<int> p3 {p1() + p2()};
  Property<int> p4 {p3};
  cout << "Assignment works..." << endl;
  cout << "Value of $p1 before assignment $p1 <-- $p2 : " << p1() << endl;
  p1 = p2;
  cout << "Value of $p1 after assignment $p1 <-- $p2: " << p1() << endl;
  cout << endl;
  cout << "Any operators that work with the underlying type will also work for a Property, provided you remember the parentheses." << endl;
  cout << "Value of $p3 after adding $p1 and $p2 and assigning to $p3: ";
  cout << p3() << endl;
  cout << "copy ctr works" << endl;
  cout << "p4: " << p4() << endl;
  cout << "Assignment by value also works." << endl;
  p1 = p4();
  cout << "p1: " << p1() << endl;
  return *this;
}

//int main() { Property<int>().test(); }
