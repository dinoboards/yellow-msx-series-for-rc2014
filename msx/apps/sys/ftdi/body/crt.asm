
REQUIRE_EXTBIO	EQU	1
REQUIRE_TIMI	EQU	2
REQUIRE_KEYI	EQU	4

	SECTION	TSR_HEADER

	PUBLIC	_keyi_next
	PUBLIC	_keyi

defc	_keyi_next = 0
defc	_keyi = 0

	DB	"ftdi    "
	DB 	REQUIRE_EXTBIO
	DB	0

	SECTION	CODE
	SECTION	code_crt_init
	SECTION code_compiler
	SECTION	bss_compiler
	SECTION IGNORE

