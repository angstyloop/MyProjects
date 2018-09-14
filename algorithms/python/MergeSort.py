'''
MergeSort recursively sorts a list of numbers using the function Merge. The recursion bottoms
out when the input to MergeSort is a list of length 1. Runtime is theta(N*logN).

Merge takes an array and the indices of two sorted subarrays. Runtime is theta(N).
'''

INFINITY = 10**10

# p, q, and r are indices marking the beginning and end of the two input arrays
def Merge(A,p,q,r):
    # n1+1, n2+1 are going to be the lengths of L,R
    n1 = q-p+1
    n2 = r-q
    L = [None]*(n1+1)
    R = [None]*(n2+1)
    # copy the appropriate subarrays into L and R
    for i in range(n1):
        L[i] = A[p+i]
    for i in range(n2):
        R[i] = A[q+1+i]
    # the last element in L and R is something bigger than every number in the array
    L[n1]=INFINITY
    R[n2]=INFINITY
    # now copy the two subarrays back into A in order from least to greatest
    i=0
    j=0
    for k in range(p,r+1):
        if L[i] <= R[j]:
            A[k] = L[i]
            i+=1
        else:
            A[k] = R[j]
            j+=1

# MergeSort uses Merge to recursively sort the subarray of A given by the indices p,r
def MergeSort(A,p,r):
    if r > p:
        q = (p+r)//2
        MergeSort(A,p,q)
        MergeSort(A,q+1,r)
        Merge(A,p,q,r)

# let's test it out to make sure everything works
from random import random

# The code below makes an array of random (possibly repeated) integers greater than or
# equal to zero but less than 100. Then it sorts the array and checks that it's sorted.
# Then it checks to make sure the sorted list has the same elements as the unsorted list.
print "-------"
print "Test #1"
print "-------"
SIZE = 100
rNums = [random()*SIZE//1 for i in range(SIZE)]
set1 = set(rNums)

MergeSort(rNums, 0, SIZE-1)
set2 = set(rNums)

srtd = True
for i in range(1, len(rNums)):
    if rNums[i] < rNums[i-1]:
        srtd = False
if srtd:
    print "The list is sorted!"
else:
    print "The list didn't get sorted..."

if set1 == set2:
    print "...and it has all the same elements it started with."
else:
    print "Hmm... Some elements are missing.! Check your code."

# Here's a shorter test using random.shuffle
print "-------"
print "Test #2"
print "-------"
from random import shuffle
a = [i for i in range(10)] # [0,1,...,9]
shuffle(a) # shuffles a in place
print "Unsorted: ", a
MergeSort(a, 0, len(a)-1)
print "Sorted: ", a


