	include	"msx.inc"
	INCLUDE	"../sio.inc"

	SECTION	CODE
	EXTERN	_sio_buf_tail
	EXTERN	_sio_buf
	EXTERN 	_sio_data_count
	EXTERN	RS_SIO_B_CMD, _sio_flags
	PUBLIC	_sio_in

_sio_in:
	LD	HL, _sio_data_count
	DI
	LD	A, (HL)
	OR	A
	JP	Z, NO_DATA_ERR				; IF (_sio_data_count == 0) RETURN
	DEC	A
	LD	(HL), A
	CP	16					; BUFFER LOW THRESHOLD
	JR	Z, segment1_rs_in_set_rts		; YES, SET RTS

SIO_IN1:
	LD	HL, (_sio_buf_tail)
	LD	C, (HL)					; C := CHAR TO BE RETURNED
	INC	L					; BUMP TAIL PTR
	LD	A, SIO_BUFSZ-1				; MASK TO BUF SIZE -1
	AND	L
	OR	_sio_buf & (256-SIO_BUFSZ)
	LD	L, A
	LD	(_sio_buf_tail), A			; SAVE UPDATED TAIL PTR
	EI
	LD	L, C
	LD	H, 0
	RET						; CHAR RETURNED IN L

NO_DATA_ERR:
	LD	HL, 0xFF00
	EI
	RET

segment1_rs_in_set_rts:
	LD	C, RC_SIOB_CMD
	SIO_CHIP_RTS_C	SIO_RTSON
	LD	HL, _sio_flags
	SET 	1, (HL)
	JP	SIO_IN1

