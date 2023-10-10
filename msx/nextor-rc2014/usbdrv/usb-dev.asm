
EXTBIO_RC2014_USB_GET_PRESENT_FN		EQU	0x81
EXTBIO_RC2014_USB_CONTROL_TRANSFER_FN		EQU	0x82
EXTBIO_RC2014_USB_OUT_TRANSFER_FN		EQU	0x83
EXTBIO_RC2014_USB_IN_TRANSFER_FN		EQU	0x84

;uint32_t extbio_rc2014(const uint16_t hl, const uint16_t de) __naked {
	PUBLIC	_extbio_rc2014
	EXTERN	_usb_control_transfer
	EXTERN	_usb_data_out_transfer
	EXTERN	_usb_data_in_transfer
	EXTERN	_extbio_rc2014_usb_get_present

_extbio_rc2014:
	PUSH	AF
	LD	A, E
	CP	EXTBIO_RC2014_USB_GET_PRESENT_FN
	JR	Z, __extbio_rc2014_usb_get_present
	CP	EXTBIO_RC2014_USB_CONTROL_TRANSFER_FN
	JR	Z, __extbio_rc2014_usb_control_transfer
	CP	EXTBIO_RC2014_USB_OUT_TRANSFER_FN
	JR	Z, __extbio_rc2014_usb_data_out_transfer
	CP	EXTBIO_RC2014_USB_IN_TRANSFER_FN
	JR	Z, __extbio_rc2014_usb_data_in_transfer

	POP	AF
	RET

__extbio_rc2014_usb_get_present:
	POP	AF
	CALL	_extbio_rc2014_usb_get_present
	; JR	extbio_handled

extbio_handled:
	EXX
	LD	D, 0
	EXX
	RET

__extbio_rc2014_usb_control_transfer:
	POP	AF

	; duplicate 6 stack bytes
	ld	bc, 6
	add	hl, bc
	ld	b, c
loop_ctrl:
	dec	hl
	ld	a, (hl)
	push	af
	inc	sp
	djnz	loop_ctrl

	call	_usb_control_transfer
	pop	af
	pop	af
	pop	af
	JR	extbio_handled

__extbio_rc2014_usb_data_out_transfer:
	POP	AF

	; duplicate 7 stack bytes
	ld	bc, 7
	add	hl, bc
	ld	b, c
loop_data_out:
	dec	hl
	ld	a, (hl)
	push	af
	inc	sp
	djnz	loop_data_out

	call	_usb_data_out_transfer
	pop	af
	pop	af
	pop	af
	inc	sp
	JR	extbio_handled

__extbio_rc2014_usb_data_in_transfer:
	POP	AF

	; duplicate 7 stack bytes
	ld	bc, 7
	add	hl, bc
	ld	b, c
loop_data_in:
	dec	hl
	ld	a, (hl)
	push	af
	inc	sp
	djnz	loop_data_in

	call	_usb_data_in_transfer
	pop	af
	pop	af
	pop	af
	inc	sp

	JR	extbio_handled
