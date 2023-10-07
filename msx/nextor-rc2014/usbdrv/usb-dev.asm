
EXTBIO_RC2014_USB_GET_PRESENT_FN		EQU	0x81
EXTBIO_RC2014_USB_CONTROL_TRANSFER_FN		EQU	0x82

;uint32_t extbio_rc2014(const uint16_t hl, const uint16_t de) __naked {
	PUBLIC	_extbio_rc2014
	EXTERN	_usb_control_transfer
	EXTERN	_extbio_rc2014_usb_get_present

_extbio_rc2014:
	PUSH	AF
	LD	A, E
	CP	EXTBIO_RC2014_USB_GET_PRESENT_FN
	JR	Z, __extbio_rc2014_usb_get_present
	CP	EXTBIO_RC2014_USB_CONTROL_TRANSFER_FN
	JR	Z, __extbio_rc2014_usb_control_transfer

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
	ld	b, 6
loop:
	dec	hl
	ld	a, (hl)
	push	af
	inc	sp
	djnz	loop

	call	_usb_control_transfer
	pop	af
	pop	af
	pop	af
	JR	extbio_handled

