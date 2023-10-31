
REQUIRE_EXTBIO	EQU	1
REQUIRE_TIMI	EQU	2

	SECTION	TSR_HEADER

	PUBLIC	_extbio_next
	PUBLIC	_extbio

defc	_extbio_next = 0
defc	_extbio = 0

	DB	"usb-prnt"
	DB 	REQUIRE_TIMI
	DB	0

	SECTION	CODE
	SECTION	code_crt_init
	SECTION code_compiler
	SECTION	bss_compiler
	SECTION IGNORE
