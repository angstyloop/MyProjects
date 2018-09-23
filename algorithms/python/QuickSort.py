# an implementation of quicksort. the last element is used as the pivot. later we'll randomize the pivot choice.

# Partitions the subarray A[p..r] of A, using the last element A[r] as the pivot. Elements less
#   than A[r] go to the left, and elements greater than or equal to the pivot go to the right.
# Runtime: O(n)
def Partition(A, p, r):
    i = p-1
    for j in range(p,r):
        if A[j] < A[r]:     # last element A[r] is used as the pivot
            i+=1
            A[i],A[j] = A[j],A[i]
    A[i+1],A[r] = A[r],A[i+1]
    return i+1

# Worst case runtime: O(n^2) i.e. when the partition is always 1 to n-1
# Best case runtime: O(n*logn) i.e. when the partition is always a constant ratio like 1/2 to 1/2 or 9/10 to 1/10
# We can get O(n*logn) EXPECTED runtime by randomizing the pivot
def QuickSort(A, p, r):
    if p < r:
        q = Partition(A, p, r)
        QuickSort(A, p, q-1)
        QuickSort(A, q+1, r)

"""
#Let's test out QuickSort before randomizing the pivot.
from sys import argv
argNums = [int(x) for x in argv[1:]]
QuickSort(argNums, 0, len(argNums)-1)
print argNums
"""

# Now let's randomize the pivot
# Runtime: O(n)
from random import random
def RPartition(A, p, r):
    i = p + int((r-p+1)*random()) #pick a random index from [p..r]
    A[i],A[r] = A[r],A[i]    # ..and swap that element with the last one
    return Partition(A, p, r)       # ..then just call Partition!

# The new version of quicksort just uses RPartition instead of Partition.
# Expected runtime: O(n*logn)
def RQuickSort(A, p, r):
    if p < r:
        q = RPartition(A, p, r)
        QuickSort(A, p, q-1)
        QuickSort(A, q+1, r)

# Everything should still work, but let's check again anyway.
from sys import argv
argNums = [int(x) for x in argv[1:]]
#RQuickSort(argNums,0,len(argNums)-1)
RPartition(argNums,0,len(argNums)-1)
print argNums


# Good to go!
