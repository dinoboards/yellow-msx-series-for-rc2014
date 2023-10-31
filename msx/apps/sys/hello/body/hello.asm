	include	"msx.inc"

	PUBLIC	_extbio
	PUBLIC	_extbio_next

	SECTION	CODE

_extbio:
	PUSH	AF
	LD	A, D
	CP	E
	JR	Z, _extbio_skip

	CP	EXTBIO_RC2014
	JR	Z, handle_extbio

_extbio_skip:
	POP	AF

	DB	$C3	; JP opcode
_extbio_next:
	DW	0

handle_extbio:
	LD	A, E
	CP	EXTBIO_RC2014_HELLO_FN
	JR	NZ, _extbio_skip

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
	DB	"hello-world\r\n", 0

