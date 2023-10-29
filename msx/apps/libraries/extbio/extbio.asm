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

	POP	IX
	RET
;
; extern uint8_t extbio_memmap_get_variable_table(memmap_extbio_variable_table** );
;
; 	PUBLIC	_extbio_memmap_get_variable_table
; _extbio_memmap_get_variable_table:
; 	PUSH	IX

; 	LD	A, 0
; 	LD	E, 1		; FUNCTION 'GET VARIABLE TABLE'
; 	CALL	EXTBIO

; 	LD	IX, 0
; 	ADD	IX, SP

; 	LD	E, (IX+4)  	; INFO_TABLE
; 	LD	D, (IX+5)

; 	EX	DE, HL
; 	LD	(HL), E
; 	INC	HL
; 	LD	(HL), D

; 	LD	L, A

; 	POP	IX
; 	RET
;
; extern uint8_t extbio_memmap_get_jump_table(uint8_t* number_of_segments, uint8_t* number_of_free_segments, void* jump_table);
;
; 	PUBLIC	_extbio_memmap_get_jump_table
; _extbio_memmap_get_jump_table:
; 	PUSH	IX

; 	LD	B, 0
; 	LD	C, 0
; 	LD	A, 0
; 	LD	E, 2		; FUNCTION 'GET JUMP TABLE'
; 	CALL	EXTBIO

; 	LD	IX, 0
; 	ADD	IX, SP

; 	LD	E, (IX+8)  	; jump table
; 	LD	D, (IX+9)
; 	EX	DE, HL
; 	LD	(HL), E
; 	INC	HL
; 	LD	(HL), D

; 	LD	L, (IX+6)  	; free segments
; 	LD	H, (IX+7)
; 	LD	(HL), A

; 	LD	L, (IX+4)  	; number of segments
; 	LD	H, (IX+5)
; 	LD	(HL), C

; 	LD	L, B

; 	POP	IX
; 	RET


;
; extern void* extbio_fossil_install(const uint16_t device_type)
;
	PUBLIC	_extbio_fossil_install
_extbio_fossil_install:
	PUSH	IX
	LD	D, 214					; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_INSTALL_FOSSIL_FN	; FUNCTION INSTALL
	CALL	EXTBIO
	POP	IX
	RET

;
; extern uint8_t extbio_sio_get_clock();
;
	PUBLIC	_extbio_sio_get_clock
_extbio_sio_get_clock:
	PUSH	IX
	LD	D, 214					; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SIO_GET_CLOCK_FN	; FUNCTION GET SIO CLOCK
	CALL	EXTBIO					; RETURNS VALUE IN L
	POP	IX
	RET

;
; extern uint8_t extbio_sio_set_clock(const uint8_t new_code) __z88dk_fastcall;
;
	PUBLIC	_extbio_sio_set_clock
_extbio_sio_set_clock:
	PUSH	IX
	LD	D, 214					; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SIO_SET_CLOCK_FN	; FUNCTION SET SIO CLOCK
	CALL	EXTBIO					; FORWARD AND RETURN VALUE IN L
	POP	IX
	RET

;
; extern uint8_t extbio_sio_get_present();
;
	PUBLIC	_extbio_sio_get_present
_extbio_sio_get_present:
	PUSH	IX
	LD	D, 214					; RC2014 EXTENDED DRIVER
	LD	E, EXTBIO_RC2014_SIO_GET_PRESENT_FN	; FUNCTION SET SIO CLOCK
	CALL	EXTBIO					; RETURN STATE IN L
	POP	IX
	RET

RG1SAV:         equ     $F3E0
VDP_INTEN:     EQU     5

	;TODO Make these extbio calls
       PUBLIC  _disableVdpInterrupts
_disableVdpInterrupts:
       DI
       LD      A, (RG1SAV)
       RES     VDP_INTEN, A            ; RESET INTERRUPT ENABLE BIT
       LD      (RG1SAV), A
       OUT     (VDP_ADDR), A
       NOP
       NOP
       LD      A, $81                  ; SELECT REGISTER 1
       OUT     (VDP_ADDR), A
       EI
       RET

       PUBLIC  _enableVdpInterrupts
_enableVdpInterrupts:
       DI
       LD      A, (RG1SAV)
       SET     VDP_INTEN, A            ; RESET INTERRUPT ENABLE BIT
       LD      (RG1SAV), A
       OUT     (VDP_ADDR), A
       NOP
       NOP
       LD      A, $81                  ; SELECT REGISTER 1
       OUT     (VDP_ADDR), A
       EI
       RET


	PUBLIC	_debugBreak
_debugBreak:
	ld b,b
	jr $+2
	RET
