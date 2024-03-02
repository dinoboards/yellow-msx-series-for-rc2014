
	PUBLIC	_commandDrawLine, _waitForCommandCompletion
	PUBLIC	__fromX, __fromY, _longSide, _shortSide, __color, _dir, __operation

	SECTION CODE

	include "msx.inc"

_commandDrawLine:
	DI
	; Set read register to 2 (status)
	LD	A, 2
	OUT	(VDP_ADDR), A
	LD	A, 0x80 | 15
	OUT	(VDP_ADDR), A

	; WAIT FOR ANY PREVIOUS COMMAND TO COMPLETE
_commandDrawLineReady:
	IN	A, (VDP_STAT)
	RRCA
	JR	C, _commandDrawLineReady

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


; _commandPset:
; 	DI
; 	; Set read register to 2 (status)
; 	LD	A, 2
; 	OUT	(VDP_ADDR), A
; 	LD	A, 0x80 | 15
; 	OUT	(VDP_ADDR), A

; 	; WAIT FOR ANY PREVIOUS COMMAND TO COMPLETE
; _commandPsetReady:
; 	IN	A, (0x99)
; 	RRCA
; 	JR	C, _commandPsetReady



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

	PUBLIC	_vramReadFast
;extern void vramReadFast(uint32_t vramAddress, uint8_t* buffer, uint16_t size);
;                      IX+4..7,               IX+8,IX+9,       IX+10, IX+11
_vramReadFast:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+8)
	LD	H, (IX+9)
	LD	C, (IX+10)
	LD	B, (IX+11)

	LD	A, (IX+4)		; vramAddress Low byte

	DI
	LD	A, 0		; BITS A14-A16 in lower part
	OUT	(VDP_ADDR), a

	LD	A, 128 + 14		; REGISTER 14
	OUT	(VDP_ADDR), A

	LD	A, 0		; BITS A0-A7
	OUT	(VDP_ADDR), A

	LD	A, 0		; BITS A8-A13
	OUT	(VDP_ADDR), A

VRAM_READ_LP:
	IN	A, (VDP_DATA)
	LD	(HL), A
	INC	HL
	DEC	BC
	LD	A, B
	OR	C
	JR	NZ, VRAM_READ_LP

	EI

	POP	IX
	RET


	PUBLIC	_vramWriteFast
;extern void vramWriteFast(uint32_t vramAddress, uint8_t* buffer, uint16_t size);
;                      IX+4..7,               IX+8,IX+9,       IX+10, IX+11
_vramWriteFast:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+8)
	LD	H, (IX+9)
	LD	C, (IX+10)
	LD	B, (IX+11)

	LD	A, (IX+4)		; vramAddress Low byte

	DI
	LD	A, 0		; BITS A14-A16 in lower part
	OUT	(VDP_ADDR), a

	LD	A, 128 + 14		; REGISTER 14
	OUT	(VDP_ADDR), A

	LD	A, 0		; BITS A0-A7
	OUT	(VDP_ADDR), A

	LD	A, 0		; BITS A8-A13
	OR	$40
	OUT	(VDP_ADDR), A

VRAM_WRITE_LP:
	LD	A, (HL)
	OUT	(VDP_DATA), A
	INC	HL
	DEC	BC
	LD	A, B
	OR	C
	JR	NZ, VRAM_WRITE_LP

	EI
	POP	IX
	RET


	PUBLIC	_vramReadSlow
;extern void vramReadSlow(uint32_t vramAddress, uint8_t* buffer, uint16_t size);
;                      IX+4..7,               IX+8,IX+9,       IX+10, IX+11
_vramReadSlow:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+8)
	LD	H, (IX+9)
	LD	C, (IX+10)
	LD	B, (IX+11)

	LD	A, (IX+4)		; vramAddress Low byte

	DI
	LD	A, 0		; BITS A14-A16 in lower part
	OUT	(VDP_ADDR), a

	LD	A, 128 + 14		; REGISTER 14
	OUT	(VDP_ADDR), A

	LD	A, 0		; BITS A0-A7
	OUT	(VDP_ADDR), A

	LD	A, 0		; BITS A8-A13
	OUT	(VDP_ADDR), A

VRAM_READS_LP:
	NOP			; 4
	NOP			; 4
	NOP			; 4
	NOP			; 4
	NOP			; 4 -> 24


	IN	A, (VDP_DATA)	; 11
	LD	(HL), A		; 7
	INC	HL		; 6
	DEC	BC		; 6
	LD	A, B		; 4
	OR	C		; 4

	JR	NZ, VRAM_READS_LP	; 12/7 -> 50

	EI

	POP	IX
	RET


	PUBLIC	_vramWriteSlow
;extern void vramWriteSlow(uint32_t vramAddress, uint8_t* buffer, uint16_t size);
;                      IX+4..7,               IX+8,IX+9,       IX+10, IX+11
_vramWriteSlow:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	L, (IX+8)
	LD	H, (IX+9)
	LD	C, (IX+10)
	LD	B, (IX+11)

	LD	A, (IX+4)		; vramAddress Low byte

	DI
	LD	A, 0		; BITS A14-A16 in lower part
	OUT	(VDP_ADDR), a

	LD	A, 128 + 14		; REGISTER 14
	OUT	(VDP_ADDR), A

	LD	A, 0		; BITS A0-A7
	OUT	(VDP_ADDR), A

	LD	A, 0		; BITS A8-A13
	OR	$40
	OUT	(VDP_ADDR), A

VRAM_WRITES_LP:
	NOP			; 4
	NOP			; 4
	NOP			; 4
	NOP			; 4
	NOP			; 4 ->


	LD	A, (HL)		; 7
	OUT	(VDP_DATA), A	; 11
	INC	HL		; 6
	DEC	BC		; 6
	LD	A, B		; 4
	OR	C		; 4


	JR	NZ, VRAM_WRITES_LP	; 12/7

	EI
	POP	IX
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
