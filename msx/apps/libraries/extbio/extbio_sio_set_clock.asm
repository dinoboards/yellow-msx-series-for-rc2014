	SECTION	CODE

	include	"msx.inc"
;
; extern uint8_t extbio_sio_set_clock(const uint8_t new_code) __z88dk_fastcall;
;
	PUBLIC	_extbio_sio_set_clock
_extbio_sio_set_clock:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SIO_SET_CLOCK_FN
	CALL	EXTBIO					; FORWARD AND RETURN VALUE IN L
	EI
	POP	IX
	RET
