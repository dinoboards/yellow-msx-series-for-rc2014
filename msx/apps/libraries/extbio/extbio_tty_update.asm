	SECTION	CODE
	include	"msx.inc"

;
; extern uint8_t tty_update(const tty_settings_t * const settings) __sdcccall(1)
;
	PUBLIC	_tty_update:
_tty_update:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_TTY_FN
	LD	C, EXTBIO_RC2014_TTY_UPDATE_FN
	CALL	EXTBIO						; RETURN A
	EI
	POP	IX
	RET
