	SECTION	CODE

	include	"msx.inc"
;
; extern uint8_t extbio_sio_get_present();
;
	PUBLIC	_extbio_sio_get_present
_extbio_sio_get_present:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_SIO_GET_PRESENT_FN
	CALL	EXTBIO					; RETURN STATE IN L
	EI
	POP	IX
	RET
