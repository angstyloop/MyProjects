# Sorts a subarray A[p..r] in quadratic time. Usually faster than recursive sorting algorithms
#   for small input size. "Small" depends on the machine but is usually between 7 and 50 elements.
# Runtime: O(n^2)

def InsertionSort(A, p, r):
    for j in range(p+1, r+1):
        key = A[j]
        i = j-1
        while i>=p and A[i]>key:
            A[i+1] = A[i]
            i -= 1
        A[i+1] = key

"""
#test it out
from sys import argv
A = [int(x) for x in argv[1:]]
InsertionSort(A, 0, len(A)-1)
print A
"""
