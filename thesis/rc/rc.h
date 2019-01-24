#include "MatrixLib.h"
#include <iostream>
#include <cmath>

template <class type>
class DiscreteTimeSeries {
    protected:
        Vector<type>* path;
        int steps;
    public:
        DiscreteTimeSeries ( Vector<type>, const int& );
        virtual ~DiscreteTimeSeries () {if (path!=nullptr) delete[] path;}
        const Vector<type>& operator[] (const int) const;
        virtual const Vector<double> Map(const Vector<double>& v) const {return v;}
        void Wash (int);
        const Vector<type>* const getPath () const;
        const int& getSteps() const;
        void run();
};

class BakersMap : public DiscreteTimeSeries<double> {
        double param;
    public:
        BakersMap (Vector<double> start, int steps, double a)
            : DiscreteTimeSeries (start, steps), param (a) {}                
            
        const Vector<double> Map (const Vector<double>&) const; 
        
        
};




