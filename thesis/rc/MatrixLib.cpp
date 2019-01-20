#include <iostream>
#include <string>
#include <locale>
#include <sstream>
using namespace std;

// dynamically-allocated, real-valued matrix class
//
class Matrix {
    protected:
        double** M;
    public:
        const int nrow, ncol;
        // constructors

        Matrix () = default;
        Matrix (int r, int c) : nrow(r), ncol(c) {
            M = new double*[nrow];
            for (int i=0; i<nrow ; ++i)
                M[i] = new double[ncol];
        }
        Matrix (int r, int c, const Matrix& other) : nrow(r), ncol(c) {
            if (r!=other.nrow || c!=other.ncol) {
                cout << "invalid constructor of Matrix from Matrix. different sizes.";
                exit(EXIT_FAILURE);
            }
            M = new double*[nrow];
            for (int i=0; i<nrow ; ++i)
                M[i] = new double[ncol];
            *this = other;
        }

        // copy constructor

        Matrix (const Matrix& other) : nrow(other.nrow), ncol(other.ncol) {
            M = new double*[nrow];
            for (int i=0; i<nrow; ++i) {
                M[i] = new double[ncol];
                for (int j=0; j<ncol; ++j)
                    M[i][j] = other.M[i][j];
            }
        }  

        // destructor
        virtual ~Matrix () {
            for (int i=0; i<nrow; ++i)
                delete[] M[i];
            delete [] M;
        }

        
        // copy assignment
        Matrix& operator= (const Matrix& rhs) {
            if (ncol != rhs.ncol || nrow != rhs.nrow) {
                cout << "Invalid copy assignment - matrices are different sizes." <<endl;
                exit(EXIT_FAILURE);
            }

            for (int i=0; i<nrow; ++i) 
                for (int j=0; j<ncol; ++j) 
                    M[i][j] = rhs.M[i][j];

            return *this;
        }

        // we'll only need the move member functions if we end up using matrix-valued
        //  functions on the rhs of equals sign [e.g. the transpose A.T() ]
        
        // move constructor
        Matrix (Matrix&& other) : nrow(other.nrow), ncol(other.ncol) {
            M = other.M;
            other.M = nullptr;
        }

        // move assignment
        Matrix& operator= (Matrix&& other) {
            if (nrow!=other.nrow || ncol!=other.ncol) {
                cout << "Invalid move assignment - matrices must be the same size." << endl;
                exit(EXIT_SUCCESS);
            } 
            for (int i=0; i<nrow; ++i)
                delete[] M[i];
            delete[] M;
            M = other.M;
            other.M = nullptr;
            return *this;
        }

        // indices

        double*& operator[] (const int i) { return M[i]; }
        double*& operator[] (const int i) const { return M[i]; }
        
        // define matrix multiplication

        Matrix operator* (const Matrix& rhs) const {
            if (this->ncol != rhs.nrow) {
                cout << "Invalid matrix multiplication: matrices don't fit." << endl;
                exit(EXIT_FAILURE);
            }
            Matrix P (nrow, ncol); //result
            for (int i=0; i<nrow; ++i)   
                for (int j=0; j<ncol; ++j) {
                    P[i][j] = 0;
                    for (int k=0; k<rhs.nrow; ++k)
                        P[i][j] += M[i][k] * rhs.M[k][j]; 
                 } 
            return P;
        }  
        
        // delete multiplication assignment for matrices
        Matrix& operator*= (const Matrix& rhs) = delete;

        // define matrix addition
        Matrix operator+ (const Matrix& rhs) const {
            if (ncol != rhs.ncol || nrow != rhs.nrow) {
                cout << "Invalid matrix addition - wrong sizes" << endl;
                exit(EXIT_FAILURE);
            }
            Matrix P (nrow, ncol);
            for (int i=0; i<nrow; ++i)
                for (int j=0; j<ncol; ++j)
                   P[i][j] = M[i][j] + rhs.M[i][j]; 
            return P;
        }
        
        // define addition assignment
        Matrix& operator+= (const Matrix& rhs) {
            *this = *this + rhs;
            return *this;
        }

        // neatly print matrix on command line, with optional precision and spacing args
        virtual void print (int precision = 1, int spacing = 2) {
            for (int i=0; i<nrow; ++i) {
                for (int j=0; j<ncol; ++j)
                    printf("%.*f%*c", precision, M[i][j], spacing, ' ');
                cout << '\n';
            }
        }
        
        // fill a matrix from a dynamic 2d array of double values. must specify dimensions
        //  of the array for safety.
        void fill (double** arr, int r, int c) {
            if ( r!=nrow || c!=ncol ) {
                cout << "Invalid fill - input wrong size." << endl;
                exit(EXIT_FAILURE);
            } 

            for (int i=0; i<nrow; ++i)
                for (int j=0; j<ncol; ++j)
                    M[i][j] = arr[i][j];
        }

        // lets user input a matrix at the command line. ignores non-numeric entries
        //  as well those exceeding the number of columns.
        void input () {
            int r = nrow;
            int c = ncol;
            // allocate 2d dynamic array to hold input floats
            double** arr = new double*[r]; 
            for (int i=0; i<r; ++i) {
                arr[i] = new double[c];
            }

            string input;
            string temp_str;
            float temp_flt;

            // for each row...
            for (int i=0; i<r; ++i) {
                stringstream ss;
                // get first line of input
                getline(cin, input);
                // ... otherwise dump the input string into the stringstream ss
                ss << input;
                // for each column
                for (int j=0; j<c; ++j) {
                    // process the input string one word at a time
                    ss >> temp_str;

                    // if the word currently held by temp_str is a newline, then we haven't
                    //  put in enough numerical entries.
                    if (ss.eof() && j<c-1) {
                        cout << "The dimensions must be " << nrow << " x " << ncol << "." << endl;
                        exit(EXIT_FAILURE);
                    }

                    // if temp_str is a float, put it in arr.
                    //  else decrement the column counter, effectively ignoring the non-numerical
                    //  input.
                    if (stringstream(temp_str) >> temp_flt)
                        arr[i][j] = temp_flt; 
                    else --j;
                    
                    //temp_str = ""; //might need this line to fix spacing
                }
            }
            // pass arr to fill and print the matrix
            fill(arr, r, c);
            //print();
            cout << endl;
        }
        
        // transpose of a matrix
        
        Matrix T () {
            Matrix tpose(ncol, nrow); //note reversed order
            for (int i=0; i<ncol; ++i) {
                for (int j=0; j<nrow; ++j) {
                    tpose[i][j] = M[j][i]; 
                }
            }
            return tpose;
        }
        // elementary row operations; these actually edit M so watch out
        void rmultiply (int i, double c); // multiplies row i by c
        void rsub (int i, int j, double c); // adds c*(row i) to row j
        void rswap (int i, int j);

        void backsub(); 
        void forwsub();
        // inverse of a (square) matrix (if it exists).
        Matrix inv();
        // converts (symmetric) M in place into the lower triangular matrix of the cholesky
        //  decomposition.
        Matrix cholesky();
};

//concatenate two matrices L and R
Matrix concat(const Matrix& L, const Matrix& R);

class AugMatrix : public Matrix {
    private:
        int acol; // index of the first column of the augmenting matrix  
    public:
        AugMatrix (const Matrix& L, const Matrix& R) 
            : Matrix(L.nrow, L.ncol+R.ncol, concat(L, R)), acol(L.ncol) {
            if (L.nrow!=R.nrow) {
                cout << "Number of rows must match." << endl;
                exit(EXIT_SUCCESS);
            }
        }
        void print (int precision = 1, int spacing = 2) {
            for (int i=0; i<nrow; ++i) {
                for (int j=0; j<acol; ++j)
                    printf("%.*f%*c", precision, M[i][j], spacing, ' ');
                cout << "|";
                printf("%*c", spacing, ' ');
                for (int j=acol; j<ncol; ++j)
                    printf("%.*f%*c", precision, M[i][j], spacing, ' ');
                cout << '\n';
            }
        } 
        // output the augmenting side of the matrix (usually an inverse matrix or
        //  vector solution to a linear equation)
        Matrix getAug () {
            Matrix temp(nrow, ncol-acol);
            for (int i=0; i<nrow; ++i)
                for (int j=0; j<ncol-acol; ++j)
                    temp[i][j] = M[i][j];
            return temp;
        }
        
};

// concatenate two matrices L and R
Matrix concat(const Matrix& L, const Matrix& R) {
    if (L.nrow!=R.nrow) exit (EXIT_FAILURE);
        Matrix temp(L.nrow, L.ncol+R.ncol);
        for (int i=0; i<temp.nrow; ++i) {
            for (int j=0; j<L.ncol; ++j) 
                temp[i][j] = L[i][j];
            for (int j=0; j<R.ncol; ++j) 
                temp[i][L.ncol+j] = R[i][j];
        }
        return temp;
}

// create an nxn identity matrix
Matrix Identity(int n) {
    Matrix temp (n, n);
    for (int i=0; i<n; ++i)
        for (int j=0; j<n; ++j)
            temp[i][j] = (i!=j) ? 0 : 1;
    return temp;
}
