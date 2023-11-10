
	SECTION	CODE
	PUBLIC	_ch_wait_int_and_get_status
	EXTERN	_ch_get_status
	EXTERN	_ch_command
	EXTERN	_JIFFY

CH376_DATA_PORT 	EQU	0x88
CH376_COMMAND_PORT	EQU 	0x89

DELAY_FACTOR		EQU	60

; HL -> timeout
; returns
; L -> error code

; ---------------------------------
; Function ch_wait_int_and_get_status
; ---------------------------------
_ch_wait_int_and_get_status:
	ld	bc, DELAY_FACTOR

keep_waiting:
	in	a, (CH376_COMMAND_PORT)
	rlca
	jp	NC, _ch_get_status

	dec	bc
	ld	a, b
	or	c
	jr	NZ, keep_waiting

	dec	hl
	ld	a, h
	or	l
	jr	NZ, _ch_wait_int_and_get_status

	in	a, (CH376_COMMAND_PORT)
	bit	4, a		 ; CH376_COMMAND_PORT & PARA_STATE_BUSY

	ld	l, 0x0c 	; USB_ERR_CH376_BLOCKED;
	ret	nz

	ld	l, 0x0d 	; USB_ERR_CH376_TIMEOUT
	ret
