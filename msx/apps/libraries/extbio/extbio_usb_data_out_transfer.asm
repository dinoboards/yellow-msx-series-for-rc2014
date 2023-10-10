	include	"msx.inc"

;
; usb_error usb_data_out_transfer(const uint8_t *buffer,
; 			          uint16_t buffer_size,
; 			          const uint8_t device_address,
; 			          endpoint_param *const endpoint);


	PUBLIC	_usb_data_out_transfer
_usb_data_out_transfer:
	PUSH	IX
	LD	D, 214		; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_USB_OUT_TRANSFER_FN		; FUNCTION CODE
	ld	hl,4
	add	hl,sp		; ARGS @ HL
	CALL	EXTBIO		; RETURN HL
	POP	IX
	RET

