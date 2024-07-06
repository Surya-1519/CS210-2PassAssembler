;   sum of first 5 numbers and storing that to memory[0]
; 	9998 = loop
; 	9997 = neg, value of A
; 	9996 = new
; 	9995 = sum
stl -4 		;   Memory[sum] = 0
ldc 1 		;   A = 1, B = 0
stl -3 		; 	Memory[new] = 1
; 	Starting the program
ldc -5
loop:
adc 1
call sum 	; B = neg, A = loop, PC = sumLabel
brlz loop
ldl -4		; A = memory[sum], B = neg
ldc 0 		; A = 0, B = sum
stnl 0 		; memory[0] = sum
HALT
; 	A = loop, B = neg here due to call
sum:
stl -1 		;	memory[loop] = A, A = neg, B = neg
stl -2 		;	memory[neg] = A, A = neg, B = neg
ldl -3 		; 	A = memory[new], B = neg
ldl -4 		;	A = memory[sum], B = new
add 		; 	A = A + B, A = new sum
stl -4 		; 	memory[sum] = A, A = new, B = new
adc 1		; 	A = new
stl -3 		;	memory[new] = A, A = new, B = new
ldl -2 		;	A = memory[neg], B = new
ldl -1		; 	A = memory[loop], B = neg
return 		;	A = neg, B = neg