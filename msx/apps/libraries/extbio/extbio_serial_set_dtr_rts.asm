	SECTION	CODE
	include	"msx.inc"

;
; extern uint8_t serial_set_dtr_rts(const uint8_t port_number, const int16_t dtr_rts_flags);
;
	PUBLIC	_serial_set_dtr_rts:
_serial_set_dtr_rts:
	PUSH	IX
	LD	D, 214						; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SERIAL_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_SERIAL_SET_DTR_RTS_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
