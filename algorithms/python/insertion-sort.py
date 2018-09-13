# sorts and array, modifying the original array

def insertion_sort(A):
    for j in range(1,len(A)):
        key = A[j]
        i = j - 1;
        while i > 0 and A[i] > key:
            A[i+1] = A[i]
            i-=1
        A[i+1] = key

# test it out

#A = [2,3,1,5,7,8,10]
#insertion_sort(A)
#print A


