	include	"msx.inc"

	SECTION	CODE
;
; extern uint16_t extbio_rc2014_hello(void);
;
	PUBLIC	_extbio_rc2014_hello

_extbio_rc2014_hello:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_HELLO_FN
	LD	HL, 0
	CALL	EXTBIO				; RETURN VERSION IN L
	EI
	POP	IX
	RET
