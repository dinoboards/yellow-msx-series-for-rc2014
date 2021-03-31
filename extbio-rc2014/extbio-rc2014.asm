
	ORG	$8000
CHPUT           EQU     00A2H

; +0	ID	Put these first two bytes at 041H and 042H ("AB") to indicate that it is an additional ROM.
; +2	INIT	Address of the routine to call to initialize a work area or I/O ports, or run a game, etc.
; +4	STATEMENT	Runtime address of a program whose purpose is to add instructions to the MSX-Basic using CALL.
; +6	DEVICE	Execution address of a program used to control a device built into the cartridge. For example, a disk interface.
; +8	TEXT	Pointer of the tokenizen Basic program contained in ROM.
; +10	Reserved	6 bytes reserved for future updates.

ROM_HEADER:
	DB	"  "
	DW	init
	DW	0
	DW	0
	DW	0
	DS	10

	ORG	$8100
init:
	LD	DE, MSG
	CALL	PRINT

	ld	hl, 50
	call	ALLOC





	ret

MSG:	DB	"This is a spike", 13, 10, 0

;-----------------------------------------------------------------------------
;
; Print a zero-terminated string on screen
; Input: DE = String address

PRINT:
	ld	a, (de)
	or	a
	ret	z
	call	CHPUT
	inc	de
	jr	PRINT

;-----------------------------------------------------------------------
;
;       ALLOC allocates specified amount of memory downward from current
;       HIMEM
;
; Inputs:
;       HL = memory size to allocate
; Outputs:
;       if successful, carry flag reset, HL points to the beginning
;                      of allocated area
;       otherwise, carry flag set, allocation not done.
;
BOOTAD	equ	0C000h		;Where boot sector is executed
;
BOTTOM	equ	0FC48h		;Pointer to bottom of RAM
HIMEM	equ	0FC4Ah		;Top address of RAM which can be used
MEMSIZ	equ	0F672h		;Pointer to end of string space
STKTOP	equ	0F674h		;Pointer to bottom of stack
SAVSTK	equ	0F6B1h		;Pointer to valid stack bottom
MAXFIL	equ	0F85Fh		;Maximum file number
FILTAB	equ	0F860h		;Pointer to file pointer table
NULBUF	equ	0F862h		;Pointer to buffer #0
;

ALLOC:
	ld	a, l		;is requested size 0?
	or	h
	ret	z		;yes, allocation always succeeds
	ex	de, hl		;calculate -size
	ld	hl, 0
	sbc	hl, de
	ld	c, l		;remember specified size
	ld	b, h
	add	hl, sp		;[HL] = [SP] - size
	ccf
	ret	c		;size too big

	ld	a, h
	cp	0C2h        	;high(BOOTAD)
	ret	c		;no room left

	ld	de, (BOTTOM)	;get current RAM bottom
	sbc	hl, de		;get memory space left after allocation
	ret	c		;no space left
	ld	a, h		;do we still have breathing room?
	cp	2              	;high(512)
	ret	c		;no,  not enough space left
;
;       Now,  requested size is legal,  begin allocation
;
	push	bc		;save -size
	ld	hl, 0
	add	hl, sp		;get current stack pointer to [HL]
	ld	e, l		;move source address to [DE]
	ld	d, h
	add	hl, bc
	push	hl		;save destination
	ld	hl, (STKTOP)
	or	a
	sbc	hl, de
	ld	c, l		;move byte count to move to [BC]
	ld	b, h
	inc	bc
	pop	hl		;restore destination
	ld	sp, hl		;destination becomes the new SP
	ex	de, hl
	ldir			;move stack contents
	pop	bc		;restore -size
	ld	hl, (HIMEM)
	add	hl, bc
	ld	(HIMEM), hl
	ld	de, -2*(2+9+256)
	add	hl, de
	ld	(FILTAB), hl	;pointer to first FCB
	ex	de, hl
	ld	hl, (MEMSIZ)	;update MEMSIZ
	add	hl, bc
	ld	(MEMSIZ), hl
	ld	hl, (NULBUF)	;update NULBUF
	add	hl, bc
	ld	(NULBUF), hl
	ld	hl, (STKTOP)	;update STKTOP
	add	hl, bc
;
;       Re-build BASIC's file structures
;
	ld	(STKTOP), hl
	dec	hl		;and SAVSTK
	dec	hl
	ld	(SAVSTK), hl
	ld	l, e		;get FILTAB in [HL]
	ld	h, d
	inc	hl		;point to first FCB
	inc	hl
	inc	hl
	inc	hl
	ld	a, 2
DSKFLL:
	ex	de, hl
	ld	(hl), e		;set address in FILTAB
	inc	hl
	ld	(hl), d
	inc	hl
	ex	de, hl
	ld	bc, 7
	ld	(hl), b		;make it look closed
	add	hl, bc
	ld	(hl), b		;clear flag byte
	ld	bc, 9+256-7
	add	hl, bc		;point to next FCB
	dec	a
	jr	nz, DSKFLL
	ret

	DS	$C000 - $

; FB03	RSTMP	50		Used by RS-232C or optical
; FB03	TOCNT	1		Used internally by RS-232C
; FB04	RSFBC	1		RS-232C LOW address
; 1		High address of RS-232C
; FB06	RSIQLN	1		Used internally by RS-232C
; FB07	MEXBIH	Five		FB07H + 0 RST 30H (0F7H)
; FB07H + 1 byte data
; FB07H + 2 (LOW)
; FB07H + 3 (HIGH)
; FB07H + 4 RET (0C9H)
; FB0C	OLDSTT	Five		FB0CH + 0 RST 30H (0F7H)
; FB0CH + 1 byte data
; FB0CH + 2 (LOW)
; FB0CH + 3 (HIGH)
; FB0CH + 4 RET (0C9H)
; FB11	OLDINT	Five		FB11H + 0 RST 30H (0F7H)
; FB11H + 1 byte data
; FB11H + 2 (LOW)
; FB11H + 3 (HIGH)
; FB11H + 4 RET (0C9H)
; FB16	DEVNUM	1		Used internally by RS-232C
; FB17	DATCNT	3		FB17H + 0 byte data
; FB17H + 1 byte pointer
; FB17H + 2 byte pointer
; FB1A	ERRORS	1		Used internally by RS-232C
; FB1B	FLAGS	1		Used internally by RS-232C
; FB1C	ESTBLS	1		Used internally by RS-232C
; FB1D	COMMSK	1		Used internally by RS-232C
; FB1E	LSTCOM	1		Used internally by RS-232C
; FB1F	LSTMOD	1		Used internally by RS-232C
; FB20	HOKVLD	1		With or without extended BIOS
; FB21	DRVTBL	8		DISK ROM slot address, etc.
