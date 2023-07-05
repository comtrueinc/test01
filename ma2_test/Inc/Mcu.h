//-----------------------------------------------------------------------------
// mcu.h
//-----------------------------------------------------------------------------
// Copyright 2016 Comtrue Inc.
//
// Program Description:
//
// Source file for USB firmware.
//

#ifndef _CTUSB_MCU_H_
#define _CTUSB_MCU_H_

//#include "Reg51.h"
#include "Global.h"

#define	SYSTEM_CLOCK            12000000    //12000000
#define BAUDRATE                115200      //57600 

#define GPIO_VALID_COUNT			25
#define GPIO_CONTINUE_MIN_COUNT		200

#define TIMER0_1MS_TICK_COUNT       (SYSTEM_CLOCK/12/1000)
#define TIMER0_500US_TICK_COUNT     ((SYSTEM_CLOCK/12/1000)/2)
#define TIMER0_125US_TICK_COUNT     ((SYSTEM_CLOCK/12/1000)/8)
#define TIMER0_100US_TICK_COUNT     ((SYSTEM_CLOCK/12/1000)/10)
#define TIMER0_50S_TICK_COUNT       ((SYSTEM_CLOCK/12/1000)/20)

#define TIMER0_1MS_VALUE	(65536-TIMER0_1MS_TICK_COUNT)
#define TIMER0_500US_VALUE	(65536-TIMER0_500US_TICK_COUNT)	
#define TIMER0_125US_VALUE	(65536-TIMER0_125US_TICK_COUNT)	
#define TIMER0_100US_VALUE	(65536-TIMER0_100US_TICK_COUNT)	
#define TIMER0_50US_VALUE	(65536-TIMER0_50S_TICK_COUNT)	

#define INTERRUPT_INT0              0    // External Interrupt 0
#define INTERRUPT_TIMER0            1    // Timer0 Overflow
#define INTERRUPT_INT1              2    // External Interrupt 1
#define INTERRUPT_TIMER1            3    // Timer1 Overflow
#define INTERRUPT_UART0             4    // Serial Port 0
#define INTERRUPT_TIMER2            5    // Timer2 Overflow

void    Timer0_Init(void);
void    Timer0_ISR(void);
void    Uart_Initial(void);
void    ExternalInt_Init(void);
void    Mcu_Initial(void);

BYTE    McuReadReg(BYTE reg);
void    McuWriteReg(BYTE reg, BYTE value);
void    McuWriteRegs(BYTE reg, BYTE len, BYTE *buff);
void    McuWriteRegMask(BYTE reg, BYTE value, BYTE mask);
BYTE    McuReadMemS(WORD addr);
WORD    McuReadMem2(WORD addr);
DWORD   McuReadMem4(WORD addr);
void    McuReadMem(WORD addr, BYTE len, BYTE *buffer);
void    McuWriteMemS(WORD addr, BYTE value);
void    McuWriteMem2(WORD addr, WORD value);
void    McuWriteMem4(WORD addr, DWORD value);
void    McuWriteMem(WORD addr, BYTE len, BYTE *buff);
void    Delay100us(WORD count);

#endif	//_CTUSB_MCU_H_
