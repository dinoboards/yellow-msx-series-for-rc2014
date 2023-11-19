
	include	"msx.inc"
	include "../sio.inc"

	SECTION	CODE
	PUBLIC	_sio2_configure_port

	EXTERN	_sio_clock_divider

_sio2_configure_port:
	LD	C, RC_SIOA_CMD
	CALL	SIO_CONFIG_PORT
	LD	C, RC_SIOB_CMD
	CALL	SIO_CONFIG_PORT
	RET

SIO_CONFIG_PORT:
	XOR	A
	OUT	(C), A
	LD	A, 018H
	OUT	(C), A

	; WR4: CLK BAUD PARITY STOP BIT
	LD	A, 4
	OUT	(C), A
	LD	A, (_sio_clock_divider)		; CLK DIV SETTINGS IN BIT 6 AND 7
	OR	00000100B			; 1 STOP BIT
	OUT	(C), A

	; WR1: INTERRUPT ON
	LD	A, 1
	OUT	(C), A
	LD	A, SIO_WR1VAL
	OUT	(C), A

	; WR2: IM2 VEC OFFSET
	LD	A, 2
	OUT	(C), A
	LD	A, 00H
	OUT	(C),A

	; WR3: 8 BIT RCV, CTS/DCD AUTO, RX ENABLE
	LD	A, 3
	OUT	(C), A
	LD	A, 0E1H
	OUT	(C), A

	; WR5: DTR, 8
	LD	A, 5
	OUT	(C), A
	LD	A, SIO_RTSOFF
	OUT	(C), A

	RET
