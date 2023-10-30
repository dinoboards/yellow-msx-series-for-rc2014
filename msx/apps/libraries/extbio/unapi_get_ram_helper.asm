	include	"msx.inc"


	SECTION	CODE
	PUBLIC	_unapi_get_ram_helper
;
; extern uint16_t get_ram_helper(void);
;
_unapi_get_ram_helper:
	PUSH	IX
	LD	DE, EXTBIO_UNAPI
	LD	HL, 0
	LD 	A, EXTBIO_UNAPI_GET_RAM_HELPER_FN
	CALL	EXTBIO
	POP	IX
	RET
