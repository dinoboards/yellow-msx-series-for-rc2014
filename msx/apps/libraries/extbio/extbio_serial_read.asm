	SECTION	CODE
	include	"msx.inc"

;
; extern uint8_t serial_read(const uint8_t port_number, uint8_t *const buf, uint16_t *size);
;
	PUBLIC	_serial_read:
_serial_read:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN ; RC2014 EXTENDED DRIVER/FUNCTION CODE
	LD	C, EXTBIO_RC2014_SERIAL_READ_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	LD	B, (HL)
	INC	HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
