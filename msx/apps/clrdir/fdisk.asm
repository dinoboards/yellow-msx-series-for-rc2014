
	SECTION BSS_UNINITIALIZED

	PUBLIC	_superBuffer
_superBuffer:	DS	0	; 4096

	SECTION	CODE

	PUBLIC	_sectorBootCode
_sectorBootCode:

	RET	NC
	LD	($C07B), DE
	LD	DE, $C078
	LD	(HL), E
	INC	HL
	LD	(HL), D
	LD	DE, $C080
	LD	C, $0F
	CALL	$F37D
	INC	A
	JP	Z, $4022
	LD	DE, $100
	LD	C, $1A
	CALL	$F37D
	LD	HL, 1
	LD	($C08E), HL
	LD	HL, $3F00
	LD	DE, $C080
	LD	C, $27
	PUSH	DE
	CALL	$F37D
	POP	DE
	LD	C, $10
	CALL	$F37D
	JP	$0100
	LD	L, B
	RET	NZ
	CALL	0
	JP	$4022
	NOP
	DB	"MSXDOS  SYS"
	nop
	nop
	nop
	nop

; uint32_t getNewSerialNumber()
	PUBLIC	_getNewSerialNumber
_getNewSerialNumber:

	LD	A, R
	XOR	B
	LD	E, A
	OR	128
	LD	B, A
GNSN_1:
	NOP
	DJNZ	GNSN_1

	LD	A, R
	XOR	E
	LD	D, A
	OR	64
	LD	B, A
GNSN_2:
	NOP
	NOP
	DJNZ	GNSN_2

	LD	A, R
	XOR	D
	LD	L, A
	OR	32
	LD	B, A
GNSN_3:
	NOP
	NOP
	NOP
	DJNZ	GNSN_3

	LD	A, R
	XOR	L
	LD	H, A

	RET
