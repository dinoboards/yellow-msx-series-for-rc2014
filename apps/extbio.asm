
RAMAD0	equ	0F341h	; Slot of the Main-Ram on page 0~3FFFh
EXTBIO	equ	0ffcah	; Call address to an extended Bios
CALSLT:      EQU    0001Ch
EXPTBL:      EQU    0FCC1h
RDSLT	EQU	0000CH

	SECTION	CODE

;
; extern void    extbio_get_dev(uint8_t* table) __z88dk_fastcall;
;
	PUBLIC	_extbio_get_dev
_extbio_get_dev:
	CALL	GETSLT		; B = NUMBER THE SLOT OF THE TABLE
	LD	DE, 0		; BROAD-CAST, FUNCTION: 'GET DEVICE NUMBER'
	JP	EXTBIO

;
; extern void extbio_get_dev_info_table(uint8_t device_id, extbio_info* info_table);
;
	PUBLIC	_extbio_get_dev_info_table
_extbio_get_dev_info_table:
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
;
; extern void rs232_link(extbio_info *p) __z88dk_fastcall;
;
	PUBLIC	_rs232_link
_rs232_link:
	LD	A, (HL)
	LD	(rs232_slot_init+1), A
	LD	(rs232_slot_open+1), A
	INC	HL
	LD	A, (HL)
	INC	HL
	LD	H, (HL)
	LD	L, A

	INC	HL
	INC	HL
	INC	HL
	LD	(rs232_slot_init+2), HL

	INC	HL
	INC	HL
	INC	HL
	LD	(rs232_slot_open+2), HL

	RET
;
; extern void rs232_init(rs232_init_params*) __z88dk_fastcall;
;
	PUBLIC	_rs232_init
_rs232_init:

	CALL	GETSLT			; B = number the slot of the table
	JP	rs232_slot_init

	PUBLIC	_rs232_open
_rs232_open:
	JP	rs232_slot_open

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


rs232_slot_jumps:

rs232_slot_init:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_open:
	RST	30H
	DB	0
	DW	0
	RET

