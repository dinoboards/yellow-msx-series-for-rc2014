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

	PUBLIC	_sio_flags, _sio_clock_divider

_sio_data_count:	DB	0		; RECEIVE DATA COUNT
_sio_flags:		DB	0
_sio_clock_divider:	DB	SIO_CLK_DIV_16

; _sio_flags
; | BIT |   SET   |   RES   |
; |-----|---------|---------|
; |  0  | NOT USED          |
; |  1  | RTS ON | RTS OFF  |

