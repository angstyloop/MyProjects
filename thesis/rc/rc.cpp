#include "rc.h"
#include <iostream>

// DiscreteTimeSeries class method definitions

// constructor
DiscreteTimeSeries::DiscreteTimeSeries (Vector start, const int& s )
    : prev (0)
    , curr (0)
    , steps (s)
    , d (start.len())
    , series (new Vector*[s]) {     //This line right here is the reason series is a Vector**
                                    //  instead of a Vector[]; I can't write stuff like
                                    //  new Vector(d)[s], and I want dynamically sized vectors.
                                    
        // initialize the vectors pointed to by the entries of series[]
        int l = start.len();
        for (int i=0; i<s; ++i) {
            series[i] = new Vector(l);
        }
        // make series[0] point to start
        *series[0] = start;
    }

// populate series
void DiscreteTimeSeries::Listen () {
    curr = 1;
    prev = 0;
    while (curr<steps) Map();// see pure virtual function in DiscreteTimeSeries for spec.
}

// observes comps components for psteps steps with the others being continually provided.
//   this approach works only in the artificial scenario where the series map is known.
//   I still need to create a child of DiscreteTimeSeries to serve as a container for arbitrary
//   input sequences.
void EchoStateNetwork::Observe(int* comps, int L, int psteps) {

    //demand psteps < steps so we don't need to reallocate
    if (psteps<steps) {
        std::cout << "Unable to observe more steps than are in the training series." 
                  << " Consider reallocating if you need a longer observation period."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    //temp vars
    int j; //this will hold index of component we're interested in observing
    Vector v(tr_series->Dim()); //this will hold approximation to input vectors

    (*this)[0] = (*this)[prev];             //move the last reservoir state to the start of the series
    (*tr_series)[0] = tr_series->Prev();    //do the same for the training series

    //reset training series indices
    tr_series->SetCurr(0);        // should be 0           
    tr_series->SetPrev(0);        // should be 0, too

    //reset reservoir series indices
    curr = 1;   //should be 1
    prev = 0;   //should be 0 

    // populate the series, swapping out the appropriate components of the input vectors after each step
    while (curr<psteps) {
        Map(); //remember, this increments curr and thus controls the loop
        // replace whatever components with those from W_out * reservoir_state
        for (int i=0; i<L; ++i) {
            j = comps[i]; // index of a particular component
            v = W_out * (*this)[prev]; //generate approximation to the previous input vector

            //helpful error message
            if (v.len()!=tr_series->Prev().len()) {
                std::cout << "Observe(): invalid Vector assignment. Wrong lengths." << std::endl;
                exit(EXIT_FAILURE);    
            }

            tr_series->Prev()[j] = v[j]; //sub in the approximated components for the corresponding
                                          // components in the previous input vector, which will be used
                                          // to generate a new current input vector next time we call
                                          // Map().
        } 
    }
}

// wash out the first n points in series
void DiscreteTimeSeries::Wash (int n) {
    Vector** temp = new Vector*[steps-n]; //reallocate
    for (int i=n; i<steps; ++i) //transfer
        *temp[i] = *(series[i]);
    delete[] series; //clean up
    series = temp; //move pointer
    temp = nullptr;
}
// BakersMap class method definitions

void BakersMap::Map (void) {
    using namespace std;
    const double& x=(*this)[prev][0], y=(*this)[prev][1], a=param;
    Vector temp(2);
    if (x<=.5) {
        temp[0] = 2*x;
        temp[1] = a*y;
    } else {
        temp[0] = 2*x-1;
        temp[1] = a*y+.5;
    }
    (*this)[prev] = (*this)[curr] = temp;
    prev = curr++;
}

// vector hyperbolic tangent
Vector tanh(Vector v) {
    for (int i=0; i<v.len(); ++i)
        v[i] = tanh(v[i]);
    return v;
}

// EchoStateNetwork class defs

// constructor
// remember start lives in the reservoir state space (the big one)
EchoStateNetwork::EchoStateNetwork (Vector start, DiscreteTimeSeries* _tr_series , const int& _steps)
    : DiscreteTimeSeries(start, _steps)
      , tr_series (_tr_series)
      , W (d, d)
      , W_in (d, _tr_series->Dim())
      , W_out (_tr_series->Dim(), d)
      , offset (Offset(this->Dim()))
      , b (.01) // seems like a fine value for the ridge regression parameter
      {
          W_in.random(W_in.ncol*W_in.nrow/2); // use density = half the entries
          W.random(W.ncol*W.nrow/2);
          _tr_series = nullptr;
      }

void EchoStateNetwork::Map (void) { 
    
    (*this)[prev] = (*this)[curr] =  W_in * tr_series->Curr() + W * (*this)[prev] + offset;  
    tr_series->Map();
    prev = curr++;
     
}

void EchoStateNetwork::Train(void) {
    // fill state and teacher matrices M and T
    Matrix<double> M (steps, d);
    Matrix<double> T (steps, tr_series->Dim());
    for (int i=0; i<steps; ++i) {

        for (int j=0; j<d; ++j)  
            M[i][j] = (*this)[i][j]; // rows of M are reservoir states

        for (int j=0; j<tr_series->Dim(); ++j)
            T[i][j] = (*tr_series)[i][j]; //rows of T are training states
    }

    // Compute W_out using Ridge Regression
    W_out = RidgeRegress(T, M, b);
}
// driver program
//
int main() {
    Vector bm_start(2);
    bm_start.random(2);
    int steps = 10;
    double a = 1./3;
    DiscreteTimeSeries* bm = new BakersMap(bm_start, steps, a);
    Vector esn_start(3);
    esn_start.random(3);

    // polymorphism in action! esn takes a EchoStateNetwork*. bm is a BakersMap*, but BakersMap is
    //  a child of EchoStateNetwork, which is an abstract base class with pure virtual function Map()
    EchoStateNetwork esn (esn_start, bm, steps);

    esn.Listen();
    esn.Train();
    esn.PrintW_out();

    const int indices_length = 1;
    int indices[indices_length]= {0};
    esn.Observe(indices, indices_length, steps);

    // find a way to test 
    delete bm;
}
