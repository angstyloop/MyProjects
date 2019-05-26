# Sean Allen
# HW 3 Problem 4 : count the number of even integers in arr

.globl main
.globl numEvens

.data
# length 10 32-bit integer array	
# numevens := 4
arr:												  		 
	.word 5, 1, 0, 3, 8, 1, 5, 2, 3, 2 	 							


# length of arr
num:
	.word 40 

.text
.align 2

main:

    la $a0, arr     	# set a0 = arr 
	#la $t0, num		# set a1 = num
    #lw $a1, 0($t0)         
	lw $a1, num			# shortcut since num is a single word
    jal numEvens 		# call function numEvens(arr, num)
 
    move    $a0, $v0    # print the number of evens
    li      $v0, 1
    syscall
 
    li      $v0, 10    	# all done, exit
    syscall


# function
# v0 <- numEvens(a0, a1)
# a0 := arr, pointer to arr[0] 
# a1 := num, length of arr
# v0 := number of even integers in arr
numEvens:
	# vars
    li  $t0, 0      # i, an array index
	li  $v0, 0      # count, the number of occurences of c, to be returned
 
    # more vars
    #   t1 := arr+i, a pointer to arr[i]
    #   t2 := arr[i]	
	#	t3 := 0 if even, 1 if odd

	## note: could probably just recycle t1 instead of using t3. but using t3
	## 		is clearer for the purposes of this assignment.

loop:
  	bge $t0, $a1, exit      # if i >= num, jump to exit
    add $t1, $a0, $t0       # now t1 is arr+i
    lw  $t2, 0($t1)         # now t2 is arr[i]
	and $t3, $t2, 1			# t3 = 0 if arr[i] even, 1 if arr[i] odd
    beq $t3, 1, skip      	# if arr[i] odd (i.e. if t3==1), jump to skip
    add $v0, $v0, 1         # otherwise ++count

skip:
    add    $t0, $t0, 4     	# ++i
    j  loop               	# jump to loop

exit:
    jr  $ra         # return count, the number of evens
