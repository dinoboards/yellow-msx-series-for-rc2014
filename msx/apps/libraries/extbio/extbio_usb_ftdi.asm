	include	"msx.inc"

;
; extern uint16_t extbio_usb_ftdi_fossil_install(const uint16_t hl) __z88dk_fastcall
;
	PUBLIC	_extbio_usb_ftdi_fossil_install
_extbio_usb_ftdi_fossil_install:
	PUSH	IX
	LD	D, 214				; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_USB_FTDI	; FUNCTION CODE
	CALL	EXTBIO				; RETURN HL
	POP	IX
	RET
