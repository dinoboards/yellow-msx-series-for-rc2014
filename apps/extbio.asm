
RAMAD0	equ	0F341h	; Slot of the Main-Ram on page 0~3FFFh
EXTBIO	equ	0ffcah	; Call address to an extended Bios
CALSLT:      EQU    0001Ch
EXPTBL:      EQU    0FCC1h

	SECTION	CODE
	PUBLIC	_getsystem

;
; extern void    extbio_get_dev(uint8_t* table) __z88dk_fastcall;
;
	PUBLIC	_extbio_get_dev
_extbio_get_dev:
	CALL	GETSLT		; B = NUMBER THE SLOT OF THE TABLE
	LD	DE, 0		; BROAD-CAST, FUNCTION: 'GET DEVICE NUMBER'
	JP	EXTBIO

;
; extern void get_dev_info_table(uint8_t device_id, extbio_info* info_table);
;
	PUBLIC	_get_dev_info_table
_get_dev_info_table:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	CALL	GETSLT	; B = number the slot of the table

	LD	D, (IX+4) 	; DEVICE_ID
	LD	L, (IX+5)  	; INFO_TABLE
	LD	H, (IX+6)

	LD	E, 0		; FUNCTION 'GET INFO'
	CALL	EXTBIO

	POP	IX
	RET

_getsystem:
	ld	hl, _table
	call	GETSLT	; B = number the slot of the table
	xor	a
	ld	d,255	; system????
	ld	e,0	; Function 'get'
	jp	EXTBIO




;----------------------------------------------------------------
; ROUTINE GETSLT
; ENTRY: HL = ADDRESS IN RAM
; OUTPUT: B = SLOT NUMBER CORRESPONDING TO THE HL ADDRESS
; MODIFIES: A AND B

GETSLT:
	PUSH	HL
	LD	A,H
	RLA
	RLA
	RLA		; BIT 6-7 TO BIT 1-0
	AND	3	; RESET UNUSED BITS
	LD	C,A
	LD	B,0
	LD	HL,RAMAD0
	ADD	HL,BC
	LD	B,(HL)	; B = SLOT NUMBER OF MAIN RAM
	POP	HL
	RET

; ;
; GETENT --Get entry address
; Entry: none
; Return: carry flag is set if no devices
; Modify: all
; ;
; device equ 8; device # 8 is RS-232C
; getent:
;         ld bc, table
;         call getsit; get slot address of TABLE
;         ld h, b
;         ld l, c
;         ld b, a
;         ld d, device; set deice number
;         ld e, 0;'get entry address' command
;         push hl; save TABLE address
;         call EXTBIO
;         pop de; restore TABLE address
;         or a
;         sbc hl, de; how many devices?
;         ret nz; there are some devices
;         scf; no devices
;         ret
; table:
;         ds 32 * 4; assume maximum 32 devices


_table:
	ds	32	; Reserve 32 bytes for the table
