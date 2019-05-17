# Lab 4 Problem 2
# Sean Allen / Katie Stallings
# This program prints the value of variable x to the console.
    .data
x:  .word   14
y:  .word   3
    .text
    .align 2
    .globl main
main:
    #la $t0, x           # load address
    #lw $s0, 0($t0)      # get value stored at that address
	lw $s0, x			 # shortcut, since x is a single word
	
    #la $t0, y           # load address
    #lw $s1, 0($t0)      # get value stored at that address
	lw $s1, y			 # same shortcut

	sub $a0, $s0, $s1   # s0 = a0 - a1 := x - y 

    li $v0, 1           # set up "print int" system call
    syscall             # execute system call

    jr $ra              # return from main method
	
