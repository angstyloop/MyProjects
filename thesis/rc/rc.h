#include "MatrixLib.h"
#include <boost/multiprecision/cpp_dec_float.hpp>
typedef boost::multiprecision::cpp_dec_float_50 xdouble;
//typedef double xdouble;
xdouble pi = boost::math::constants::pi<xdouble>();

class DiscreteTimeSeries {
    protected:
        int     prev
            ,   curr
            ,   steps
            ,   d
            ,   max_d
            ;

        Vector** series;
        Vector start;

    public:
        DiscreteTimeSeries ( Vector init_cond, int steps );
        virtual ~DiscreteTimeSeries () {if (series!=nullptr) delete[] series;}
        Vector& operator[] (const int);
        virtual void Map(void) =0; 
            //series[curr] = f (series[prev]);
            //prev = curr++;
        
        // need this here for polymorphism with EchoStateNetwork
        virtual Vector O_Series (int i) { return (*this)[0]; }

        virtual void Wash (int);
        const int& Dim() const {return d;}
        Vector& Curr() {return (*this)[curr];}
        Vector& Prev() {return (*this)[prev];} // eq. to (*this)[PrevIndex()]
        int CurrIndex () {return curr;}
        int PrevIndex () {return prev;}
        void SetCurr(int i) {curr = i;}
        void SetPrev(int i) {prev = i;}
        void SetPrevComp(int i, double val) {(*this)[prev][i] = val;}

        void SetStart (Vector _start) {start=_start;}
        const Vector& GetStart () const {return start;}

        void RandomStart() {start.RandomReals();}
        
        // shrink dimension of vectors in esn series by dN 
        virtual void Shrink(int m) { 
            d = d>m? d-m: 0; 
            start.setnrow(d);
            for ( int i=0; i<steps; ++i ){
                (*this)[i].setnrow(d);
            }
        }

        virtual void Restore() { 
            d = max_d;
            start.setnrow(d);
            for ( int i=0; i<steps; ++i ){
                (*this)[i].setnrow(d);
            }
        }


        void Listen(void);
        void PrintSeries();
        int Steps(void) {return steps;}
};

Matrix<double> RidgeRegress(Matrix<double>, Matrix<double>, double);

class EchoStateNetwork : public DiscreteTimeSeries {
    protected:
        DiscreteTimeSeries* in_series; 
        Vector** o_series;
        Matrix<double> W, W_in, W_out;
        Vector offset;
        double  sigma    = 1        //     
            ,   b        = .00001
            ,   dens     = .5
            ,   spec_rad = 0.9;
    public:
        EchoStateNetwork (Vector, DiscreteTimeSeries*, const int&);
        void Map(void);

        void Save_Pred(double**);
        void Save_OG(double**);
        //fill M with rowvectors obt
        void Train(int);
        void Observe(int[], int);
        void PrintW_out(void) {W_out.Print();}
        void PrintTr_Series(void);
        void PrintPred_Series(void);
        Vector In_Series(int i) {return (*in_series)[i];}
        Vector O_Series (int i) {return *(o_series[i]);}
        void RidgeTrace(Matrix<double>**, int, double);
        void Wash(int);

        void Predict(void);
        void Tune();

        virtual void Shrink(int m) {
            // shrink dimension of esn vectors in time series by m
            DiscreteTimeSeries::Shrink(m);

            // shrink ncol and nrow of W
            W.setnrow(d);
            W.setncol(d);

            // shrink nrow of W_in
            W_in.setnrow(d);

            // shrink ncol of W_out
            W_out.setncol(d); 

            // shrink dimension of offset vector
            offset.setnrow(d);

            // shrink dimensions of initial condition
            start.setnrow(d);
        }

        virtual void Restore() {
            DiscreteTimeSeries::Restore();
            W.setnrow(d);
            W.setncol(d);
            W_out.setncol(d);
            offset.setnrow(d);;
        }
        
        // setters and getters

        void SetSpecRad (double rad) {spec_rad = rad;};
        const double& SpecRad (void) const {return spec_rad;}

        void SetB (double _b) {b=_b;}
        const double& GetB () const {return b;}

        void SetSigma (double _sigma) {sigma=_sigma;};
        const double& GetSigma () const {return sigma;}

        void SetDens (double _dens) {dens=_dens;}
        const double& GetDens () const {return dens;}

        // generate random W_in where each entry is -1 or 1
        void RandomW_in ();

        // generate random internal matrix from interval [-1,1]
        void RandomW ();
        
        // generate random W and W_in from a real interval (instead of int)
        void RandomParms (double, double, double); 

        double PlotRidgeTrace();
        void Listen();
        void Load(double**, int, int);
        void Drive(DiscreteTimeSeries*);
        int In_Series_Dim (void) {return in_series->Dim();}
};

class BakersMap : public DiscreteTimeSeries {
    protected:
        double a, c;
        xdouble x=xdouble(3)/xdouble(255);
        xdouble y=xdouble(192)/xdouble(255);

    public:
        BakersMap (Vector start, int steps, double _a, double _c)
            : DiscreteTimeSeries (start, steps), a (_a), c (_c) {}                
            
        void SetC (double _c) {c=_c;}
        double GetC () {return c;}
        void Map(void); 
};

// Ridge Regresssion Algorithm
// T : teacher matrix
// M : state matrix
// b : RR parameter
// returns the matrix (W_out) that minimizeds the RR error function 
Matrix<double> RidgeRegress(Matrix<double> T, Matrix<double> M, double b) {
    //build the matrix M * M.T() + b * I and invert
    Matrix<double> temp = M.T()*M;
    for (int i=0; i<temp.ncol(); ++i)
        temp[i][i] += b;
    // invert temp and do the remaining matrix multiplications
    return T.T() * M * temp.inv_cholesky();
}

 // this is an important overload, since Vector** series is an array of Vector*,
 //   and the syntax to return a vector is cumbersome.
 Vector& DiscreteTimeSeries::operator[] (int i) {
     if (i>=0 && i<steps)
         return *(series[i]);
     std::cout << "DiscreteTimeSeries: index out of range." << std::endl;
     exit (EXIT_FAILURE);  
 }

// convert a double to a 1-d Vector
 Vector doubleToVector(double c) {
    Vector v (1);
    v[0] = c;
    return v;
 }

// a generic class for functions of a single variable like sin(x)
class ScalarFunction : public DiscreteTimeSeries {
    // step size for generating {sin(i*stp_z)}
    double stp_sz;
    // generating function
    double (*f)(double);
    public:
        // constructor takes a function pointer and initializes base class
        ScalarFunction (double (*_f)(double), double _start, int _steps, double _stp_sz) 
            : DiscreteTimeSeries(doubleToVector(_start), _steps)
              , stp_sz (_stp_sz) 
              , f (_f) {}

        // pure virtual in DiscreteTimeSeries
        void Map(void);            
        double GetStepSize() {return stp_sz;}
        void SetStepSize(double _stp_sz) {stp_sz=_stp_sz;}
};

class DataSeries : public DiscreteTimeSeries {
    public:
        DataSeries (int _d, int _steps)
            : DiscreteTimeSeries(Vector::ZeroVector(_d), _steps) {}

        void Map() {prev=curr++;}

};

