#ifndef _CTUSB_HID_H_
#define _CTUSB_HID_H_

#define	    ENDPOINT_HID_OUT	5
#define	    ENDPOINT_HID_IN		6

#define     HID_CMD_REG_WRITE           0xA0
#define     HID_CMD_REG_READ            0xA2
#define     HID_CMD_RAM_WRITE           0xA8
#define     HID_CMD_RAM_READ            0xAA
#define     HID_CMD_I2C_WRITE           0xB0
#define     HID_CMD_I2C_READ            0xB2
#define     HID_CMD_SPI_WRITE           0xB8
#define     HID_CMD_SPI_READ            0xBA

#define     HID_CMD_FLASH_WRITE         0xF0
#define     HID_CMD_FLASH_READ          0xF2
#define     HID_CMD_FW_RESET            0xFE


typedef struct _HID_REPORT_INFO_
{
    BYTE                id;
    BYTE                header_len;
    BYTE                vendor;
    BYTE                command;
    BYTE                cmd_len;
    BYTE                data_len;
    BYTE                reseverd[2];
    BYTE                buffer[56];
}HID_REPORT_INFO, *PHID_REPORT_INFO;	

//	------------------------------------
//		Function Prototypes
//	------------------------------------
enum
{
    STATE_HID_IDLE          =0x00,
    STATE_HID_WAIT_TRIGGER  =0x01,
    STATE_HID_SET_STATUS    =0x02,
    STATE_HID_WAIT_SEND     =0x03,
    STATE_HID_SET_NORMAL    =0x04,
    STATE_HID_WAIT_FINISH   =0x05,
};

enum
{
    HID_USAGE_NONE          =0x00,
    HID_USAGE_VOL_UP        =0x01,
    HID_USAGE_VOL_DOWN      =0x02,
    HID_UASGE_MUTE          =0x04,
    HID_USAGE_STOP          =0x08,
    HID_USAGE_PLAY          =0x10,
    HID_USAGE_NEXT          =0x20,
    HID_USAGE_PREV          =0x40,
};

enum
{
    STATE_JD_IDLE           =0x00,
    STATE_JD_WAIT_TRIGGER   =0x01,
    STATE_JD_SET_STATUS     =0x02,
    STATE_JD_WAIT_SEND      =0x03,
 };

typedef struct _HID_GOLBAL_INFO
{
    BYTE    flags;
    BYTE    hid_state;
    BYTE    hid_usage;
} HID_GOLBAL_INFO, *PHID_GOLBAL_INFO;	
//	------------------------------------
//		Function Prototypes
//	------------------------------------
void HidInit(void);
void HidInSetStatus(BYTE value);
BYTE HidInSendOK(void);
BYTE HidFeatureParse(void);

void JackInit(void);
void JackSetChange(BYTE id);
BYTE JackInSendOK(void);
void JackUpdateStatus(BYTE status);
#endif  //__CTUSB_HID_H__
