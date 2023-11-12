	INCLUDE	"msx.inc"

; 	PUBLIC	_install_timi_hook
; 	EXTERN	_original_timi_hook	; need to be the relocated value?????
; 	EXTERN	_timi

; _install_timi_hook:
; 	LD	DE, _original_timi_hook
; 	LD	HL, H_TIMI
; 	LD	BC, 5
; 	LDIR

; 	DI
; 	LD	A, $C3			; JUMP OP CODE
; 	LD	(H_TIMI), A

; 	LD	HL, _timi
; 	LD	(H_TIMI+1), HL		; SET SLOT ADDRESS
; 	EI

; 	RET
