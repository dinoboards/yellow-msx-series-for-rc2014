
	include	embinc.asm

	STRUCT endpoint_paramx
BASE:					; Offset to the base of the data structure
number_toggle			db
max_packet_size			db
	ENDS

	STRUCT device_configx
BASE:
type				db
max_packet_size			db
interface_number		db
tag				db
address				db
bulk_out_endpoint		endpoint_paramx
bulk_in_endpoint		endpoint_paramx
intr_in_endpoint		endpoint_paramx
	ENDS

	STRUCT usb_work_areax
BASE:					; Offset to the base of the data structure
storage_device1			device_configx
storage_device2			device_configx
storage_device3			device_configx
storage_device4			device_configx
hub_config			device_configx
cdc_config			device_configx
printer_config			device_configx
next_storage_device_index	db
printer_time_throttle_flag	db
	ENDS

	STRUCT ST_WRKAREA
RD_CNT		DB	0	; COUNT OF SECTORS TO BE READ
INDX_LW		DB	1	; SECTOR TO BE READ
INDX_HI		DB	2	; SECTOR TO BE READ 	; MUST BE LESS THAN 65536
DEST_LW		DB	3	; SECTOR DESTINATION ADDRESS (LOW)
DEST_HI		DB	4	; (HIGH)
RD_CND_RQ	DB	5	; NUMBER OF SECTORS REQUESTED
PRESENT		DB	6	; BIT FIELD FOR DETECTED DEVICES (BIT 0 -> COMPACTFLASH/IDE, BIT 1-> MSX-MUSIC NOR FLASH)
USB		usb_work_areax
		; DS	16	; reserve

	ENDS

	DISPLAY "WRKAREA bytes: ",/D,ST_WRKAREA

DRIVER_ASM_END			EQU		$5000
_usb_host_init:			EQU		DRIVER_ASM_END	; entry point within cextens section
_usb_dev_info:			EQU  	        DRIVER_ASM_END+3
_usb_lun_info:			EQU		DRIVER_ASM_END+6
_usb_dev_status:		EQU		DRIVER_ASM_END+9
_usb_dev_read:			EQU		DRIVER_ASM_END+12
_usb_dev_write:			EQU		DRIVER_ASM_END+15
_fn_enum:			EQU		DRIVER_ASM_END+18
_get_number_of_usb_drives:	EQU		DRIVER_ASM_END+21
_drv_direct4:			EQU		DRIVER_ASM_END+24
