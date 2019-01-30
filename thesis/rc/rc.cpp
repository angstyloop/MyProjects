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
        (*this)[i].T().Print();     
    }
}


// observes comps components starting at index begin and continuing until the end of series is
//   reached, with the other components being continually provided.
//
//   this approach works only in the artificial scenario where the series map is known.
//   I still need to create a child of DiscreteTimeSeries to serve as a container for arbitrary
//   input sequences.
//
void EchoStateNetwork::Observe(int* comps, int L, int begin) {
    
    // if begin >= steps, we won't observe anything! and we'll segfault.
    if (begin>=steps) {
        std::cout << "EchoStateNetwork::Observe: error: required begin < steps." << std::endl;
        exit(EXIT_FAILURE);
    }

    //this will hold index of component we're interested in observing
    int j; 
    //this will hold approximations to input vectors
    Vector v(tr_series->Dim()); 

    //set training and reservoir series indices so that we start observing at index begin
    tr_series->SetCurr(begin+1);       
    tr_series->SetPrev(begin);        
    curr = begin+1;   
    prev = begin;  

    // populate the series, swapping out the appropriate components of the input vectors
    //  after each step
    while (curr<steps) {
        //remember, this increments curr and thus controls the loop
        Map(); 
        // replace whatever components with those from W_out * reservoir_state
        for (int i=0; i<L; ++i) {
            // index of a particular component
            j = comps[i]; 
            //generate approximation to the previous input vector
            v = W_out * (*this)[prev]; 
            //helpful error message
            if (v.len()!=tr_series->Prev().len()) {
                std::cout << "Observe(): invalid Vector assignment. Wrong lengths." << std::endl;
                exit(EXIT_FAILURE);    
            }

            tr_series->Prev()[j] = v[j]; //sub in the approximated components for the corresponding
                                          // components in the previous input vector, which will be 
                                          // used to generate a new current input vector next time we 
                                          // call Map().
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
    if (x<-.5 || x>.5 || y<-.5 || y>.5) {
        std::cout << "BakersMap: Domain Error: domain is the unit square centered at (0,0)." << std::endl;
        exit(EXIT_FAILURE);
    }
    if (x>0) {
        temp[0] = 2*x-.5; 
        temp[1] = a*(y+.5);
    } else {
        temp[0] = 2*x+.5;
        temp[1] = a*(y+.5)-.5;
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
          W_in.random(W_in.ncol*W_in.nrow/2,-1, 1); // use density = half the entries
          W.random(100,-1,1); //the densty really influences the cholesky and fwsub algorithms
          //W_in.Print();
          //W.Print();
          _tr_series = nullptr;
      }

void EchoStateNetwork::RidgeTrace(Matrix<double>** trace, int N) {
    const int lim=10;
    const double db = lim * 1.0 / N; 
    b=0;

    for (int i=0; i<N; ++i) {
        Train();
        // the thing pointed to by the pointer pointed to by (trace+i)
        *trace[i] = W_out;
        b+=db;
    }
}

void EchoStateNetwork::PrintTr_Series (void) {
    for (int i=0; i<steps; ++i)
        (*tr_series)[i].T().Print();
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

///////////////////////////////////////////
////////////////////////////////////////////
// Tag &&&
//
// Driver program to test everything. Comment this out and leave it when finished testing.
int main() {
    Vector bm_start(2);
    bm_start.random(2,-.5,.5);
    //bm_start.Print();
    
    int steps=200, N=100;
    double a = 1./3;

    //bm will be an argument to esn, be sure to delete later
    //DiscreteTimeSeries* bm = new BakersMap(bm_start, steps, a);

    /////////////////////////////////
    //// Testing Scalar Function ////
    /////////////////////////////////

    // let's use the sin function instead of bakers map
    double (*f)(double);
    f = sin;
    ScalarFunction sin_func(f, 0, steps);  




    Vector esn_start(N);
    esn_start.random(N, -1,1);
    //esn_start.Print();

    // polymorphism in action! esn takes a EchoStateNetwork*. bm is a BakersMap*, but BakersMap is
    //  a child of EchoStateNetwork, which is an abstract base class with pure virtual function Map()
    EchoStateNetwork esn (esn_start, &sin_func/*bm*/, steps);

    esn.Listen();
    //esn.PrintSeries();
    //std::cout << std::endl;
    esn.PrintTr_Series();
    //std::cout << std::endl;

    // save the x inputs to compare to observed
    double actual_x[steps];
    for (int i=0; i<steps; ++i)
        actual_x[i] = (esn.Tr_Series(i))[0];

    //////////////////////////////////////////
    //// Testing EchoStateNetwork::Train()////
    //////////////////////////////////////////

    //esn.Train();

    //works, just commenting out for now
    //esn.PrintW_out();

    /////////////////////////////////////////////
    //// Testing EchoStateNetwork::Observe() ////
    /////////////////////////////////////////////
    
    // indices is an array of indices of input vectors that we want to observe
    const int indices_length = 1;
    int indices[indices_length]= {1}; //observe the x component

    // start observing at the middle step
    //esn.Observe(indices, indices_length, steps/2-1);

    // save the new tr_series (with observed inputs)
    double obs_x[steps];
    for (int i=0; i<steps; ++i)
        obs_x[i] = (esn.Tr_Series(i))[1];

    // starting at steps/2-1 print the actual and observed x values side by side
    for (int i=steps/2-1; i<steps; ++i) {
        std::cout << actual_x[i] << "              " << obs_x[i] << std::endl;
        ;
    }
   


    ////////////////////////////
    //// Testing RidgeTrace ////
    ////////////////////////////
    
    // goal: print ridge trace to facilitate the selection of b
    
    // number of increments
    //int num_inc=50;

    // allocate an array of Matrix pointers to hold the W_out's; delete later;
    //Matrix<double>** trace = new Matrix<double>*[num_inc]; 
    //for (int i=0; i<num_inc; ++i)
    //    trace[i] = new Matrix<double>(2, N); // dimensions of W_out:
                                             //(input_dimension, reservoir_dimension)

    // get num_inc W_out's into trace 
    //esn.RidgeTrace(trace, num_inc);    

    //iterate through trace and print each W_out
    //for (int i=0; i<num_inc; ++i) {
        //trace[i]->Print(); 
        //std::cout << std::endl;
    //    ;
    //}
   
    // it prints! now we need to get make an array for each entry in W_out
    // lets make an array of these arrays (we're flattening out W_out over 50 b steps)
    //double** W_out_entries = new double*[2 * N]; // [input_dim * res_dim]
    //for (int i=0; i<2*N; ++i) {
    //    W_out_entries[i] = new double[num_inc];
    //}
      
    // each of these arrays we can feed to our dislin routine. hopefully we can get them all plotted
    //  on the same graph

    // clean up dyn. alloc. memory
    //delete bm;
    //delete[] trace;
    //for (int i=0; i<num_inc;++i)
    //    delete trace[i];
}
