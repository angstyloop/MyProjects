"""
An object implementation of a max heap that support sorting and priority queues.
For now, each heap element is equal to its key. To implement a priority queue, we
need to create a Heap Element object that contains the key and the handle.
"""

# heap elements should contain a key and a handle
class HeapElement():
    def __init__(self, key, handle=None):
        self.key = key
        self.handle = handle

# takes a list of heap elements, and returns a max heap.
class MaxHeap():
    def __init__(self, heap):
        self.heapsize = len(heap)
        self.heap = heap
        self.BuildMaxHeap()

    # the following staticmethods are defined a bit differently than
    #   in CLRS since CLRS uses index order 1 and we python uses 0.
    @staticmethod
    def Parent(i):
        return (i+1)//2-1

    @staticmethod
    def Left(i):
        return 2*(i+1)-1

    @staticmethod
    def Right(i):
        return 2*(i+1)+1-1

    def MaxHeapify(self, i):
        l = MaxHeap.Left(i)
        r = MaxHeap.Right(i)
        # lists are mutable so we can nickname self.heap H for brevity
        H = self.heap
        # unfortunately we can't nickname heapsize since scalars are immutable
        if l < self.heapsize and H[l].key > H[i].key:
            largest = l
        else:
            largest = i
        if r < self.heapsize and H[r].key > H[largest].key:
            largest = r
        if largest != i:
           H[i], H[largest] = H[largest], H[i]
           self.MaxHeapify(largest)

    def BuildMaxHeap(self):
        i = self.heapsize//2 - 1
        while i >= 0:
            self.MaxHeapify(i)
            i-=1

    def HeapSort(self):
        # lists are mutable so we can nickname self.heap H for brevity
        H = self.heap
        while self.heapsize > 1:
            H[0].key, H[self.heapsize-1].key = H[self.heapsize-1].key, H[0].key
            self.heapsize -= 1
            self.MaxHeapify(0)
        # we have to set heapsize back to the length of the heap now
        self.heapsize = len(self.heap)

    # returns a handle corresponding to the application object with the largest key
    # runtime: O(1)
    def Max(self):
        return self.heap[0].handle

    # returns a handle to the element of the max heap with the largest key.
    # runtime: O(logn)
    def ExtractMax(self):
        if self.heapsize < 1:
            print "Heap underflow."
            return -1
        MAX = self.heap[0].handle
        self.heap[0] = self.heap[self.heapsize-1]
        self.heapsize -= 1
        self.MaxHeapify(0)
        return MAX

    # Increases the key of the heap element at index i to "key"
    # runtime: O(logn)
    def IncreaseKey(self, i, key):
        H = self.heap
        if H[i].key >= key:
            print "New key must exceed current key."
            return -1
        H[i].key = key
        while i > 0 and H[i].key > H[MaxHeap.Parent(i)].key:
            H[i], H[MaxHeap.Parent(i)] = H[MaxHeap.Parent(i)], H[i]
            i = MaxHeap.Parent(i)

    # Inserts a new HeapElement object at the appropriate key. Uses IncreaseKey().
    # Tacks it onto the end of the heap and then increases it to the right key.
    #runtime: O(logn)
    def Insert(self,element):
        # we assume key is always positive. then -1 is always less than key.
        # these two lines make sure IncreaseKey doesn't throw an error.
        key = element.key
        element.key = -1

        self.heapsize += 1
        # check to see if there's "empty space" in the heap. if not, append. if so just assign.
        # note this will potentially overwrite elements that were previously extracted by
        # ExtractMax().
        if self.heapsize > len(self.heap):
            self.heap.append(element)
        else:
            # overwrites a previously extracted element
            self.heap[self.heapsize-1] = element
        self.IncreaseKey(self.heapsize-1,key)


"""
# let's test it out:
A = [HeapElement(x,x) for x in range(0,10)]
H = MaxHeap(A)
H.Insert(HeapElement(10,"Hello World!"))
print H.heapsize
print len(H.heap)
print H.Max()
"""
