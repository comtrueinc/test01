#ifndef __CTUSB_UART_H__
#define __CTUSB_UART_H__

#define     UART_RX_SUPPORT                     0
//	------------------------------------
//		Function Prototypes
//	------------------------------------
void printf(char *p_formate, unsigned char value);
void print_str(char *p_formate);
void put_char(char ch) ;
void SerialInDone(void);
void SerialDataIn(void);

#endif  //__CTUSB_UART_H__
