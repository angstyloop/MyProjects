// run a reservoir computer for prediction

Matrix<double>* runRC(
        Matrix<double>& W
    ,   Matrix<double>& Wfb
    // using Result struct now instead
    //,   Matrix<double>& Wout
    //,   Matrix<double>& M
    ,   Result* res_ptr
    ,   int N
    ,   int K
    ,   int L
    ,   int time
    ,   double a
    ,   double zeta)
{
    
    // make Wout a reference to the output matrix (Wout) stored in *(res_ptr-mptr)
    Matrix<double>& Wout = *(res_ptr->mptr);

    Matrix<double>* outputs_ptr = new Matrix<double> (time, L);
    Matrix<double>& outputs = *outputs_ptr;

    outputs.Fill(0);

    Vector  x (N)
        ,   y (L)
        ,   offset (N)
        ;

    // copy the last row of the state collection matrix M
    //  (saved and pointed to by res_ptr->vptr) to x
    x = *(res_ptr->vptr);

    y.Fill(0);
    offset.Fill(zeta);

    for (int t=0; t<time; ++t) {
        x = (1.-a)*x + a*Tanh(W*x + Wfb*y + offset);
        y = Wout*x;
        // copy y to the first row of outputs
        for (int j=0; j<L; ++j)
            outputs[t][j] = y[j];
    } 
    return outputs_ptr;
}

