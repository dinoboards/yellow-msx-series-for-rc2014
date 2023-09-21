

	EXTERN	_USBPRT
	PUBLIC	_drv_direct4

; Function : Sends one character to printer
; Input    : A  - ASCII code of character to send
; Output   : Carry flag set if failed
; Registers: F

; hooked to H_LPTO
_drv_direct4:
	PUSH	HL
	PUSH	AF
	PUSH	DE
	PUSH	BC
	LD	L, A
	CALL	_USBPRT
	POP	BC
	POP	DE
	POP	AF
	RL	L
	POP	HL
	RET
