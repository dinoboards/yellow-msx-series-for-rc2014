
	SECTION	CODE

FOSSIL_JUMP_TABLE_REF	EQU	0F3FEh
FOSSIL_JUMP_TABLE_SLOT	EQU	0F400h
FOSSIL_MARK_1		EQU	0xf3fc
FOSSIL_MARK_2		EQU	0xf3fd

; //This routine check whether Fossil Driver is installed in RAM or not
; // Returns: 1 -> Driver found in memory
; //			0 -> Driver not installed
; extern void*       fossil_link();

	PUBLIC	_fossil_link
_fossil_link:
	LD	A, (FOSSIL_MARK_1)		; GET FIRST MARK OF FOSSIL
	LD	HL, 0				; HL=0
	CP	'R'				; IS IT THE RIGHT ONE?
	JR	NZ, NOT_FOUND
	LD	A, (FOSSIL_MARK_2)		; GET SECOND MARK OF FOSSIL
	CP	'S'				; IS IT THE RIGHT ONE?
	JR	NZ, NOT_FOUND
	LD	HL, 1

	LD	HL, (FOSSIL_JUMP_TABLE_REF)	; GET ADDRES OF JUMP TABLE
	LD	DE, FOSSIL_JUMP_TABLE		; POINT TO MY OWN TABLE
	LD	BC, 21*3			; NUMBER OF ENTRY'S AT THIS MOMENT
	LDIR					; MAKE A COPY OF THE TABLE

	LD	HL, (FOSSIL_JUMP_TABLE_REF)	; GET ADDRES OF JUMP TABLE

	RET

NOT_FOUND:
	LD	HL, 0
	RET
;
; extern uint8_t         fossil_ex_link(fossile_jump_table* jtable) __z88dk_fastcall;
;
	PUBLIC	_fossil_ex_link
_fossil_ex_link:
	LD	A, (FOSSIL_MARK_1)		; GET FIRST MARK OF FOSSIL
	CP	'R'				; IS IT THE RIGHT ONE?
	JR	NZ, NOT_FOUND
	LD	A, (FOSSIL_MARK_2)		; GET SECOND MARK OF FOSSIL
	CP	'X'				; IS IT THE RIGHT ONE?
	JR	NZ, NOT_FOUND

	LD	A, (FOSSIL_JUMP_TABLE_SLOT)
	LD	(HL), A
	INC	HL
	LD	DE, (FOSSIL_JUMP_TABLE_REF)
	LD	(HL), E
	INC	HL
	LD	(HL), D


	; LD	HL, (FOSSIL_JUMP_TABLE_REF)	; GET ADDRES OF JUMP TABLE
	; LD	DE, FOSSIL_JUMP_TABLE		; POINT TO MY OWN TABLE
	; LD	BC, 21*3			; NUMBER OF ENTRY'S AT THIS MOMENT
	; LDIR					; MAKE A COPY OF THE TABLE

	; LD	HL, (FOSSIL_JUMP_TABLE_REF)	; GET ADDRES OF JUMP TABLE

	RET

;
; Jump Table to use in ML
;
; This table will be filled in by the GetFossil routine, after that you can
; simply call these addresses in the table
;

	PUBLIC	_fossil_getversion, _fossil_init, _fossil_deinit, _fossil_get_info, __fossil_set_baud, _fossil_chars_in_buf
FOSSIL_JUMP_TABLE:

_fossil_getversion:	JP	0	; 0
_fossil_init:		JP	0	; 1
_fossil_deinit:		JP	0	; 2
__fossil_set_baud:	JP	0	; 3
fossil_set_protocol:	JP	0	; 4
fossil_channel:		JP	0	; 5
fossil_rs_in:		JP	0	; 6
fossil_rs_out:		JP	0	; 7
fossil_rs_in_stat:	JP	0	; 8
_fossil_rs_out_stat:	JP	0	; 9
fossil_dtr:		JP	0	; 10
fossil_rts:		JP	0	; 11
fossil_carrier:		JP	0	; 12
_fossil_chars_in_buf:	JP	0	; 13
_fossil_size_of_buf:	JP	0	; 14
_fossil_flush_buf:	JP	0	; 15
fossil_fastint:		JP	0	; 16
fossil_hookstat:	JP	0	; 17
_fossil_chput_hook:	JP	0	; 18
_fossil_keyb_hook:	JP	0	; 19
_fossil_get_info:	JP	0	; 20

;
; extern void fossil_rs_out(char char) __z88dk_fastcall;
;
	PUBLIC	_fossil_rs_out
_fossil_rs_out:
	LD	A, L
	JR	fossil_rs_out

;
; extern void fossil_protocol(uint8_t x) __z88dk_fastcall;
;
	PUBLIC	_fossil_set_protocol
_fossil_set_protocol:
	LD	H, L
	JR	fossil_set_protocol

;
; extern void fossil_protocol(uint8_t x) __z88dk_fastcall;
;
	PUBLIC	_fossil_channel
_fossil_channel:
	LD	H, L
	JR	fossil_channel

;
; extern char fossil_rs_in();
;
	PUBLIC	_fossil_rs_in
_fossil_rs_in:
	CALL	fossil_rs_in
	LD	L, A
	RET
;
;extern uint8_t fossil_rs_in_stat();
;
	PUBLIC	_fossil_rs_in_stat
_fossil_rs_in_stat:
	CALL	fossil_rs_in_stat
	LD	L, A
	RET

;
; extern void fossil_dtr(uint8_t x) __z88dk_fastcall;
;
	PUBLIC	_fossil_dtr
_fossil_dtr:
	LD	H, L
	JR	fossil_dtr

;
; extern void fossil_rts(uint8_t x) __z88dk_fastcall;
;
	PUBLIC	_fossil_rts
_fossil_rts:
	LD	H, L
	JR	fossil_rts

;
;extern uint8_t _fossil_carrier();
;
	PUBLIC	_fossil_carrier
_fossil_carrier:
	CALL	fossil_carrier
	LD	L, A
	RET
;
; extern void fossil_fastint(uint8_t x) __z88dk_fastcall;
;
	PUBLIC	_fossil_fastint
_fossil_fastint:
	LD	H, L
	JR	fossil_fastint
;
; extern void fossil_hookstat(uint8_t x) __z88dk_fastcall;
;
	PUBLIC	_fossil_hookstat
_fossil_hookstat:
	LD	H, L
	JR	fossil_hookstat
