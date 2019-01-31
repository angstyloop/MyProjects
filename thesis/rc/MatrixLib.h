#include <cfloat>
#include <iostream>
#include <cstring>
#include <string>
#include <locale>
#include <sstream>
#include <random>
#include <cmath>

#define OFF .01
#define EPS .000000001
#define HT_LIM

// dynamically-allocated, real-valued matrix class
template <class type>
class Matrix {
    protected:
        type** M;
    public:
        int nrow, ncol;
        // constructors

        Matrix () : nrow(0), ncol(0) {}

        Matrix (int r, int c) : nrow(r), ncol(c) {
            M = new type*[nrow];
            for (int i=0; i<nrow ; ++i)
                M[i] = new type[ncol];
        }
        Matrix (int r, int c, const Matrix& other) : nrow(r), ncol(c) {
            if (r!=other.nrow || c!=other.ncol) {
                std::cout << "invalid constructor of Matrix from Matrix. different sizes.";
                exit(EXIT_FAILURE);
            }
            M = new type*[nrow];
            for (int i=0; i<nrow ; ++i)
                M[i] = new type[ncol];
            *this = other;
        }

        // copy constructor

        Matrix (const Matrix& other) : nrow(other.nrow), ncol(other.ncol) {
            M = new type*[nrow];
            for (int i=0; i<nrow; ++i) {
                M[i] = new type[ncol];
                for (int j=0; j<ncol; ++j)
                    M[i][j] = other.M[i][j];
            }
        }  

        // destructor
        virtual ~Matrix () {
            if (M != nullptr) {
            for (int i=0; i<nrow; ++i)
                delete[] M[i];
            delete [] M;
            }    
        }

        
        // copy assignment
        Matrix& operator= (const Matrix& rhs) {
            // if matrix dims dont match we have to delete and reallocate
            if (nrow!=rhs.nrow || ncol!=rhs.ncol) {
                //delete
                for (int i=0; i<nrow; ++i)
                    delete M[i]; 
                delete M;
                //set new dims
                nrow = rhs.nrow;
                ncol = rhs.ncol;
                // reallocate
                M = new type*[nrow];
                for (int i=0; i<nrow; ++i)
                    M[i] = new type[ncol];
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
            if (this!=&other) {
                M = other.M;
                other.M = nullptr;
            }
        }

        // move assignment
        Matrix& operator= (Matrix&& other) {
            if (this != &other) {
                for (int i=0; i<nrow; ++i)
                    delete[] M[i];
                delete[] M;
                M = other.M;
                other.M = nullptr; // destructor checks for nullptr
                nrow = other.nrow;
                ncol = other.ncol;
            }  
            return *this;
        }

        // indices
        type*& operator[] (const int i) { return M[i]; }
        type*& operator[] (const int i) const { return M[i]; }
        
        // define matrix multiplication
        virtual Matrix operator* (const Matrix& rhs) const {
            if (this->ncol != rhs.nrow) {
                std::cout << "Invalid matrix multiplication: matrices don't fit." << std::endl;
                exit(EXIT_FAILURE);
            }
            Matrix P (this->nrow, rhs.ncol); //result
            for (int i=0; i<nrow; ++i)   
                for (int j=0; j<rhs.ncol; ++j) {
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
                std::cout << "Invalid matrix addition - wrong sizes" << std::endl;
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
        virtual void Print (int precision = 5, int spacing = 2) const {
            for (int i=0; i<nrow; ++i) {
                for (int j=0; j<ncol; ++j)
                    printf("%.*f%*c", precision, M[i][j], spacing, ' ');
                std::cout << '\n';
            }
        }
        
        // fill a matrix from a dynamic 2d array of type values. must specify dimensions
        //  of the array for safety.
        void Fill (type** arr, int r, int c) {
            if ( r!=nrow || c!=ncol ) {
                std::cout << "Invalid fill - input wrong size." << std::endl;
                exit(EXIT_FAILURE);
            } 

            for (int i=0; i<nrow; ++i)
                for (int j=0; j<ncol; ++j)
                    M[i][j] = arr[i][j];
        }

        // fill with a constant instead
        void Fill (double d) {
            for (int i=0; i<nrow; ++i)
                for (int j=0; j<ncol; ++j)
                    M[i][j] = d;
        }

        // lets user input a matrix at the command line. ignores non-numeric entries
        //  as well those exceeding the number of columns.
        void input () {
            int r = nrow;
            int c = ncol;
            // allocate 2d dynamic array to hold input floats
            type** arr = new type*[r]; 
            for (int i=0; i<r; ++i) {
                arr[i] = new type[c];
            }

            std::string input;
            std::string temp_str;
            float temp_flt;

            // for each row...
            for (int i=0; i<r; ++i) {
                std::stringstream ss;
                // get first line of input
                getline(std::cin, input);
                // ... otherwise dump the input string into the stringstream ss
                ss << input;
                // for each column
                for (int j=0; j<c; ++j) {
                    // process the input string one word at a time
                    ss >> temp_str;

                    // if the word currently held by temp_str is a newline, then we haven't
                    //  put in enough numerical entries.
                    if (ss.eof() && j<c-1) {
                        std::cout << "The dimensions must be " << nrow << " x " << ncol << "." << std::endl;
                        exit(EXIT_FAILURE);
                    }

                    // if temp_str is a float, put it in arr.
                    //  else decrement the column counter, effectively ignoring the non-numerical
                    //  input.
                    if (std::stringstream(temp_str) >> temp_flt)
                        arr[i][j] = temp_flt; 
                    else --j;
                    
                    //temp_str = ""; //might need this line to fix spacing
                }
            }
            // pass arr to fill and print the matrix
            Fill(arr, r, c);
            //Print();
            std::cout << std::endl;
        }
        
        // transpose of a matrix
        
        const Matrix T () const {
            Matrix tpose(ncol, nrow); //note reversed order
            for (int i=0; i<ncol; ++i) {
                for (int j=0; j<nrow; ++j) {
                    tpose[i][j] = M[j][i]; 
                }
            }
            return tpose;
        }
        // elementary row operations; these actually edit M so watch out
        void rmultiply (int i, type c); // multiplies row i by c
        void rsub (int i, int j, type c); // adds c*(row i) to row j
        void rswap (int i, int j);

        void backsub(); 
        void forwsub();

        // inverse of a positive-definite real matrix (if it exists).
        Matrix inv_cholesky();

        // converts (symmetric) M in place into the lower triangular matrix of the cholesky
        //  decomposition.
        Matrix cholesky();

        void random(int, double begin=-1, double end=1); 

        //set all entries to zero
        void zero();

        void DenseRandom(int);

        void GenerateSymmetricReservoir (Matrix&, Matrix&, double);

        // spectral radius: the largest eigenvalue. calculated with power iteration.
        double LargEigvl (void); 

}; //end of Matrix<type> class

//concatenate two matrices L and R
template <class type>
Matrix<type> concat(const Matrix<type>& L, const Matrix<type>& R);

class AugMatrix : public Matrix<double> {
    private:
        int acol; // index of the first column of the augmenting matrix  
    public:
        AugMatrix (const Matrix<double>& L, const Matrix<double>& R) 
            : Matrix<double>(L.nrow, L.ncol+R.ncol, concat(L, R)), acol(L.ncol) {
            if (L.nrow!=R.nrow) {
                std::cout << "Number of rows must match." << std::endl;
                exit(EXIT_SUCCESS);
            }
        }
        void Print (int precision = 1, int spacing = 2) const {
            for (int i=0; i<nrow; ++i) {
                for (int j=0; j<acol; ++j)
                    printf("%.*f%*c", precision, M[i][j], spacing, ' ');
                std::cout << "|";
                printf("%*c", spacing, ' ');
                for (int j=acol; j<ncol; ++j)
                    printf("%.*f%*c", precision, M[i][j], spacing, ' ');
                std::cout << '\n';
            }
        } 
        // output the augmenting side of the matrix (usually an inverse matrix or
        //  vector solution to a linear equation)
        Matrix getAug () {
            Matrix temp(nrow, ncol-acol);
            for (int i=0; i<nrow; ++i)
                for (int j=0; j<ncol-acol; ++j)
                    temp[i][j] = M[i][acol+j];
            return temp;
        }
        
};

// concatenate two matrices L and R
template <class type>
Matrix<type> concat(const Matrix<type>& L, const Matrix<type>& R) {
    if (L.nrow!=R.nrow) exit (EXIT_FAILURE);
        Matrix<type> temp(L.nrow, L.ncol+R.ncol);
        for (int i=0; i<temp.nrow; ++i) {
            for (int j=0; j<L.ncol; ++j) 
                temp[i][j] = L[i][j];
            for (int j=0; j<R.ncol; ++j) 
                temp[i][L.ncol+j] = R[i][j];
        }
        return temp;
}

// create an nxn identity matrix
template <class type>
Matrix<type> Identity(int n) {
    Matrix<type> temp (n, n);
    for (int i=0; i<n; ++i)
        for (int j=0; j<n; ++j)
            temp[i][j] = (i!=j) ? 0 : 1;
    return temp;
}

class Vector : public Matrix<double> {
    int length;
    public:
        //ctors
        Vector () : length(0) {};
        Vector (int dim) : Matrix<double>(dim, 1), length(dim) {} // vectors are column  

        // copy ctor
        Vector (const Vector& that) : Matrix<double>(that), length(that.nrow) {} 
      
        // move assignment
        Vector& operator= (Vector&& that) {
            if (this!=&that) {
                for (int i=0; i<length; ++i)
                    delete[] M[i];
                delete M;
                M = that.M;
                that.M = nullptr;
            } 
            return *this;
        }

        Vector& operator= (Matrix<double>&& that) {
            Matrix<double>::operator=(that);
            return *this;
        }

        // move ctor
        Vector (Vector&& that) : Matrix(that.nrow, that.ncol) {
            M = that.M;
            that.M = nullptr;
        }


        // copy assignment
        Vector& operator= (Vector& that) {
            for (int i=0; i<length; ++i)
                M[i][0] = that.M[i][0];
            return *this;
        }
        
        // uses Matrix Fill
        void Fill (double* arr, int r) {
            double** temp = new double*[r];
            for (int i=0; i<r; ++i)
                temp[i] = new double[1];
            for (int i=0; i<r; ++i)
                temp[r][1] = arr[i];
            Matrix<double>::Fill(temp, r, 1);
        }
        
        // uses matrix fill
        void Fill (double d) {
            double** temp = new double*[length];
            for (int i=0; i<length; ++i) {
                temp[i] = new double[1];
                temp[i][1] = d;
            }
            Matrix<double>::Fill(temp, length, 1);
        }
        
        //indices
        double& operator[] (const int i) const { return M[i][0]; }

        //dot product
        double operator*(Vector& that) {
            if (this->length != that.length) {
                std::cout << "Dot product needs two vectors of the same length..." << std::endl; 
                exit(EXIT_SUCCESS);
            }
            double sum = 0;
            for (int i=0; i<length; ++i) 
                sum += (*this)[i] * that[i];
            return sum;
        }

        //norm
        double Norm (void) { 
            return sqrt( (*this) * (*this) ); 
        }

        // scalar multiplication
        Vector operator*(double c) {
            Vector v = *this;
            for (int i=0; i<length; ++i) {v[i] *= c;}
            return *this; 
        }
        const int& len() const {return length;}

            
};

// Matrix * vector multiplication
//Vector operator*(Matrix<double>& A, Vector& b) {
//    Vector v_temp(b.len());
//    Matrix<double> m_temp = A.Matrix::operator*(b);
//    for (int i=0; i<b.len(); ++i)
//        v_temp[i] = m_temp[i][0];
//    return v_temp;
//}

// scalar multiplication (other side)
Vector operator*(double c, Vector v) { return v*c; }



// seed for random generator stuff
std::random_device rd;

template <class type>
void Matrix<type>::rmultiply (int i, type c) {
    for (int j=0; j<ncol; ++j)
        M[i][j] *= c;
}

template <class type>
void Matrix<type>::rsub (int i, int j, type c) {
    for (int k=0; k<ncol; ++k)
        M[j][k] = M[j][k] + c * M[i][k];
}

template <class type>
void Matrix<type>::rswap (int i, int j) {
    double temp;
    for (int k=0; k<ncol; ++k) {
        temp = M[i][k];
        M[i][k] = M[j][k];
        M[j][k] = temp;
    }
}

// wrapper for sqrt function gives an error if the arg is negative
double Sqrt(double x) {
    if (x<0) {
        std::cout << "Square root of negative number" << x << " encountered. Exiting." << std::endl;
        exit(EXIT_FAILURE);
    }
    else return sqrt(x);
}
// cholesky lower triangular matrix. can easily do in place, but we need an output.
// only works for positive-semidefinite matrices. will only work for ridge regression
// if M has no nonnegative entries.
template <>
Matrix<double> Matrix<double>::cholesky () {
    if (nrow != ncol) {
        std::cout << "Need a pos. semidef. real symmetric matrix." << std::endl;
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
template <>
void Matrix<double>::forwsub () {
    int i,k;
    for (i=0; i<nrow; ++i) {
        if (M[i][i] < DBL_MIN) {
            std::cout << "Encountered zero on diagonal. System not invertible." << std::endl;
            exit(EXIT_FAILURE);
        }
        rmultiply(i, 1/M[i][i]);
        for (k=i+1; k<nrow; ++k)
            rsub(i, k, -M[k][i]);
    } 
}

template <>
Matrix<double> Matrix<double>::inv_cholesky () {
    if (nrow!=ncol) {
        std::cout << "Matrix must be square." << std::endl;
        exit (EXIT_FAILURE);
    }
    Matrix<double> N = Identity<double>(nrow); // will be the inverse of the cholesky L
    Matrix<double> L = cholesky(); // the lower triangular matrix L in cholesky factorization 
    AugMatrix A (L, N);
    A.forwsub();
    N = A.getAug();
    return N.T() * N; // since M= L*L.T; M.inv = L.T.inv*L.inv = N.T * N
}

template <class type>
void Matrix<type>::zero() {
    for (int i=0; i<nrow; ++i)
        for (int j=0; j<ncol; ++j)
            M[i][j] = 0;
}
// randomly assigns real numbers in the interval [begin, end] to dens randomly selected
//  entries of a matrix. to get a randomly generated matrix, just make a new uninitialized matrix
//  and call random(). if a real zero is randomly generated for the entry, random() re-rolls. 
//  when a nonzero element is encountered, random() assumes that's an element
//  we already placed and skips it without decrementing dens. this will on average contribute
//  significantly to runtime as dens increases. however, for low dens, this should work fine.
//
//  begin and end are set to -1 and 1 by default.
template <>
void Matrix<double>::random(int dens, double begin, double end) {
    if (dens > ncol*nrow) {
        std::cout << "Density must not exceed the number of entries ..." << std::endl;
        exit(EXIT_FAILURE);
    }
    int i, j;
    double c;

    //random_device rd;
    std::default_random_engine entry_gen(rd());
    std::uniform_real_distribution<> entry_dist (begin, end);
    // DISLIN doesn't like std::bind()
    //auto rand_entry = std::bind(entry_dist, entry_gen);

    std::default_random_engine i_gen(rd());
    std::uniform_int_distribution<int> i_dist (0, nrow-1);
    //auto rand_i = std::bind(i_dist, i_gen);

    std::default_random_engine j_gen(rd());
    std::uniform_int_distribution<int> j_dist (0, ncol-1);
    //auto rand_j = std::bind(j_dist, j_gen);
    
    // initialize entries to zero
    zero();
    
    while (dens > 0) {
        c = entry_dist(entry_gen);
        if (abs(c) < DBL_MIN) continue; //re-roll if we get zero, since we're aiming for a spec. density

        i = i_dist(i_gen);
        j = j_dist(j_gen);

        if (M[i][j] < DBL_MIN) {
            M[i][j] = c;
            --dens;
        }
    }
}

// this is just Fisher-Yates shuffle: selects k integers randomly from an array of integers 0..n-1.
int* RandomSelect(int n, int out[], int k) {
    // indices
    int i, j;
    // random number generator
    //random_device rd;
    std::default_random_engine generator(rd());

    //put the first k elements of {0,...,n-1} in output array in random order
    out[0] = 0;
    for (i=1; i<k; ++i) {
        std::uniform_int_distribution<int> dist(0, i-1);
        j = dist(generator);
        out[i] = out[j];
        out[j] = i;
    }

    //put the remaining elements in randomly
    for (i=k; i<n; ++i) {
        std::uniform_int_distribution<int> dist(0, i);
        j = dist(generator);
        if (j<k)
            out[j] = i;
    }
    return out;
}

// a template overload of RandomSelect (fisher-yates shuffle)
template <class T>
T* RandomSelect(T S[], int n, T out[], int k) {
    int* integers = new int[k];
     integers = RandomSelect(n, integers, k); // to hold the output of Randomselect(int,int)
    for (int i=0; i<k; ++i)
       out[i] = S[integers[i]];  // use the output of original RandomSelect as indices
    delete[] integers;
    return out; // return selected objects
}

template <>
void Matrix<double>::DenseRandom(int dens) {
    int i, k=0, p=nrow*ncol; //Indices and num. entries.
    int** picked_pairs = new int*[dens];    // to hold randomly selected pairs
    int** ij_pairs = new int*[p]; //To store all pairs.
    for (i=0; i<p; ++i) {
        ij_pairs[i] = new int[2]; 
    }
    // initialize matrix to zeroes
    zero();
    //random generator stuff
    //random_device rd;
    std::default_random_engine gen(rd());
    std::uniform_real_distribution<double> dist(-1,1);
    //auto roll = std::bind(dist, gen); 

    for (i=0; i<nrow; ++i) {    // Generate [i,j] pairs
        for (int j=0; j<ncol; ++j) {
            ij_pairs[k][0] = i; 
            ij_pairs[k++][1] = j;
        }
    }
    // randomly select dens pairs from all pairs
    picked_pairs = RandomSelect<int*>(ij_pairs, p, picked_pairs, dens);
    // assign randomly from [-1,1] to each picked entry
    for (i=0; i<dens; ++i) {
        M[picked_pairs[i][0]][picked_pairs[i][1]] = dist(gen);
    }
}

// generate the eigendecomposition of a symmetric reservoir matrix, such that the orthogonal matrix
//  consists of random entries except for zeroes on the diagonal, and the diagonal matrix contains random
//  eigenvalues less than the parameter spec_rad


template <>
void Matrix<double>::GenerateSymmetricReservoir (Matrix<double>& Q, Matrix<double>& D, double spec_rad) {
    std::random_device rd;
    std::default_random_engine gen(rd());

    std::uniform_real_distribution<double> dist(-1,1);
    //auto roll = std::bind(dist, gen); 
        
    double sum_sqrs; // to track the sum of squares of row entries so we can normalize rows
    for (int i=0; i<nrow; ++i) {
        sum_sqrs = 0;
        //// randomly assign entries to row i
        for (int j=0; j<ncol; ++j) {
            Q[i][j] = i==j ? 0 : dist(gen); 
            sum_sqrs += Q[i][j]*Q[i][j];
        }
        // normalize row i
        for (int j=0; j<ncol; ++j)
            Q[i][j] /= sqrt(sum_sqrs);
    }

    for (int i=0; i<nrow; ++i)
        D[i][i] = dist(gen);

    // use eigenfactorization to generate M
    *this = Q * D * Q.T();
}

Vector Offset (int dim) {
    Vector temp (dim);
    for (int i=0; i<dim; ++i)
        temp[i]= OFF;
    return temp;;
}

//future goal: generate a random orthogonal matrix in O(n^3) time using Stewart's algorithm


// spectral radius: the largest eigenvalue. calculated with power iteration.
template<>
double Matrix<double>::LargEigvl (void) {
    // defines convergence
    double eps = .0001, prev_eig, curr_eig;
    // temp storage
    Vector prev(ncol), curr(ncol);
    // random first vector
    curr.random(ncol);
    // set first eig to double max so the loop won't end after one iteration
    curr_eig = DBL_MAX;
    // iterate until cvg (when eigenvalue doesnt change more than eps)
    do {
        //normalize; scalar multiplication is defined in Vector
        curr = 1/curr.Norm()*curr;
        prev = curr;
        prev_eig = curr_eig;
        // matrix * vector  multiplication
        curr = (*this)*prev;
        // store the potential eigenvalue; these are dot products
        curr_eig = (prev*curr) / (prev*prev);

    } while (abs(curr_eig - prev_eig) > eps);

    return curr_eig;
}

