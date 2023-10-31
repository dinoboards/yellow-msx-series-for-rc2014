
	SECTION	CODE

;
; extern void memmap_link(void *p) __z88dk_fastcall;
;
	PUBLIC	_memmap_link
_memmap_link:
	LD	DE, jump_table
	LD	BC, 030H
	LDIR
	RET


; HL+0h	ALL_SEG
; 	Use: Allocate a segment
; 	Entry: A=0 for the user, A=1 for the system
; 	       B = Mapper slot in the form FxxxSSPP
; 	           If xxx=000, allocate a segment to the specified slot
; 		   If xxx=001, allocate a segment in another slot than the one specified
; 		   If xxx=010, allocate a segment to the specified slot if free,
; 		    otherwise in another slot
; 		   If xxx=011, allocate a segment in another slot than the one
; 		    specified if free otherwise try in specified slot
; 	       B = 0 for main Memory Mapper
; 	Output:  F = Carry is set if nothing has been allocated
; 		 A  = Segment number
; 		 B  = Slot number

; extern uint8_t memmap_allocate_segment(uint8_t for_user, uint8_t mapper_slot, uint8_t *allocated_segment, uint8_t *allocated_slot)
	PUBLIC	_memmap_allocate_segment
_memmap_allocate_segment:
	PUSH	IX
	LD	IX, 0
	ADD	IX, SP

	LD	A, (IX+4)
	LD	B, (IX+5)
	PUSH	IX
	CALL	allocate_segment
	POP	IX

	LD	L, (IX+6)		; allocated_segment
	LD	H, (IX+7)
	LD	(HL), A

	LD	L, (IX+8)		; allocated_slot
	LD	H, (IX+9)
	LD	(HL), B

	LD	L, 255
	JR	C, not_allocated
	LD	L, 0
not_allocated:

	POP	IX
	RET


; HL+3h	FRE_SEG
; 	Use: Free a segment
; 	Entry: A = Segment number
; 	       B = Mapper slot in the form FxxxSSPP (0 for the main Memory Mapper)
; 	Output: F = Carry is set if nothing has been freed

; HL+6h	RD_SEG
; 	 Use: Read a byte in a segment
; 	 Entry: A = Segment number
; 		HL = Address to read from the segment
; 	 Output: A = Byte read
; 	 Modifies: AF
; 	 Note: This routine disables interrupts

; HL+9h	WR_SEG
; 	 Use: Write a byte in a segment
; 	 Entry: A = Segment number
; 		HL = Address to write on the segment
; 	 Modifies: AF
; 	 Note: This routine disables interrupts

; HL+Ch	CAL_SEG
; 	 Use: Inter segment call
; 	 Entry: IYh = Segment number
; 		IX = Address
; 	 Modifies: All except AF, BC, DE and HL
; 	 Note: AF, BC, DE and HL can be used as parameters for the routine to call.

; HL+Fh	CALLS
; 	 Use: Inter segment call
; 	 Entry: The three bytes following the instruction call CALLS
; 	 	eg:	call CALLS
; 	 		db Slot ID
; 	 		dw ADDRESS
; 	 Modifies: All except AF, BC, DE and HL
; 	 Note: AF, BC, DE and HL can be used as parameters for the routine to call.

; HL+12h	PUT_PH
; 	 Use: Select a segment on the corresponding memory page at the specified address.
; 	 Entry: HL = Address (in fact the bits 6~7 of H specify the page number)
; 		A = Segment number
; 	 Modifies: Nothing

; HL+18h	PUT_P0
; 	 Use: Select a segment on page 0 (0000h~3FFFh)
; 	 Entry: A = Segment number
; 	 Modifies: Nothing

; HL+1Bh	GET_P0
; 	 Use: Get the segment number on page 0 (0000h~3FFFh)
; 	 Output: A = Segment number
; 	 Modifies: A

; HL+1Eh	PUT_P1
; 	 Use: Select a segment on page 1 (4000h~7FFFh)
; 	 Entry: A = Segment number
; 	 Modifies: Nothing

; HL+21h	GET_P1
; 	 Use: Get the segment number on page 1 (4000h~7FFFh)
; 	 Output: A = Segment number
; 	 Modifies: A

; HL+24h	PUT_P2
; 	 Use: Select a segment on page 2 (8000h~BFFFh)
; 	 Entry: A = Segment number
; 	 Modifies: Nothing

; HL+27h	GET_P2
; 	 Use: Get the segment number on page 2 (8000h~BFFFh)
; 	 Output: A = Segment number
; 	 Modifies: A

; HL+2Ah	PUT_P3
; 	 Use: Select a segment on page 3 (C000h~FFFFh)
; 	 Entry: A = Segment number
; 	 Modifies: Nothing

; HL+2Dh	GET_P3
; 	 Use: Get the segment number on page 3 (C000h~FFFFh)
; 	 Output: A = Segment number
; 	 Modifies: A


; HL+12h  PUT_PH
;          Use: Select a segment on the corresponding memory page at the specified address.
;          Entry: HL = Address (in fact the bits 6~7 of H specify the page number)
;                 A = Segment number
;          Modifies: Nothing

	PUBLIC	_memmap_put_page
_memmap_put_page:
	CALL	put_page
	LD	L, A
	RET


; HL+15h  GET_PH
;          Use: Get the selected segment number on the corresponding memory page at the
;                  specified address.
;          Entry: HL = Address (in fact the bits 6~7 of H specify the page number)
;          Output: A = Segment number
;          Modifies: Nothing

	PUBLIC	_memmap_get_page
_memmap_get_page:
	CALL	get_page
	LD	L, A
	RET

	PUBLIC	_memmap_get_page_0
_memmap_get_page_0:
	CALL	get_page_0
	LD	L, A
	RET

	PUBLIC	_memmap_put_page_0
_memmap_put_page_0:
	CALL	put_page_0
	LD	L, A
	RET

	PUBLIC	_memmap_get_page_1
_memmap_get_page_1:
	CALL	get_page_1
	LD	L, A
	RET

	PUBLIC	_memmap_put_page_1
_memmap_put_page_1:
	LD	A, L
	CALL	put_page_1
	RET

	PUBLIC	_memmap_get_page_2
_memmap_get_page_2:
	CALL	get_page_2
	LD	L, A
	RET

	PUBLIC	_memmap_put_page_2
_memmap_put_page_2:
	LD	A, L
	JP	put_page_2

	PUBLIC	_memmap_get_page_3
_memmap_get_page_3:
	CALL	get_page_3
	LD	L, A
	RET

	PUBLIC	_memmap_put_page_3
_memmap_put_page_3:
	LD	A, L
	CALL	put_page_3
	RET

jump_table:
allocate_segment:
	JP	0

free_segment:
	JP	0

read_segment:
	JP	0

write_segment:
	JP	0

call_segment:
	JP	0

calls_segment:
	JP	0

put_page:
	JP	0

get_page:
	JP	0

put_page_0:
	JP	0

get_page_0:
	JP	0

put_page_1:
	JP	0

get_page_1:
	JP	0

put_page_2:
	JP	0

get_page_2:
	JP	0

put_page_3:
	JP	0

get_page_3:
	JP	0

