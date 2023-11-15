	SECTION	CODE
	include	"msx.inc"

;
; extern usb_error ftdi_set_baudrate(device_config_ftdi *const ftdi, const int32_t baudrate);
;
	PUBLIC	_ftdi_set_baudrate
_ftdi_set_baudrate:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_USB_FTDI_FN
	LD	C, EXTBIO_RC2014_USB_FTDI_SET_BAUDRATE_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	POP	IX
	RET
