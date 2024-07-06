; Code to find max from first n elements of memory array
;
; Writing the memory
ldc 0
ldc 0
; A = 0, B = 0
ldc 6
ldc 0
stnl 0
ldc 7
ldc 0
stnl 1
ldc 4
ldc 0
stnl 2
ldc 8
ldc 0
stnl 3
;
; 9998 = loop, 9997 = neg, 9996 = max, 9995 = ind, 9994 = tmp
ldc -1
stl -4 			; memory[ind] = -1
ldc 0
ldc 0
ldnl 0 			; A = memory[0]
stl -3 			; memory[max] = memory[0], A = 0, B = 0
; 
; Starting the program
ldc -4
Loop:
adc 1
call Compare	; B = neg, A = loop, PC = Compare
brlz Loop
ldl -3 			; A = memory[max]
ldc 0 			; A = 0, B = max
stnl 0 			; memory[0] = max
HALT
;
;	A = loop, B = neg
Compare:
stl -1 		; 	memory[loop] = A, A = neg, B = neg
stl -2		;	memory[neg] = A, A = neg, B = neg
ldl -3		; 	A = memory[max], B = neg
ldl -4		; 	A = memory[ind]+1, B = max
adc 1
stl -4		;	memory[ind] = A = currInd, A = max, B = max
ldl -4		; 	A = memory[ind], B = max
ldnl 0 		;   A = memory[i], B = earlier max
sub			; 	A = B - A
brlz Store	; 	if (A < 0) Store
ldl -2		;	A = memory[neg], A = neg, B = earlier max
ldl -1		;	A = memory[loop], A = loop, B = B - A
return		;   A = neg, B = neg
;
; A = B - A, B = earlier max
Store:
ldl -4		;	A = memory[ind], A = earlier max, B = earlier max
ldnl 0		;	A = memory[i], B = earlier max
stl -3		;	memory[max] = A, A = earlier max, B = earlier max
ldl -2		;	A = memory[neg], B = max
ldl -1		;	A = memory[loop], A = loop, B = neg
return		;   A = neg, B = neg