
	SECTION	CODE
	PUBLIC	_ch_wait_int_and_get_status
	EXTERN	_ch_get_status
	EXTERN	_ch_command
	EXTERN	_JIFFY

CH376_DATA_PORT 	EQU	0x88
CH376_COMMAND_PORT	EQU 	0x89

; HL -> timeout
; returns
; L -> error code

; ---------------------------------
; Function ch_wait_int_and_get_status
; ---------------------------------
_ch_wait_int_and_get_status:
	ld	de, (_JIFFY)
	add	hl, de
	ex	de, hl

l_ch_wait_int_and_get_status_00102:
	in	a, (CH376_COMMAND_PORT)
	rlca
	jp	NC, _ch_get_status

	ld	hl, (_JIFFY)
	ld	a, e
	sub	a, l
	ld	a, d
	sbc	a, h
	jp	P, l_ch_wait_int_and_get_status_00102

	in	a, (CH376_COMMAND_PORT)
	bit	4, a		 ; CH376_COMMAND_PORT & PARA_STATE_BUSY
	jr	Z, l_ch_wait_int_and_get_status_00106

	ld	l, 0x0c 	; USB_ERR_CH376_BLOCKED;
	ret

l_ch_wait_int_and_get_status_00106:
	ld	l, 0x0d 	; USB_ERR_CH376_TIMEOUT
	ret
