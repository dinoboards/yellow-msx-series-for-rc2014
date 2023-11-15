	SECTION code_clib
	PUBLIC  fputc_cons_native
	PUBLIC  _fputc_cons_native

	INCLUDE	"msxdos.inc"

_fputc_cons_native:
.fputc_cons_native:
	LD      HL, 2
	ADD     HL, SP
	LD	E, (HL)
	PUSH	IX
	LD	C, $02
	CALL	BDOS
	POP	IX
	RET
