	SECTION	CODE
	include	"msx.inc"

;
; usb_error usb_control_transfer(const setup_packet *const cmd_packet,
;        			 void *const               buffer,
;        			 const uint8_t             device_address,
;        			 const uint8_t             max_packet_size);

	PUBLIC	_usb_control_transfer
_usb_control_transfer:
	PUSH	IX
	LD	D, 214		; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_USB_CONTROL_TRANSFER_FN		; FUNCTION CODE
	ld	hl,4
	add	hl,sp		; ARGS @ HL
	CALL	EXTBIO		; RETURN HL
	POP	IX
	RET

