#include <iostream>
#include <string>
#include <locale>
#include <sstream>
using namespace std;

// dynamically allocated matrix class
class Matrix {
    private:
        int nrow, ncol;
        double** M;
    public:
        // constructors
        Matrix () {};
        Matrix (int r, int c) : nrow(r), ncol(c) {
            M = new double*[nrow];
            for (int i=0; i<nrow ; ++i)
                M[i] = new double[ncol];
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
        ~Matrix () {
            for (int i=0; i<nrow; ++i)
                delete[] M[i];
            delete [] M;
        }

        
        // copy assignment
        Matrix& operator= (const Matrix& rhs) {
            if (ncol != rhs.ncol || nrow != rhs.nrow) {
                cout << "Invalid matrix assignment - different sizes." <<endl;
                exit(EXIT_FAILURE);
            }

            for (int i=0; i<nrow; ++i) 
                for (int j=0; j<ncol; ++j) 
                    M[i][j] = rhs.M[i][j];

            return *this;
        }

        // we'll only need the move member functions if we end up using matrix-valued
        //  functions on the rhs of equals sign
        
        // move constructor
        Matrix (Matrix&& other) : nrow(other.nrow), ncol(other.ncol) {
            M = other.M;
            other.M = nullptr;
        }

        // move assignment
        Matrix& operator= (Matrix&& other) {
            nrow = other.nrow;
            ncol = other.ncol;
            for (int i=0; i<nrow; ++i)
                delete[] M[i];
            delete[] M;
            M = other.M;
            other.M = nullptr;
            return *this;
        }

        // overload indices
        double*& operator[] (const int i) { return M[i]; }
        double*& operator[] (const int i) const { return M[i]; }
        
        // define matrix multiplication

        Matrix operator* (const Matrix& rhs) const {
            if (this->ncol != rhs.nrow) {
                cout << "Can't multiply these matrices." << endl;
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
        
        Matrix& operator+= (const Matrix& rhs) {
            *this = *this + rhs;
            return *this;
        }

        // neatly print matrix on command line, with optional precision and spacing args
        
        void print (int precision = 1, int spacing = 2) {
            for (int i=0; i<nrow; ++i) {
                for (int j=0; j<nrow; ++j)
                    printf("%.*f%*c", precision, M[i][j], spacing, ' ');
                cout << '\n';
            }
        }
        
        // fill a matrix from a dynamic 2d array of double values. must specify dimensions.

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
                // dump the input string into the stringstream ss
                ss << input;
                // for each column
                for (int j=0; j<c; ++j) {
                    // process the input string one word at a time
                    ss >> temp_str;
                    
                    // if temp_str is a float ...
                    if (stringstream(temp_str) >> temp_flt)
                        arr[i][j] = temp_flt; 
                    //else exit(EXIT_FAILURE);
                    
                    //temp_str = "";
                }
            }
            // pass arr to fill and print the matrix
            fill(arr, r, c);
            cout << endl;
            print();
            cout << endl;
        }
        
};

int main() {
    Matrix a(3,3);
    a.input();
    Matrix b(3,3);
    b = a*b;
    b.print();
    return 0;
}
