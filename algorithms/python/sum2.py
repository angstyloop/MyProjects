# Write an algorithm sum2(x,A) that returns True if there are two unique
# numbers in A that sum to x, and False otherwise. The algorithm below is
# O(n*logn).

# LSum(x,n,A,s,e) returns True if there is an element of A[s:e+1] of A
# other than A[n] such that the sum of A[n] and that element is x, and
# returns False otherwise. I wrote it recursively just to show that I could.
# A loop version will probably run faster. Both versions are O(n)

# Recursive version
"""
def LSum(x,n,A,s,e):
    if s < e:
        m = (e+s)//2
        return LSum(x,n,A,s,m) or LSum(x,n,A,m+1,e)
    return A[n]+A[s]==x and s!=n
"""
# Loop version
def LSum(x,n,A,s,e):
    i=s
    while i<=e:
        if x==A[i]+A[n] and n!=s:
            return True
        i+=1
    return False

# BSum(x,A,s,e) returns True if two distinct elements of the subarray A[s:e+1]
# sum to equal x. I wrote the algorithm recursively because it has O(n*logn),
# as opposed to the version that used two for loops which would be O(n**2).
def BSum(x,A,s,e):
    if s<e:
        m=(e+s)//2
        return BSum(x,A,s,m) or BSum(x,A,m+1,e)
    return LSum(x,s,A,0,len(A)-1)

def sum2(x,A):
    return BSum(x,A,0,len(A)-1)

A = [1,2,3,4,5]
print (sum2(10,A))
