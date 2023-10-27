	include	"msx.inc"

;
; usb_error usb_data_in_transfer_n(uint8_t *buffer,
; 			         uint8_t *buffer_size,
; 			         const uint8_t device_address,
; 			         endpoint_param *const endpoint);

	PUBLIC	_usb_data_in_transfer_n
_usb_data_in_transfer_n:
	PUSH	IX
	LD	D, 214		; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_USB_IN_TRANSFER_N_FN		; FUNCTION CODE
	ld	hl,4
	add	hl,sp		; ARGS @ HL
	CALL	EXTBIO		; RETURN HL
	POP	IX
	RET

