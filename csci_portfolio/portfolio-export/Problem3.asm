# Sean Allen
# HW 3 Problem 3: return the number of occurences of the letter c in str

.globl main
.globl countOccurences

.data 

# global string of length 11
# number of c's : 5
str:
	.asciiz "abcccabcabc"

len:
	.word 11

.text
.align 2

main: 

	la $a0, str			# set a0 = str
	lw $a1, len 		# set a1 = str.length
	li $a2, 'c'			# set a2 = c
	jal countOccurences	# call function countOccurences(str, str.length, c)

    move    $a0, $v0 	# print the number of occurences of the letter c in str
	li      $v0, 1          
    syscall

	li      $v0, 10    # all done, exit
    syscall


# function
# v0 <- countOccurences(a0, a1, a2)
# args
# a0 := str, a pointer to str[0]
# a1 := length of str
# a2 := character c to be counted
countOccurences:
	# vars
	li	$t0, 0		# i, an array index
	li	$v0, 0		# count, the number of occurences of c, to be returned
	
	# more vars
	#	t1 := str+i, a pointer to str[i]
	#	t2 := str[i]

loop:
	bge	$t0, $a1, exit		# if i >= str.length, jump to exit
	add	$t1, $a0, $t0		# now t1 is str+i 
	lb	$t2, 0($t1)			# now t2 is str[i]
	bne	$t2, $a2, skip		# if str[i] != c, jump to skip
	add	$v0, $v0, 1			# otherwise ++count
	
skip: 
	 add	$t0, $t0, 1		# ++i
	 j	loop				# jump to loop	

exit:
	jr	$ra			# return count, the number of occurences of c

