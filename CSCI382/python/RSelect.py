# Uses a random partitioning algorithm RPartition to select the ith order statistic (i.e.
#   the ith smallest value) of an array
#   in expected linear time, with worst-case quadratic runtime.

# Partition the subarray A[p..r] of a around a random element. Elements strictly less than the
#   pivot go to the left.
# Runtime: O(n)

# Regular partition: partitions around the last element.
# Runtim: O(n)
def Partition(A, p, r):
    i = p-1
    for j in range(p,r):
        if A[j] < A[r]:
            i+=1
            A[j],A[i] = A[i],A[j]
    A[r],A[i+1] = A[i+1],A[r]
    return i+1

# Random partition:
# Runtime: O(n)
from random import random
def RPartition(A, p, r):
    i = p + int(random()*(r-p+1)) # pick a random index from [p..r]
    A[i],A[r] = A[r],A[i]
    return Partition(A, p, r)

# Recursively select from the side of the partition where we know the ith order statistic
#   must be. Recursion stops when the pivot itself is the ith order statistic, or when Select
#   sees a length 1 list.
# Expected Runtime: <O(n)>; Worst case: <O(n^2)>
def RSelect(A, p, r, i):
    if p==r:
        return A[p]     # we've recursed down to a length 1 list
    q = RPartition(A, p, r)
    k = q-p+1
    if k==i:
        return A[q]     # the pivot value is the answer
    elif i<k:
        return RSelect(A, p, q-1, i)
    else:
        return RSelect(A, q+1, r, i-k)

