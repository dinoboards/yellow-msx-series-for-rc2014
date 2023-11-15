	SECTION	CODE
	include	"msx.inc"

;
; usb_error usb_data_in_transfer(uint8_t *buffer,
; 			         uint16_t buffer_size,
; 			         const uint8_t device_address,
; 			         endpoint_param *const endpoint);

	PUBLIC	_usb_data_in_transfer
_usb_data_in_transfer:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_USB_IN_TRANSFER_FN
	LD	HL, 4
	ADD	HL, SP		; ARGS @ HL
	CALL	EXTBIO		; RETURN HL
	POP	IX
	RET

