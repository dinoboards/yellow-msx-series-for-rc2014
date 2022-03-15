

// 	STRUCT WRKAREA
// BASE:					; Offset to the base of the data structure
// STATUS:			db 		; bit 0 = CH376s present, bit 1 = initialised, bit 2 = USB device present, bit 3 = USB device mounted, bit 5 = DSK changed, bit 7 = Flash disk present

typedef struct _ch376_work_area {
  // uint8_t status;
  uint8_t present : 1;
} ch376_work_area;

typedef struct _work_area {
  uint8_t   read_count;             // COUNT OF SECTORS TO BE READ
  uint16_t  index;                  // sector number to be read
  uint8_t*  dest;                   // destination write address
  uint8_t   read_count_requested;   // number of sectors requested
  uint8_t   present;                // BIT FIELD FOR DETECTED DEVICES (BIT 0 -> COMPACTFLASH/IDE, BIT 1-> MSX-MUSIC NOR FLASH)
  ch376_work_area   ch376;
} work_area;

extern work_area* get_work_area();
