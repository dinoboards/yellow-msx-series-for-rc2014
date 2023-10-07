
EXTBIO_RC2014_USB_GET_PRESENT_FN		EQU	0x81
EXTBIO_RC2014_USB_GET_DESCRIPTOR_FN		EQU	0x82

;uint32_t extbio_rc2014(const uint16_t hl, const uint16_t de) __naked {
	PUBLIC	_extbio_rc2014
	EXTERN	_extbio_rc2014_usb_get_descriptor
	EXTERN	_extbio_rc2014_usb_get_present

_extbio_rc2014:
	PUSH	AF
	LD	A, E
	CP	EXTBIO_RC2014_USB_GET_PRESENT_FN
	JR	Z, __extbio_rc2014_usb_get_present
	CP	EXTBIO_RC2014_USB_GET_DESCRIPTOR_FN
	JR	Z, __extbio_rc2014_usb_get_descriptor

	POP	AF
	RET

__extbio_rc2014_usb_get_present:
	POP	AF
	CALL	_extbio_rc2014_usb_get_present
	; JR	_extbio_handled

_extbio_handled:
	EXX
	LD	D, 0
	EXX
	RET

__extbio_rc2014_usb_get_descriptor:
	POP	AF
	; HL => ADDRESS OF DESCRIPTOR BUFFER
	; A => DEVICE NUMBER
	PUSH	HL
	PUSH	AF
	inc	sp
	CALL	_extbio_rc2014_usb_get_descriptor
	pop	af
	inc	sp
	JR	_extbio_handled

