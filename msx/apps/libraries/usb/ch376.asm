
	SECTION	CODE
	EXTERN	_ch_get_status
	EXTERN	_ch_command
	EXTERN	_JIFFY

CH376_DATA_PORT 	EQU	0x88
CH376_COMMAND_PORT	EQU 	0x89

DELAY_FACTOR		EQU	60

CMD01_RD_USB_DATA0	EQU	0x27	; Read data block from current USB interrupt endpoint buffer or host endpoint receive buffer
					; output: length, data stream

CMD10_WR_HOST_DATA	EQU	0x2C 	; Write a data block to the send buffer of the USB host endpoint
					; input: length, data stream

CH_CMD_RD_USB_DATA0	EQU	CMD01_RD_USB_DATA0
CH_CMD_WR_HOST_DATA	EQU	CMD10_WR_HOST_DATA

; HL -> timeout
; returns
; L -> error code

; ---------------------------------
; Function ch_wait_int_and_get_status
; ---------------------------------
	PUBLIC	_ch_wait_int_and_get_status
_ch_wait_int_and_get_status:
	ld	bc, DELAY_FACTOR

keep_waiting:
	in	a, (CH376_COMMAND_PORT)
	rlca
	jp	nc, _ch_get_status

	dec	bc
	ld	a, b
	or	c
	jr	nz, keep_waiting

	dec	hl
	ld	a, h
	or	l
	jr	nz, _ch_wait_int_and_get_status

	in	a, (CH376_COMMAND_PORT)
	bit	4, a		; CH376_COMMAND_PORT & PARA_STATE_BUSY

	ld	l, 0x0C 	; USB_ERR_CH376_BLOCKED;
	ret	nz

	ld	l, 0x0D 	; USB_ERR_CH376_TIMEOUT
	ret

; uint8_t ch_read_data(uint8_t *buffer) __sdcccall(1);
	PUBLIC	_ch_read_data
_ch_read_data:
	; ch_command(CH_CMD_RD_USB_DATA0);
	push	hl
	ld	l, CH_CMD_RD_USB_DATA0
	call	_ch_command
	pop	hl

	ld	c, CH376_DATA_PORT
	in	b, (c)

	ld	a, b
	ini
	inir

	ret

;const uint8_t *ch_write_data(const uint8_t *buffer, uint8_t length) __sdcccall(1)
	PUBLIC	_ch_write_data
_ch_write_data:
;libraries/usb/ch376.c:125: ch_command(CH_CMD_WR_HOST_DATA);
	ld	l, CH_CMD_WR_HOST_DATA
	call	_ch_command

	ld	iy, 2
	add	iy, sp
	ld	l, (iy+0)
	ld	h, (iy+1)
	ld	a, (iy+2)

	ld	c, CH376_DATA_PORT

; CH376_DATA_PORT = length;
	out	(c), a

	or	a
	ret	z

	ld	b, a
	otir
	ret
