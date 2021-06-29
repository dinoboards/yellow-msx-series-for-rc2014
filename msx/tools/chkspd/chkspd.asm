;****************************************************************************************
;*											*
;*	CHKSPD: CPU speed estimation and RTC calibration program			*
;*								http://www.gr8bit.ru	*
;*	Copyleft (cl) 2012 by Eugeny Brychkov						*
;*											*
;*				License and T&C						*
;*											*
;* You can use and modify this software's source and executable, given two		*
;* conditions are met: (a) you keep this Copyleft statement in the source code,		*
;* and you do not distribute software and resulting executables on the commercial	*
;* basis and for the purposes, not related to GR8BIT platform and/or MSX standard,	*
;* without written permission of Eugeny Brychkov (eugeny_brychkov@hotmail.com)		*
;*											*
;****************************************************************************************
;*			File name: CHKSPD.MAC						*
;****************************************************************************************

; Special thanks to Daniel Vik for information about VDP timing

; Version 1.0	October 02, 2012
;		- Initial release

	.z80
	aseg
	org	0100h

; (definitions)
RG0SAV	equ	0f3dfh
RG1SAV	equ	0f3e0h
RG9SAV	equ	0ffe8h

; (PROGRAM START)
	; (copy VDP R#9 value to temp storage)
	ld	a,(RG9SAV)
	ld	(CurRg9),a

	; (display title string of the program)
	ld	de,IniStr
	call	_PRSTR

	; (look up for command line modifiers)
	ld	hl,080h

NextChar:
	inc	hl
	ld	a,(hl)
	or	a
	jr	z,EndCmd
	cp	0dh
	jr	z,EndCmd
	cp	20h
	jr	z,NextChar

	inc	hl
	and	0dfh			; uppercase
	cp	'M'
	jr	z,M1State
	cp	'V'
	jr	z,VDPState
	cp	'R'
	jr	z,RTCState
	cp	'F'
	jr	z,VRFreq
	cp	'X'
	jr	z,XTALFr
	cp	'H'
	jr	z,PrnHelp

CmdEr:
	ld	de,CmdErr
	call	_PRSTR
	ret

M1State:
	call	Validate
	jr	c,CmdEr
	ld	(DefltM),a
	jr	NextChar
VDPState:
	call	Validate
	jr	c,CmdEr
	ld	(DefltV),a
	jr	NextChar
RTCState:
	call	Validate
	jr	c,CmdEr
	ld	(DefltR),a
	jr	NextChar
VRFreq:
	ld	a,(hl)
	ex	de,hl
	ld	hl,CurRg9
	cp	'5'
	jr	nz,cp60Hz
	set	1,(hl)			; set to 50Hz
	ex	de,hl
	jr	NextChar
cp60Hz:
	cp	'6'
	jr	nz,CmdEr
	res	1,(hl)			; set to 60Hz
	ex	de,hl
	jr	NextChar
XTALFr:
	call	GetXTAL
	jr	c,CmdEr			; error in XTAL value
	jr	NextChar
PrnHelp:
	push	hl
	ld	de,Help
	call	_PRSTR
	pop	hl
	jr	NextChar

EndCmd:
	; (main loop timing)
	ld	a,(DefltM)
	ld	de,CHG1+1
	ld	c,LOPCMD
	call	MulAdd			; M1 wait states for main loop
	; (INT timing)
	ld	de,CHG2+1
	ld	c,INTCMD
	call	MulAdd			; M1 wait states for interrupt routine
	ld	a,(DefltV)			; one VPD access during interrupt routine
	ld	c,a
	ld	b,0
	ld	hl,(CHG2+1)
	add	hl,bc
	ld	(CHG2+1),hl

	; (identify and set vertical retrace frequency)
	ld	a,(CurRg9)			; MSX2 VDP R#9 value to set
	di
	out	(099h),a
	ld	b,a
	ld	a,089h
	out	(099h),a
	ei

	; (filling XTAL string output value)
	ld	hl,(XTAL)
	ld	(DVDT),hl
	ld	hl,(XTAL+2)
	ld	(DVDT+2),hl			; DVDT=XTAL to display
	ld	hl,XTVal-1			; pointer to string
	call	Str8Digits

	; (divide XTAL frequency by 6)
	ld	hl,(XTAL)
	ld	(DVDT),hl
	ld	hl,(XTAL+2)
	ld	(DVDT+2),hl
	ld	hl,6
	ld	(DVSR),hl
	ld	l,h
	ld	(DVSR+2),hl
	call	DIVIDE			; now DVDT contains XTAL/6
	ld	hl,(DVDT)
	ld	(XTAL),hl
	ld	hl,(DVDT+2)
	ld	(XTAL+2),hl

	; (calculate count variables)
	ld	hl,016c4h
	ld	de,01h			; 313*228 = PAL mode, ~50Hz
	ld	a,(CurRg9)
	and	02h
	jr	nz,PalMode

	ld	hl,0e958h
	ld	de,0h			; 262*228 = NTSC mode, ~60Hz
PalMode:
	ld	(OneLine),hl
	ld	(OneLine+2),de
	push	hl
	push	de			; preserve OneLine value
	ld	(DVSR),hl
	ld	(DVSR+2),de
	call	DIVIDE			; DVDT=XTAL\OneLine, RMDR=XTAL%OneLine

	ld	hl,(DVDT)
	ld	(VFreqI),hl
	ld	hl,(DVDT+2)
	ld	(VFreqI+2),hl
	ld	hl,(RMDR)
	ld	(VFreqR),hl
	ld	hl,(RMDR+2)
	ld	(VFreqR+2),hl		; preserve integer and fraction

	; (prepare VR string for printout)
	ld	a,08h
	ld	(Msize),a		; now 64-bit mathematics
	ld	hl,DVDT
	call	ClrVal			; clear 64-bit DVDT
	ld	hl,DVSR
	call	ClrVal			; clear 64-bit DVSR

	ld	hl,(XTAL)
	ld	(DVDT),hl
	ld	hl,(XTAL+2)
	ld	(DVDT+2),hl
	ld	hl,04240h
	ld	(DVSR),hl
	ld	hl,0fh
	ld	(DVSR+2),hl
	call	MULTIPLY		; RMDR=XTAL*1000000 (*one million)

	ld	hl,RMDR
	ld	de,DVDT
	ld	bc,8
	ldir				; copy 64-bit RMDR to DVDT

	pop	de
	pop	hl
	ld	(DVSR),hl
	ld	(DVSR+2),de		; DVSR's bytes 4-7 are 0s, no need to reset
	call	DIVIDE			; XTAL*1000000/(Oneline*6) -> DVDT integer contains 6 digits of fraction

	ld	a,04h
	ld	(Msize),a		; back to 32-bit mathematics
	ld	hl,VRVal-1
	call	Str8Digits		; convert DVDT to the string for output

	; (display vertical refresh rate)
	ld	de,XTVRStr
	call	_PRSTR

	; (set up number of testing loops)
	ld	a,(VFreqI)
	inc	a			; first loop is synchronization
	ld	(Hz+1),a

	; (disable interrupts)
	di
	; (set new interrupt vector)
	ld	hl,(039h)
	ld	(Save38),hl
	ld	hl,Sub38
	ld	(039h),hl
	; (turn vertical retrace interrupt on)
	ld	a,(RG0SAV)
	ld	(SavRg0),a
	and	0fh			; disable digitize, LP and HR interrupts
	ld	e,80h
	call	WriVReg
	ld	a,(RG1SAV)
	ld	(SavRg1),a
	or	060h			; enable screen and VR interrupt
	ld	e,81h
	call	WriVReg
	; set current status register to S#0
	xor	a
	out	(099h),a
	ld	a,08fh
	out	(099h),a

	; (wait for 61 VR interrupts to ensure VR mode change occur properly)
	ld	b,61
IntWait:
	in	a,(099h)
	and	080h
	jr	z,IntWait
	djnz	IntWait

	; (restoration of the test loop command - commented)
;	ld	hl,18h+(0100h-(PrExit+2-WaitLoop))*256
;	ld	(PrExit),hl

	; Register map
	; Primary set: HL=counter LSW, DE=addition to LSW (D=0), C=099h, B=temp var
	; Secondary set: HL=counter MSW, DE=addition to MSW (=0), C=99h, A=ticker

	; (definition of the length of the following loop in T-cycles)
	LOPLEN	equ	11+ 4+15+ 4+12		; total length in T-cycles excluding addition waitstates
	LOPCMD	equ	1 + 1+ 2+ 1+ 1		; 5 commands = 6 M1 T-cycles

	; (initializing variables)
	xor	a
	ld	h,a
	ld	l,a			; LSW
CHG1:	ld	de,LOPLEN		; LSW addition
	ld	b,a			; assigned to ticket within INT procedure
	ld	c,099h
	exx
	ld	h,a
	ld	l,a
	ld	d,a
	ld	e,a
	ld	c,099h
	exx
	ex	af,af'
	xor	a
	ex	af,af'

	; (calibrating: wait for first interrupt to occur)
	ei
WFirst:
	rr	b			; go ahead if B becomes 1 (CY=1)
	jr	nc,WFirst

WaitLoop:
	add	hl,de		; 11	(19)
	exx			; 4	(d9)
	adc	hl,de		; 15	(ed 5a) - two M1 cycle instruction
	exx			; 4	(d9)
PrExit:	jr	WaitLoop	; 12	(20 f9)

	; (adjust for INT procedure timing)
	ld	a,(Hz+1)
	dec	a
	ld	b,a
CHG2:	ld	de,INTLEN
	call	Add32

	; (preserve T-cycle count)
	ld	(SpdCnt),hl
	ld	(DVDT),hl
	exx
	ld	(SpdCnt+2),hl
	ld	(DVDT+2),hl
	exx

	; (calculate T-cycles for one VDP vertical frame)
	ld	hl,(VFreqI)
	ld	(DVSR),hl
	ld	hl,(VFreqI+2)
	ld	(DVSR+2),hl
	call	DIVIDE			; DVDT contains T-cycles for one frame

	; (calculate addition to CPU T-cycles)
	ld	a,08h
	ld	(Msize),a		; switch to 64-bit math as * can be larger that 32-bit
	ld	hl,0
	ld	(DVDT+4),hl
	ld	(DVDT+6),hl		; clear upper 32 bits of the DVSR (it contains T-cycles for one frame)
	ld	de,DVSR
	ld	hl,VFreqR
	ld	bc,8
	ldir				; now DVSR contains fraction of the frequency
	call	MULTIPLY		; RMDR=Fraction of frequency*T-cycles for one frame

	ld	de,DVDT
	push	de
	ld	hl,RMDR
	ld	bc,8
	ldir				; now DVDT contains result of multiplication above

	ld	de,DVSR
	ld	hl,OneLine
	ld	bc,8
	ldir
	call	DIVIDE			; now DVDT contains addition to the CPU T-cycles counted

	ld	a,4
	ld	(Msize),a		; get back to 32-bit math

	; (calculate final value for T-cycles)
	ld	de,SpdCnt
	pop	hl			; HL <- DVDT
	call	AddDH			; (DE)=(DE)+(HL)

	di
	; (restore initial VDP interrupt settings)
	ld	a,(SavRg0)
	ld	e,80h
	call	WriVReg
	ld	a,(SavRg1)
	ld	e,81h
	call	WriVReg
	; (resetting interrupt vector)
	ld	hl,(Save38)
	ld	(039h),hl
	ei
InvDe1:	jr	DispLine

InvDev:
	; (display message about unexpected interrupt and exit)
	ld	de,UnxStr
	call	_PRSTR
	ret

DispLine:

	; ; SET CPU to 7,372,700
	; ld	a, 00
	; ld	(SpdCnt), a
	; ld	a, $80
	; ld	(SpdCnt+1), a
	; ld	a, $70
	; ld	(SpdCnt+2), a
	; ld	a, $00
	; ld	(SpdCnt+3), a

	; $0038 4000 => 3,686,400
	ld	a, 00
	ld	(SpdCnt), a
	ld	a, $40
	ld	(SpdCnt+1), a
	ld	a, $38
	ld	(SpdCnt+2), a
	ld	a, $00
	ld	(SpdCnt+3), a

	; (display number of T-cycles) ?? freq of cpu?
	ld	a,(SpdCnt+3)
	call	Hex
	ld	a,(SpdCnt+2)
	call	Hex
	ld	a,(SpdCnt+1)
	call	Hex
	ld	a,(SpdCnt)
	call	Hex

	; (display 5 space chars)
	ld	de,Blank5
	call	_PRSTR

	; (calculate MHz)
	ld	hl,SpdCnt
	ld	de,DVDT
	ld	bc,4
	ldir
	ld	hl,MHz1-1
	call	Str8Digits

	; (display MHz)
	ld	de,MHz
	ld	a,(de)
	cp	'0'
	jr	nz,NoSkip
	inc	de			; skip first char in speed if it's '0'
NoSkip:
	call	_PRSTR

	; (now SpdCnt contains estimated value of T-cycles per 1 second)
	; (check for RTC presense)
	di
	ld	a,0dh
	out	(0b4h),a		; mode register
	ld	a,8
	out	(0b5h),a		; RTC register page 0, timer enabled
	xor	a
	out	(0b4h),a		; 1-second counter
	nop
	in	a,(0b5h)		; get current seconds number
	ei
	cp	0ffh
	jr	nz,RTCOk		; no RTC chip is installed

	ld	de,NRTCStr
	call	_PRSTR
	ret

RTCOk:
	; (correcting wait state values)
	ld	a,(DefltM)
	ld	de,CHG3+1
	ld	c,RTCCMD
	call	MulAdd			; M1 wait states for RTC loop
	ld	hl,(CHG3+1)
	ld	bc,(DefltR)
	ld	b,0
	add	hl,bc
	ld	(CHG3+1),hl		; one RTC access instruction

RTCLoop:
	; (start RTC period calc loop)

	RTCLEN	equ	11+ 4+15+ 4+11+ 4+ 4+12		; T-STATES in RTCWait loop
	RTCCMD	equ	 1+ 1+ 2+ 1+ 1+ 1+ 1+ 1		; M1 States in RTCWait loop

	; (initialize variables)
	xor	a
	ld	h,0xFF
	ld	l,0x00
CHG3:	ld	de, RTCLEN
	exx
	ld	h,0xFF
	ld	l,0xFF
	ld	d,a
	ld	e,a
	exx

	di
	; (wait until next second begins)
	ld	c,0fh			; masking byte
	in	a,(0b5h)
	and	c
	inc	a			; next second
	daa
	and	c
	ld	b,a

WaitSec:
	in	a,(0b5h)
	and	c
	cp	b
	jr	nz,WaitSec

	inc	a		; 4
	daa			; 4
	and	c		; 4
	ld	b, a		; 4 ; next second
RTCWait:
	add	hl,de		; 11	(19)
	exx			; 4	(d9)
	adc	hl,de		; 15	(ed 5a) - two M1 cycle instruction
	exx			; 4	(d9)
	in	a, (0b5h)		; 11	(db b5)
	and	c		; 4	(a1)
	cp	b		; 4	(b8)
	jr	nz,RTCWait		; 12(m)	(20 f5)	;7 (not met)

	; (preserve counter)
	ld	(RTCCnt),hl
	exx
	ld	(RTCCnt+2),hl
	exx

	; (get and display second ticker)
	in	a,(0b5h)		; get seconds LOW
	and	0fh		; reset high nibble
	ld	b,a
	ld	a,1		; seconds HIGH location
	out	(0b4h),a
	nop
	in	a,(0b5h)		; get seconds HIGH
	rlca
	rlca
	rlca
	rlca
	and	0f0h
	or	b
	ei
	call	Hex

	; (back to the register 0 of the RTC)
	xor	a
	out	(0b4h),a

	; (display space char)
	ld	e,20h
	call	_PRCHR

	; (calculate difference between counters)
	ld	hl,(SpdCnt)
	ld	bc,(RTCCnt)
	and	a			; reset CY
	sbc	hl,bc
	ex	de,hl
	ld	hl,(SpdCnt+2)
	ld	bc,(RTCCnt+2)
	sbc	hl,bc			; HLDE is counter difference

	; (checking and printing sign)
	ld	a,'+'
	bit	7,h
	jr	z,NotNeg

	; result negative: change sign to '-' and negate value
	push	hl
	xor	a			; reset CY and A=0
	ld	h,a
	ld	l,a
	sbc	hl,de
	ld	e,l
	ld	d,h
	pop	bc
	ld	h,a
	ld	l,a
	sbc	hl,bc			; HLDE is negated value
	ld	a,'-'
NotNeg:
	ld	(Sign),a

	; (preserve dufference)
	ld	(RTCCnt),de
	ld	(RTCCnt+2),hl

	; (display sign)
	push	de
	push	hl
	ld	e,a
	call	_PRCHR

	; (display difference HIGH)
	pop	hl
	push	hl
	ld	a,h
	call	Hex
	pop	hl
	ld	a,l
	call	Hex
	pop	de
	push	de
	ld	a,d
	call	Hex
	pop	de
	ld	a,e
	call	Hex

	; (display '/')
	ld	e,'/'
	call	_PRCHR

	; (calculating seconds per hour)
	; max value displayable is 90000h
	ld	hl,(SpdCnt)
	ld	(DVDT),hl
	ld	hl,(SpdCnt+2)
	ld	(DVDT+2),hl
	ld	hl,0e10h		; 3600
	ld	(DVSR),hl
	ld	hl,0
	ld	(DVSR+2),hl
	call	DIVIDE			; T-cycle count / 3600
	ld	hl,(DVDT)
	ld	(DVSR),hl
	ld	hl,(DVDT+2)
	ld	(DVSR+2),hl		; move divident to divisor

	; (calculate difference in seconds)
	ld	hl,(RTCCnt+2)
	ld	de,0fh
	or	a
	sbc	hl,de
	jr	nc,LrgNum		; too large number, display -.-

	; (multiply RTC counter by 4096 - shift by 12 bits left)
	ld	de,(RTCCnt)
	ld	hl,(RTCCnt+2)		; HLDE = RTC counter
	ld	h,l
	ld	l,d
	ld	d,e
	ld	e,0			; *256
	ld	b,4
Shift4:
	sla	d
	rl	l
	rl	h
	djnz	Shift4			; *16

	ld	(DVDT),de
	ld	(DVDT+2),hl
	call	DIVIDE			; RTCCnt*4096/(TCycle/3600)

	ld	hl,401
	ld	(DVSR),hl
	ld	hl,0
	ld	(DVSR+2),hl
	call	DIVIDE			; RTCCnt*4096/(TCycle/3600)/401

	ld	hl,0ah
	ld	(DVSR),hl
	call	DIVIDE

	; (fraction)
	ld	hl,SHStr1-1
	ld	a,(RMDR)
	add	a,'0'
	ld	(hl),a
	dec	hl
	dec	hl

	; (3 chars of integer part)
	ld	b,3
SecNxt:
	exx
	call	DIVIDE
	exx
	ld	a,(RMDR)
	add	a,'0'
	ld	(hl),a
	dec	hl
	djnz	SecNxt

	; (skip 0s)
	inc	hl
	cp	30h
	jr	nz,NotZero
	inc	hl
	ld	a,(hl)
	cp	30h
	jr	nz,NotZero
	inc	hl
NotZero:
	ex	de,hl
	jr	DspDiff
LrgNum:
	ld	de,LrNum
DspDiff:
	; (display difference string)
	call	_PRSTR

	ld	de,Ahead
	ld	a,(Sign)
	cp	'+'
	jr	z,Positive
	ld	de,Behind
Positive:
	call	_PRSTR

	; (loop: program is stopped by CTRL-C)
	jp	RTCLoop


	; (add 32-bit words)
Add32:
	add	hl,de
	exx
	adc	hl,de
	exx
	djnz	Add32
	ret

	; (write VDP register)
WriVReg:
	out	(099h),a
	ld	a,e
	out	(099h),a
	ret
						; +11 is for interrupt routine RST38, and 2 for interrupt response wait
INTLEN	equ	4+11+ 8+ 7+ 4+ 4+ 7+ 7+ 4+ 4+10	+11+2	; T-cycle count for int routine except last one
INTCMD	equ	1+ 2+ 2+ 1+ 1+ 1+ 1+ 1+ 1+ 1+ 1	+ 1				; 11 commands + one RST

Sub38:
	ex	af,af'		; 4	(08)
	; read VDP status register #0 to clear VR interrupt
	in	b,(c)		; 11	(ed 40) - two M1 cycle instruction
	bit	7,b		; 8	(cb 78) - two M1 cycle instruction
	jr	z,UnxInt	; 7 (n)	(28 12)

	inc	a		; 4	(3c)	; increase ticker
	ld	b,a		; 4	(47)
Hz:	cp	51		; 7	(fe 33)	; default value = 50Hz
	jr	z,LastTick	; 7 (n)	(28 03)	; 12 (met)

	ex	af,af'		; 4	(08)
	ei			; 4	(fb)
	ret			; 10	(c9)

LastTick:
	xor	a		; 4
	ld	(PrExit),a	; 13
	ld	(PrExit+1),a	; 13
	; (exit without enabling interrupts)
	ex	af,af'
	ret			; 10

UnxInt:
	; (interrupt occured from unexpected source)
	xor	a
	ld	(InvDe1),a	; remove JR and display "Unexpected interrupt"
	ld	(InvDe1+1),a
	jr	LastTick	; and end the test

Save38:	dw	0
SavRg0:	db	0
SavRg1:	db	0
CurRg9:	db	0

UnxStr:
	db	"Interrupt from unexpected source",0dh,0ah
	db	"occur. Can't proceed with the test",0dh,0ah,"$"

	; (print hexadecimal representation of A)
Hex:
	push	af
	rra
	rra
	rra
	rra
	call	PrH
	pop	af
	call	PrH
	ret
PrH:
	and	0fh
	add	a,'0'
	cp	'9'+1
	jr	c,NoHex
	add	a,'A'-'9'-1
NoHex:
	ld	e,a
_PRCHR:
	ld	c,2
Call5:
	call	5
	ret
_PRSTR:
	ld	c,9
	jr	Call5


; ----------------------------- Initializes RMDR ------------------------------
; clears remainder and sets b to number of bits to process during math operation
; in: none
; out: B=number of bits
; corrupt: A, C
ClrRMDR:
	; (clear remainder)
	ld	hl,RMDR

; ------------------------ Initializes Math varilable -------------------------
; clears variable and sets b to number of bits to process during math operation
; in: HL=pointer to variable
; out: B=number of bits
; corrupt: A, C
ClrVal:
	; (clear value in (HL))
	ld	bc,(Msize-1)		; operand size in bytes into B
	push	hl			; preserve pointer
Value0:
	ld	(hl),0
	inc	hl
	djnz	Value0
	ld	a,(Msize)		; operand size in bytes into A
	add	a,a
	add	a,a
	add	a,a			; in bytes * 8 = in bits
	ld	b,a
	pop	hl			; restore pointer
	ret

; --------------------------- Divide two numbers ------------------------------
; divides two numbers of (Msize) in size
; in: DVDT=divident, DVSR=divisor
; out: DVDT=division result integer, RMDR=division remainder
; corrupt: all regs; (DVSR) is preserved
DIVIDE:
	call	ClrRMDR
	call	ShDVDl			; shift divident left

DIVLoop:
	ld	hl,RMDR
	call	ShLeft			; rotate C into LSB of remainder
	jr	c,DIVSubs

	call	RMmDVS
	jr	nc,DIVNoAdd

	call	DVSpRM			; add it back - substraction result was negative
DIVNoAdd:
	ccf				; invert C flag

DIVSh:
	call	ShDVDl
	djnz	DIVLoop
	ret

DIVSubs:
	call	RMmDVS
	jr	DIVSh

RMmDVS:	; RMDR=RMDR-DVSR
	; substract divisor from remainder, returns C flag
	ld	hl,DVSR
	ld	de,RMDR
	push	bc
	xor	a			; CY=0
	ld	bc,(Msize-1)		; operand size in bytes into B
SubLoop:
	ld	a,(de)
	sbc	a,(hl)
	ld	(de),a
	inc	hl
	inc	de
	djnz	SubLoop
	pop	bc
	ret

; *** RMDR=RMDR+DVSR
DVSpRM:
	; now add divisor to remainder
	ld	hl,DVSR
	ld	de,RMDR

; ------------------ Add values in (DE) and (HL) and stor into (DE) --------------------
; (DE)=(DE)+(HL), according to Msize
; in: HL, DE
; out: B=number of bits
; corrupt: A, HL, DE
AddDH:
	push	bc
	xor	a			; CY=0
	ld	bc,(Msize-1)		; operand size in bytes into B
AddLoop:
	ld	a,(de)
	adc	a,(hl)
	ld	(de),a
	inc	hl
	inc	de
	djnz	AddLoop
	pop	bc
	ret


; *** Shift value left in continuous way
ShlCon:
	push	hl			; preserve starting byte
	and	a			; clear CY (bit 7 of first byte will be=0)
	call	ShLeft
	pop	hl
	ret	nc			; MSbit is 0, do not raise bit 0 of first byte
	set	0,(hl)			; otherwise raise the bit to have value cyclically rotated
	ret

; *** Shift divident left
ShDVDl:	; (in: CY flag; out: CY flag)
	ld	hl,DVDT
; *** Shift variable left shiftng current CY in and shifting new CY out
ShLeft:
	push	bc
	ld	bc,(Msize-1)		; operand size in bytes into B
ShDVDT:
	rl	(hl)
	inc	hl
	djnz	ShDVDT			; rotate divident left to C
	pop	bc
	ret

; *** Shift (HL) right
ShRigh:
	push	bc
	ld	a,(Msize)		; operand size in bytes into C
	ld	c,a
	ld	b,0
	add	hl,bc
	dec	hl			; starting MSB when shifting left!
	ld	b,a			; byte counter
	push	hl			; preserve starting byte
	and	a			; clear CY (bit 7 of first byte=0)
ShRig:
	rr	(hl)
	dec	hl
	djnz	ShRig			; rotate divident left to C
	pop	hl
	pop	bc
	ret	nc			; LSbit is 0, do not raise bit 7 of first byte
	set	7,(hl)			; otherwise raise the bit to have value cyclically rotated
	ret

; --------------------------- Multiply two numbers ------------------------------
; multiplies two numbers of (Msize) in size
; in: DVDT=operand 1, DVSR=operand 2
; out: RMDR=multiply of operands
; corrupt: all regs; (DVSR) and (DVDT) are preserved
MULTIPLY:
	call	ClrRMDR			; clear remainder and set B to 64
MulLoop:
	; (rotate DVDT right)
	ld	hl,DVDT
	call	ShRigh			; get CY=1 if LSB of DVDT is 1
	call	c,DVSpRM		; add DVSR to RMDR is CY=1
	ld	hl,DVSR
	call	ShlCon			; shift DVSR left continuously
	djnz	MulLoop
	ret

; (validates hexadecimal char and returns its hex representation)
; in: A=char 0-9, a-f
; out: A=respecvie hex value 0-f
Validate:
	ld	a,(hl)			; get value char
	sub	'0'
	ret	c
	ld	b,a
	sub	0ah
	ld	a,b
	ccf
	ret	nc
	and	0dfh			; uppercase
	sub	'A'-'0'
	ret	c
	ld	b,a
	sub	'F'-'A'+1
	ld	a,b
	ccf
	ret	c
	add	a,0ah			; CY is reset
	ret

; in: DE=value to tackle, A and B are multipliers
; A is preserved
MulAdd:
	or	a
	ret	z			; no change if A = 0
	push	af
	push	hl
	ld	b,0			; BC now add#1
	ld	h,b
	ld	l,b

AddByte:
	add	hl,bc
	dec	a
	jr	nz,AddByte

	ld	a,(de)
	add	a,l
	ld	(de),a
	inc	de
	ld	a,(de)
	adc	a,h
	ld	(de),a
	pop	hl
	pop	af
	ret

; -------------------------- (exchange DVDT and DVSR) -----------------------------
; in: none
; assumptions: math size=4
; out: none
; corrupt: HL, DE, BC
ExDDDV:
	ld	hl,DVDT
	ld	de,DVSR

	ld	bc,(Msize-1)		; operand size in bytes into B
ExLoop:
	ld	a,(de)
	ld	c,a
	ld	a,(hl)
	ld	(hl),c
	ld	(de),a
	inc	hl
	inc	de
	djnz	ExLoop
	ret

; ------------- (get XTAL frequency value from the command line string [8 digits]) -------------
; in: HL=pointer to command line pos
; assumptions: math size=4
; out: CY=1 if error in value (then HL does not matter)
GetXTAL:
	exx				; switch to alternate register set (preserve HL)
	; (clear XTAL memory cells [by default it contains default value])
	ld	hl,XTAL
	call	ClrVal			; XTAL=0

	; (clear DVDT - will contain current digit)
	ld	hl,DVDT
	call	ClrVal

	; (set initial value for RMDR)
	ld	hl,09680h
	ld	(DVSR),hl
	ld	hl,098h
	ld	(DVSR+2),hl		; 10,000,000 multiplier

	exx
	ld	b,8			; 8 digits of the XTAL freq value (B in primary register set)
XTALLoop:
	ld	a,(hl)
	inc	hl
	sub	'0'
	ret	c			; <'0'
	cp	0ah
	ccf
	ret	c			; >'9'
	exx

	; (calculate current factor)
	ld	(DVDT),a		; digit from the command line
	call	MULTIPLY		; RMDR=digit[DVDT]*current factor[DVSR]

	; (add RMDR and XTAL)
	ld	de,XTAL
	ld	hl,RMDR
	call	AddDH

	; (divide divisor by 10)
	call	ExDDDV			; exchange DVDT and DVSR
	ld	a,0ah
	ld	(DVSR),a		; set divisor to 10 (other bytes a 0s)
	call	DIVIDE
	call	ExDDDV			; exchange DVDT and DVSR back (DVSR contains new factor)

	exx
	djnz	XTALLoop
	exx

	; (now check for minimal frequency = 20260000Hz = 13524A0h)
	ld	hl,(XTAL)
	ld	de,024a0h
	and	a
	sbc	hl,de
	ld	hl,(XTAL+2)
	ld	de,0135h
	sbc	hl,de
	ret	c			; frequency less than minimum

	; (now check for maximal frequency = 22550000Hz = 15815F0h)
	ld	de,(XTAL)
	ld	hl,015f0h
	and	a
	sbc	hl,de
	ld	de,(XTAL+2)
	ld	hl,0158h
	sbc	hl,de
	ret	c			; frequency more than maximum

	exx
	dec	hl
	and	a			; reset CY = no error, frequency OK
	ret

; ------------------ (convert number to the string [8 digits]) ------------------
; in: HL=pointer to the last char of the string buffer, DVDT=number to convert
; assumptions: math size=4, display format xx.xxxxxx
; out: none
Str8Digits:
	ex	de,hl			; preserve buffer address
	ld	hl,0ah
	ld	(DVSR),hl
	ld	l,h
	ld	(DVSR+2),hl		; set up DVDT and DVSR
	ex	de,hl
	ld	b,8			; 8 digits
DispXT:
	push	bc
	push	hl
	call	DIVIDE
	pop	hl
	ld	a,(RMDR)
	add	a,30h
	ld	(hl),a
	dec	hl
	pop	bc
	ld	a,b
	cp	03h
	jr	nz,NoSDot
	dec	hl			; skip dot
NoSDot:
	djnz	DispXT
	ret

	; (waitstate values)
DefltM:	db	1		; number of default wait states
DefltV:	db	0
DefltR:	db	0

CmdErr:
	db	"Error in command line",0dh,0ah,"$"
IniStr:
	db	"GR8BIT CPU speed estimation",0dh,0ah
	db	"and RTC calibration program",0dh,0ah
	db	"Usage:",0dh,0ah
	db	"CHKSPD[M*][V*][R*][H][X@][F#]",0dh,0ah,"$"
Help:
	db	"where * is hex (0..F)",0dh,0ah
	db	"wait state representation",0dh,0ah,0ah
	db	"M M1 cycle wait states",0dh,0ah
	db	"V VDP wait states",0dh,0ah
	db	"R RTC wait states",0dh,0ah
	db	"X VDP clk freq (Hz, 8 digits)",0dh,0ah
	db	"F VR freq to #=5/6 (50/60Hz)",0dh,0ah,0ah
	db	"Default setting is <M1V0R0>",0dh,0ah
	db	"and X=21.480MHz VDP clock",0dh,0ah,"$"
XTVRStr:
	db	0dh,0ah,"VDP XTAL: "
	db	"??.??????"
XTVal:	db	" MHz",0dh,0ah,"VR rate:  "
	db	"??.??????"
VRVal:	db	" Hz",0dh,0ah,0ah
	db	"T-Cycles/s   Z80 CPU speed",0dh,0ah,"$"

NRTCStr:
	db	"Can't proceed: RTC is not installed",0dh,0ah,"$"
MHz:
	db	"??.??????"
MHz1:	db	" MHz",0dh,0ah,0ah,"T  T-cycle delta",0dh,0ah,"$"
Blank5:	db	"    "
Blank1:	db	" $"
SHStr:	db	"???.?"
SHStr1:	db	" $"
LrNum:	db	"-.-$"
Ahead:	db	"ahead    ",0dh,"$"
Behind:	db	"behind   ",0dh,"$"
Sign:	db	0

SpdCnt:
	dw	0
	dw	0			; CPU speed test counter
RTCCnt:
	dw	0
	dw	0			; RTC counter
XTAL:
	dw	0b796h,0147h,0,0	; VDP XTAL frequency in Hz, default 21480000Hz
	;						21477270MHz => 0147 B796
Msize:	db	4			; Size of math operand in bytes (2=word math, 4=dword match, 8=qword math)
					; (default math operand size=4 bytes [dword])
DVDT:	dw	0,0,0,0
DVSR:	dw	0,0,0,0
RMDR:	dw	0,0,0,0
VFreqI:	dw	0,0,0,0			; vertical frequency, integer part
VFreqR:	dw	0,0,0,0			; vertical frequency, remainder part
OneLine:
	dw	0,0,0,0

	end
;
;
;
;
;
;
;
