# An implementation of the Kadane maximum subarray algorithm.

# If we don't care about the indices of the subarray, the algorithm for getting the maximum sum is very brief.
# Runtime: O(n)
def MaxSubArraySum(A):
    maxEndsHere = maxSoFar = A[0]
    for x in A[1:]:
        maxEndsHere = max(maxEndsHere + x, x)
        maxSoFar = max(maxSoFar, maxEndsHere)
    return maxSoFar

# If we want the indices we have to note when maxSoFar changes, I did this by explicitly using nested if statements
#   instead of the max() function. The leftmost maximum subarray is preferred. We could get the rightmost by
#   fiddling with the strictness of the inequalities.
# Runtime: O(n)
def MaxSubArray(A):
    maxEndsHere = maxSoFar = A[0]
    l = r = 0
    for i in range(1,len(A)):
        if maxEndsHere >= 0:
            maxEndsHere += A[i]
            if maxEndsHere > maxSoFar:
                maxSoFar = maxEndsHere
                r=i
        else:
            maxEndsHere = A[i]
            if maxEndsHere > maxSoFar:
                maxSoFar = maxEndsHere
                r=l=i
    return [maxSoFar,l,r]


"""
# let's test it out
A = [1,-5,3,2,-10,1,-1,3,2]
# The leftmost max subarray is [3,2] and the sum is 5. The indices of subarray are 2,3.
print MaxSubArraySum(A) == 5
print MaxSubArray(A) == [5,2,3] # 2 and 3 are indices, not the elements of the subarray
"""
# everything seems to work, so let's take support command line arrays
from sys import argv
A = [int(x) for x in argv[1:]]
res = MaxSubArray(A)
s = "The maximum subarray has indices (" + str(res[1]) + ", " + str(res[2]) + ") and sums to " + str(res[0]) + "."
print s
