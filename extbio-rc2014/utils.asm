
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

