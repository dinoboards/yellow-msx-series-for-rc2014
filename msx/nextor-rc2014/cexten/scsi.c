#include "scsi.h"

_scsi_command_block_wrapper scsi_command_block_wrapper = {{0x55, 0x53, 0x42, 0x43}, {0, 0, 0, 0}, 0, 0, 0, 0};

_scsi_packet_inquiry scsi_packet_inquiry = {0x12, 0, 0, 0, 0x24, 0, {0, 0, 0, 0, 0, 0}};