	SECTION	CODE
	include	"msx.inc"

;
; extern uint8_t serial_purge_buffers(const uint8_t port_number) __z88dk_fastcall;
;
	PUBLIC	_serial_purge_buffers:
_serial_purge_buffers:
	PUSH	IX
	LD	D, 214						; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SERIAL_FN			; FUNCTION CODE
	LD	C, EXTBIO_RC2014_SERIAL_PURGE_BUFFERS_SUB_FN
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
