
;-----------------------------------------------------------------------------
;
; PRINT A ZERO-TERMINATED STRING ON SCREEN
; INPUT: DE = STRING ADDRESS

PRINT:
	LD	A, (DE)
	OR	A
	RET	Z
	BIOS_FN	CHPUT
	INC	DE
	JR	PRINT


;--------------------------------------------------------
;       GTSL1   Get current slot id for page 2 (0x8000-0xBFFF)
;       Entry   None
;       Return  A       Slot address as follows
;       Modify  Flags
;
;           FxxxSSPP
;           |   ||||
;           |   ||++-- primary slot # (0-3)
;           |   ++---- secondary slot # (0-3)
;           |          00 if not expanded
;           +--------- 1 if secondary slot # specified
;
;       This value can later be used as an input parameter
;       for the RDSLT, WRSLT, CALSLT, ENASLT and 'RST 10H'
;
GETSL10:
        PUSH    HL              ;Save registers
        PUSH    DE

	BIOS_FN	RSLREG
        RRCA
        RRCA			; page 1
        RRCA
        RRCA			; page 2
        AND     11B             ;[A]=000000PP
        LD      E,A
        LD      D,0             ;[DE]=000000PP

        LD      HL,EXPTBL
        ADD     HL,DE           ;[HL]=EXPTBL+000000PP
        LD      E,A             ;[E]=000000PP
        LD      A,(HL)          ;A=(EXPTBL+000000PP)
        AND     80H             ;Use only MSB
        JR      Z,GTSL1NOEXP

        OR      E               ;[A]=F00000PP
        LD      E,A             ;save primary slot number
        INC     HL              ;point to SLTTBL entry
        INC     HL
        INC     HL
        INC     HL
        LD      A,(HL)          ;get current expansion slot register
        RRCA
        RRCA			; page 1
        RRCA
        RRCA			; page 2
        AND     11B             ;[A] = 000000SS
        RLCA
        RLCA                    ;[A] = 0000SS00
        OR      E               ;[A] = F000SSPP
;
GTSL1END:
        POP     DE
        POP     HL
        RET
GTSL1NOEXP:
        LD      A,E             ;[A] = 000000PP
        JR      GTSL1END

;--------------------------------------------------------
;
;	ASLW1	Get address of slot work
;	Entry	None
;	Return	HL	address of slot work
;	Modify	DE
;
ASLW10:
	CALL	GETSL10		;[A] = F000SSPP, SS = 00 if not expanded
	AND	00001111B	;[A] = 0000SSPP
	LD	L,A		;[A] = 0000SSPP
	RLCA
	RLCA
	RLCA
	RLCA			;[A] = SSPP0000
	AND	00110000B	;[A] = 00PP0000
	OR	L		;[A] = 00PPSSPP
	AND	00111100B	;[A] = 00PPSS00
	OR	10B		;[A] = 00PPSSBB
;
;	Now, we have the sequence number for this cartridge
;	as follows.
;
;	00PPSSBB
;	  ||||||
;	  ||||++-- higher 2 bits of memory address (1)
;	  ||++---- seconday slot # (0..3)
;	  ++------ primary slot # (0..3)
;
	RLCA			;*=2
	LD	E,A
	LD	D,0		;[DE] = 0PPSSBB0
	LD	HL,SLTWRK
	ADD	HL,DE
	RET


;--------------------------------------------------------
;
;	RSLW1	Read slot work
;	Entry	None
;	Return	HL	Content of slot work
;	Modify	DE
;
RSLW10:
	CALL	ASLW10		;[HL] = address of slot work
	LD	A,(HL)
	INC	HL
	LD	H,(HL)		;[HL] = (slot work)
	LD	L, A
	RET


;--------------------------------------------------------
;
;	WSLW1	Write slot work
;	Entry	HL	Data to write
;	Return	None
;	Modify	DE
;
WSLW10:
	PUSH	HL
	CALL	ASLW10		;[HL] = address of slot work
	POP	DE
	LD	(HL),E
	INC	HL
	LD	(HL),D
	RET
;
; CALSUB
;
; In: IX = address of routine in MSX2 SUBROM
;     AF, HL, DE, BC = parameters for the routine
;
; Out: AF, HL, DE, BC = depending on the routine
;
; Changes: IX, IY, AF', BC', DE', HL'
;
; Call MSX2 subrom from MSXDOS. Should work with all versions of MSXDOS.
;
; Notice: NMI hook will be changed. This should pose no problem as NMI is
; not supported on the MSX at all.
;
CALSUB:	EXX
	EX	AF, AF'			; STORE ALL REGISTERS
	LD	HL, EXTROM
	PUSH	HL
	LD	HL, 0C300h
	PUSH	HL			; PUSH NOP ; JP EXTROM
	PUSH	IX
	LD	HL, 021DDh
	PUSH	HL			; PUSH LD IX, <ENTRY>
	LD	HL, 03333h
	PUSH	HL			; PUSH INC SP; INC SP
	LD	HL, 0
	ADD	HL, SP			; HL = OFFSET OF ROUTINE
	LD	A, 0C3h
	LD	(H_NMI), A
	LD	(H_NMI+1), HL		; JP <ROUTINE> IN NMI HOOK
	EX	AF, AF'
	EXX				; RESTORE ALL REGISTERS
	LD	IX, NMI
	LD	IY, (EXPTBL-1)
	CALL	CALSLT			; CALL NMI-HOOK VIA NMI ENTRY IN ROMBIOS
					; NMI-HOOK WILL CALL SUBROM
	EXX
	EX	AF, AF'	  		; STORE ALL RETURNED REGISTERS
	LD	HL, 10
	ADD	HL, SP
	LD	SP, HL	  		; REMOVE ROUTINE FROM STACK
	EX	AF, AF'
	EXX				; RESTORE ALL RETURNED REGISTERS
	RET

; DE_MULT_C
; IN:
;   C AND DE
 ;OUT:
;   AHL IS THE PRODUCT, WITH A BEING THE UPPER 8 BITS
;   BC = 0
;   DE IS PRESERVED
DE_MULT_C:
	LD	A, C
	LD	BC, $0800
	LD	H, C
	LD	L, C

DE_MULT_C_1:
	ADD	HL, HL
	RLA         		 ; CHECK MOST-SIGNIFICANT BIT OF ACCUMULATOR
	JR	NC, DE_MULT_C_2  ; IF ZERO, SKIP ADDITION
	ADD	HL, DE
	ADC	A, C

DE_MULT_C_2:
	DJNZ	DE_MULT_C_1
	RET
