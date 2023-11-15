	SECTION	CODE

	include	"msx.inc"

	PUBLIC  _enableVdpInterrupts
_enableVdpInterrupts:
	DI
	LD	A, (RG1SAV)
	SET	VDP_INTEN, A            ; RESET INTERRUPT ENABLE BIT
	LD	(RG1SAV), A
	OUT	(VDP_ADDR), A
	NOP
	NOP
	LD	A, $81                  ; SELECT REGISTER 1
	OUT	(VDP_ADDR), A
	EI
	RET

