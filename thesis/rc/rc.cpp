#include "rc.h"
#include "TrimmableLinkedList.h"
#include <iostream>

//#define OFF .5 
//#define EPS .00000001

// DiscreteTimeSeries class method definitions

// constructor
DiscreteTimeSeries::DiscreteTimeSeries (Vector _start, int s )
    // data members
    : prev (0)
    , curr (1)
    , steps (s)
    , d (_start.len())
    , max_d (_start.len())
    , series (new Vector*[s]) 
    , start (_start) {     
                                    
        for (int i=0; i<s; ++i) {
            series[i] = new Vector(d);
        }

        (*this)[0] = start;

    }

// populate series
void DiscreteTimeSeries::Listen () {
    prev = 0;
    curr = 1;
    while (curr<steps) Map();// see pure virtual function in DiscreteTimeSeries for spec.
}

// populate series
void EchoStateNetwork::Listen () {
    in_series->SetPrev(0);
    in_series->SetCurr(1);
    prev = 0;
    curr = 1;
    while (curr<steps) Map();
}
void DiscreteTimeSeries::PrintSeries() {
    for (int i=0; i<steps; ++i) {
        (*this)[i].T().Print();     
    }
}

// load in a 2-d array where the columns are vectors
void EchoStateNetwork::Load(double** arr, int _d, int _L) {
    if ( _d != in_series->Dim() || _L != steps ){
        std::cout << "size of 2d array must match length and dimension of in_series" << std::endl;
        exit(EXIT_FAILURE);
    }

    for (int i=0; i<steps; ++i)
        for (int j=0; j<d; ++j)
           (*in_series)[i][j] = arr[j][i]; 
}

// do what listen does while also writing to o_series
void EchoStateNetwork::Drive(DiscreteTimeSeries* novel_series) {
    int begin = 0;
    Vector temp(in_series->Dim());

    // re-random esn start
    //(*this)[0].random(this->d,-1, 1);

    // replace in_series with novel_series
    delete in_series;
    in_series = novel_series;

    while (curr<steps) {
        // generate response output vector
        temp = W_out * (*this)[prev]; 

        
        // save the response output vector for later so we can compare
        *(o_series[in_series->PrevIndex()]) = temp;


        //Call map to iterate hidden indices
        Map();

    } 

    //one more time to get the last predicted output 
    temp = W_out*(*this)[prev];

    // save the original input for later so we can compare
    *(o_series[in_series->PrevIndex()]) = in_series->Prev();
    (*in_series)[in_series->PrevIndex()] = temp;
    
    //set hidden indices up for subsequent printing/graphing.
    //begin should be the index of the first predicted vector;
    //curr should be the index of the last training vector;
    prev = begin;
    in_series->SetPrev(prev);
    curr = prev+1;
    in_series->SetCurr(curr);


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
        //helpful error message
        if (v.len()!=in_series->Prev().len()) {
            std::cout << "Observe(): invalid Vector assignment. Wrong lengths." << std::endl;
            exit(EXIT_FAILURE);    
        }

        // save the original input for later so we can compare
        *(o_series[in_series->PrevIndex()]) = in_series->Prev();

        //generate approximation to the previous input vector
        v = W_out * (*this)[prev]; 

        // replace whatever components with those from W_out * reservoir_state
        for (int i=0; i<L; ++i) {
            // index of a particular component
            j = comps[i]; 

            // attn!  save original input to o_series
            
            in_series->SetPrevComp(j,v[j]); //sub in the approximated components for the corresponding
                                          // components in the previous input vector, which will be 
                                          // used to generate a new current input vector next time we 
                                          // call Map().
        }
        // Map() should go at the end since Wash() starts leaves curr = first zeroed vector;
        //  prev = curr - 1;
        Map();
    }

    //one more time to get the last predicted output ...

    //generate approximation to the previous input vector
    v = W_out * (*this)[prev]; 
    
    // save the original input for later so we can compare
    *(o_series[in_series->PrevIndex()]) = in_series->Prev();

    // replace whatever components with those from W_out * reservoir_state
        for (int i=0; i<L; ++i) {
            // index of a particular component
            j = comps[i]; 
            in_series->SetPrevComp(j,v[j]); 
        }

    
    //set hidden indices up for subsequent printing/graphing.
    //begin should be the index of the first predicted vector;
    //curr should be the index of the last training vector;
    prev = begin;
    in_series->SetPrev(prev);
    curr = prev+1;
    in_series->SetCurr(curr);

    
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
      , offset (Offset(d)) {

        in_series->SetDim(_in_series->Dim());

        // initialize "current" and "previous" indices of in_series, just in case they're outta
        //  wack. if the indices aren't right then listen() will be wrong
        in_series->SetCurr(1);
        in_series->SetPrev(0);

        //initialize o_series
        o_series = new Vector*[steps];
        for (int i=0; i<steps; ++i)
            o_series[i] = new Vector(in_series->Dim());
        
        // fill W_in with sigma (=1)
        W_in.Fill(sigma);
      }
      

//double LargLyapunov()

// generate random W_in where each entry is 1 or -1
void EchoStateNetwork::RandomW_in() {
    W_in.RandomSigns();
}

// Generate a random W with entries from the interval [-1,1].
//  (EchoStateNetwork::Map() does the multiplying with the spectral radius,
//   as well as the sparsifying)
void EchoStateNetwork::RandomW() {
    double  eig;

    // Calculate largest eigenvalue of random W, and if it's nan, re-random W
    // and recalculate eigenvalue until it is a good value.
    do {
        W.RandomReals();
        eig = abs(W.LargEigvl()); 
    } while (isnan(eig) || eig<EPS);
     
    // Divide by largest eigenvalue 
    W = (1./eig) * W;
}

// should work like Observe(); picks up where Wash() left off.
void EchoStateNetwork::Predict (void) {
    int begin = prev;
    Vector temp(in_series->Dim());
    while (curr<steps) {
        //Swap in approximate input for real one.
        // If we want to define an output function later,
        //  this is where it would be used.
        //W_out.Print();
        //(*this)[prev].Print();
        //std::cout << "ok" << std::endl;
        temp = W_out * (*this)[prev]; 

        //std::cout<<temp.len()<<std::endl;

        // save the original input for later so we can compare
        *(o_series[in_series->PrevIndex()]) = in_series->Prev();

        // swap out each component of the input vector for the predicted input vector.
        //  I use a for loop here just so it looks exactly like Observe()
        for (int i=0; i<temp.len(); ++i) {
            in_series->SetPrevComp(i, temp[i]);
        }

        //in_series->Prev().Print();

        //(*in_series)[in_series->PrevIndex()] = temp;
        //(*in_series)[in_series->PrevIndex()].Print();

        //Call map to iterate hidden indices
        Map();
    } 

    //one more time to get the last predicted output 
    temp = W_out*(*this)[prev];

    // save the original input for later so we can compare
    *(o_series[in_series->PrevIndex()]) = in_series->Prev();
    (*in_series)[in_series->PrevIndex()] = temp;
    
    //set hidden indices up for subsequent printing/graphing.
    //begin should be the index of the first predicted vector;
    //curr should be the index of the last training vector;
    prev = begin;
    in_series->SetPrev(prev);
    curr = prev+1;
    in_series->SetCurr(curr);
}

void EchoStateNetwork::Tune() {
    const int& 
            N_max   = 100
        ,   dN  = 10
        ,   short_run = 10
        ,   long_run  = 500
        ,   wsteps  =   1000
        ,   tsteps  =   1000
        ;

    int     N    = N_max
        ,   N_op = N_max;   
        ;

    const double&
            B_max   = 1
        ,   dB      = .3
        ,   s_max   = 1
        ,   ds      = .3
        ,   p_max   = 1
        ,   dp      = .3
        ;

    double  min_error = DBL_MAX
        ,   B    = B_max
        ,   B_op = B_max
        ,   s    = s_max
        ,   s_op = s_max
        ,   p    = p_max
        ,   p_op = p_max
        ;

    EchoStateNetwork esn (this->start, this->in_series, this->steps);
    RandomW();
    esn.W = W;
    RandomW_in();
    esn.W_in = W_in;
    

    // generate a shuffled array of matrix indices to draw from
    //  at the end of each p iteration
    //
 
   int i, j, k; // k = counter for array of pairs of indices
   i=j=k=0;

    int** picked_pairs = new int*[esn.W.nrow()*esn.W.ncol()];    // to hold randomly selected pairs
    int** picked_pairs_copy = new int*[esn.W.nrow()*esn.W.ncol()];    
    int** ij_pairs = new int*[esn.W.nrow()*esn.W.ncol()];        // to store all pairs.
        

    for (i=0; i<esn.W.nrow()*esn.W.ncol(); ++i) {
        ij_pairs[i] = new int[2];
        picked_pairs_copy[i] = new int[2];
        //picked_pairs[i] = new int[2];
    }

    for (i=0; i<esn.W.nrow(); ++i) {             // Generate [i,j] pairs
        for (j=0; j<esn.W.ncol(); ++j) {
            ij_pairs[k][0] = i;
            ij_pairs[k][1] = j;
            k++;
        }
    }
 
    // randomly select z pairs from all pairs
    RandomSelect<int*>(ij_pairs, p, picked_pairs, N*N);

    for (int i=0; i<N*N; ++i) {
        picked_pairs_copy[i][0] = picked_pairs[i][0];
        picked_pairs_copy[i][1] = picked_pairs[i][1];
    }

    // now use picked_pairs, a shuffled array of int[2],
    //  to make a trimmable linked list, where the key 
    //  for each node is max(i,j), and the data is the
    //  [i,j] pair. it might be possible to skip the
    //  intermediate array picked_pairs, but we'd have
    //  to change RandomSelect so let's just leave it
    //  for now.

    TrimmableLinkedList tll (N), tll_copy(N);

    Node** node_ptrs = new Node*[N*N];
    Node** node_ptrs_copy = new Node*[N*N];

    for (int i=0; i<N*N; ++i) {

        node_ptrs[i] = new Node(picked_pairs[i]);
        tll.Insert(node_ptrs[i]);

        node_ptrs_copy[i] = new Node(picked_pairs_copy[i]);
        tll_copy.Insert(node_ptrs_copy[i]);
        //std::cout<< tll.head->data[0] << std::endl;
    }
    
    // Remove sentinel from tll so it's just a closed loop
    tll.Ring();
    tll_copy.Ring();

    // generate many echo state networks in order to minimize
    //  the error = abs. val. of the difference between the
    //  first short_run predicted/actual terms + difference in abs.
    //  val of long_run predicted/actual terms
    int z1, z0;
    int* ij_pair;
    for (N=N_max; N>1; N-=dN) {

        z1=z0=0;

        for (p=p_max; p>0; p-=dp) {

            esn.SetDens(p);

            // zero entries until the new density p is reached
            for (z1=int((1.-p)*N*N); z0<z1; ++z0) {
                ij_pair = tll.Read();
                esn.W[ij_pair[0]][ij_pair[1]] = 0;
                tll.Next();
            }

            for (s=s_max; s>0; s-=ds) {

                esn.SetSigma(s);

                for (B=B_max; B>0; B-=dB) {

                    esn.SetB(b);

                    // Do basically what lorenz_echo.cpp does,
                    //  but compute error instead of printing.
                    
                    //std::cout<<(*esn.in_series)[0].nrow()<<std::endl;
                    esn.Listen();
                    //std::cout<<"after listen: "<<(*esn.in_series)[0].nrow()<<std::endl;
                    esn.Wash(wsteps);
                    //std::cout<<"after wash: "<<(*esn.in_series)[0].nrow()<<std::endl;
                    esn.Train(tsteps);
                    //std::cout<<"after train: "<<(*esn.in_series)[0].nrow()<<std::endl;
                    esn.Predict();
                    //std::cout<<"after predict: "<<(*esn.in_series)[0].nrow()<<std::endl;

                    double error = 0;

                    // sum the square differences of the first short_run
                    //  points in in_series and o_series
                    for (int i=0; i<short_run; ++i) {
                        for (int j=0; j<esn.In_Series_Dim(); ++j) {
                            error += ( (*esn.in_series)[i][j] - (*esn.o_series[i])[j] ) 
                                  * ( (*esn.in_series)[i][j] - (*esn.o_series[i])[j] );
                        }
                    }

                    double in_max = 0;
                    double o_max = 0;
                    double temp = 0;

                    // get the highest square magnitude term from the short_run number
                    //  of terms starting at the long_runth term of *in_series
                    for (int i=0; i<short_run; ++i) {
                        temp = (*esn.in_series)[long_run-1+i]*(*esn.in_series)[long_run-1+i];;
                        if (temp > in_max)
                            in_max = temp;
                    }

                    // get the max for *o_series
                    for (int i=0; i<short_run; ++i) {
                        temp = (*esn.o_series[long_run-1+i])*(*esn.o_series[long_run-1+i]);
                        if (temp > o_max)
                            o_max = temp;
                    }                    
                    
                    // add the square difference in the maxes to error
                    error += (o_max - in_max) * (o_max - in_max);

                    // If error is smaller than min_error, save new optimal
                    //  values.
                    if (error < min_error) {
                        min_error = error;
                        N_op = N;
                        B_op = B;
                        s_op = s;
                        p_op = p;
                    }

                    // reset initial conditions
                    // fix this -> should work now
                    esn.Reset(); 

                    // progress, for debugging
                    std::cout   << "N: " << N << "   " 
                                << "p: " << p << "   "
                                << "s: " << s << "   "
                                << "B: " << B << "   "
                                << "min_error: " << min_error << "   "
                                << "error: " << error << "   "
                                << std::endl;


                } // end of loop

            } // end of loop
            
            // zero one more entry in W for the next iteration of the p loop
            
        } // end of p loop
        
        // revising. we don't want to do this if N=2
        if (true/*N>2*/) {
            // esn.W is all zeroes now, restore to this->W
            esn.W = W;

            // reduce the internal dimension of all vectors/matrices
            //  by dN.
            esn.Shrink(dN);

            // Shrink this->W too so we can restore esn.W again next
            //  time after it's all zeroed out.
            W.setnrow(N-dN);
            W.setncol(N-dN);
        
            // trim linked list of ij pairs down to include only
            //  i,j <= N-dN
            tll.Trim(N-dN);
        }

    } // end of N loop

    // adopt optimal values
    this->SetB(B_op);
    this->SetSigma(s_op);
    this->SetDens(p_op); 

    // to effectively change N, we have to do all this
    this->d = N_op;
    

    this->W.setnrow(d);
    this->W.setncol(d);
    this->W.compress();
    
    this->W_in.setnrow(d);
    this->W_in.compress();
    
    this->W_out.setncol(d);
    this->W_out.compress();
    esn.W_out.compress();
    this->W_out = esn.W_out;
    
    this->offset.setnrow(d);
    this->offset.compress();

    // reset the first vector to start
    this->Reset();

    // set the size of every vector in series
    for (int i=0; i<steps; ++i) {
        (*this)[i].setnrow(d);
        (*this)[i].compress();
    }

    // put the right number of zeroes in the right spots of this->W
    tll_copy.Trim(d);
    tll_copy.Ring();
    
    for (z0=0, z1=int((1.-p)*W.nrow()*W.ncol()); z0<z1; ++z0) {
        ij_pair = tll_copy.Read();
        W[ij_pair[0]][ij_pair[1]] = 0;
        tll_copy.Next();
    }

    
    //clean up dynamic memory
    for (int i=0; i<esn.W.nrow()*esn.W.ncol(); ++i)
        delete[] ij_pairs[i];
    delete[] ij_pairs;
    delete[] picked_pairs;
    delete[] node_ptrs;
    delete[] node_ptrs_copy;

    //all done. this esn is optimized for prediction
}

void EchoStateNetwork::RidgeTrace(Matrix<double>** trace, int N, double db=.1) {
    b=0;
    for (int i=0; i<N; ++i) {
        Train(steps);
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
    
    (*this)[curr] =  sigma * W_in * in_series->Prev() + spec_rad * W * (*this)[prev] /*+offset*/;  

    in_series->Map();
    prev = curr++;
     
}

// call Wash() before Train
void EchoStateNetwork::Train(int t) {
    // we don't want to train on any zeroes left by Wash
    t = t < curr ? t : curr;

    // fill state and teacher matrices M and T
    Matrix<double> M (t, d);
    Matrix<double> T (t, in_series->Dim());

    // iterate over the nonzero vectors, stopping before the first
    //  zeroed vector left by Wash();
    for (int i=0; i<t; ++i) {

        for (int j=0; j<d; ++j) {
            M[i][j] = (*this)[i][j]; // rows of M are reservoir states
        }

        for (int j=0; j<in_series->Dim(); ++j)
            T[i][j] = (*in_series)[i][j]; //rows of T are training states
    }

    // Compute W_out using Ridge Regression
    //T.Print();
    W_out = RidgeRegress(T, M, b);

    // put last point in series at the front and zero the rest
    
    Vector temp1 = (*in_series)[t-1];
    
    Vector temp2 = (*this)[t-1];

    Wash(steps-1);

    (*in_series)[0] = temp1;

    (*this)[0] = temp2;


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

// save 2-d array of Vectors from DiscreteTimeSeries *sine to a (tposed) 2-d array sine_series
void EchoStateNetwork::Save_Pred (double** pred_series) {
    for (int i=0; i<this->In_Series_Dim(); ++i) {
        for (int j=0; j<this->Steps(); ++j) {
            pred_series[i][j] = this->In_Series(j)[i];
        }
    }
}

// to save original series to compare to predicted series later
void EchoStateNetwork::Save_OG (double** o_series) {
    for (int i=0; i<this->In_Series_Dim(); ++i) {
        for (int j=0; j<this->Steps(); ++j) { 
            o_series[i][j] = this->O_Series(j)[i];
        }
    }
}

