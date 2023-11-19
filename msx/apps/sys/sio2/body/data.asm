	include	"msx.inc"
	include "../sio.inc"

	SECTION	SIO_BUFFER
	ALIGN   256
	PUBLIC	_sio_buf, _sio_buf_head, _sio_buf_tail, _sio_data_count

_sio_buf:		DS	SIO_BUFSZ, $00	; RECEIVE RING BUFFER
_sio_buf_end:
_sio_buf_head:		DW	_sio_buf	; BUFFER HEAD POINTER
_sio_buf_tail:		DW	_sio_buf	; BUFFER TAIL POINTER

	SECTION	DATA

	PUBLIC	RS_FLAGS, _sio_clock_divider

_sio_data_count:	DB	0		; RECEIVE DATA COUNT
; RS_SIO_A_CMD:	DB	0
; RS_SIO_B_CMD:	DB	0
; RS_SIO_CLK_DIV:	DB 	0
RS_FLAGS:	DB	0

_sio_clock_divider:	DB	SIO_CLK_DIV_64


