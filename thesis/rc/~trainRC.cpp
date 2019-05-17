// Train a reservoir computer for prediction

#include "/usr/local/dislin/Lorenz.h"
#include "generateW.cpp"
#include "generateWin.cpp"


struct Result {
    Matrix<double>* mptr;
    Vector* vptr;
};

Result* trainRC(
            Matrix<double>& W
        ,   Matrix<double>& Wfb
        ,   int N
        ,   int K
        ,   int L
        ,   Matrix<double> teacher
        ,   int washout
        ,   double a
        ,   double zeta
        ,   double beta
        ,   int time)
        {

    int k;

    Matrix<double>* Woutptr = new Matrix<double>(L, N);
    Matrix<double>& Wout = *Woutptr;

    Matrix<double>  M (time-washout, N)
                ,   T (time-washout, L)
                ;

    M.Fill(0);
    T.Fill(0);

    Vector  x (N)
        ,   y (L)
        ,   offset (N)
        ;

    x.Fill(0);
    y.Fill(0);
    offset.Fill(zeta);

    for (int t=0; t<time; ++t) {
        x = (1-a)*x + a*tanh(W*x + Wfb*y + offset);
        if (t > washout) {
            k = t - washout;
            //copy x to kth row of M
            for (int j=0; j<N; ++j)
                M[k][j] = x[j];
            //copy tth row of teacher to kth row of T
            for (int j=0; j<L; ++j)
                T[k][j] = teacher[t][j];
        }
        //copy tth row of teacher to y
        for (int j=0; j<L; ++j)
            y[j] = teacher[t][j];
    }
    // build Wout = T.T()*M*(M.T()*M+beta*I).inverse

    // temporary matrix temp
    Matrix<double> temp = M.T()*M;

    // add beta to the diagonal
    for (int i=0; i<W.nrow(); ++i)
        temp[i][i] += beta;

    // the rest
    Wout = T.T() * M * temp.inv_cholesky();


    // put Wout and the last row of M into a Result struct and return
    //
    Result* res_ptr = new Result;
    res_ptr->mptr = &Wout;

    Vector      *vector_ptr = new Vector (L)
            ,   &vector     = *vector_ptr
            ;
    for (int j=0; j<N; ++j)
        vector[j] = M[M.nrow()-1][j];
    res_ptr->vptr = vector_ptr;

    return res_ptr;

}
