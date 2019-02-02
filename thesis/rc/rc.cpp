#include "rc.h"
#include <iostream>

#define HT_EPS .0001
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
Vector& tanh(Vector& v) {
    for (int i=0; i<v.len(); ++i)
        v[i] = tanh(v[i]);
    return v;
}

// inverse vector hyperbolic tangent
Vector atanh(const Vector& v) {
    Vector temp(v.len());
    for (int i=0; i<v.len(); ++i)
        temp[i] = atanh(v[i]);
    return temp;
}

// EchoStateNetwork class defs

// constructor
//     note: remember start lives in the reservoir state space (the big one)
EchoStateNetwork::EchoStateNetwork (Vector start, DiscreteTimeSeries* _in_series , const int& _steps)
    : DiscreteTimeSeries(start, _steps)
      , in_series (_in_series)
      , W (d, d)
      , W_in (d, _in_series->Dim())
      //, W_out (_in_series->Dim(), d)
      , offset (Offset(this->Dim()))
      , b (0.1) {} // seems like a fine value for the ridge regression parameter
      

// Output the ridge trace as a 2-d array where...
//  > the columns are the components (flattened 
//      row by row, appending on the right) of
//      W_out for a given b value;
//  > the rows are a single component of W_out
//      varied over a range of b.
//

void EchoStateNetwork::RandomParms(double W_in_dens, double W_dens){
    double c = W_in_dens*W_in.ncol*W_in.nrow;
    W_in_dens>.5 ? W.DenseRandom(floor(c))
                 : W_in.random(floor(c),-1, 1); 
    
    // Calculate largest eigenvalue of rand W, and if it's nan, re-random W
    // and recalculate eigenvalue until it is a good value.
    do {
        c= W_dens*W.ncol*W.nrow;
        W_dens>.5 ? W.DenseRandom(c)
                  : W.random(floor(c),-1,1); //The densty really affects inv alg
        c = abs(W.LargEigvl()); 
    } while (isnan(c) || c<EPS);
     
    // Divide by largest eigenvalue and multiply by spec_rad<1 to ensure
    //    echo state property. spec_rad set to .9 by default. use getter/
    //    setter methods to change it.
    for (int i=0; i<W.nrow; ++i)
        for (int j=0; j<W.ncol; ++j)
            W[i][j] *= spec_rad/c;
    //W.Print();
}

// should work like Observe(); picks up where Wash() left off.
void EchoStateNetwork::Predict (void) {
    int begin = prev;
    Vector temp(d);
    while (curr<steps) {
        //Swap in approximate input for real one.
        // If we want to define an output function later,
        //  this is where it would be used.
        temp = W_out * (*this)[prev]; 
        (*in_series)[in_series->PrevIndex()] = temp;
        //Call map to iterate hidden indices
        Map();
    } 
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
//
// Driver program to test everything. Comment this out and leave it when finished testing.
/*
int main() {
    /////////////////////////////////////////////
    ////          Sine Wave Generator        ////
    /////////////////////////////////////////////
    int psteps = 50;               //target number of predicted steps
    int tsteps = 100;               //target number of training steps
    int steps = psteps + tsteps;    //number of total steps
    int N = 3;                      //number of nodes
    Vector esn_start(N);            //reservoir initial state
    esn_start.random(N, -1, 1);
    //esn_start.Print();

    // Generate sine output, wash it, and put it in an array.
    DiscreteTimeSeries* sine = new ScalarFunction(sin, 0, steps);
    // dimension of input vectors (for ScalarFunction objects d = 1) 
    int d = sine->Dim();
    sine->Listen(); 
    //sine->PrintSeries();

    // Make a 2d array, where each row is a sequence of the i+1th component of
    //  vectors in the sin series, exclude the the first with psteps vectors
    //  in each row (i.e. start pulling from *sine at tsteps)
    double** sin_series = new double*[d];

    // fill it up
    for (int i=0; i<d; ++i) {
        sin_series[i] = new double[psteps];
        for (int j=psteps; j<steps; ++j) {
            sin_series[i][j-psteps] = (*sine)[j][i];   
        }
    } 
    
    //print to check; OKAY
    //for (int i=0; i<d; ++i) {
    //    for (int j=0; j<psteps; ++j)
    //        std::cout << sin_series[i][j] << std::endl;
    //    std::cout << std::endl;
    //}

    // delete so we can reuse sine 
    delete sine;

    sine = new ScalarFunction(sin, 0, steps);
    EchoStateNetwork esn (esn_start, sine, steps);
    esn.Listen();
    esn.Wash(psteps);
    esn.Train();//&&&
    //esn.PrintSeries();
    esn.Predict();
    //esn.PrintTr_Series();
    //std::cout << std::endl;
    //esn.PrintPred_Series();
    //std::cout<<std::endl;
    // Make a 2d array, where each row is a sequence of the i+1th component of
    //  vectors in the predicted series.
    
    double** pred_series = new double*[d];

    // fill it up
    for (int i=0; i<d; ++i) {
        pred_series[i] = new double[psteps];
        for (int j=tsteps; j<steps; ++j) {
            pred_series[i][j-tsteps] = (*sine)[j][i];   
        }
    } 

    //print to check; OKAY
    for (int i=0; i<d; ++i) {
        for (int j=0; j<psteps; ++j)
            std::cout << pred_series[i][j] << "   " << sin_series[i][j] << std::endl;
        std::cout << std::endl;
     }

    



    /////////////////////////////////////////////
    
    //Vector bm_start(2);
    //bm_start.random(2,-.5,.5);
    //bm_start.Print();
    
    //int steps=10, N=50;
    //double a = 1./3;

    //bm will be an argument to esn, be sure to delete later
    //DiscreteTimeSeries* bm = new BakersMap(bm_start, steps, a);
    //EchoStateNetwork esn (esn_start, bm, steps);

    //esn.Listen();

    /////////////////////////////////////////////
    //// Testing EchoStateNetwork::Observe() ////
    /////////////////////////////////////////////
    
    // indices is an array of indices of input vectors that we want to observe
    //const int indices_length = 1;
    //int indices[indices_length]= {1}; //observe the x component

    // starts observing at index curr, which is where Wash() leaves off
    //esn.Observe(indices, indices_length);

    // save the new in_series (with observed inputs)
    //double obs_x[steps];
    //for (int i=0; i<steps; ++i)
    //    obs_x[i] = (esn.In_Series(i))[0];

    //  print the actual and observed x values side by side
    //for (int i=esn.CurrIndex; i<steps; ++i) {
    //    std::cout << actual_x[i] << "              " << obs_x[i] << std::endl;
    //    ;
    //}
  
    //do something with the actual and observed xvalues with DISLIN


    ////////////////////////////
    //// Testing RidgeTrace ////
    /////////////////////////////
    
    // goal: print ridge trace to facilitate the selection of b
    
    // number of increments
    int num_inc=50;

    // allocate an array of Matrix pointers to hold the W_out's; delete later;
    Matrix<double>** trace = new Matrix<double>*[num_inc]; 
    for (int i=0; i<num_inc; ++i)
        trace[i] = new Matrix<double>(sine->Dim(), N); // dimensions of W_out:
                                                     //(input_dimension, reservoir_dimension)

    // get num_inc W_out's into trace 
    esn.RidgeTrace(trace, num_inc);    
   
    // now we need to get make an array for each entry in W_out
    // lets make an array of these arrays (we're flattening out W_out over 50 b steps)
    double** W_out_entries = new double*[sine->Dim() * N]; // [input_dim * res_dim]
    for (int i=0; i<sine->Dim(); ++i) {
        for (int j=0; j<N; ++j) {
            W_out_entries[i*N+j] = new double[num_inc];
            for (int k=0; k<num_inc; ++k) {
                W_out_entries[i*N+j][k] = (*trace[k])[i][j];
            }
        }
    }
  
    // print to make sure W_out_entries looks good... OKAY
    for (int i=0; i<sine->Dim()*N; ++i){
        std::cout << std::endl;
        for (int j=0; j<num_inc; ++j)
            std::cout << W_out_entries[i][j] << " ";
    }

    // each of these arrays we can feed to our dislin routine. hopefully we can get them all plotted
    //  on the same graph

    // clean up dyn. alloc. memory
    //delete bm;
    delete sine;
    for (int i=0; i<num_inc; ++i)
        delete trace[i];
    delete[] trace;
    //for (int i=0; i<num_inc; ++i)
    //delete[] W_out_entries;
}*/
