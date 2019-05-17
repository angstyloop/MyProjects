# Lab 4 Problem 3:
# Sean Allen / Katie Stallings

# Read in two integers from the user and print
#   out the larger of the two values.
#

	.globl main

	.data

xprompt: 
	.asciiz "x: "

yprompt: 
	.asciiz "y: "

	.text

	.align 2

main:
	# var cheatsheet
	# 	s0 := x
	#	s1 := y

	li      $v0, 4          # prompt user for x
    la      $a0, xprompt
    syscall

	li 		$v0, 5 		# get x from user
	syscall
    move    $s0, $v0


	li      $v0, 4          # prompt user for y
    la      $a0, yprompt
    syscall

	li 		$v0, 5 		# get y from user
	syscall
    move    $s1, $v0

	bge $s0, $s1, printx 	# if x >= y, jump to printx
	j printy	

printx:
	li      $v0, 1		# print x
    move    $a0, $s0
    syscall
	j exit

printy:
	li      $v0, 1		# print y
    move    $a0, $s1
    syscall
	#j exit

exit:
	jr $ra				# return from main

