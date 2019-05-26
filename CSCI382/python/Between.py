# Between(A,x,y) returns the number of elements in that are greater than x and less than y.

def Between(A,x,y):
    i=0
    j=len(A)-1
    while i<=j:
        if A[i]<=x:
            i+=1
        elif A[j]>=y:
            j-=1
        else:
            break
    return j-i+1

from sys import argv

try:
    x=int(argv[1])
    y=int(argv[2])
    print Between([1,2,3,7,8,9],x,y)

except IndexError:
    print "Provide exactly two arguments."


