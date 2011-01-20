jmp B
A: inc 
   dprint
   push 0
   ret

B:
	push 20
	call A