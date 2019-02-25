#include "/usr/local/dislin/Lorenz.h"

int main () {



    int     i = 0   // indices
        ,   k = 0
        ,   N = 10
        ,   p = N*N
        ,   z = p-2
        ;

    Matrix<double> res (N+1, N+1);
    res.RandomReals();

    res.setnrow(N);
    res.setncol(N);

    int** picked_pairs = new int*[z];    // to hold randomly selected pairs
    int** ij_pairs = new int*[p];        // to store all pairs.

    for (i=0; i<p; ++i) {
        ij_pairs[i] = new int[2];
    }

    for (i=0; i<N; ++i) {             // Generate [i,j] pairs
        for (int j=0; j<N; ++j) {
            ij_pairs[k][0] = i;
            ij_pairs[k][1] = j;
            k++;
        }
    }

    // randomly select z pairs from all pairs
    RandomSelect<int*>(ij_pairs, p, picked_pairs, z);

    // zero the entry corresponding to each selected pair
    for (i=0; i<z; ++i) {
        res[picked_pairs[i][0]][picked_pairs[i][1]] = 0;
    }

    // clean up dynamic memory
    delete[] picked_pairs;

    for (i=0; i<p; ++i)
        delete[] ij_pairs[i];

    delete[] ij_pairs;


    return 0;
}    


