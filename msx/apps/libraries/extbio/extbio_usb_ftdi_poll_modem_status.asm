	SECTION	CODE
	include	"msx.inc"

;
; extern usb_error ftdi_poll_modem_status(device_config_ftdi *const ftdi, uint16_t * const status);
;
	PUBLIC	_ftdi_poll_modem_status
_ftdi_poll_modem_status:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_USB_FTDI_FN
	LD	C, EXTBIO_RC2014_USB_FTDI_POLL_MODEM_STATUS_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	POP	IX
	RET
