#include "MatrixLib.h"

class DiscreteTimeSeries {
    protected:
        int prev, curr, steps, d;
        Vector** series;
    public:
        DiscreteTimeSeries ( Vector, const int& );
        virtual ~DiscreteTimeSeries () {if (series!=nullptr) delete[] series;}
        Vector& operator[] (const int);
        virtual void Map(void) =0; 
            //series[curr] = f (series[prev]);;
            //prev = curr++;=0
            
        virtual void Wash (int);
        const int& Dim() const {return d;}
        Vector Curr() {return (*this)[curr];}
        Vector Prev() {return (*this)[prev];}
        int CurrIndex () {return curr;}
        int PrevIndex () {return prev;}
        void SetCurr(int i) {curr = i;}
        void SetPrev(int i) {prev = i;}
        void Listen(void);
        void PrintSeries();
};

Matrix<xdouble> RidgeRegress(Matrix<xdouble>, Matrix<xdouble>, xdouble);

class EchoStateNetwork : public DiscreteTimeSeries {
    protected:
        DiscreteTimeSeries* in_series; 
        Matrix<xdouble> W, W_in, W_out;
        const Vector offset;
        xdouble b;
        xdouble spec_rad = 0.9;
    public:
        EchoStateNetwork (Vector, DiscreteTimeSeries*, const int&);
        void Map(void);
        //fill M with rowvectors obt
        void Train(void);
        void Observe(int[], int);
        void PrintW_out(void) {W_out.Print();}
        void PrintTr_Series(void);
        void PrintPred_Series(void);
        Vector& In_Series(int i) {return (*in_series)[i];}
        void RidgeTrace(Matrix<xdouble>**, int, xdouble);
        void Wash(int);
        void Predict(void);
        void SetSpecRad (xdouble rad) {spec_rad = rad;};
        xdouble SpecRad (void) {return spec_rad;}

        void SetB (xdouble _b) {b=_b;}
        xdouble GetB () {return b;}
        void RandomParms (double, double); 
        xdouble PlotRidgeTrace();
        void Listen();
};

class BakersMap : public DiscreteTimeSeries {
    protected:
        xdouble param, c;
    public:
        BakersMap (Vector start, int steps, xdouble a, xdouble _c)
            : DiscreteTimeSeries (start, steps), param (a), c (_c) {}                
            
        void SetC (xdouble _c) {c=_c;}
        xdouble GetC () {return c;}
        void Map(void); 
};

// Ridge Regresssion Algorithm
// T : teacher matrix
// M : state matrix
// b : RR parameter
// returns the matrix (W_out) that minimizeds the RR error function 
Matrix<xdouble> RidgeRegress(Matrix<xdouble> T, Matrix<xdouble> M, xdouble b) {
    //build the matrix M * M.T() + b * I and invert
    Matrix<xdouble> temp = M.T()*M;
    for (int i=0; i<temp.ncol; ++i)
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

// convert a xdouble to a 1-d Vector
 const Vector xdoubleToVector(xdouble d) {
    Vector* v = new Vector(1);
    (*v)[0] = d;
    return *v;
 }

// a generic class for functions of a single variable like sin(x)
class ScalarFunction : public DiscreteTimeSeries {
    // step size for generating {sin(i*stp_z)}
    xdouble stp_sz;
    // generating function
    xdouble (*f)(xdouble);
    public:
        // constructor takes a function pointer and initializes base class
        ScalarFunction (xdouble (*_f)(xdouble), xdouble _start, int _steps, xdouble _stp_sz) 
            : DiscreteTimeSeries(xdoubleToVector(_start), _steps)
              , stp_sz (_stp_sz) 
              , f (_f) {}

        // pure virtual in DiscreteTimeSeries
        void Map(void);            
        xdouble GetStepSize() {return stp_sz;}
        void SetStepSize(xdouble _stp_sz) {stp_sz=_stp_sz;}
};

//debuggin 
/*int main() {
    Matrix<xdouble> M (5,5);
    
}*/
