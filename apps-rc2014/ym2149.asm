
	SECTION code_user

	PUBLIC	_ym2149Probe, _getControllerDirection, _getControllerButton

PSG_REGS:	EQU	$A0             ; PSG register write port
PSG_DATA:	EQU	$A1             ; PSG value write port
PSG_STAT:	EQU	$A2             ; PSG value read port

PSG_PD_RG:	EQU	$00		; PERIOD REGISTER (A TO C)
PSG_LV_RG:	EQU	$08		; LEVEL REGISTERS (A TO C)

	; RETURNS 0 IF NOT FOUND
	; -1 IF FOUND YM
_ym2149Probe:
	LD	A, 3			; R3 - 2ND CHANNEL HIGH BYTE
	DI
	OUT	(PSG_REGS), A
	LD	A, $55
	OUT	(PSG_DATA), A
	IN	A,(PSG_STAT)
	EI
	CP	$55			; REGISTER
	JR	Z, found
;
	LD	L, 0
	RET

found:					; RESET THE CHIP
	XOR	A
	LD	C, PSG_REGS
	LD	B, 10

clearAllRegisters:
	DI
	OUT	(C), B
	OUT	(PSG_DATA), A
	DEC	B
	JP	P, clearAllRegisters

	LD	A,$07
	DI
	OUT	(PSG_REGS),A
	LD	A,$B8		; PORT B AS OUTPUT, PORT A AS INPUT, NOISE CHANNEL NOT MIXED, TONE CHANNELS MIXED
	OUT	(PSG_DATA),A

	LD	A, $0F
	OUT	(PSG_REGS),A
	LD	A, $8F		; STROBE OFF, TRIGGERS ON
	OUT	(PSG_DATA),A
	EI
	LD	L, 255
	RET



; UINT8_T GETCONTROLLERDIRECTION(UINT8_T PORT)
;--------------------------------
; FUNCTION : RETURNS THE JOYSTICK STATUS
; INPUT	   : L  - JOYSTICK NUMBER TO TEST (0 = CURSORS, 1 = PORT 1, 2 = PORT 2)
; OUTPUT   : L  - DIRECTION (1 to 8)
; REGISTERS: ALL
_getControllerDirection:
; JOY_STC1:
; ;PSG REG 15H
; ;0J001111
; ;PSG REG 14H
; ;00BARLDU
	LD	E, $00
	DEC	L
	JR	Z,controller1
	SET	6, E	; SELECT STICK 2
controller1:
	LD	A,$0F
	DI
	OUT	(PSG_REGS),A
	IN	A,(PSG_STAT)

	AND	$BF
	OR	E
	OUT	(PSG_DATA),A

	LD	A,$0E
	OUT	(PSG_REGS),A
	IN	A,(PSG_STAT)
	EI
	CPL
	AND	$0F	; 0000RLDU

	LD	HL, directMappingTable
	LD	B, 0
	LD	C, A
	ADD	HL, BC
	LD	A, (HL)
	AND	A
	LD	L, A
	RET

 directMappingTable:
	;	0   1   2   3   4   5   6   7
	DB	$00,$01,$05,$00,$07,$08,$06,$07
	;	8   9   A   B   C   D   E   F
	DB	$03,$02,$04,$03,$00,$01,$05,$00


;--------------------------------
; FUNCTION : RETURNS CURRENT TRIGGER STATUS
; INPUT    : L  - TRIGGER BUTTON TO TEST
;	1 = PORT 1, BUTTON A
;	2 = PORT 2, BUTTON A
;	3 = PORT 1, BUTTON B
;	4 = PORT 2, BUTTON B
; OUTPUT   : L  - #00 TRIGGER BUTTON NOT PRESSED
;	#1 TRIGGER BUTTON PRESSED
_getControllerButton:
	LD	A, L
	DEC	A
	LD	E, $03   ; ENABLE TRIG A+B OF STICK 1
	LD	B, A
	AND	$01
	JR	Z, controller1Buttons
	LD	E, $4C   ; ENABLE TRIG A+B OF STICK 2 AND SELECT STICK 2
 controller1Buttons:
	LD	A, $0F

	DI
	OUT	(PSG_REGS),A
	IN	A,(PSG_STAT)

	AND	$BF
	OR	E
	OUT	(PSG_DATA),A

	LD	A, B
	LD	B, $10
	AND	$02
	JR	Z, buttonA
	LD	B, $20
buttonA:
	LD	A, $0E

	OUT	(PSG_REGS),A
	IN	A,(PSG_STAT)

	EI
	LD	L,1
	AND	B
	RET	Z

	LD	L, 0
	RET
