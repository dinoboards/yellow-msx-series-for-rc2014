	include "msx.inc"
	include "../sio.inc"

	PUBLIC	_timi
	PUBLIC	_timi_next

	EXTERN	_sio_data_count
	EXTERN	_sio_buf_head
	EXTERN	RS_FLAGS

_timi:
sio_interrupt:
	EXX
	LD	HL, _sio_data_count	; buffer size ptr
	LD	D, SIO_BUFSZ-10		; buffer high mark
	EXX

	LD	E, RC_SIOB_CMD		; E => CMD, D => DAT
	LD	D, RC_SIOB_DAT

	LD	HL, RS_FLAGS		; IS OPENED?
	BIT	3, (HL)			; FLAG PORT OPEN?
	JR	Z, SIO_INT_ABORT

	LD	HL, (_sio_buf_head)	; HL IS HEAD - ADDR OF NEXT BYTE TO BE WRITTEN IN BUFFER

	; RECEIVE CHARACTER INTO BUFFER
SIO_INTRCV1:
	LD	C, D			; DAT PORT
	IN	B, (C)			; READ PORT

	EXX
	LD	A, (HL)			; GET COUNT
	INC	A			; INCREMENT THE COUNT
	JR	Z, SIO_INTRCV4		; BAIL OUT IF BUFFER FULL, RCV BYTE DISCARDED

	LD	(HL), A			; AND SAVE IT
	CP	D			; HIT HIGH WATER MARK?
	EXX
	JR	NZ, SIO_INTRCV2		; IF NOT, BYPASS CLEARING RTS

	LD	C, E			; CMD PORT
	SIO_CHIP_RTS_C	SIO_RTSOFF
	LD	A, (RS_FLAGS)		; SET BIT FLAG FOR RTS OFF
	AND	11111101B		; RES	1, A
	LD	(RS_FLAGS), A

SIO_INTRCV2:
	LD	(HL), B			; SAVE CHARACTER RECEIVED IN BUFFER AT HEAD
	INC	L			; BUMP HEAD POINTER

SIO_INTRCV3:
	; CHECK FOR MORE PENDING...
	XOR	A
	LD	C, E			; CMD PORT
	OUT	(C), A			; READ REGISTER 0
	IN	A, (C)
	RRA				; READY BIT TO CF
	JP	C, SIO_INTRCV1		; IF SET, DO SOME MORE

	LD	A, (RS_FLAGS)
	AND	00000010B		; BIT 1, A
	JR	Z, SIO_UPDATE_HEAD_PTR	; ABORT NOW IF RTS IS OFF

	; TEST FOR NEW BYTES FOR A SHORT PERIOD OF TIME
	LD	B, 95
SIO_MORE:
	IN	A, (C)			; C IS CMD PORT
	RRA				; READY BIT TO CF
	JR	C, SIO_INTRCV1		; IF SET, DO SOME MORE
	DJNZ	SIO_MORE

COMMAND_RETURN_FROM_INT	EQU	0x38

SIO_UPDATE_HEAD_PTR:
	LD	(_sio_buf_head), HL

SIO_INTRCV4:
	; LATCH OFF SIO/2 INTERRUPT STATE
	LD	C, RC_SIOA_CMD
	XOR	A
	OUT	(C), A
	LD	A, COMMAND_RETURN_FROM_INT
	OUT	(C), A

	DB	$C3	; JP opcode
_timi_next:
	DW	0

SIO_INT_ABORT:
	LD	C, D			; DAT PORT
	IN	A, (C)
	LD	C, E			; CMD PORT
	; PORT NOT OPENED, SO IGNORE BYTE, RESET RTS AND EXIT
	SIO_CHIP_RTS_C	SIO_RTSOFF
	RES	1, (HL)			; SET BIT FLAG FOR RTS OFF
	JR	SIO_INTRCV4



