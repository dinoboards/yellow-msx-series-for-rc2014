	SECTION	CODE
	include	"msx.inc"

;
; uint8_t serial_get_available_ports(uint8_t *available ports) __z88dk_fastcall;
;
	PUBLIC	_serial_get_available_ports
_serial_get_available_ports:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN
	LD	C, EXTBIO_RC2014_SERIAL_GET_AVAILABLE_PORTS_FN
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
