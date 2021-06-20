
RAMAD0	equ	0F341h	; Slot of the Main-Ram on page 0~3FFFh
EXTBIO	equ	0ffcah	; Call address to an extended Bios
CALSLT:      EQU    0001Ch
EXPTBL:      EQU    0FCC1h
RDSLT	EQU	0000CH

RS232_FN_COUNT:	EQU	13	; EXTENDED BIOS RS232 HAS 13 JUMP INSTRUCTIONS

	SECTION	CODE

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
; extern void* extbio_fossil_install()
;
	PUBLIC	_extbio_fossil_install
_extbio_fossil_install:
	PUSH	IX
	LD	D, 214		; RC2014 EXTENDED DRIVER
	LD	E, 1		; FUNCTION INSTALL
	CALL	EXTBIO
	POP	IX
	RET
;
; extern void rs232_link(extbio_info *p) __z88dk_fastcall;
;
	PUBLIC	_rs232_link
_rs232_link:
	LD	A, (HL)				; RETRIEVE SLOT ID
	ld	de, rs232_slot_init+1		; WRITE IT TO THE LINK FUNCTIONS
	ld	b,  RS232_FN_COUNT
loop1:
	ld	(de), a
	inc	de
	inc	de
	inc	de
	inc	de
	inc	de
	djnz	loop1

	INC	HL				; RETRIEVE THE JUMP TABLE ADDRESS
	LD	E, (HL)
	INC	HL
	LD	D, (HL)				; DE IS JUMP TABLE ADDR - FIRST 3 ARE IGNORED, FOLLOWED BY JUMPS
	INC	DE
	INC	DE
	INC	DE


	LD	HL, rs232_slot_init+2		; WRITE THE JUMP ADDRESSES
	LD	B, RS232_FN_COUNT
LOOP:
	ld	(hl), e				; WRITE JUMP TABLE REF TO LINK FUNCTION
	inc	hl
	ld	(hl), d
	inc	hl

	inc	hl
	inc	hl
	inc	hl				; HL REFERENCES NEXT LOCAL LINK FUNCTION

	INC	DE
	INC	DE
	INC	DE				; DE NOW PTS TO NEXT JUMP INSTRUCTION

	DJNZ	LOOP

	RET
;
; extern void rs232_init(rs232_init_params*) __z88dk_fastcall;
;
	PUBLIC	_rs232_init
_rs232_init:
	CALL	GETSLT			; B = number the slot of the table
	JP	rs232_slot_init

; extern void rs232_open(uint8_t mode, uint8_t buffer_length, uint8_t* buffer);
	PUBLIC	_rs232_open
_rs232_open:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	E, (IX+4)
	LD	C, (IX+5)
	LD	L, (IX+6)
	LD	H, (IX+7)
	CALL	rs232_slot_open

	POP	IX
	RET

	PUBLIC	_rs232_close
_rs232_close:
	JP	rs232_slot_close

;extern uint8_t rs232_sndchr(const char ch) __z88dk_fastcall;
	PUBLIC	_rs232_sndchr
_rs232_sndchr:
	LD	A, L
	CALL	rs232_slot_sndchr
	LD	L, 0
	RET	Z
	LD	L, 1
	RET

;
; extern uint16_t rs232_getchr();
;
	PUBLIC	_rs232_getchr
_rs232_getchr:
	CALL	rs232_slot_getchr
	LD	H, 0
	LD	L, A
	RET	Z
	LD	H, 1
	RET

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

; extern uint8_t getSlotPage0(void* p) __z88dk_fastcall;
	PUBLIC	_getSlotPage0
_getSlotPage0:
	CALL	GETSLT
	LD	L, B
	LD	H, 0
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

rs232_slot_stat:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_getchr:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_sndchr:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_close:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_eof:
	RST	30H
	DB	0
	DW	0
	RET

;
; extern uint16_t rs232_loc();
;
	PUBLIC	_rs232_loc
_rs232_loc:
rs232_slot_loc:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_lof:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_backup:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_sndbrk:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_dtr:
	RST	30H
	DB	0
	DW	0
	RET

rs232_slot_setchn:
	RST	30H
	DB	0
	DW	0
	RET



; FOSSILE STUFF

; extern void fossil_link(void* jumpTable) __z88dk_fastcall
;
	PUBLIC	_fossil_link

_fossil_link:
	LD	(FOSSIL_JUMP_TABLE), HL
	LD	(marker+1), HL
	INC	HL
	INC	HL
	INC	HL
	ld	(marker2+1), HL
	RET

FOSSIL_JUMP_TABLE:	DW	0
; extern uint16_t fossil_get_version()
;
	PUBLIC	_fossil_get_version
_fossil_get_version:
	PUSH	IX
marker:
	CALL	0
	POP	IX
	RET

;
; extern void fossil_init();
;
	PUBLIC	_fossil_init
_fossil_init:
marker2:
	JP	0
	RET
