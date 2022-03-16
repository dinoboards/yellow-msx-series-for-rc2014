

typedef struct __scsi_command_status_wrapper {
  uint8_t cbwsignature[4];
  uint8_t cbwtag[4];
  uint8_t cbwresidue[4];
  uint8_t cbwstatus;
} _scsi_command_status_wrapper;

// 	STRUCT WRKAREA
// BASE:					; Offset to the base of the data structure
// STATUS:			db 		; bit 0 = CH376s present, bit 1 = initialised, bit 2 = USB device present, bit 3 = USB device mounted, bit 5 = DSK changed, bit 7 = Flash disk present

typedef struct __usb_device_info {
  uint8_t device_address;
  uint8_t interface_id;
  uint8_t config_id;
  uint8_t max_packet_size;
  uint8_t data_bulk_in_endpoint_id;
  uint8_t data_bulk_out_endpoint_id;
  uint8_t data_bulk_in_endpoint_toggle;
  uint8_t data_bulk_out_endpoint_toggle;
} _usb_device_info;

typedef struct __search_device_info {
  uint8_t num_configs;
  uint8_t num_interfaces;
  uint8_t num_endpoints;
  uint8_t wanted_class;
  uint8_t wanted_sub_class;
  uint8_t wanted_protocol;
} _search_device_info;

typedef struct __hub_device_info {
  uint8_t hub_ports;
  uint8_t hub_port_status[4];
} _hub_device_info;

typedef struct __scsi_devinfo {
  uint8_t tag;
  uint8_t max_luns;
  uint8_t buffer[0x24];       // longest response (inquiry) we want to absorb during init
  uint8_t csw[sizeof(_scsi_command_status_wrapper)];//			ds	_scsi_command_status_wrapper
  uint8_t vendorid[8];
	uint8_t ignored1;
  uint8_t productid[16];
	uint8_t ignored2;
  uint8_t productrev[8];
  uint8_t ignored3;
} _scsi_devinfo;

typedef uint8_t usb_descriptor_block[8];

typedef struct __usb_descriptor_blocks {
  usb_descriptor_block  cmd_get_device_descriptor;
  usb_descriptor_block  cmd_set_address;
  usb_descriptor_block  cmd_set_configuration;
  usb_descriptor_block  cmd_get_string;
  usb_descriptor_block  cmd_get_config_descriptor;
  usb_descriptor_block  cmd_set_interface;
  usb_descriptor_block  cmd_set_packet_filter; 
  usb_descriptor_block  cmd_set_idle; 
  usb_descriptor_block  cmd_set_protocol;
  usb_descriptor_block  reserved[3];
  usb_descriptor_block  cmd_get_hub_descriptor; 
  usb_descriptor_block  cmd_set_hub_port_feature;
  usb_descriptor_block  cmd_get_hub_port_status;
  usb_descriptor_block  cmd_get_max_luns; 
  usb_descriptor_block  cmd_mass_storage_reset;
} _usb_descriptor_blocks;

typedef struct _ch376_work_area {
  uint8_t               present : 1;
  uint8_t               max_device_address;
  _usb_device_info      usb_device_info;
  _usb_device_info      storage_device_info;
  _search_device_info   search_device_info;
  _hub_device_info		  hub_device_info;
  _scsi_devinfo         scsi_device_info;
  uint8_t               usb_descriptor[140];    // memory area to hold the usb device+config descriptor of the current interrogated device

  _usb_descriptor_blocks usb_descriptor_blocks;

  // usb_descriptor_block  cmd_get_device_descriptor;
  // usb_descriptor_block  cmd_set_address;

// USB_DESCRIPTORS		ds	USB_DESCRIPTORS_END - USB_DESCRIPTORS_START ; **

// NXT_DIRECT		ds	NXT_DIRECT_END - NXT_DIRECT_START
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
