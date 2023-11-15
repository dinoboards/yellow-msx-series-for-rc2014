	SECTION	CODE
	include	"msx.inc"

;
; uint8_t serial_get_driver_name(const uint8_t port_number, const char *driver_name);
;
	PUBLIC	_serial_get_driver_name
_serial_get_driver_name:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN
	LD	C, EXTBIO_RC2014_SERIAL_GET_DRIVER_NAME_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	LD	B, (HL)
	INC	HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
