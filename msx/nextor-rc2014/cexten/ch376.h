#ifndef __CH376
#define __CH376

#define CH_CMD_GET_IC_VER   0x01
#define CH_CMD_ENTER_SLEEP        0x03

#define CH_CMD_SET_SPEED    0x04
#define CH_CMD_RESET_ALL    0x05
#define CH_CMD_CHECK_EXIST  0x06
#define CH_CMD_SET_RETRY    0x0B
#define CH_CMD_SET_USB_ADDR 0x13
#define CH_CMD_SET_USB_MODE 0x15
#define CH_CMD_GET_STATUS   0x22
#define	CH375_CMD_UNLOCK_USB 0x23
#define CH_CMD_RD_USB_DATA0 0x27
#define CH375_CMD_RD_USB_DATA_UNLOCK     0x28
#define CH_CMD_WR_HOST_DATA 0x2C
#define CH_CMD_CLR_STALL    0x41
#define CH_CMD_ISSUE_TKN_X  0x4E

#define CH_SPEED_LOW  2
#define CH_SPEED_HIGH  0


#define CH_ENTER_SLEEP 3
#define CH_MODE_HOST_RESET 7
#define CH_MODE_HOST       6

// return codes
#define CH_ST_RET_SUCCESS 0x51
#define CH_ST_RET_ABORT   0x5F

// CH376 result codes
#define CH_USB_INT_SUCCESS    0x14
#define CH_USB_ERR_FOUND_NAME 0x43

#define USB_STALL 0x2e

typedef enum _ch376_pid { CH_PID_SETUP = 0x0D, CH_PID_IN = 0x09, CH_PID_OUT = 0x01 } ch376_pid;

#endif
