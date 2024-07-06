00000 		  ;   sum of first 5 numbers and storing that to memory[0]
00000 		  ; 	9998 = loop
00000 		  ; 	9997 = neg, value of A
00000 		  ; 	9996 = new
00000 		  ; 	9995 = sum
00000 fffffc03 stl -4 		;   Memory[sum] = 0
00001 000100 ldc 1 		;   A = 1, B = 0
00002 fffffd03 stl -3 		; 	Memory[new] = 1
00003 		  ; 	Starting the program
00003 fffffb00 ldc -5
00004 		  loop:
00004 000101 adc 1
00005 00050d call sum 	; B = neg, A = loop, PC = sumLabel
00006 fffffd10 brlz loop
00007 fffffc02 ldl -4		; A = memory[sum], B = neg
00008 000000 ldc 0 		; A = 0, B = sum
00009 000005 stnl 0 		; memory[0] = sum
0000a 000012 HALT
0000b 		  ; 	A = loop, B = neg here due to call
0000b 		  sum:
0000b ffffff03 stl -1 		;	memory[loop] = A, A = neg, B = neg
0000c fffffe03 stl -2 		;	memory[neg] = A, A = neg, B = neg
0000d fffffd02 ldl -3 		; 	A = memory[new], B = neg
0000e fffffc02 ldl -4 		;	A = memory[sum], B = new
0000f 000006 add 		; 	A = A + B, A = new sum
00010 fffffc03 stl -4 		; 	memory[sum] = A, A = new, B = new
00011 000101 adc 1		; 	A = new
00012 fffffd03 stl -3 		;	memory[new] = A, A = new, B = new
00013 fffffe02 ldl -2 		;	A = memory[neg], B = new
00014 ffffff02 ldl -1		; 	A = memory[loop], B = neg
00015 00000e return 		;	A = neg, B = neg
