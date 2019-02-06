#include "rc.h"
#include <iostream>

//#define OFF .5 
//#define EPS .00000001

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

// populate series
void EchoStateNetwork::Listen () {
    in_series->SetCurr(1);
    in_series->SetPrev(0);
    DiscreteTimeSeries::Listen();
}
void DiscreteTimeSeries::PrintSeries() {
    for (int i=0; i<steps; ++i) {
        (*this)[i].T().Print();     
    }
}


// observes comps components starting at index curr and continuing until the end of series is
//   reached, with the other components being continually provided.
//
//   this approach works only in the artificial scenario where the series map is known.
//   I still need to create a child of DiscreteTimeSeries to serve as a container for arbitrary
//   input sequences.
//
void EchoStateNetwork::Observe(int* comps, int L) {
    int begin = prev;

    // note if prev >= steps, we won't observe anything! and we'll segfault.
    if (begin>=steps) {
        std::cout << "EchoStateNetwork::Observe: error: required begin < steps." << std::endl;
        exit(EXIT_FAILURE);
    }

    //this will hold index of component we're interested in observing
    int j; 
    //this will hold approximations to input vectors
    Vector v(in_series->Dim()); 

    // edit: Wash() already does this
    //set training and reservoir series indices so that we start observing at index begin
    //in_series->SetCurr(begin+1);       
    //in_series->SetPrev(begin);        
    //curr = begin+1;   

    // populate the series, swapping out the appropriate components of the input vectors
    //  after each step
    while (curr<steps) {
        // edit: this should go at the end 
        ////remember, this increments curr and thus controls the loop
        //Map(); 
        // replace whatever components with those from W_out * reservoir_state
        for (int i=0; i<L; ++i) {
            // index of a particular component
            j = comps[i]; 
            //generate approximation to the previous input vector
            v = W_out * (*this)[prev]; 
            //helpful error message
            if (v.len()!=in_series->Prev().len()) {
                std::cout << "Observe(): invalid Vector assignment. Wrong lengths." << std::endl;
                exit(EXIT_FAILURE);    
            }

            in_series->Prev()[j] = v[j]; //sub in the approximated components for the corresponding
                                          // components in the previous input vector, which will be 
                                          // used to generate a new current input vector next time we 
                                          // call Map().
        }
        // Map() should go at the end since Wash() starts leaves curr = first zeroed vector;
        //  prev = curr - 1;
        Map();
    }
    // reset the hidden indices, so we can graph real vs. observed starting at begin+1.
    prev = begin;
    curr = prev + 1;
}

// wash out the first n points in series
void DiscreteTimeSeries::Wash (int n) {

    // move the last steps-n vectors to the front of series
    for (int i=n; i<steps; ++i) {
        (*this)[i-n] = (*this)[i];
    }
    // make a zero vector
    Vector temp(d);
    temp.Fill(EPS);

    // zero out the remaining n vectors
    for (int i=steps-n; i<steps; ++i)
        (*this)[i] = temp;

    // set curr to the index of the first zeroed vector
    // set prev just before that
    curr = steps-n;
    prev = curr-1;
    
    //now the indices are ready for an Observe() call
}

// BakersMap class method definitions
void BakersMap::Map (void) {
    using namespace std;
    //const double& x=(*this)[prev][0], y=(*this)[prev][1], a=param;
    //Vector temp(2);
    /*if (x<-c/2 || x>c/2 || y<-c/2 || y>c/2) {
        std::cout << "x: " << x << ", y: "<< y<< std::endl;
        std::cout << "BakersMap: Domain Error: domain is the unit square centered at (0,0)." << std::endl;
        exit(EXIT_FAILURE);
    }*/
    
    if (x<0) {
        x = 2*x+c/2;
        y = a*(y+c/2)-c/2;
    } else {
        x = 2*x-c/2; 
        y = a*(y+c/2);
    }

    //(*this)[curr] = temp;
    (*this)[curr][0] = double(x);
    (*this)[curr][1] = double(y);
    prev = curr++;
}

// EchoStateNetwork class defs

// constructor
//     note: remember start lives in the reservoir state space (the big one)
EchoStateNetwork::EchoStateNetwork (Vector start, DiscreteTimeSeries* _in_series , const int& _steps)
    : DiscreteTimeSeries(start, _steps)
      , in_series (_in_series)
      , W (d, d)
      , W_in (d, _in_series->Dim())
      , W_out (_in_series->Dim(), d)
      , offset (Offset(this->Dim())) {}
      


void EchoStateNetwork::RandomParms(double W_in_dens, double W_dens){
    double eig;
    double c = W_in_dens*W_in.ncol*W_in.nrow;
    W_in_dens>.5 ? W.DenseRandom(floor(c))
                 : W_in.random(floor(c),-1, 1); 
    
    // Calculate largest eigenvalue of rand W, and if it's nan, re-random W
    // and recalculate eigenvalue until it is a good value.
    do {
        c = W_dens*W.ncol*W.nrow;
        W_dens>.5 ? W.DenseRandom(int(floor(c)))
                  : W.random(floor(c),-1,1); //The densty really affects inv alg
        eig = abs(W.LargEigvl()); 
    } while (isnan(eig) || eig<EPS);
     
    // Divide by largest eigenvalue and multiply by spec_rad<1 to ensure
    //    echo state property. spec_rad set to .9 by default. use getter/
    //    setter methods to change it.
    for (int i=0; i<W.nrow; ++i)
        for (int j=0; j<W.ncol; ++j)
            W[i][j] *= spec_rad/c;
}

// should work like Observe(); picks up where Wash() left off.
void EchoStateNetwork::Predict (void) {
    int begin = prev;
    Vector temp(in_series->Dim());
    while (curr<steps) {
        //Swap in approximate input for real one.
        // If we want to define an output function later,
        //  this is where it would be used.
        //W_out.Print(0);
        temp = W_out * (*this)[prev]; 

        //for bakers map the output function is a tanh
        //for (int i=0; i<temp.len(); ++i) {
        //    temp[i] = tanh(temp[i]);
        //}
        
        (*in_series)[in_series->PrevIndex()] = temp;
        //(*in_series)[in_series->PrevIndex()].Print();
        //Call map to iterate hidden indices
        Map();
    } 

    temp = W_out*(*this)[prev];
    //for bakers map 
    //for (int i=0; i<temp.len(); ++i) 
    //    temp[i] = tanh(temp[i]);
    (*in_series)[in_series->PrevIndex()] = temp;
    
    //set hidden indices up for subsequent printing/graphing.
    //begin should be the index of the first predicted vector;
    //curr should be the index of the last training vector;
    prev = begin;
    in_series->SetPrev(prev);
    curr = prev+1;
    in_series->SetCurr(curr);
}

void EchoStateNetwork::RidgeTrace(Matrix<double>** trace, int N, double db=.1) {
    b=0;
    for (int i=0; i<N; ++i) {
        Train();
        // the thing pointed to by the pointer pointed to by (trace+i)
        *trace[i] = W_out;
        b+=db;
    }
    b=0;
}

void EchoStateNetwork::Wash(int n) {
    this->DiscreteTimeSeries::Wash(n);
    in_series->DiscreteTimeSeries::Wash(n);
}

// call after Listen(), Observe() or Train()
// prints all the training inputs
void EchoStateNetwork::PrintTr_Series (void) {
    for (int i=0; i<curr; ++i)
        (*in_series)[i].T().Print();
}

// same as for PrintTr_Series, except prints all the predicted outputs
void EchoStateNetwork::PrintPred_Series (void) {
    for (int i=curr; i<steps; ++i)
        (*in_series)[i].T().Print();
}

void EchoStateNetwork::Map (void) { 
    
    // in_series lags this->series by 1. series[0] is r_0. in_series[0] is the first input
    (*this)[curr] =  W_in * in_series->Prev() + W * (*this)[prev] + offset;  
    in_series->Map();
    prev = curr++;
     
}

// call Wash() before Train
void EchoStateNetwork::Train(void) {
    // fill state and teacher matrices M and T
    Matrix<double> M (curr, d);
    Matrix<double> T (curr, in_series->Dim());
    // iterate over the nonzero vectors, stopping before the first
    //  zeroed vector left by Wash();
    for (int i=0; i<curr; ++i) {

        for (int j=0; j<d; ++j) {
            M[i][j] = (*this)[i][j]; // rows of M are reservoir states
        }

        for (int j=0; j<in_series->Dim(); ++j)
            T[i][j] = (*in_series)[i][j]; //rows of T are training states
    }

    // Compute W_out using Ridge Regression
    //T.Print();
    W_out = RidgeRegress(T, M, b);
}


/////////////////////////////////////////////
////       ScalarFunction class methods  ////
/////////////////////////////////////////////

//constructor defined in rc.h

// pure virtual in base class
void ScalarFunction::Map(void) {
    const double start = (*this)[0][0];
    (*this)[curr][0] = f(start + curr * stp_sz);
    prev = curr++;
}
