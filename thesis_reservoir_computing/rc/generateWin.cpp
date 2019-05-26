
Matrix<double>* generateWin(int N, int K, double din, double sigma) {
    Matrix<double>* Winptr  = new Matrix<double>(N, K);
    Matrix<double>& Win = *Winptr;

    Win.RandomReals(-sigma, sigma);
    Win = Win.RandomZeroes(int(1.*(1.-din)*N*K));

    return Winptr;
}

// for testing
/*
int main() {
    Matrix<double>* Win = generateWin(5, 3, .3, 1);
    Win->Print();
    return 0;
}
*/
