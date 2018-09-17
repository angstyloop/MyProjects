/* The goal here is to implement heapsort by making a class MaxHeap that knows a method HeapSort for sorting
 * itself.
 */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

class MaxHeap
{
    private:
        int heapsize;
        vector<int> heap;
        
        static int Parent(int i);
        static int Left(int i);
        static int Right(int i);

        void Swap(int i, int j)
        {
            int c = heap[i];
            heap[i] = heap[j];
            heap[j] = c;
        }
        void MaxHeapify(int i);
        void BuildMaxHeap();
        
    public:
        MaxHeap(vector<int>&);
        void HeapSort();
        
        // getter methods for heap and heapsize
        int getheapsize() const {return heapsize;}
        vector<int> getheap() const {return heap;}
        
};

int MaxHeap::Parent(int i) { return (i+1)/2-1; } // implicit floor division, adjust for index order 0
int MaxHeap::Left(int i) { return 2*(i+1)-1; } 
int MaxHeap::Right(int i) { return 2*(i+1)+1-1; }

void MaxHeap::MaxHeapify(int i)
{
    int l = Left(i);
    int r = Right(i);
    int largest;
    if ( l < heapsize && heap[l] > heap[i] ) largest = l;
    else largest = i;
    if ( r < heapsize && heap[r] > heap[largest] ) largest = r;
    if ( largest != i )
    {
        Swap(i, largest);
        MaxHeapify(largest);
    }
}

void MaxHeap::BuildMaxHeap()
{
    for ( int i = heapsize/2-1; i >= 0; i-- ) MaxHeapify(i);
}

MaxHeap::MaxHeap(vector<int> &v)
{
    heapsize = v.size();
    heap = v;
    BuildMaxHeap();
}

void MaxHeap::HeapSort()
{
    while (heapsize > 0)
    {
        Swap(0, heapsize-1);
        heapsize--;
        MaxHeapify(0);
    }
    // let's reset heapsize to the length of heap (a vector)
    heapsize = heap.size();
}

int main(int argc, char **argv)
{
    // these two lines are the best way I've come up with for putting command line arguments
    //  into a vector of integers
    vector<int> args(argc-1);
    for (int i=0; i < argc-1; i++) args[i] = atoi(argv[i+1]);

    MaxHeap h = args; 
    h.HeapSort();
    for (auto x: h.getheap()) cout << x << " "; 
    cout << endl;
}
