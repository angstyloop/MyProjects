# an object implementation of HeapSort (and the operations required to write
#   HeapSort).

# takes an array and optional heapsize, turns the array into a MaxHeap.
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
        if l < self.heapsize and H[l] > H[i]:
            largest = l
        else:
            largest = i
        if r < self.heapsize and H[r] > H[largest]:
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
            H[0], H[self.heapsize-1] = H[self.heapsize-1], H[0]
            self.heapsize -= 1
            self.MaxHeapify(0)


"""
# let's test it out on an array from the command line
from sys import argv
a = MaxHeap([int(x) for x in argv[1:]])
a.HeapSort()
print a.heap
"""
