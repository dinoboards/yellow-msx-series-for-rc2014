	SECTION	CODE
	include	"msx.inc"

;
; uint8_t spike_set_clks(const uint8_t port_number, const int32_t baudrate);
;
	PUBLIC	_spike_set_clks
_spike_set_clks:
	PUSH	IX
	LD	D, 214						; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SERIAL_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_SPIKE_SET_CLKS_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
