	SECTION	CODE

	include	"msx.inc"

	;TODO Make these extbio calls
       PUBLIC  _disableVdpInterrupts
_disableVdpInterrupts:
	DI
	LD	A, (RG1SAV)
	RES	VDP_INTEN, A		; RESET INTERRUPT ENABLE BIT
	LD	(RG1SAV), A
	OUT	(VDP_ADDR), A
	NOP
	NOP
	LD	A, $81			; SELECT REGISTER 1
	OUT	(VDP_ADDR), A
	EI
	RET
