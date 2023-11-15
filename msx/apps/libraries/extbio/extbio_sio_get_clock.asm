	SECTION	CODE

	include	"msx.inc"
;
; extern uint8_t extbio_sio_get_clock();
;
	PUBLIC	_extbio_sio_get_clock
_extbio_sio_get_clock:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SIO_GET_CLOCK_FN
	CALL	EXTBIO					; RETURNS VALUE IN L
	EI
	POP	IX
	RET

