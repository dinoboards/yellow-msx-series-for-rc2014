	SECTION	CODE
	include	"msx.inc"

;
; extern uint8_t serial_set_flowctrl(const uint8_t port_number, const int8_t flowctrl);
;
	PUBLIC	_serial_set_flowctrl:
_serial_set_flowctrl:
	PUSH	IX
	LD	D, 214						; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SERIAL_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_SERIAL_SET_FLOWCTRL_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
