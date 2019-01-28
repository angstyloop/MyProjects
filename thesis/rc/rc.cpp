#include "rc.h"
#include <iostream>

// DiscreteTimeSeries class method definitions

// constructor
DiscreteTimeSeries::DiscreteTimeSeries (Vector start, const int& s )
    // data members
    : prev (0)
    , curr (1)
    , steps (s)
    , d (start.len())
    , series (new Vector*[s]) {     //This line right here is the reason series is a Vector**
                                    //  instead of a Vector[]; I can't write stuff like
                                    //  new Vector(d)[s], and I want dynamically sized vectors.
                                    //  A different approach would be to make Vector a template class,
                                    //  and then use a Vector of Vector.
                                    
        // initialize the vectors pointed to by the entries of series[]
        int l = start.len();
        for (int i=0; i<s; ++i) {
            series[i] = new Vector(l);
        }
        // make series[0] point to start. see DiscreteTimeSeries::operator[].
        (*this)[0] = start;
    }

// populate series
void DiscreteTimeSeries::Listen () {
    curr = 1;
    prev = 0;
    while (curr<steps) Map();// see pure virtual function in DiscreteTimeSeries for spec.
}

void DiscreteTimeSeries::PrintSeries() {
    for (int i=0; i<steps; ++i) {
        (*this)[i].T().print();     
    }
}

// observes comps components for psteps steps with the others being continually provided.
//   this approach works only in the artificial scenario where the series map is known.
//   I still need to create a child of DiscreteTimeSeries to serve as a container for arbitrary
//   input sequences.
void EchoStateNetwork::Observe(int* comps, int L, int psteps) {

    //demand psteps < steps so we don't need to reallocate
    if (psteps<steps) {
        std::cout << "Unable to observe more steps than are in the training series." 
                  << " Consider reallocating or writing if you need a longer observation period."
                  << std::endl;
        exit(EXIT_FAILURE);
    }
    //temp vars
    int j; //this will hold index of component we're interested in observing
    Vector v(tr_series->Dim()); //this will hold approximations to input vectors

    (*this)[0] = (*this)[prev];             //move the last reservoir state to the start of the series
    (*tr_series)[0] = tr_series->Prev();    //do the same for the training series

    //reset training series indices
    tr_series->SetCurr(1);       
    tr_series->SetPrev(0);        

    //reset reservoir series indices
    curr = 1;   
    prev = 0;  

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
    //if (x<-.5 || x>.5 || y<-.5 || y>.5) {
    //    std::cout << "BakersMap: Domain Error: domain is the unit square centered at (0,0)." << std::endl;
    //    exit(EXIT_FAILURE);
    //}
    if (x>0) {
        temp[0] = (2*(x+.5)-1)-.5;
        temp[1] = (a*(y+.5)+.5)-.5;
    } else {
        temp[0] = (2*(x+.5))-.5;
        temp[1] = (a*(y+.5))-.5;
    }
    (*this)[curr] = temp;
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
//     note: remember start lives in the reservoir state space (the big one)
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

void EchoStateNetwork::PrintTr_Series (void) {
    for (int i=0; i<steps; ++i)
        (*tr_series)[i].T().print();
}

void EchoStateNetwork::Map (void) { 
    
    // tr_series lags this->series by 1. series[0] is r_0. tr_series[0] is the first input
    (*this)[curr] =  W_in * tr_series->Prev() + W * (*this)[prev] + offset;  
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

// Driver program to test everything. Comment this out and leave it when finished testing.
int main() {
    Vector bm_start(2);
    bm_start.random(2,-.5,.5);
    //bm_start.print();
    
    int steps=20, N=3;
    double a = 1./3;

    //bm will be an argument to esn, be sure to delete later
    DiscreteTimeSeries* bm = new BakersMap(bm_start, steps, a);

    Vector esn_start(N);
    esn_start.random(N, -.5,.5);
    //esn_start.print();

    // polymorphism in action! esn takes a EchoStateNetwork*. bm is a BakersMap*, but BakersMap is
    //  a child of EchoStateNetwork, which is an abstract base class with pure virtual function Map()

    // change ESN ctr to let you pick the starting point. that way we can compare real inputs
    //  to observed ones
    EchoStateNetwork esn (esn_start, bm, steps);

    esn.Listen();
    //esn.PrintSeries();
    //std::cout << std::endl;
    esn.PrintTr_Series();

    std::cout << std::endl;

    esn.Train();

    //works, just commenting out for now
    //esn.PrintW_out();

    // indices is an array of indices of input vectors that we want to observe
    const int indices_length = 1;
    int indices[indices_length]= {0}; //observe the x component

    esn.Observe(indices, indices_length, steps);
    esn.PrintTr_Series();

    //make regression parameter b initiliazable via EchoStateNetwork ctr.
    
    // print ridge trace to pick b
    
    // find a way to test 

       


    // clean up dyn. alloc. memory
    //delete bm;
}
