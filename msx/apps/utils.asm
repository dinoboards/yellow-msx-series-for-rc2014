
	EXTERN	CPUSCL
	PUBLIC	VDELAY, DELAY

	SECTION	CODE

	include	"cfg.inc"

; DELAY 16US * DE (CPU SPEED COMPENSATED)
; REGISTER DE, A, AND FLAGS DESTROYED
; NO COMPENSATION FOR Z180 MEMORY WAIT STATES
; THERE IS A 27TS OVERHEAD FOR CALL/RET PER INVOCATION
;   IMPACT OF OVERHEAD DIMINISHES AS DE AND/OR CPU SPEED INCREASES
;
; CPU SCALER (CPUSCL) = (CPUHMZ - 2) FOR 16US OUTER LOOP COST
;   NOTE: CPUSCL MUST BE > 0!
;
; EXAMPLE: 8MHZ CPU, DE=6250 (DELAY GOAL IS .1 SEC OR 100,000US)
;   INNER LOOP = ((16 * 6) - 5) = 91TS
;   OUTER LOOP = ((91 + 37) * 6250) = 800,000TS
;   ACTUAL DELAY = ((800,000 + 27) / 8) = 100,003US
;
	; --- TOTAL COST = (OUTER LOOP + 27) TS ------------------------+
VDELAY:				; 17TS (FROM INVOKING CALL)		|
;									|
	; --- OUTER LOOP = ((INNER LOOP + 37) * DE) TS ---------+	|
	LD	A,(CPUSCL)	; 13TS				|	|
;								|	|
VDELAY1:			;				|	|
	; --- INNER LOOP = ((CPUSCL * 16) - 5) TS ------+	|	|
	DEC	A		; 4TS			|	|	|
	JR	NZ,VDELAY1	; 12TS (NZ) / 7TS (Z)	|	|	|
	; ----------------------------------------------+	|	|
;								|	|
	DEC	DE		; 6TS				|	|
	LD	A,D		; 4TS				|	|
	OR	E		; 4TS				|	|
	JP	NZ,VDELAY	; 10TS				|	|
	;-------------------------------------------------------+	|
;									|
	RET			; 10TS (FINAL RETURN)			|
	;---------------------------------------------------------------+

; DELAY 16US (CPU SPEED COMPENSATED) INCUDING CALL/RET INVOCATION
; REGISTER A AND FLAGS DESTROYED
; NO COMPENSATION FOR Z180 MEMORY WAIT STATES
; THERE IS AN OVERHEAD OF 3TS PER INVOCATION
;   IMPACT OF OVERHEAD DIMINISHES AS CPU SPEED INCREASES
;
; CPU SCALER (CPUSCL) = (CPUHMZ - 2) FOR 16US + 3TS DELAY
;   NOTE: CPUSCL MUST BE >= 1!
;
; EXAMPLE: 8MHZ CPU (DELAY GOAL IS 16US)
;   LOOP = ((6 * 16) - 5) = 91TS
;   TOTAL COST = (91 + 40) = 131TS
;   ACTUAL DELAY = (131 / 8) = 16.375US
;
	; --- TOTAL COST = (LOOP COST + 40) TS -----------------+
DELAY:				; 17TS (FROM INVOKING CALL)	|
	LD	A,(CPUSCL)	; 13TS				|
;								|
DELAY1:				;				|
	; --- LOOP = ((CPUSCL * 16) - 5) TS ------------+	|
	DEC	A		; 4TS			|	|
	JR	NZ,DELAY1	; 12TS (NZ) / 7TS (Z)	|	|
	; ----------------------------------------------+	|
;								|
	RET			; 10TS (RETURN)			|
	;-------------------------------------------------------+
;



	SECTION	DATA

CPUSCL:	DB	CPUMHZ - 2		; OTHERWISE 2 LESS THAN PHI MHZ
