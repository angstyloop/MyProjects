#include "MatrixLib.cpp"
#include <cmath>

#define EPS 0.00001

void Matrix::rmultiply (int i, double c) {
    for (int j=0; j<ncol; ++j)
        M[i][j] *= c;
}

void Matrix::rsub (int i, int j, double c) {
    for (int k=0; k<ncol; ++k)
        M[j][k] = M[j][k] + c * M[i][k];
}

void Matrix::rswap (int i, int j) {
    double temp;
    for (int k=0; k<ncol; ++k) {
        temp = M[i][k];
        M[i][k] = M[j][k];
        M[j][k] = temp;
    }
}

// wrapper for sqrt function gives an error if the arg is negative
double Sqrt(double x) {
    if (x>=0)
        return sqrt(x);
    else {
        cout << "Square root of negative number encountered. Exiting." << endl;
        exit(EXIT_FAILURE);
    }
}
// cholesky lower triangular matrix. can easily do in place, but we need an output.
Matrix Matrix::cholesky () {
    if (nrow != ncol) {
        cout << "Need a pos. def. real symmetric matrix." << endl;
        exit(EXIT_SUCCESS);
    }
    double sum;
    Matrix L(nrow, ncol);
    // set the first entry
    L[0][0] = Sqrt(M[0][0]);
    // zero out the rest of the first row
    for (int j=1; j<ncol; ++j)
        L[0][j] = 0;
    for (int i=1; i<nrow; ++i) {
        // fill <= the diagonal using cholesky algorithm
        for (int j=0; j<=i; ++j) {
            sum = 0;
            for (int k=0; k<=j-1; ++k)
                sum += L[i][k]*L[j][k];
            if (i==j)
                L[i][j] = Sqrt(M[j][j] - sum);
            else
                L[i][j] = 1/L[j][j] * (M[i][j] - sum);
        }
        // zero out above the diagonal
        for (int j=i+1; j<ncol; ++j)
            L[i][j] = 0;
    }
    // output the lower triangular matrix
    return L;
}

// uses forward substitution to solve a lower triangular system. assumes the matrix of
//  coefficients is lower triangular. most useful for AugMatrix objects.
void Matrix::forwsub () {
    int i,k;
    for (i=0; i<nrow; ++i) {
        if (M[i][i] < EPS) {
            cout << "Encountered zero on diagonal. System not invertible." << endl;
            exit(EXIT_FAILURE);
        }
        rmultiply(i, 1/M[i][i]);
        for (k=i+1; k<nrow; ++k)
            rsub(i, k, -M[k][i]);
    } 
}

Matrix Matrix::inv () {
    if (nrow!=ncol) {
        cout << "Matrix must be square." << endl;
        exit (EXIT_FAILURE);
    }
    Matrix N = Identity(nrow); // will be the inverse of the cholesky L
    Matrix L = cholesky(); // the lower triangular matrix L in cholesky factorization 
    AugMatrix A (L, N);
    A.forwsub();
    N = A.getAug();
    return N.T() * N; // since M= L*L.T; M.inv = L.T.inv*L.inv = N.T * N
}

int main() {
    Matrix a(3,3);
    a.input();
    Matrix b = a.cholesky() * a;
    b.print();    
    return 0;
}

