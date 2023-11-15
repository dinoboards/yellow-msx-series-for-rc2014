	SECTION	CODE
	include	"msx.inc"

;
; extern uint16_t extbio_rc2014_get_version(void);
;
	PUBLIC	_extbio_rc2014_get_version

_extbio_rc2014_get_version:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_GET_VERSION_FN
	CALL	EXTBIO				; RETURN VERSION IN L
	EI
	POP	IX
	RET
