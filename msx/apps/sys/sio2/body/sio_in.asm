	include	"msx.inc"
	INCLUDE	"../sio.inc"

	SECTION	CODE
	EXTERN	_sio_buf_tail
	EXTERN 	_sio_data_count
	EXTERN	RS_SIO_B_CMD, RS_FLAGS
	PUBLIC	_sio_in

_sio_in:
	LD	HL, _sio_data_count
	DI						; ATOMIC DEC (_sio_data_count)
	DEC	(HL)
	LD	A, (HL)
	EI
	CP	SIO_BUFSZ/4				; BUFFER LOW THRESHOLD
	JR	Z, segment1_rs_in_set_rts		; YES, SET RTS

SIO_IN1:
	LD	HL, (_sio_buf_tail)
	LD	C, (HL)					; C := CHAR TO BE RETURNED
	INC	L					; BUMP TAIL PTR
	LD	(_sio_buf_tail), HL				; SAVE UPDATED TAIL PTR
	LD	L, C
	RET						; CHAR RETURNED IN L

segment1_rs_in_set_rts:
	DI
	LD	C, RC_SIOB_CMD
	SIO_CHIP_RTS_C	SIO_RTSON
	LD	HL, RS_FLAGS
	SET 	1, (HL)
	EI
	JP	SIO_IN1

