#ifndef _CTUSB_Gpio_H_
#define _CTUSB_Gpio_H_
#include "Hid.h"
/*
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
*/
typedef struct _GPIO_GOLBAL_INFO
{
    BYTE    flags;
    BYTE    hid_state;
    BYTE    hid_usage;
    BYTE    in_status;
    BYTE    in_count;
} GPIO_GOLBAL_INFO, *PGPIO_GOLBAL_INFO;	

#ifdef	_CTUSB_GPIO_C_
GPIO_GOLBAL_INFO gpio={0};
#else
extern GPIO_GOLBAL_INFO gpio;
#endif

void GpioInit(void);
void GpioMonitor(void);

void InitGpioPortA(BYTE function, BYTE direction);
void SetGpioPortAOut(BYTE abit, BYTE level);
void InitGpioPortB(BYTE direction);
void SetGpioPortBOut(BYTE abit, BYTE level);
BYTE GpioToUsage(BYTE status);
#endif
