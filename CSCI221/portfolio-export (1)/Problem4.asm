# Lab 4 Problem 4
# Sean Allen / Katie Stallings

# This program read in two positive integers, a and b, from the user and
# prints a * b.
    .globl main
    .text
    .align 2
main:
    li   $v0, 5         # set up reading of first int
    syscall

    move $t0, $v0       # store first int, a, into $t0
    blt  $t0, 0, done   # go to done if a not positive

    li   $v0, 5         # set up reading of second int
    syscall

    move $t1, $v0       # store second int, b, into $t1
    blt  $t1, 0, done   # go to done if b not positive

    mul $a0, $t0, $t1	# store a*b in a0 for print

    li   $v0, 1       	# set up "print int" system call
    syscall           	# execute system call

done:
	jr   $ra          	# return from main method
