00000 		  ; Code to find max from first n elements of memory array
00000 		  ;
00000 		  ; Writing the memory
00000 000000 ldc 0
00001 000000 ldc 0
00002 		  ; A = 0, B = 0
00002 000600 ldc 6
00003 000000 ldc 0
00004 000005 stnl 0
00005 000700 ldc 7
00006 000000 ldc 0
00007 000105 stnl 1
00008 000400 ldc 4
00009 000000 ldc 0
0000a 000205 stnl 2
0000b 000800 ldc 8
0000c 000000 ldc 0
0000d 000305 stnl 3
0000e 		  ;
0000e 		  ; 9998 = loop, 9997 = neg, 9996 = max, 9995 = ind, 9994 = tmp
0000e ffffff00 ldc -1
0000f fffffc03 stl -4 			; memory[ind] = -1
00010 000000 ldc 0
00011 000000 ldc 0
00012 000004 ldnl 0 			; A = memory[0]
00013 fffffd03 stl -3 			; memory[max] = memory[0], A = 0, B = 0
00014 		  ;
00014 		  ; Starting the program
00014 fffffc00 ldc -4
00015 		  Loop:
00015 000101 adc 1
00016 00050d call Compare	; B = neg, A = loop, PC = Compare
00017 fffffd10 brlz Loop
00018 fffffd02 ldl -3 			; A = memory[max]
00019 000000 ldc 0 			; A = 0, B = max
0001a 000005 stnl 0 			; memory[0] = max
0001b 000012 HALT
0001c 		  ;
0001c 		  ;	A = loop, B = neg
0001c 		  Compare:
0001c ffffff03 stl -1 		; 	memory[loop] = A, A = neg, B = neg
0001d fffffe03 stl -2		;	memory[neg] = A, A = neg, B = neg
0001e fffffd02 ldl -3		; 	A = memory[max], B = neg
0001f fffffc02 ldl -4		; 	A = memory[ind]+1, B = max
00020 000101 adc 1
00021 fffffc03 stl -4		;	memory[ind] = A = currInd, A = max, B = max
00022 fffffc02 ldl -4		; 	A = memory[ind], B = max
00023 000004 ldnl 0 		;   A = memory[i], B = earlier max
00024 000007 sub			; 	A = B - A
00025 000310 brlz Store	; 	if (A < 0) Store
00026 fffffe02 ldl -2		;	A = memory[neg], A = neg, B = earlier max
00027 ffffff02 ldl -1		;	A = memory[loop], A = loop, B = B - A
00028 00000e return		;   A = neg, B = neg
00029 		  ;
00029 		  ; A = B - A, B = earlier max
00029 		  Store:
00029 fffffc02 ldl -4		;	A = memory[ind], A = earlier max, B = earlier max
0002a 000004 ldnl 0		;	A = memory[i], B = earlier max
0002b fffffd03 stl -3		;	memory[max] = A, A = earlier max, B = earlier max
0002c fffffe02 ldl -2		;	A = memory[neg], B = max
0002d ffffff02 ldl -1		;	A = memory[loop], A = loop, B = neg
0002e 00000e return		;   A = neg, B = neg
