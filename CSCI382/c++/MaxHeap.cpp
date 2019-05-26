/* An object implementation of a max heap that supports sorting and priority queues. */

/* the priority queue stuff still needs testing */

#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
using namespace std;

struct HeapElement
{
    void *handle;   // points to an application object. remember void pointers must be explicitly cast.
    int key;
};

class MaxHeap
{
    private:
        int heapsize;
        vector<HeapElement *> heap;
        
        static int Parent(int i);
        static int Left(int i);
        static int Right(int i);

        void Swap(int i, int j)
        {
            HeapElement * c = heap[i];
            heap[i] = heap[j];
            heap[j] = c;
        }
        void MaxHeapify(int i);
        void BuildMaxHeap();
        
    public:
        MaxHeap(vector<HeapElement *> &);
        void HeapSort();
        
        // getter methods for heap and heapsize
        int getheapsize() const {return heapsize;}
        vector<HeapElement *> getheap() const {return heap;}

        // these methods support usage of MaxHeap objects as priority queues
        void* Max() {return heap[0]->handle;}
        void* ExtractMax();
        void IncreaseKey(int i, int newKey);
        void Insert(HeapElement* newElement);
        
        
};

int MaxHeap::Parent(int i) { return (i+1)/2-1; } // implicit floor division, adjust for index order 0
int MaxHeap::Left(int i) { return 2*(i+1)-1; } 
int MaxHeap::Right(int i) { return 2*(i+1)+1-1; }

void MaxHeap::MaxHeapify(int i)
{
    int l = Left(i);
    int r = Right(i);
    int largest;
    if ( l < heapsize && heap[l]->key > heap[i]->key ) largest = l;
    else largest = i;
    if ( r < heapsize && heap[r]->key > heap[largest]->key ) largest = r;
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

MaxHeap::MaxHeap(vector<HeapElement*> &v)
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

void* MaxHeap::ExtractMax()
{
    if (heapsize < 1)
    {
        cout << "Heap underflow." << endl;
        exit(EXIT_FAILURE);
    } else {
        void* max = heap[0]->handle;
        delete heap[0]; //free the memory pointed to by the extracted pointer
        heap[0] = heap[heapsize-1]; //copy the last pointer to the first
        heap.pop_back(); //remove the old copy
        heapsize--;
        MaxHeapify(0);
        return max;
    }
}

void MaxHeap::IncreaseKey(int i, int newKey)
{
    if (heap[i]->key >= newKey)
    {
        cout << "New key must be greater than current key." << endl;
        exit(EXIT_FAILURE);
    } else {
        heap[i]->key = newKey;
        while (i > 0 && heap[i]->key > heap[Parent(i)]->key)
        {
            Swap(i, Parent(i));
            i = Parent(i);
        }
    }
}

void MaxHeap::Insert(HeapElement* newElement)
{
    // save the element's key so we can set it to -1 for IncreaseKey
    int key = newElement->key;
    // -1 is less than every possible key, so IncreaseKey won't break.
    newElement->key = -1;
    heapsize++;
    if (heapsize > heap.size()) {
        heap.push_back(newElement);
    } else {
        heap[heapsize-1] = newElement;
    }
    IncreaseKey(heapsize-1, key);
}

int main(int argc, char **argv)
{
    // these two lines are the best way I've come up with for putting command line arguments
    //  into a vector of HeapElement pointers.
    vector<HeapElement *> argHE;
    for (int i=0; i < argc-1; i++) argHE.push_back(new HeapElement);
    for (int i=0; i < argc-1; i++){
        argHE[i]->key = atoi(argv[i+1]);
        argHE[i]->handle = &(argHE[i]->key); // setting handle (a void*) equal to the key address (an int*)
    }                                         // just to test things out
    MaxHeap h = argHE; 
    for (int i=0; i<h.getheapsize(); i++) {
        cout << "key: " << h.getheap()[i]->key << "\t" << "handle: " << *((int*)h.getheap()[i]->handle) << endl;
    }
    cout << endl;
    HeapElement* e = new HeapElement;
    e->key = 3;
    e->handle = &e->key;
    h.Insert(e);
    for (int i=0; i<h.getheapsize(); i++) {
        cout << "key: " << h.getheap()[i]->key << "\t" << "handle: " << *((int*)h.getheap()[i]->handle) << endl;
    }
    
    // The delete below will fail if h.heap contains degenerate pointers (ptrs that point to the same
    // HeapElement object). This shouldn't happen in practice, since different HeapElements should have
    // different keys, even if they contatain ptrs to the same application object. However, the modern
    // workaround for this is to use shared_ptr<T> instead. Since you can't cast shared_ptr<void> to
    // shared_ptr<int> the same way you can cast void* to anything, I'd have to modify  my hackney example
    // to make HeapElement::handle a shared_ptr<int> specifically.
    // Ideally I'd make an ApplicationObject class and then make HeapElement::handle a 
    // shared_ptr<ApplicationObject>.
    for (int i=0; i< h.getheap().size(); i++) {
        delete h.getheap()[i];
    }
}


