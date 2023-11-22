	SECTION	CODE
	include	"msx.inc"

;
; extern uint16_t serial_get_rx_buffer_size(const uint8_t port_number) __z88dk_fastcall
;
	PUBLIC	_serial_get_rx_buffer_size:
_serial_get_rx_buffer_size:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SERIAL_FN
	LD	C, EXTBIO_RC2014_SERIAL_GET_RX_BUFFER_SIZE_SUB_FN
	LD	B, L
	CALL	EXTBIO						; RETURN HL
	EI
	POP	IX
	RET
