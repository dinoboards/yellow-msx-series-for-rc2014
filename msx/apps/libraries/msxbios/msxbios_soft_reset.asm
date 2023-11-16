

	include "msx.inc"

	SECTION	CODE

BIOS_RESET:	EQU	0

	PUBLIC	_softReset
_softReset:
	LD	IY, (EXPTBL-1)
	LD	IX, BIOS_RESET
	JP	CALSLT

