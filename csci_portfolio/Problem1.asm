# Lab 5 Problem 1

# Sean Allen / Melanie Marideth

	.data

# allocate memory for a 20-char string plus the null char 
str:
	.byte 21

	.text
	.align 2
	.globl main

##function for Part (d) (main)
main:

	addiu $sp, $sp, -4
	sw $ra, 0($sp)

	# read a string from the user

	li $v0, 8 	# 8 means read string for syscall
	la $a0, str # address of input buffer (str)
	li $a1, 21	# length of input buffer (read_string adds the '\0' for us)
	syscall

	# convert it to uppercase

	jal convertString 	# recall a0=str still

	# print it

	li $v0, 4		# means print string for syscall
	la $a0, str 
	syscall

	# return

main_return:
	lw $ra, 0($sp)		# load return address
	addiu $sp, $sp, 4   # shrink stack
	jr $ra


## function for Part (a)
isLowerCase:
	# a0 = character c
	# v0 <- boolean result

	# NOTE: c is lowercase <=>  c >= 97 and c <= 122	

	blt $a0, 97, return0  # if c is NOT lowercase, jump to return0
	bgt $a0, 122, return0 #

	#otherwise return 1

return1:
	li $v0, 1
	jr $ra

return0:
	li $v0, 0
	jr $ra

## function for Part (b)
getUpperCase:
	# a0 = character c
	# v0 <- c - 32 or 0 if not isLowerCase(c)

	# We're going to call isLowerCase(), so we need
	# 	to save the return address to the stack

	addiu $sp, $sp, -4	# grow stack
	sw $ra, 0($sp)		# save return address

	
	jal isLowerCase 	# if c is lower case, jump to islower
	beq $v0, 1, islower	

isupper:				# if c is NOT lower case, return 0
	li $v0, 0
	lw $ra, 0($sp) 		# load return address 
	addiu $sp, $sp, 4	# shrink stack
	jr $ra

islower:				# return c - 32
	add $v0, $a0, -32
	lw $ra, 0($sp)	
	addiu $sp, $sp, 4
	jr $ra

## function for Part (c)
convertString:
	# a0 = address of string
	# v0 <- void
	
	addiu $sp, $sp, -4
	sw $ra, 0($sp)

	move $s0, $a0	# save address into save thingy

	lb $s1, 0($s0)				#   load current character (no sign extension)

loop:
	beq $s1, $zero, isnull				# while not the null character 
	move $a0, $s1						# 	 move current char to a0 for isLowerCase()
	jal isLowerCase						# 	 call isLowerCase()
	beq $v0, $zero, next				#   	if current character is lowercase...
	jal getUpperCase					#   		v0 = current character to uppercase
	sb $v0, 0($s0)						#			save v0 to the actual string


next:									#		else jump to next, without changing current character
	add $s0, $s0, 1						#   		increment current address
	lb $s1, 0($s0)						# 			load the next byte
	b loop

isnull:							# we've reached the end of the string
	lw $ra, 0($sp)				# so we're done.
	addiu $sp, $sp, 4			
	jr $ra	

	
	


	




