	PUBLIC _install_extbio_hook
	include "msx.inc"


	; HL -> address of where to copy H_BEXT to (_extbio_next)
	; DE -> address of new H_BEXT routine (_extbio)

_install_extbio_hook:
	LD	A, (RS_HOKVLD)
	BIT	0, A
	JR	NZ, INST_HOOK

	; THERE IS NO EXTBIO INSTALL - SO WE HAVE TO INITIALISE
	LD	A, 1
	LD	(RS_HOKVLD), A
	LD	A, $C9
	LD	(H_BEXT), A		; SET OPERATION CODE OF 'RET'

INST_HOOK:
	PUSH	HL
					; extbio_next -> DE
	LD	HL, H_BEXT
	LD	BC, 5
	LDIR

	POP	HL			; extbio

	DI

	LD	A, $C3			; JUMP OP CODE
	LD	(H_BEXT), A
	LD	(H_BEXT+1), HL		; extbio

	XOR	A
	LD	(H_BEXT+3), A
	LD	(H_BEXT+4), A

	EI
	RET

	PUBLIC	_install_keyi_hook

_install_keyi_hook:
	PUSH	HL
					; keyi_next -> DE
	LD	HL, H_KEYI
	LD	BC, 5
	LDIR

	POP	HL			; keyi

	DI

	LD	A, $C3			; JUMP OP CODE
	LD	(H_KEYI), A
	LD	(H_KEYI+1), HL		; keyi

	XOR	A
	LD	(H_KEYI+3), A
	LD	(H_KEYI+4), A

	EI
	RET
