	SECTION	CODE

	include	"msx.inc"

	EXTERN	GETSLT
;
; extern void extbio_get_dev_info_table(uint8_t device_id, extbio_info* info_table);
;
	PUBLIC	_extbio_get_dev_info_table
_extbio_get_dev_info_table:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	CALL	GETSLT		; B = number the slot of the table

	LD	D, (IX+4) 	; DEVICE_ID
	LD	L, (IX+5)  	; INFO_TABLE
	LD	H, (IX+6)

	LD	E, 0		; FUNCTION 'GET INFO'
	CALL	EXTBIO
	EI

	POP	IX
	RET
