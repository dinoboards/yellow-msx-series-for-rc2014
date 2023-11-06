	include	"msx.inc"

	PUBLIC	_extbio
	PUBLIC	_extbio_next

	SECTION	CODE

_extbio:
	PUSH	AF
	LD	A, D
	CP	E
	JR	Z, EXBIO_EXIT

	CP	EXTBIO_RC2014
	JR	Z, handle_extbio

EXBIO_EXIT:
	POP	AF

	DB	$C3	; JP opcode
_extbio_next:
	DW	0

handle_extbio:
	LD	A, E
	CP	EXTBIO_RC2014_INSTALL_FOSSIL_FN
	JR	NZ, EXBIO_EXIT

EXTBIO_RC_INSTALL_FOSSIL:
	LD	A, H
	OR	A
	JR	NZ, EXBIO_EXIT  		; NOT FOSSIL FOR FTDI

	LD	A, L
	CP	1
	JR	NZ, EXBIO_EXIT			; NOT FOSSIL FOR FTDI

	LD	HL, -1
	POP	AF
	RET

	PUBLIC	_timi
	PUBLIC	_timi_next

_timi:
	DB	$C3	; JP opcode
_timi_next:
	DW	0

	SECTION	DATA

	PUBLIC	_helloworld
_helloworld:
	DB	"ftdi\r\n", 0

