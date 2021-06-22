
; allows for installation of expansion devices that contain extra OS subroutines

ROM_INIT:
	CALL	PROBE_HARDWARE

	LD	HL, 0			; MARK FOSSIL DRIVER NOT INSTALLED
	LD	(FSMARK), HL
	LD	(FSTABL), HL

	LD	A, (HOKVLD)
	BIT	0, A
	JR	NZ, INJECT_HOOK_HANDLERS

	; THERE IS NO EXTBIO INSTALL - SO WE HAVE TO INITIALISE
	LD	A, 1
	LD	(HOKVLD), A
	LD	A, $C9
	LD	(MEXBIH+0), A		; SET OPERATION CODE OF 'RET'
	JR	INST_HOOK

INJECT_HOOK_HANDLERS:
	LD      HL, FLAGS		; ENSURE RS232 IS MARKED AS CLOSED
	RES     3, (HL)			; FLAG PORT CLOSED

	LD	DE, MEXBIH		; COPY EXTBIO HOOK FUNCTION FROM H_BEXT TO MEXBIH
	LD	HL, H_BEXT
	LD	BC, 5
	LDIR

INST_HOOK:
	LD	DE, OLDINT		; COPY HOOK FUNCTION FROM H_KEYI TO OLDINT
	LD	HL, H_KEYI
	LD	BC, 5
	LDIR

	DI

	LD	A, $F7			; 'RST 30H' INTER-SLOT CALL OPERATION CODE
	LD	(H_BEXT), A		; SET NEW HOOK OP-CODE

	CALL	GETSL10			; GET OUR SLOT ID (EXPECT 3-3 $F7)
	LD	(H_BEXT+1), A		; SET SLOT ADDRESS

	LD	HL, EXTBIO		; GET OUR INTERRUPT ENTRY POINT
	LD	(H_BEXT+2), HL		; SET NEW INTERRUPT ENTRY POINT

	LD	A, $C9			; 'RET' OPERATION CODE
	LD	(H_BEXT+4), A		; SET OPERATION CODE OF 'RET'
	EI

	LD	HL, FOSSIL_DRV_LENGTH	; ALLOC MEMORY FOR FOSSIL JUMP TABLE
	CALL	ALLOC			; THIS MAY NOT WORK IF IT HAPENS BEFORE DISK ROM
	LD	(WORK), HL
	JR	C, ALLOC_FAILED

	; FOR PERFORMANCE REASONS, WE KNOW WE ARE SLOT 3-3 - SO JUST HARD CODE THE SLTWRK OFFSET
	; CALL	WSLW10

	LD	DE, FOSSIL_DRV_START
	EX	DE, HL
	LD	BC, FOSSIL_DRV_LENGTH
	LDIR

	; relocate the fossil driver
	LD	IX, (WORK)
	LD	HL, FOSSILE_DRV_MAP + 2
	LD	BC, (FOSSILE_DRV_MAP)

	exx
	ld	de, (WORK)	; DE' => amount required to be added to
	exx

LOOP:
	PUSH	BC

	LD	B, 8
	LD	A, (HL)
	INC	HL
NEXT:
	RLCA
	JR	NC, SKIP

	EXX
	LD	L, (IX)
	INC	IX
	LD	H, (IX)
	ADD	HL, DE
	LD	(IX), H
	DEC	IX
	LD	(IX), L
	EXX

SKIP:
	INC	IX
	DEC	B
	JR	NZ, NEXT

	POP	BC
	DEC	BC
	LD	A, C
	OR	B
	JR	NZ, LOOP

	LD	A, $C3                  ; JUMP
	DI
	LD	(H_KEYI), A             ; SET JMP INSTRUCTION
	EXX
	LD	HL, SIO_INT             ; SET TO JUMP TO SIO_INIT IN PAGE 3 RAM
	ADD	HL, DE
	LD	(H_KEYI+1), HL          ; SET NEW INTERRUPT ENTRY POINT
	EXX
	EI

	RET

ALLOC_FAILED:
	LD	DE, MSG.ALLOC_FAILED
	CALL	PRINT
	RET

MSG.ALLOC_FAILED:
	DB	"RC2014 Driver: Insufficient page 3 memory available", 13, 10, 0

FOSSIL_DRV_START:
if SYMBOL_ONLY=1
	INCBIN	"bin/fossil_xxx.bin"
else
	INCBIN	"bin/fossil_000.bin"
endif
FOSSIL_DRV_LENGTH	EQU	$-FOSSIL_DRV_START

FOSSILE_DRV_MAP:
if SYMBOL_ONLY=1
	INCBIN	"bin/fossil-map-xxx.bin"
else
	INCBIN	"bin/fossil-map.bin"
endif
