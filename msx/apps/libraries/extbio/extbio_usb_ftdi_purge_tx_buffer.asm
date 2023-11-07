	SECTION	CODE
	include	"msx.inc"

;
; extern usb_error extern usb_error ftdi_purge_tx_buffer(device_config_ftdi *const ftdi);
;
	PUBLIC	_ftdi_purge_tx_buffer;
_ftdi_purge_tx_buffer:
	PUSH	IX
	LD	D, EXTBIO_RC2014				; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_USB_FTDI_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_USB_FTDI_PURGE_TX_BUFFER_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	POP	IX
	RET
