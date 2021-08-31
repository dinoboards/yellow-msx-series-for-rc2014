
	SECTION	CODE

Z80_INT		EQU	$38
RETI_CODE	EQU	$4DED
JP_CODE		EQU	$C3

RP5RTC_REG	EQU	$B4
RP5RTC_DAT	EQU	$B5

	EXTERN	_measurement

	PUBLIC	_measure_timing

;   //Disable INTS
;   //HOOK INT TO NULL OP
;   // ENABLE INTS
;   //WAIT FOR FIRST INT
;   //HOOK INT TO ABORT LOOP

;   // BEGIN
;   //  CALL LOOP  DO LOOP
;   // ON INT:
;   //  SAVE LOOP COUNTER
;   //  CHANGE INT HANDLER TO NULL OP
;   // MODIFY STACK AND RETURN FROM LOOP/VIA INT
;   // END


_measure_timing:
	DI

	LD	HL, 0
	LD	DE, 1
	EXX
	LD	HL, 0
	LD	DE, 0
	EXX


	LD	A, 0 ;REG_1SEC
	OUT	(RP5RTC_REG), A
	IN	A, (RP5RTC_DAT)
	AND	$0F
	LD	B, A

	; WAIT FOR A SECOND TICK
wait_for_starting_tick:
	IN	A, (RP5RTC_DAT)
	AND	$0F				; 7		1
	CP	B				; 4		1
	JR	Z, wait_for_starting_tick	; 7		1
	LD	B, A				; 4		1

wait_for_sec_change:				; T-STATE	M-WAIT STATE
	ADD	HL,DE				; 11		1
	EXX					; 4		1
	ADC	HL,DE				; 15		2
	EXX					; 4		1

	IN	A, (RP5RTC_DAT)			; 11		1
	AND	$0F				; 7		1
	CP	B				; 4		1
	JR	Z, wait_for_sec_change		; 12		1

						; 68		9
	EXX
	LD	(_measurement+2), HL
	EXX
	LD	(_measurement), HL

	EI
	RET
