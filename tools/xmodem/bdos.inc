;
; MSX-DOS Functions - http://msx2.org/MCCM%20Millennium%20CD1/MENU32/TECHDOC/msxdos2/topic_0.htm

TERM0:		EQU	0
CONIN:		EQU	1	; L=A = Character from keyboard
CONOUT:		EQU	2	; E = Character to be output
AUXIN:		EQU	3	; L=A = Input character
AUXOUT:		EQU	4	; E = Character to be output.
STROUT:		EQU	9	; DE = Address of $ terminated string
CONST:		EQU	11	; L=A = 00H if no key was pressed, FFH if a key was pressed.
FCLOSE:		EQU	16	; DE = Pointer to opened FCB. L=A = 0FFH if not successful, 0 if successful
FDEL: 		EQU	19	; DE = Pointer to unopened FCB. L=A = 0FFH if no files deleted, 0 if files deleted OK
WRSEQ:  	EQU	21	; DE = Pointer to opened FCB. L=A = 01H if error (disk full), 0 if write was successful
FMAKE:		EQU	22	; M DE = Pointer to unopened FCB. L=A = 0FFH if unsuccessful, 0 if successful
SETDTA:	 	EQU	26	; DE = Required Disk Transfer Address

DFCB:		EQU	5CH	; Default File Control Block
DFCB.FNAME:	EQU	DFCB+1	; File name
DFCB.EXT:	EQU	DFCB+9	; File extension
DFCB.CR:	EQU 	DFCB+32 ; Current record

BDOS:		EQU	5

BDOSFN	MACRO	fn
	LD	C, fn
	CALL	BDOS
	ENDM

