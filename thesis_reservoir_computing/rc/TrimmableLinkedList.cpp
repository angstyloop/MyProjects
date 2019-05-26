//#include "MatrixLib.h"
//#include <iostream>
#include "TrimmableLinkedList.h"
int main() {
    // generate a shuffled array of matrix indices to draw from
    //  at the end of each p iteration
    //
 
   int N=100;
   int i, j, k; // k = counter for array of pairs of indices
   i=j=k=0;

    int** picked_pairs = new int*[N*N];    // to hold randomly selected pairs
    int** ij_pairs = new int*[N*N];        // to store all pairs.
        

    for (i=0; i<N*N; ++i) {
        ij_pairs[i] = new int[2];
    }

    for (i=0; i<N; ++i) {             // Generate [i,j] pairs
        for (j=0; j<N; ++j) {
            ij_pairs[k][0] = i;
            ij_pairs[k][1] = j;
            k++;
        }
    }
 
    // randomly select z pairs from all pairs
    //RandomSelect<int*>(ij_pairs, N*N, picked_pairs, N*N);

    // now use picked_pairs, a shuffled array of int[2],
    //  to make a trimmable linked list, where the key 
    //  for each node is max(i,j), and the data is the
    //  [i,j] pair. it might be possible to skip the
    //  intermediate array picked_pairs, but we'd have
    //  to change RandomSelect so let's just leave it
    //  for now.

    TrimmableLinkedList tll (N);
    std::cout << "OK" << std::endl;

    Node** pair_node_ptrs = new Node*[N*N];

    for (int i=0; i<N*N; ++i) {
        pair_node_ptrs[i] = new Node(ij_pairs[i]);
        tll.Insert(pair_node_ptrs[i]);
    }


    // Copy the trimmable linked list so we can use it
    //  to zero the appropriate elements of this->W 
    //  after we optimize.
    TrimmableLinkedList tll_copy = tll;    

    // clean up dynamic arrays. we don't need them anymore.
    for (int i=0; i<N*N; ++i)
        delete[] ij_pairs[i];
    delete[] ij_pairs;
    delete[] picked_pairs;
}
