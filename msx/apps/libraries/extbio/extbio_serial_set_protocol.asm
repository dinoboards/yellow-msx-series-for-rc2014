	SECTION	CODE
	include	"msx.inc"

;
; uint8_t serial_set_protocol(const uint8_t port_number, const uint16_t protocol);
;
	PUBLIC	_serial_set_protocol:
_serial_set_protocol::
	PUSH	IX
	LD	D, 214						; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SERIAL_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_SERIAL_SET_PROTOCOL_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	POP	IX
	RET
