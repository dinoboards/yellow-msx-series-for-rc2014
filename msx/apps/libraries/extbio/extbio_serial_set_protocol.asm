	SECTION	CODE
	include	"msx.inc"

;
; uint8_t serial_set_protocol(const uint8_t port_number, const uint16_t protocol);
;
	PUBLIC	_serial_set_protocol:
_serial_set_protocol::
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN
	LD	C, EXTBIO_RC2014_SERIAL_SET_PROTOCOL_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	LD	B, (HL)
	INC	HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
