	SECTION	CODE
	include	"msx.inc"

;
; extern uint16_t extbio_usb_get_present();
;
	PUBLIC	_extbio_usb_get_present
_extbio_usb_get_present:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_USB_GET_PRESENT_FN
	CALL	EXTBIO		; RETURN HL
	POP	IX
	RET
