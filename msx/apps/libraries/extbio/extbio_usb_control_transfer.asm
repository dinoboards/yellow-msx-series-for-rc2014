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
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_USB_CONTROL_TRANSFER_FN
	LD	HL, 4
	ADD	HL, SP		; ARGS @ HL
	CALL	EXTBIO		; RETURN HL
	POP	IX
	RET

