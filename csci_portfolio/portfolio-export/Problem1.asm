# Sean Allen
# HW 3 Problem 1 :  sum the numbers 1 .. n, where n=input

.globl	main

.data
# define some strings
msg0:
	.asciiz	"Enter a natural number: "
msg1:
	.asciiz	"The first "
msg2:
	.asciiz	" integers sum to: "
endl:
	.asciiz	"\n"

.text
.align 2
## vars
##   n:   $s0
##   sum: $s1
##   i:   $s2

main:
	li		$v0, 4			# print msg0
	la		$a0, msg0
	syscall

	li		$v0, 5			# get n from input
	syscall
	move	$s0, $v0

	li		$s1, 0			# sum=0
	li		$s2, 0			# i=0
for:
	blt		$s0, $s2, endf	# if n < i exit
	add		$s1, $s1, $s2	#     sum += i;
	add		$s2, $s2, 1		#	  ++i;     
	b		for				# continue
endf:

	li		$v0, 4			# print msg1
	la		$a0, msg1
	syscall

	li		$v0, 1			# print n
	move	$a0, $s0
	syscall

	li		$v0, 4			# print msg2
	la		$a0, msg2
	syscall

	li		$v0, 1			# print sum
	move	$a0, $s1
	syscall

	li		$v0, 4			# print endl twice
	la		$a0, endl
	syscall
	li		$v0, 4
	la		$a0, endl
	syscall

	li		$v0, 10			# all done, exit
	syscall

