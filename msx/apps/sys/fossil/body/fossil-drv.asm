
	include	"msx.inc"
	SECTION	CODE

	PUBLIC	getversion
;
; Version H.L (H,L packed BCD)
;
getversion:
	LD	HL, 0x0101
	RET

