; Switch page 1 to the driver's allocated 16k segment
; push old segment number onto stack
; A = new segment number
push_page_1	MACRO
		CALL	MEMAP_GET_P1		; CAPTURE CURRENT ALLOCATE RAM SEGMENT IN PAGE 2
		PUSH	AF
		LD	A, (RS_P1_SEG)
		CALL	MEMAP_PUT_P1		; SWITCH IN OUR SEGMENT
		ENDM

; switch page 1 back the original allocated 16 segment
; A = original segment number
pop_page_1	MACRO
		POP	AF			; RESTORE RAM SEGMENT IN PAGE 2
		CALL	MEMAP_PUT_P1
		ENDM

