	SECTION	CODE
	include	"msx.inc"

;
; extern uint16_t extbio_usb_get_present();
;
	PUBLIC	_extbio_usb_get_present
_extbio_usb_get_present:
	PUSH	IX
	LD	D, 214		; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_USB_GET_PRESENT_FN		; FUNCTION CODE
	CALL	EXTBIO		; RETURN HL
	POP	IX
	RET
