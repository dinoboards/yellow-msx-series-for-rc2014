	SECTION	CODE
	include	"msx.inc"

;
; extern uint8_t serial_write(const uint8_t port_number, const uint8_t *const buf, const uint8_t size);
;
	PUBLIC	_serial_write:
_serial_write:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN
	LD	C, EXTBIO_RC2014_SERIAL_WRITE_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	LD	B, (HL)
	INC	HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
