	SECTION	CODE

	include	"msx.inc"


	EXTERN	GETSLT
;
; extern uint8_t extbio_get_dev(uint8_t* table) __z88dk_fastcall;
;
	PUBLIC	_extbio_get_dev
_extbio_get_dev:
	PUSH	IX
	CALL	GETSLT		; B = NUMBER THE SLOT OF THE TABLE
	LD	DE, 0		; BROAD-CAST, FUNCTION: 'GET DEVICE NUMBER'
	CALL	EXTBIO
	POP	IX
	RET
