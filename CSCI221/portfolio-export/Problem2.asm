# Sean Allen
# get the max of n input integers, n= first input

.globl	main

.data

# define some strings
msg0:
	.asciiz	"Enter a natural number: "
msg1:
	.asciiz	"Enter "
msg2:
	.asciiz	" positive integers: "
msg3:
	.asciiz	" The max integer is: "
endl:
	.asciiz	"\n"

.text
.align 2
## vars
##   n:   $s0
##   max: $s1
##   i:   $s2
##	 in:  $s3

main:
	li		$v0, 4			# print msg0
	la		$a0, msg0
	syscall

	li		$v0, 5			# get n from input
	syscall
	move	$s0, $v0

	li		$v0, 4			# print endl
	la		$a0, endl
	syscall

	li		$v0, 4			# print msg1
	la		$a0, msg1
	syscall

	li		$v0, 1			# print n
	move	$a0, $s0
	syscall

	li		$v0, 4			# print msg2
	la		$a0, msg2
	syscall

	li		$v0, 4			# print endl
	la		$a0, endl
	syscall

	#li		$s1, 0			# max=0
	li		$s2, 0			# i=0
for:
	beq		$s2, $s0, endf	# if i==n exit

	li		$v0, 5			# get in from input
	syscall					
	move	$s3, $v0

	ble		$s3, $s1, inc   # if in <= max jump to increment
	move $s1, $s3			# otherwise replace max with in	
inc:
	add		$s2, $s2, 1		#	  ++i;     
	b		for				# continue
endf:

	li		$v0, 4			# print msg3
	la		$a0, msg3
	syscall

	li		$v0, 1			# print max
	move	$a0, $s1
	syscall

	li		$v0, 4			# print endl twice
	la		$a0, endl
	syscall
	li		$v0, 4
	la		$a0, endl
	syscall

	li		$v0, 10			# all done
	syscall

