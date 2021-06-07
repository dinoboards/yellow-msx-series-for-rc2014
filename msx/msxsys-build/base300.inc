
; OPTROM/LOGO ROM AT Slot 0, 0x8000
; OPTROM_MODE	EQU	0		; OPTROM/LOGO ROM LOADED FROM SUBROM - SLOT/OPTENT
OPTROM_MODE	EQU	1			; OPTROM/LOGO ROM LOADED FROM SLOT 0/OPTENT
OPTENT_ALT	EQU	08010H			; SUBROM WILL LOAD OPTROM AT THIS ADDRESS IN IN MODE 1


MSXVER	EQU	2			; MSX version


OPTENT	EQU	07A00H			; optrom entrypoint - for building optrom

;	RTCENA	use RP-501
;		not defined  = Do not use RP-501
;		defined      = Use RP-501

; RTCENA	EQU	0

;	RESENA	use reset hardware
;		not defined  = Do not use reset hardware
;		defined      = Use reset hardware

; RESENA	EQU	0

;	OPTM	Optimized code
;		0 = Use orginal code
;		1 = Use optimized code

OPTM	EQU	0


;	CNTRY	Country
;		0 = Japan
;		1 = USA
;		2 = International
;		3 = UK
;		4 = France
;		5 = Germany
;		6 = Italy
;		7 = Spain
;		8 = Arabic
;		9 = Korea
;		10 = Russia
;		11 = AUSSIE!

CNTRY	EQU	11

;	F4INV	F4 (reset) register
;		0 = not inverted
;		1 = inverted

F4INV	EQU	1



;	symbols used to procedure alternate code

;	INTHZ 	interrupt frequency
;	CHRGEN	character generator
; 		0 = japanese
;		1 = international
;		2 = USSR ??
;	DATFMT	date format
; 		0 = Y-M-D
; 		1 = M-D-Y
;		2 = D-M-Y
;	KEYTYP	keyboard layout
;		0 = Japanese
;		1 = International (QWERTY/other)
;		2 = French (AZERTY)
;		3 = English
;		4 = German (DIN)
;		5 = USSR
;		6 = Spanish
;		7 = Swedish ??
;	BASVER	0 = Japanese
;		1 = International




	IF	CNTRY EQ 11

; AUS MSX settings

INTHZ	EQU	50
CHRGEN	EQU	1
DATFMT	EQU	2
KEYTYP	EQU	1
BASVER	EQU	1

	ENDIF

	IF	CNTRY EQ 0

; Japanese MSX settings

INTHZ	EQU	60
CHRGEN	EQU	0
DATFMT	EQU	0
KEYTYP	EQU	0
BASVER	EQU	0

	ENDIF

	IF	CNTRY EQ 1

; USA MSX settings

INTHZ	EQU	60
CHRGEN	EQU	1
DATFMT	EQU	1
KEYTYP	EQU	1
BASVER	EQU	1

	ENDIF

	IF	CNTRY EQ 2

; International MSX settings

INTHZ	EQU	50
CHRGEN	EQU	1
DATFMT	EQU	1
KEYTYP	EQU	1
BASVER	EQU	1

	ENDIF

	IF	CNTRY EQ 3

; UK MSX settings

INTHZ	EQU	50
CHRGEN	EQU	1
DATFMT	EQU	2
KEYTYP	EQU	3
BASVER	EQU	1

	ENDIF

	IF	CNTRY EQ 4

; French MSX settings

INTHZ	EQU	50
CHRGEN	EQU	1
DATFMT	EQU	2
KEYTYP	EQU	2
BASVER	EQU	1

	ENDIF

	IF	CNTRY EQ 5

; German MSX settings

INTHZ	EQU	50
CHRGEN	EQU	1
DATFMT	EQU	2
KEYTYP	EQU	4
BASVER	EQU	1

	ENDIF

	IF	CNTRY EQ 6

; Italian MSX settings
; Dateformat is not sure

INTHZ	EQU	50
CHRGEN	EQU	1
DATFMT	EQU	2
KEYTYP	EQU	1
BASVER	EQU	1

	ENDIF

	IF	CNTRY EQ 7

; Spanish MSX settings

INTHZ	EQU	50
CHRGEN	EQU	1
DATFMT	EQU	1
KEYTYP	EQU	6
BASVER	EQU	1

	ENDIF

	IF	CNTRY EQ 8

; Arabic MSX settings
; Unknown

INTHZ	EQU	60
CHRGEN	EQU	0
DATFMT	EQU	0
KEYTYP	EQU	0
BASVER	EQU	0

	ENDIF

	IF	CNTRY EQ 9

; Korean MSX settings

INTHZ	EQU	60
CHRGEN	EQU	0
DATFMT	EQU	0
KEYTYP	EQU	0
BASVER	EQU	0

	ENDIF

	IF	CNTRY EQ 10

; Russian MSX settings

INTHZ	EQU	60
CHRGEN	EQU	2
DATFMT	EQU	1
KEYTYP	EQU	5
BASVER	EQU	1

	ENDIF


	IF	BASVER EQ 0

CHRCUR	EQU	05CH		; yen sign
CHRFLN	EQU	"&"
CHRVLN	EQU	"@"
	ENDIF

	IF	(BASVER EQ 1) AND (CNTRY NE 3)
CHRCUR	EQU	"$"
CHRFLN	EQU	"\"
CHRVLN	EQU	"&"
	ENDIF

	IF	(BASVER EQ 1) AND (CNTRY EQ 3)

CHRCUR	EQU	0ACH		; pound sign
CHRFLN	EQU	"\"
CHRVLN	EQU	"&"

	ENDIF

