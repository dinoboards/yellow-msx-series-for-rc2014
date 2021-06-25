
	include "msx.inc"

	SECTION	CODE

;----------------------------------------------------------------
; ROUTINE GETSLT
; ENTRY: HL = ADDRESS IN RAM
; OUTPUT: B = SLOT NUMBER CORRESPONDING TO THE HL ADDRESS
; MODIFIES: A AND B

	PUBLIC	GETSLT
GETSLT:
	PUSH	HL
	LD	A,H
	RLA
	RLA
	RLA		; BIT 6-7 TO BIT 1-0
	AND	3	; RESET UNUSED BITS
	LD	C,A
	LD	B,0
	LD	HL,RAMAD0
	ADD	HL,BC
	LD	B,(HL)	; B = SLOT NUMBER OF MAIN RAM
	POP	HL
	RET

; extern uint8_t getSlotPage0(void* p) __z88dk_fastcall;
	PUBLIC	_getSlotPage0
_getSlotPage0:
	CALL	GETSLT
	LD	L, B
	LD	H, 0
	RET
