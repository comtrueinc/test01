#ifndef _CTUSB_GPIO_H_
#define _CTUSB_GPIO_H_

// --------------------------------------------------------------
// GPIO usage configuration
// --------------------------------------------------------------
// PORT A
#define GPIOA_INPUT_TEST1_PIN                   0
#define GPIOA_INPUT_TEST2_PIN                   BIT2              

#define GPIOA_OUTPUT_TEST1_PIN                  BIT5
#define GPIOA_OUTPUT_TEST2_PIN                  0

#define GPIOA_INPUT_PIN_USAGE                   (GPIOA_INPUT_TEST1_PIN | GPIOA_INPUT_TEST2_PIN)
#define GPIOA_OUTPUT_PIN_USAGE                  (GPIOA_OUTPUT_TEST1_PIN | GPIOA_OUTPUT_TEST2_PIN)

// PORT B
#define GPIOB_INPUT_HID_VOL_UP                  BIT4            // GPIOB4
#define GPIOB_INPUT_HID_VOL_DOWN                BIT1            // GPIOB1
#define GPIOB_INPUT_HID_START                   BIT2            // GPIOB2
#define GPIOB_INPUT_HID_MUTE                    0               // useless
#define GPIOB_INPUT_HID_STOP                    0               // useless

#define GPIOB_OUTPUT_TEST1_PIN                  BIT3

#define GPIOB_INPUT_PIN_USAGE                   (GPIOB_INPUT_HID_VOL_UP|GPIOB_INPUT_HID_VOL_DOWN|GPIOB_INPUT_HID_START)
#define GPIOB_OUTPUT_PIN_USAGE                  (GPIOB_OUTPUT_TEST1_PIN)

#define GPIO_KEYS_DEBOUND_MAX                   20

typedef struct _GPIO_GOLBAL_INFO
{
    //BYTE    flags;
    BYTE    in_status;
    BYTE    in_count;
    BYTE    hid_status;
} GPIO_GOLBAL_INFO, *PGPIO_GOLBAL_INFO;	

#ifdef	_CTUSB_GPIO_C_
GPIO_GOLBAL_INFO gpio={0};
#else
extern GPIO_GOLBAL_INFO gpio;
#endif

void GpioInit(void);
void GpioMonitor(void);

BYTE GpioToHidStatus(BYTE status);

void InitGpioPortA(BYTE function, BYTE direction);
void SetGpioPortAOut(BYTE abit, BYTE level);
void InitGpioPortB(BYTE direction);
void SetGpioPortBOut(BYTE abit, BYTE level);
BYTE GpioToUsage(BYTE status);
#endif
