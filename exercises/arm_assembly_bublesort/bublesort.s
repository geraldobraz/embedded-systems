meuarray:
	.word 10
	.word 7
	.word 2
	.word 3
	.word 9
	.word 5
	.word 2
	.word 7
	.word 8
_start:	
ADR R0, meuarray
MOV R1, #8	;Counter = ArraySize - 1
MOV R5, #0	;Swap flag set to zero

loop:
	LDR R2, [R0] 	;Load current element value into R2
	ADD R4, R0, #4  ;Going to the next address and saving it into R4
	LDR R3, [R4]	;Load next element value into R3
	CMP R3,R2		;Comparison between R2 and R3
	STRLT R3, [R0]  ;If R3 < R2 then a swap both
	STRLT R2, [R4]
	MOVLT R5, #1	;If a swap occured, set R5 to 1
	MOV R0,R4		;Mov R0 to the next address
	SUBS R1, R1, #1	;Subtracts 1 from counter
	BNE loop

swap:
	CMP R5,#1 			;Checks if during the loop a swap occured
	MOVEQ R5,#0			;If so, reset R5 flag
	MOVEQ R1, #8		;Reinitialize counter
	ADREQ R0, meuarray	;Point to first address on the array 
	BGE loop
	

