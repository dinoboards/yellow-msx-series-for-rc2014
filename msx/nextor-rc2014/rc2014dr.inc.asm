
	include	embinc.asm

	STRUCT endpoint_param
BASE:					; Offset to the base of the data structure
number		db
toggle		db
max_packet_size	db
	ENDS

	STRUCT usb_work_area
BASE:					; Offset to the base of the data structure
bulk_out_endpoint	endpoint_param
bulk_in_endpoint	endpoint_param
intr_in_endpoint	endpoint_param
usb_device		db
max_packet_size		db
bConfigurationvalue	db
interface_number	db
	ENDS

	STRUCT ST_WRKAREA
RD_CNT		DB	0	; COUNT OF SECTORS TO BE READ
INDX_LW		DB	1	; SECTOR TO BE READ
INDX_HI		DB	2	; SECTOR TO BE READ 	; MUST BE LESS THAN 65536
DEST_LW		DB	3	; SECTOR DESTINATION ADDRESS (LOW)
DEST_HI		DB	4	; (HIGH)
RD_CND_RQ	DB	5	; NUMBER OF SECTORS REQUESTED
PRESENT		DB	6	; BIT FIELD FOR DETECTED DEVICES (BIT 0 -> COMPACTFLASH/IDE, BIT 1-> MSX-MUSIC NOR FLASH)
USB		usb_work_area
	ENDS

	DISPLAY "WRKAREA bytes: ",/D,ST_WRKAREA

_usb_host_init:		EQU		$5800	; entry point within cextens section
_usb_dev_info:		EQU  	        $5803
_usb_lun_info:		EQU		$5806
_usb_dev_status:	EQU		$5809
_usb_dev_read:		EQU		$580C
_usb_dev_write:		EQU		$580F
