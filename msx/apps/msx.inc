
; V9958 REGISTERS
VDP_DATA:       equ     $98             ; VDP data port (VRAM read/write)
VDP_ADDR:       equ     $99             ; VDP address (write only)
VDP_STAT:       equ     $99             ; VDP status (read only)
VDP_PALT:       equ     $9A             ; VDP palette latch (write only)
VDP_REGS:       equ     $9B             ; VDP register access (write only)


; MSX BIOS CALLS
FORCLR	EQU	$0F3E9
BAKCLR	EQU	$0F3EA
BDRCLR	EQU	$0F3EB
CHGCLR	EQU	$00062
INIT32	EQU	$00006
CALSLT	EQU	$0001C
EXPTBL	EQU	$0FCC1
INITXT	equ 	$6C
LINL40	equ	$0F3AE
CHGMOD	equ	$05f
INIPLT	equ	$00141

RAMAD0	equ	0F341h	; Slot of the Main-Ram on page 0~3FFFh
EXTBIO	equ	0ffcah	; Call address to an extended Bios
RDSLT	EQU	0000CH

FSTABL	EQU	$F400		; FOSSIL JUMP TABLE ADDRESS
FSMARK	EQU	$F3FE		; CONTAINS "RS" IF INSTALLED AND ACTIVE
