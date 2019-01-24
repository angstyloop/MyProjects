#include "rc.h"
#include <iostream>

// DiscreteTimeSeries class method definitions

// constructor
template <class type>
DiscreteTimeSeries<type>::DiscreteTimeSeries (Vector<type> start, const int& s )
    : path (new Vector<type>[s]), steps (s) {
    path = new Vector<type> [s];
    path[0] = start;
}

// populate path
template <class type>
void DiscreteTimeSeries<type>::run () {
    for (int i=1; i<steps; ++i)
        path[i] = Map(path[i-1]);
}
// wash out the first n points in path
template <class type>
void DiscreteTimeSeries<type>::Wash (int n) {
    Vector<type>* temp = new Vector<type>[steps-n]; //reallocate
    for (int i=n; i<steps; ++i) //transfer
        temp[i] = path[i];
    delete[] path; //clean up
    path = temp; //move pointer
    temp = nullptr;
}

// getter function for the path; returns a read-only array of Vectors (i.e. a const reference)
template <class type>
const Vector<type>* const DiscreteTimeSeries<type>::getPath () const { return path; }

template <class type>
const int& DiscreteTimeSeries<type>::getSteps () const { return steps; }

template <class type>
const Vector<type>& DiscreteTimeSeries<type>::operator[] (const int i) const {
    if (i>=0 && i<steps)
        return path[i];
    std::cout << "DiscreteTimeSeries: index out of range." << std::endl;
    exit (EXIT_FAILURE);
}

// BakersMap class method definitions
//

const Vector<double> BakersMap::Map (const Vector<double>& that) const {
    const double& x=that[0], y=that[1], a=param;
    Vector<double> temp(2);
    if (x<=.5) {
        temp[0] = 2*x;
        temp[1] = a*y;
    } else {
        temp[0] = 2*x-1;
        temp[1] = a*y+.5;
    }
    return temp;
}

// driver program
//
int main() {
    Vector<double> start(2);
    start[0] = .741;
    start[1] = .234;
    int steps = 10;
    double a = .134;
    BakersMap b(start, steps, a); 
    b.run();
    for (int i=0; i<steps; ++i)
        b[i].T().print();
}
