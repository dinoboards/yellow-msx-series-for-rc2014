	SECTION	CODE
	include	"msx.inc"


	PUBLIC	_unapi_get_ram_helper
;
; extern uint16_t unapi_get_ram_helper(void* reduced_mapper_table, uint8_t* number_of_entries) __sdcccall(1);
;
_unapi_get_ram_helper:
	PUSH	IX
	PUSH	DE
	PUSH	HL
	LD	DE, EXTBIO_UNAPI
	LD	HL, 0
	LD 	A, EXTBIO_UNAPI_GET_RAM_HELPER_FN
	CALL	EXTBIO

	POP	DE
	LD	(DE), C
	INC	DE
	LD	(DE), B

	POP	DE
	LD	(DE), A

	EX	DE, HL

	POP	IX
	RET
