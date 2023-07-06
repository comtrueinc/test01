#include "Reg51.h"
#include "Mcu.h"
#include "Settings.h"
#include "Uart.h"
#include "Hid.h"

#define _CTUSB_GPIO_C_
#include "Gpio.h"
#undef  _CTUSB_GPIO_C_

#define  PRINT_GPIO_COMMON              1

#if (PRINT_GPIO_COMMON)
#define DBGPRINT_GPIO_COMMON(_FMT_,_X_)          printf(_FMT_,_X_)
#else
#define DBGPRINT_GPIO_COMMON(_FMT_,_X_)         
#endif

void GpioInit(void)
{
    BYTE old_bank;           
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MISC1);
    // Default GPIO On: A5/A2(0x24) B4-B0(0x1F)
    McuWriteReg(0x08,0x24);
//    McuWriteReg(0x10,0x1F);
//  I2c Slave(1/0), I2c Master (4/3), pwm1/pwm0 (5/2), UART (7/6)
    McuWriteReg(0x10,0x24);
    // Default Input
    McuWriteReg(0x0A,0xFF);
    McuWriteReg(0x12,0xFF);
    McuWriteReg(0x00,old_bank);
}

/*
void GpioInit(void)
{
    BYTE old_bank;

    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00, BANK_MISC1);

    // Config Port A Function
    // 0: normal function,    1: gpio
    McuWriteReg(0x08, (GPIOA_INPUT_PIN_USAGE|GPIOA_OUTPUT_PIN_USAGE));
    // GPIO A Direction
    // 0: output,    1: input
    McuWriteReg(0x0A, ~GPIOA_OUTPUT_PIN_USAGE);

    // Config Port B Function
    // 0: normal function,    1: gpio
    McuWriteReg(0x10,(GPIOB_INPUT_PIN_USAGE|GPIOB_OUTPUT_PIN_USAGE));
    // GPIO B Direction
    // 0: output,    1: input
    McuWriteReg(0x12,~GPIOB_OUTPUT_PIN_USAGE);

    McuWriteReg(0x00,old_bank);
}
*/
#if (HID_GPIO_KEY)
BYTE GpioToHidStatus(BYTE status)
{
    BYTE ret_status;
    ret_status=0;
    if((status&GPIOB_INPUT_HID_VOL_UP))
        ret_status |= BIT0;
    if((status&GPIOB_INPUT_HID_VOL_DOWN))
        ret_status |= BIT1;
    if((status&GPIOB_INPUT_HID_START))
        ret_status |= BIT2;
    return ret_status;
}

void GpioMonitor(void)
{
	BYTE old_bank;
    BYTE curr_status;
    //WORD curr_gpio_status;

	old_bank = McuReadReg(0x00);
    McuWriteReg(0x00, BANK_MISC1);

	curr_status  = ~(McuReadReg(0x13));    // 0:press 1:release
    curr_status &= GPIOB_INPUT_PIN_USAGE;
    //if(curr_status) printf("curr_status=%x\r\n",curr_status);
	//curr_gpio_status = ((McuReadReg(0x13) & GPIOB_INPUT_PIN_USAGE)<<8);
	//curr_gpio_status |= (McuReadReg(0x0B) & GPIOA_INPUT_PIN_USAGE);
    //if(curr_status) 
    //    printf("curr_status=%x\r\n",curr_status);

	if(curr_status != gpio.in_status)
	{
        gpio.in_count=0;
        gpio.in_status = curr_status;
	}
	else if(gpio.in_count<255)
	{
        gpio.in_count++;
	}
	if(gpio.in_count==GPIO_KEYS_DEBOUND_MAX && curr_status)
	{
        hid.status=GpioToHidStatus(curr_status); 
        DBGPRINT_GPIO_COMMON("hid.status=%x\r\n", hid.status);
	}

    McuWriteReg(0x00, old_bank);
}
#endif