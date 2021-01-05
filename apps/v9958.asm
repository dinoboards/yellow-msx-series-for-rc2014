
	PUBLIC	_outDat, _outCmd, _commandDrawLine, _waitForCommandCompletion
	PUBLIC	__fromX, __fromY, _longSide, _shortSide, __color, _dir, __operation

	SECTION CODE

	include "msx.inc"


; void outDat(uint8_t b) __z88dk_fastcall
_outDat:
	LD	A, L
	OUT	(VDP_DATA), A
	RET

; void outCmd(uint8_t b) __z88dk_fastcall
_outCmd:
	LD	A, L
	OUT	(VDP_ADDR), A
	RET


_commandDrawLine:
	DI
	; Set read register to 2 (status)
	LD	A, 2
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

waitForCommandCompletionLoop:
	IN	A, (0x99)
	RRCA
	JR	C, waitForCommandCompletionLoop

	; SET INDIRECT REGISTER TO 36
	LD	A, 36
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 17
	OUT	(VDP_ADDR), A

	LD	HL, __fromX
	LD	C, VDP_REGS
	LD	B, 11
	OTIR

	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	EI
	RET

_waitForCommandCompletion:
	DI
	; Set read register to 2
	LD	A, 2
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

_waitForCommandCompletionLoop:
	IN	A, (0x99)
	RRCA
	JR	C, _waitForCommandCompletionLoop

	XOR	A
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	EI
	RET

	SECTION	DATA

__fromX:	DW	0
__fromY:	DW	0
_longSide:	DW	0
_shortSide:	DW	0
__color:	DB	0
_dir:		DB	0
__operation:	DB	0

	SECTION IGNORE
