	SECTION	CODE
	include	"msx.inc"

;
; extern uint8_t serial_set_dtr_rts(const uint8_t port_number, const int16_t dtr_rts_flags);
;
	PUBLIC	_serial_set_dtr_rts:
_serial_set_dtr_rts:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN
	LD	C, EXTBIO_RC2014_SERIAL_SET_DTR_RTS_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	LD	B, (HL)
	INC	HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
