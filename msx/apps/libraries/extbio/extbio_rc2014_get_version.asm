	SECTION	CODE
	include	"msx.inc"

;
; extern uint16_t extbio_rc2014_get_version(void);
;
	PUBLIC	_extbio_rc2014_get_version

_extbio_rc2014_get_version:
	PUSH	IX
	LD	D, EXTBIO_RC2014		; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_GET_VERSION_FN	; FUNCTION CODE
	CALL	EXTBIO				; RETURN VERSION IN L
	EI
	POP	IX
	RET
