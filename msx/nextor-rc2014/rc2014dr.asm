    ; RC2014 ROM DISK driver for Nextor 2.0
    ; NEXTOR DEVICE BASED MODEL

	ORG	$4000
	DS	$100
;-----------------------------------------------------------------------------
;
; STANDARD BIOS AND WORK AREA ENTRIES

CHPUT			EQU	$00A2	; CHARACTER OUTPUT
CHGET			EQU	$009F	; CHARACTER INPUT
MSXDOS_RC2014_INIT	EQU	$8100
SLOT_3_3_ID		EQU	$8F	;
SLOT_3_1_ID		EQU	$87	;
RDSLT			EQU	$000C

DRV_START:

;-----------------------------------------------------------------------------
;
; Miscellaneous constants
;

;This is a 2 byte buffer to store the address of code to be executed.
;It is used by some of the kernel page 0 routines.

CODE_ADD	equ	0F84Ch


;-----------------------------------------------------------------------------
;
; Driver configuration constants
;

;Driver type:
;   for device-based

;DRV_TYPE	equ	1

;Driver version

VER_MAIN	equ	1
VER_SEC		equ	0
VER_REV		equ	0


;-----------------------------------------------------------------------------
;
; Error codes for DEV_RW
;


ERR.NCOMP	equ	0FFh
ERR.WRERR	equ	0FEh
ERR.DISK	equ	0FDh
ERR.NRDY	equ	0FCh
ERR.DATA	equ	0FAh
ERR.RNF		equ	0F9h
ERR.WPROT	equ	0F8h
ERR.UFORM	equ	0F7h
ERR.SEEK	equ	0F3h
ERR.IFORM	equ	0F0h
ERR.IDEVL	equ	0B5h
ERR.IPARM	equ	08Bh



;* This routine reads a byte from another bank.
;  Must be called by using CALBNK to the desired bank,
;  passing the address to be read in HL:
;    ld a,<bank number>
;    ld hl,<byte address>
;    ld ix,RDBANK
;    call CALBNK

RDBANK	equ	403Ch


;* This routine temporarily switches kernel main bank
;  (usually bank 0, but will be 3 when running in MSX-DOS 1 mode),
;  then invokes the routine whose address is at (CODE_ADD).
;  It is necessary to use this routine to invoke CALBAS
;  (so that kernel bank is correct in case of BASIC error)
;  and to invoke DOS functions via F37Dh hook.
;
;  Input:  Address of code to invoke in (CODE_ADD).
;          AF, BC, DE, HL, IX, IY passed to the called routine.
;  Output: AF, BC, DE, HL, IX, IY returned from the called routine.

CALLB0	equ	403Fh



;* This address contains one byte that tells how many banks
;  form the Nextor kernel (or alternatively, the first bank
;  number of the driver).

K_SIZE	equ	40FEh


;* This address contains one byte with the current bank number.

CUR_BANK	equ	40FFh


;-----------------------------------------------------------------------------
;
; Built-in format choice strings
;

NULL_MSG  equ     781Fh	;Null string (disk can't be formatted)
SING_DBL  equ     7820h ;"1-Single side / 2-Double side"


;-----------------------------------------------------------------------------
;
; Driver signature
;
	db	"NEXTOR_DRIVER",0


;-----------------------------------------------------------------------------
;
; Driver flags:
;    bit 0: 0 for drive-based, *1 for device-based
;    bit 2: 1 if the driver implements the DRV_CONFIG routine
;             (used by Nextor from v2.0.5)

	db	1+4


;-----------------------------------------------------------------------------
;
; Reserved byte
;

	db	0


;-----------------------------------------------------------------------------
;
; Driver name
;

DRV_NAME:
	db	"RC2014 ROM DISK Driver"
	ds	32-($-DRV_NAME)," "


;-----------------------------------------------------------------------------
;
; Jump table for the driver public routines
;

	; These routines are mandatory for all drivers
        ; (but probably you need to implement only DRV_INIT)

	jp	0;DRV_TIMI
	jp	DRV_VERSION
	jp	DRV_INIT
	jp	DRV_BASSTAT
	jp	DRV_BASDEV
	jp	DRV_EXTBIO
	jp	DRV_DIRECT0
	jp	DRV_DIRECT1
	jp	DRV_DIRECT2
	jp	DRV_DIRECT3
	jp	DRV_DIRECT4
	jp	DRV_CONFIG

	ds	12

	; These routines are mandatory for device-based drivers

	jp	DEV_RW
	jp	DEV_INFO
	jp	DEV_STATUS
	jp	LUN_INFO


;=====
;=====  END of data that must be at fixed addresses
;=====


;-----------------------------------------------------------------------------
;
; Timer interrupt routine, it will be called on each timer interrupt
; (at 50 or 60Hz), but only if DRV_INIT returns Cy=1 on its first execution.

; DRV_TIMI:
; 	ret


;-----------------------------------------------------------------------------
;
; Driver initialization routine, it is called twice:
;
; 1) First execution, for information gathering.
;    Input:
;      A = 0
;      B = number of available drives
;      HL = maximum size of allocatable work area in page 3
;    Output:
;      A = Not used for this device based driver
;      HL = size of required work area in page 3
;      Cy = 1 if DRV_TIMI must be hooked to the timer interrupt, 0 otherwise
;
; 2) Second execution, for work area and hardware initialization.
;    Input:
;      A = 1
;      B = number of allocated drives for this controller
;
;    The work area address can be obtained by using GWORK.
;
;    If first execution requests more work area than available,
;    second execution will not be done and DRV_TIMI will not be hooked
;    to the timer interrupt.
;
;    If first execution requests more drives than available,
;    as many drives as possible will be allocated, and the initialization
;    procedure will continue the normal way
;    (for drive-based drivers only. Device-based drivers always
;     get two allocated drives.)
CALSLT	EQU	0001Ch

DRV_INIT:
	OR	A
	JR	NZ, DRV_INIT2

DRV_INIT1:
	LD	DE, INFO_S
	CALL	PRINT

	;NOTE THAT CY IS 0 (NO INTERRUPT HOOKING NEEDED)
	XOR	A
	LD	HL, ST_TOTAL
	RET

DRV_INIT2:
	CALL	MY_GWORK			; PASS WORK AREA ADDRESS TO EXT-BIO

	PUSH	IX

	CALL	DRV_INIT_CF

	POP	IX
	JR	Z, DRV_CF_NOT			; NOT PRESENT

	LD	A, PRES_CF

DRV_CF_STATE_STORE:
	LD	(IX+ST_PRESENT), A

	LD	HL, $4000
	LD	DE, MSX_MARKER_1
	LD	B, 2
	CALL	TEST_MARKER
	RET	NZ				; NO MSX MUSIC DETECTED

	LD	HL, $401C
	LD	DE, MSX_MARKER_2
	LD	B, 4
	CALL	TEST_MARKER
	RET	NZ				; NO MSX MUSIC DETECTED

	LD	A, (IX+ST_PRESENT)
	OR	PRES_MS
	LD	(IX+ST_PRESENT), A		; SET BIT FLAG TO INDICATE MSX-MUSIC ROM IS PRESENT

	RET

DRV_CF_NOT:
	XOR	A
	JR	DRV_CF_STATE_STORE

TEST_MARKER:
	LD	A, $87		; slot 3-1 for music
	PUSH	DE
	PUSH	BC
	CALL	RDSLT
	POP	BC
	POP	DE
	EX	DE, HL
	CP	(HL)
	EX	DE, HL
	RET	NZ
	INC	DE
	INC	HL
	DJNZ	TEST_MARKER
	RET

MSX_MARKER_1:
	DEFB	"AB"
MSX_MARKER_2:
	DEFB	"OPLL"

;-----------------------------------------------------------------------------
;
; Obtain driver version
;
; Input:  -
; Output: A = Main version number
;         B = Secondary version number
;         C = Revision number

DRV_VERSION:
	ld	a,VER_MAIN
	ld	b,VER_SEC
	ld	c,VER_REV
	ret


;-----------------------------------------------------------------------------
;
; BASIC expanded statement ("CALL") handler.
; Works the expected way, except that if invoking CALBAS is needed,
; it must be done via the CALLB0 routine in kernel page 0.

DRV_BASSTAT:
	scf
	ret


;-----------------------------------------------------------------------------
;
; BASIC expanded device handler.
; Works the expected way, except that if invoking CALBAS is needed,
; it must be done via the CALLB0 routine in kernel page 0.

DRV_BASDEV:
	scf
	ret


;-----------------------------------------------------------------------------
;
; Extended BIOS hook.
; Works the expected way, except that it must return
; D'=1 if the old hook must be called, D'=0 otherwise.
; It is entered with D'=1.

DRV_EXTBIO:
	RET

;-----------------------------------------------------------------------------
;
; Direct calls entry points.
; Calls to addresses 7850h, 7853h, 7856h, 7859h and 785Ch
; in kernel banks 0 and 3 will be redirected
; to DIRECT0/1/2/3/4 respectively.
; Receives all register data from the caller except IX and AF'.

DRV_DIRECT0:
DRV_DIRECT1:
DRV_DIRECT2:
DRV_DIRECT3:
DRV_DIRECT4:
	ret


;-----------------------------------------------------------------------------
;
; Get driver configuration
; (bit 2 of driver flags must be set if this routine is implemented)
;
; Input:
;   A = Configuration index
;   BC, DE, HL = Depends on the configuration
;
; Output:
;   A = 0: Ok
;       1: Configuration not available for the supplied index
;   BC, DE, HL = Depends on the configuration
;
; * Get number of drives at boot time (for device-based drivers only):
;   Input:
;     A = 1
;     B = 0 for DOS 2 mode, 1 for DOS 1 mode
;   Output:
;     B = number of drives
;
; * Get default configuration for drive
;   Input:
;     A = 2
;     B = 0 for DOS 2 mode, 1 for DOS 1 mode
;     C = Relative drive number at boot time
;   Output:
;     B = Device index
;     C = LUN index

DRV_CONFIG:
	CP	1
	JR	Z, DRV_CONFIG_COUNT
	CP	2
	JR	Z, DRV_CONFIG_GET

	LD	A, 1
	RET

DRV_CONFIG_COUNT:
	LD	B, 1
	CALL	DRV_GET_PRESENT
	BIT	BIT_PRES_CF, A
	JR	Z, DRV_CFG_NO_CF
	INC	B

DRV_CFG_NO_CF:
	BIT	BIT_PRES_MS, A
	JR	Z, DRV_CFG_NO_MS
	INC	B

DRV_CFG_NO_MS:
	XOR	A
	RET

DRV_CONFIG_GET:
	LD	A, C
	CP	0
	JR	Z, DRV_CFG_A
	CP	1
	JR	Z, DRV_CFG_B

	LD	A, 1
	RET

DRV_CFG_A:
	LD	B, 1
DRV_CFG:
	LD	C, 1
	XOR	A
	RET

DRV_CFG_B:
	LD	B, 2
	JR	DRV_CFG


;=====
;=====  BEGIN of DEVICE-BASED specific routines
;=====

;-----------------------------------------------------------------------------
;
; Read or write logical sectors from/to a logical unit
;
;Input:    Cy=0 to read, 1 to write
;          A = Device number, 1 to 7
;          B = Number of sectors to read or write
;          C = Logical unit number, 1 to 7
;          HL = Source or destination memory address for the transfer
;          DE = Address where the 4 byte sector number is stored.
;Output:   A = Error code (the same codes of MSX-DOS are used):
;              0: Ok
;              .IDEVL: Invalid device or LUN
;              .NRDY: Not ready
;              .DISK: General unknown disk error
;              .DATA: CRC error when reading
;              .RNF: Sector not found
;              .UFORM: Unformatted disk
;              .WPROT: Write protected media, or read-only logical unit
;              .WRERR: Write error
;              .NCOMP: Incompatible disk.
;              .SEEK: Seek error.
;          B = Number of sectors actually read (in case of error only)

DEV_RW:
	EI
	JR	NC, DEV_READ		; ARE WE READING?

	CALL	DEVICE_MAPPING
	CP	DEV_MAP_CF
	JP	Z, DEV_WRT_CF

	JR	DEV_RW_ERR

DEV_READ:
	CALL	DEVICE_MAPPING
	CP	DEV_MAP_ROM
	JP	Z, DEV_READ_ROM

	CP	DEV_MAP_CF
	JP	Z, DEV_READ_CF

	CP	DEV_MAP_MS
	JP	Z, DEV_READ_MS

DEV_RW_ERR:
	LD	A, ERR.IDEVL
	LD	B, 0
	RET

;-----------------------------------------------------------------------------
;
; Device information gathering
;
;Input:   A = Device index, 1 to 7
;         B = Information to return:
;             0: Basic information
;             1: Manufacturer name string
;             2: Device name string
;             3: Serial number string
;         HL = Pointer to a buffer in RAM
;Output:  A = Error code:
;             0: Ok
;             1: Device not available or invalid device index
;             2: Information not available, or invalid information index
;         When basic information is requested,
;         buffer filled with the following information:
;
;+0 (1): Numer of logical units, from 1 to 7. 1 if the device has no logical
;        units (which is functionally equivalent to having only one).
;+1 (1): Device flags, always zero in Beta 2.
;
; The strings must be printable ASCII string (ASCII codes 32 to 126),
; left justified and padded with spaces. All the strings are optional,
; if not available, an error must be returned.
; If a string is provided by the device in binary format, it must be reported
; as an hexadecimal, upper-cased string, preceded by the prefix "0x".
; The maximum length for a string is 64 characters;
; if the string is actually longer, the leftmost 64 characters
; should be provided.
;
; In the case of the serial number string, the same rules for the strings
; apply, except that it must be provided right-justified,
; and if it is too long, the rightmost characters must be
; provided, not the leftmost.

DEV_INFO:
	CALL	DEVICE_MAPPING
	CP	DEV_MAP_CF
	JR	Z, DEV_INFO_CF

	CP	DEV_MAP_ROM
	JR	Z, DEV_INFO_ROM

	CP	DEV_MAP_MS
	JR	Z, DEV_INFO_MS

DEV_INFO_NO_DEV:
	LD	A, 1
	RET


DEV_INFO_CF:
	LD	A, B
	CP	0
	JR	Z, DEV_INFO_0

	JP	DEV_INFO_CF_FLDS

DEV_INFO_ROM:
	LD	A, B
	CP	0
	JR	Z, DEV_INFO_0

	CP	1
	JR	Z, DEV_INFO_RM_MAN

	CP	2
	JR	Z, DEV_INFO_RM_NAME

	CP	3
	JR	Z, DEV_INFO_RM_SERIAL

	LD	A, 1
	RET

DEV_INFO_MS:
	LD	A, B
	CP	0
	JR	Z, DEV_INFO_0

	CP	1
	JR	Z, DEV_INFO_RM_MAN

	CP	2
	JR	Z, DEV_INFO_MS_NAME

	LD	A, 1
	RET

DEV_INFO_0:
	LD	(HL), 1
	INC	HL
	LD	(HL), 0
	XOR	A
	RET

DEV_INFO_RM_MAN:
	EX	DE, HL
	LD	HL, ROM_MAN
	LD	BC, ROM_MAN_LEN
	LDIR
	LD	B, 64-ROM_MAN_LEN

RIGHT_PAD:
	EX	DE, HL
	LD	A, " "
RIGHT_PAD_LP:
	LD	(HL), A
	INC	HL
	DJNZ	RIGHT_PAD_LP

	XOR	A
	RET

DEV_INFO_RM_NAME:
	EX	DE, HL
	LD	HL, ROM_DRV_NAME
	LD	BC, ROM_DRV_NAME_LEN
	LDIR
	LD	B, 64-ROM_DRV_NAME_LEN
	JR	RIGHT_PAD

DEV_INFO_RM_SERIAL:
	LD	BC, 63
	ADD	HL, BC

	LD	(HL), '1'
	DEC	HL
	LD	(HL), '2'
	DEC	HL

LEFT_PAD:
	LD	B, 62
	LD	A, " "

LEFT_PAD_LP:
	LD	(HL), A
	DEC	HL
	DJNZ	LEFT_PAD_LP

	XOR	A
	RET

DEV_INFO_MS_NAME:
	EX	DE, HL
	LD	HL, MSX_MUSIC_NAME
	LD	BC, MSX_MUSIC_NAME_LEN
	LDIR
	LD	B, 64-MSX_MUSIC_NAME_LEN
	JR	RIGHT_PAD

;-----------------------------------------------------------------------------
;
; Obtain device status
;
;Input:   A = Device index, 1 to 7
;         B = Logical unit number, 1 to 7
;             0 to return the status of the device itself.
;Output:  A = Status for the specified logical unit,
;             or for the whole device if 0 was specified:
;                0: The device or logical unit is not available, or the
;                   device or logical unit number supplied is invalid.
;                1: The device or logical unit is available and has not
;                   changed since the last status request.
;                2: The device or logical unit is available and has changed
;                   since the last status request
;                   (for devices, the device has been unplugged and a
;                    different device has been plugged which has been
;                    assigned the same device index; for logical units,
;                    the media has been changed).
;                3: The device or logical unit is available, but it is not
;                   possible to determine whether it has been changed
;                   or not since the last status request.
;
; Devices not supporting hot-plugging must always return status value 1.
; Non removable logical units may return values 0 and 1.
;
; The returned status is always relative to the previous invokation of
; DEV_STATUS itself. Please read the Driver Developer Guide for more info.

DEV_STATUS:
	CALL	DEVICE_MAPPING
	CP	DEV_MAP_ROM
	JR	Z, DEV_STATUS_ROM
	CP	DEV_MAP_CF
	JR	Z, DEV_STATUS_CF
	CP	DEV_MAP_MS
	JR	Z, DEV_STATUS_MS

DEV_STATUS_ERR:
	XOR	A
	RET

DEV_STATUS_ROM:
	LD	A, 1
	RET

DEV_STATUS_MS:
DEV_STATUS_CF:
	LD	A, 3
	RET

;-----------------------------------------------------------------------------
;
; Obtain logical unit information
;
;Input:   A  = Device index, 1 to 7
;         B  = Logical unit number, 1 to 7
;         HL = Pointer to buffer in RAM.
;Output:  A = 0: Ok, buffer filled with information.
;             1: Error, device or logical unit not available,
;                or device index or logical unit number invalid.
;         On success, buffer filled with the following information:
;
;+0 (1): Medium type:
;        0: Block device
;        1: CD or DVD reader or recorder
;        2-254: Unused. Additional codes may be defined in the future.
;        255: Other
;+1 (2): Sector size, 0 if this information does not apply or is
;        not available.
;+3 (4): Total number of available sectors.
;        0 if this information does not apply or is not available.
;+7 (1): Flags:
;        bit 0: 1 if the medium is removable.
;        bit 1: 1 if the medium is read only. A medium that can dinamically
;               be write protected or write enabled is not considered
;               to be read-only.
;        bit 2: 1 if the LUN is a floppy disk drive.
;+8 (2): Number of cylinders
;+10 (1): Number of heads
;+11 (1): Number of sectors per track
;
; Number of cylinders, heads and sectors apply to hard disks only.
; For other types of device, these fields must be zero.

LUN_INFO:
	CALL	DEVICE_MAPPING
	CP	DEV_MAP_ROM
	JR	Z, LUN_INFO_ROM
	CP	DEV_MAP_CF
	JP	Z, LUN_INFO_CF
	CP	DEV_MAP_MS
	JP	Z, LUN_INFO_MS

LUN_INFO_ERR:
	LD	A, 1
	RET


;-----------------------------------------------------------------------------
; CODE FOR ROM DRIVER (PRIMARY BANK)
;-----------------------------------------------------------------------------

SEC_RAW_CPY	EQU	04100H

DEV_WRT_ROM:
	LD	A, ERR.WPROT
	LD	B, 0
	RET

DEV_READ_ROM:
	CALL	MY_GWORK

	LD	(IX+ST_RD_CNT), B
	LD	(IX+ST_RD_CND_RQ), B
	LD	A, (DE)			; RETRIEVE SECTOR NUMBER  (ASSUMING LESS THEN 255 FOR MOMENT)
	LD	(IX+ST_INDX_LW), A
	INC	DE
	LD	A, (DE)
	LD	(IX+ST_INDX_HI),A

	INC	DE
	LD	A, (DE)
	OR	A
	JR	Z, SKIP1

SKIP1:
	LD	(IX+ST_DEST_LW), L
	LD	(IX+ST_DEST_HI), H

DEV_RW_LP:
	CALL	SEC_CPY

	PUSH	HL
	LD	L, (IX+ST_INDX_LW)		; INCREMENT SECTOR IDX
	LD	H, (IX+ST_INDX_HI)
	INC	HL
	LD	(IX+ST_INDX_LW), L
	LD	(IX+ST_INDX_HI), H
	POP	HL
skip:

	LD	A, (IX+ST_RD_CNT)	; MORE SECTORS TO GO?
	DEC	A
	LD	(IX+ST_RD_CNT), A
	JR	NZ, DEV_RW_LP

	LD	B, (IX+ST_RD_CND_RQ)	; LOAD SECTORED COPIED COUNT
	XOR	A			; RETURN SUCCESS
	RET

SEC_CPY:
	PUSH	HL			; SAVE DEST ADDR
	LD	DE, SECCNT		; FIND THE BANK FOR SECTOR
	LD	C, (IX+ST_INDX_LW)
	LD	B, (IX+ST_INDX_HI)
	CALL	DIV16

	LD	A, C			; BANK ID TO A
	ADD	A, SECBNK
	LD	C, L			; SECTOR OFFSET TO C
	POP	HL			; RESTORE DEST ADDR
	;	A <= BANK FOR SECTOR
	;	C <= SECTOR RELATIVE OFFSET WITHIN BANK (SECTOR MOD SECCNT)

	LD	IX, SEC_RAW_CPY
	JP	CALBNK			; CONTINUE COPY IN OTHER BANK






LUN_INFO_ROM:
	LD	A, B
	CP	1
	JR	NZ, LUN_INFO_ROM_INVALID
	LD	A, 0			; standard block device
	LD	(HL), 0
	INC	HL

	LD	(HL), 0			; sector size 512
	INC	HL
	LD	(HL), 2
	INC	HL

	LD	(HL), SECTOT AND 255
	INC	HL
	LD	(HL), SECTOT / 256
	INC	HL
	LD	(HL), 0
	INC	HL
	LD	(HL), 0
	INC	HL

	LD	(HL), 2			; read only
	INC	HL

	LD	(HL), 0			; cyclinders
	INC	HL
	LD	(HL), 0
	INC	HL

	LD	(HL), 0			; heads
	INC	HL

	LD	(HL), 0			; sectors per track
	RET

LUN_INFO_ROM_INVALID:
	LD	A, 1
	RET

; Given requested device index in A, and the present of Compact Flash and MSX-MUSIC, Rom
; identify the mapping
;Input:   A  = Device index, 1 to 7
;         B  = Logical unit number, 1 to 7
;         HL = Pointer to buffer in RAM.
;Output:  A = 1 -> embedded ROM Disk
;	    = 2 -> Compact Flash
;	    = 3 -> MSX-MUSIC
;	    = 0 -> no drive present

DEVICE_MAPPING:
	CP	1
	JR	Z, DEVICE_INDX_1

	CP	2
	JR	Z, DEVICE_INDX_2

	CP	3
	JR	Z, DEVICE_INDX_3

DEVICE_NONE:
	XOR	A			; RETURN NO DRIVE MAPPED
	RET

DEVICE_INDX_1:
	CALL	DRV_GET_PRESENT
	BIT	BIT_PRES_CF, A
	JR	NZ, DEVICE_CF
	BIT	BIT_PRES_MS, A
	JR	NZ, DEVICE_MS

DEVICE_ROM:
	LD	A, DEV_MAP_ROM			; EMBEDDED ROM DRIVE
	RET

DEVICE_INDX_2:
	CALL	DRV_GET_PRESENT
	BIT	BIT_PRES_CF, A
	JR	Z, DEVICE_INDX_2_1
	BIT	BIT_PRES_MS, A
	JR	NZ, DEVICE_MS
	JR	DEVICE_ROM

DEVICE_INDX_2_1:
	AND	(PRES_CF | PRES_MS)
	JR	Z, DEVICE_NONE
	JR	DEVICE_ROM

DEVICE_INDX_3:
	CALL	DRV_GET_PRESENT
	AND	(PRES_CF | PRES_MS)
	CP	(PRES_CF | PRES_MS)
	JR	Z, DEVICE_ROM
	JR	DEVICE_NONE

DEVICE_CF:
	LD	A, DEV_MAP_CF
	RET

DEVICE_MS:
	LD	A, DEV_MAP_MS
	RET

;-----------------------------------------------------------------------------
;
; Print a zero-terminated string on screen
; Input: DE = String address

PRINT:
	ld	a,(de)
	or	a
	ret	z
	call	CHPUT
	inc	de
	jr	PRINT


;-----------------------------------------------------------------------------
;
; Obtain the work area address for the driver
; Input: A=1  to obtain the work area for the master, 2 for the slave
; Preserves A

MY_GWORK:
	PUSH	AF
	XOR	A
	EX	AF,AF'
	XOR	A
	LD	IX, GWORK
	CALL	CALBNK
	push	hl
	ld	l, (IX)
	ld	h, (IX+1)
	push	hl
	pop	ix
	pop	hl
	POP	AF
	RET

;-----------------------------------------------------------------------------
; SET NZ IF CF IS PRESENT

DRV_GET_PRESENT:
	PUSH	BC
	PUSH	DE
	PUSH	HL

	CALL	MY_GWORK
	LD	A, (IX+ST_PRESENT)

	POP	HL
	POP	DE
	POP	BC
	RET

; DIV16     [BC]=[BC]/[DE], remainder in HL.  Preserves DE, IX, IY.

DIV16:
	ld	hl,0
	ld	a,17
div_loop:
	rl	c
	rl	b
	dec	a
	ret	z
;
	adc	hl,hl
	jr	nc,fit_16bit
	or	a		;Remainder exceeds 16 bit
	sbc	hl,de		;This always generates carry
	jr	div_loop
fit_16bit:
	sbc	hl,de
	jr	nc,no_rest
	add	hl,de		;Restore since divisor didn't fit
no_rest:
	ccf			;Prepare quotient bit
	jr	div_loop

	include		cfdrv.asm
	include		musicdriver.asm
;-----------------------------------------------------------------------------
;
; End of the driver code

	include 	version.asm
			DB	"-------------------------------", 13, 10, 0

ROM_MAN:		DB	"Homebrew"
ROM_MAN_LEN		EQU	$-ROM_MAN

ROM_DRV_NAME:		DB	"Embedded ROM Disk"
ROM_DRV_NAME_LEN	EQU	$-ROM_DRV_NAME

CF_INFO:		DB	"Compact Flash Driver", 0
CF_INFO_LN		EQU	$-CF_INFO

MSX_MUSIC_NAME:		DB	"MSX-MUSIC ROM Disk"
MSX_MUSIC_NAME_LEN	EQU	$-MSX_MUSIC_NAME

; 4000h   +---------------------+
;         |     Page 0 code     |
;         +---------------------+
; 40FEh   |          K          |
;         +---------------------+
; 40FFh   |       Bank ID       |
; 4100h   +---------------------+
;         |                     |
;         |     Additional      |
;         |     driver code     |
;         |                     |
;         |                     |
; 7FD0h   +---------------------+
;         | Bank switching code |
; 7FFFh   +---------------------+

; SECTOR_INDX_START	EQU	07F00h
; SECTORS			EQU	05F00h		; 8k of data (16 sectors)

; 	PUBLIC	SECSTRT, SECEND, DATSIZE
; DRV_END:

; SECSTRT:
; 	DS	SECCNT * SECSIZ, 0
; SECEND:
; DATSIZE	EQU	SECCNT * SECSIZ

	; EXPORT	DRVEND
; DRVEND:
	end
