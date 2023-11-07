	SECTION	CODE
	include	"msx.inc"

;
; extern usb_error ftdi_set_line_property2(device_config_ftdi *const ftdi, const uint16_t protocol_bits);
;
	PUBLIC	_ftdi_set_line_property2
_ftdi_set_line_property2:
	PUSH	IX
	LD	D, EXTBIO_RC2014				; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_USB_FTDI_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_USB_FTDI_SET_LINE_PROPERTY2_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	POP	IX
	RET
