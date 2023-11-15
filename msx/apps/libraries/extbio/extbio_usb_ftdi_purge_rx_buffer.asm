	SECTION	CODE
	include	"msx.inc"

;
; extern usb_error extern usb_error ftdi_purge_rx_buffer(device_config_ftdi *const ftdi);
;
	PUBLIC	_ftdi_purge_rx_buffer;
_ftdi_purge_rx_buffer:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_USB_FTDI_FN
	LD	C, EXTBIO_RC2014_USB_FTDI_PURGE_RX_BUFFER_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	POP	IX
	RET
