
Matrix<double>* generateW(int N, double p, double dW) {

    Matrix<double>* Wptr = new Matrix<double>(N,N);
    Matrix<double>& W = *Wptr;
    double  eig; 


    // Calculate largest eigenvalue of random W, and if it's nan, re-random W
    // and recalculate eigenvalue until it is a good value.
    do {
        W.RandomReals(-1,1);
        W = W.RandomZeroes(int(1.*(1.-1.*dW)*N*N));
        eig = abs(W.LargEigvl());
    } while (isnan(eig) || eig<EPS);
 
    // Divide by largest eigenvalue, and multiply by spectral radius p
    W = (1.*p/eig) * W;
    return Wptr;     
}


// for testing
/*
int main() {
    Matrix<double>* W = generateW(5,.76,.8);
    double eig;

    W->Print(); 
    
    eig = W->LargEigvl();
    std::cout   << std::endl
                << eig
                << std::endl;
    assert(eig-.76 < EPS);
    delete W;
}
*/

