	SECTION	CODE

	include	"msx.inc"

	PUBLIC	_debugBreak
_debugBreak:
	ld b,b
	jr $+2
	RET
