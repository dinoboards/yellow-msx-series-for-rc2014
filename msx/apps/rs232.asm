
	include	"msx.inc"

RS232_FN_COUNT:	EQU	13	; EXTENDED BIOS RS232 HAS 13 JUMP INSTRUCTIONS

	SECTION	CODE
	EXTERN	GETSLT
;
; extern void rs232_link(extbio_info *p) __z88dk_fastcall;
;
	PUBLIC	_rs232_link
_rs232_link:
	LD	A, (HL)				; RETRIEVE SLOT ID
	ld	de, rs232_slot_init+3		; WRITE IT TO THE LINK FUNCTIONS
	ld	b,  RS232_FN_COUNT
loop1:
	LD	(DE), A
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	INC	DE
	djnz	loop1

	INC	HL				; RETRIEVE THE JUMP TABLE ADDRESS
	LD	E, (HL)
	INC	HL
	LD	D, (HL)				; DE IS JUMP TABLE ADDR - FIRST 3 ARE IGNORED, FOLLOWED BY JUMPS
	INC	DE
	INC	DE
	INC	DE


	LD	HL, rs232_slot_init+4		; WRITE THE JUMP ADDRESSES
	LD	B, RS232_FN_COUNT
LOOP:
	ld	(hl), e				; WRITE JUMP TABLE REF TO LINK FUNCTION
	inc	hl
	ld	(hl), d
	inc	hl

	inc	hl
	inc	hl
	inc	hl
	inc	hl
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

rs232_slot_jumps:
rs232_slot_init:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_open:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_stat:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_getchr:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_sndchr:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_close:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_eof:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

;
; extern uint16_t rs232_loc();
;
	PUBLIC	_rs232_loc
_rs232_loc:
rs232_slot_loc:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_lof:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_backup:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_sndbrk:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_dtr:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

rs232_slot_setchn:
	PUSH	IX
	RST	30H
	DB	0
	DW	0
	EI
	POP	IX
	RET

