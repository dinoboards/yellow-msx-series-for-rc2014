	SECTION	CODE
	include	"msx.inc"

;
; uint8_t serial_set_baudrate(const uint8_t port_number, const int32_t baudrate);
;
	PUBLIC	_serial_set_baudrate
_serial_set_baudrate:
	PUSH	IX
	LD	D, 214						; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SERIAL_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_SERIAL_SET_BAUDRATE_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	POP	IX
	RET
