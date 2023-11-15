	SECTION	CODE
	include	"msx.inc"

;
; extern uint8_t serial_demand_read(const uint8_t port_number, uint8_t *const buf, uint16_t *size, const uint16_t timeout_ms);
;
	PUBLIC	_serial_demand_read:
_serial_demand_read:
	PUSH	IX
	LD	D, 214						; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SERIAL_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_SERIAL_DEMAND_READ_SUB_FN
	LD	HL, 4
	ADD	HL, SP						; ARGS @ HL
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
