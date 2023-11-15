	SECTION	CODE

	include	"msx.inc"
;
; extern void* extbio_fossil_install()
;
	PUBLIC	_extbio_fossil_install
_extbio_fossil_install:
	PUSH	IX
	LD	DE, EXTBIO_RC2014 << 8 | EXTBIO_RC2014_INSTALL_FOSSIL_FN
	CALL	EXTBIO
	EI
	POP	IX
	RET
