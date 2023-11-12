
	include	"msx.inc"
	SECTION	CODE

	PUBLIC	getversion
;
; Version H.L (H,L packed BCD)
;
getversion:
	LD	HL, 0x0101
	RET

	PUBLIC	_timi
	PUBLIC	_original_timi_hook
	EXTERN	_drv_timi

_timi:
	CALL	_drv_timi

_original_timi_hook:
	DB	5
