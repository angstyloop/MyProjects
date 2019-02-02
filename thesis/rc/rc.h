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

Matrix<double> RidgeRegress(Matrix<double>, Matrix<double>, double);

class EchoStateNetwork : public DiscreteTimeSeries {
    protected:
        DiscreteTimeSeries* in_series; 
        Matrix<double> W, W_in, W_out;
        const Vector offset;
        double b;
        double spec_rad = 0.9;
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
        void RidgeTrace(Matrix<double>**, int, double);
        void Wash(int);
        void Predict(void);
        void SetSpecRad (double rad) {spec_rad = rad;};
        double SpecRad (void) {return spec_rad;}
        void Listen (void);
        void SetB (double _b) {b=_b;}
        double GetB () {return b;}
        void RandomParms (double, double); 
        double PlotRidgeTrace();
};

class BakersMap : public DiscreteTimeSeries {
    protected:
        double param;
    public:
        BakersMap (Vector start, int steps, double a)
            : DiscreteTimeSeries (start, steps), param (a) {}                
            
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

// convert a double to a 1-d Vector
 const Vector DoubleToVector(double d) {
    Vector* v = new Vector(1);
    (*v)[0] = d;
    return *v;
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
            : DiscreteTimeSeries(DoubleToVector(_start), _steps)
              , stp_sz (_stp_sz) 
              , f (_f) {}

        // pure virtual in DiscreteTimeSeries
        void Map(void);            
        double GetStepSize() {return stp_sz;}
        void SetStepSize(double _stp_sz) {stp_sz=_stp_sz;}
};

//debuggin 
/*int main() {
    Matrix<double> M (5,5);
    
}*/
