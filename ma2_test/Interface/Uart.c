//#include <intrins.h>
#include "Reg51.h"
#include "Mcu.h"
#include "Uart.h"
#include "mi2c.h"

void put_char(char ch)
{
//    BYTE i;
    EA=0;
    SBUF = ch;   // transfer to uart

    while(!TI);         // wait transfer completed
    	TI = 0;         // clear flag
    EA=1;
}
//////////////////////////////////////////////////////////////
// Put hexadecimal byte to uart
//
// Arguments: ucVal - output character
//////////////////////////////////////////////////////////////
void put_hex(BYTE value)
{
    char digi[2];

    digi[1] = (value>>4)&0x0F;
    digi[0] = value&0x0F;

    if(digi[1]>=10)
        put_char(digi[1] - 10 + 'A');
    else
        put_char(digi[1] + '0');

    if(digi[0]>=10)
        put_char(digi[0] - 10 + 'A');
    else
        put_char(digi[0] + '0');
}

//////////////////////////////////////////////////////////////
// Put integer to uart
//
// Arguments: ucVal - output character
//////////////////////////////////////////////////////////////
void put_int(BYTE value)
{
    char digi[3];

    digi[2] = value/100;
    digi[1] = (value - digi[2]*100)/10;
    digi[0] = (value - digi[2]*100 - digi[1]*10);

    if(value>=100)
        put_char(digi[2] + '0');
    if(value>=10)
        put_char(digi[1] + '0');

    put_char(digi[0] + '0');

}
//////////////////////////////////////////////////////////////////////////////
// Put string to uart with variable argument
//
// Arguments: pFmt - string address
//            wVal - print variable
//////////////////////////////////////////////////////////////////////////////
void printf(char *p_formate, BYTE value)
{
    BYTE ch_buf;                 // character buffer


    while (1)
    {
        ch_buf = *p_formate;   // get a character
        if (ch_buf == '\0')   // check end of string
            break;

        if (ch_buf == '%')              // check special case
        {
            switch(*(++p_formate))      // check next character
            {
                case 'x':                 // hexadecimal number
                case 'X':
                    put_hex(value);
                    break;
                case 'd': // decimal number
                case 'i':
                    put_int(value);
                    break;
                default:
                    break;
            } // switch
        }
        else // general
            put_char(ch_buf); // put a character

        p_formate++; // next
    } // while
//    UART_Buffer_Size = 0;
//    UART_Input_First = 0;
//    UART_Buffer[0] = 0;
}
/*
//////////////////////////////////////////////////////////////////////////////
// Put string to uart with variable argument
//
// Arguments: pFmt - string address
//            wVal - print variable
//////////////////////////////////////////////////////////////////////////////
void print_str(char *p_formate)
{
	printf(p_formate,0);
}
//////////////////////////////////////////////////////////////////////////////
// Put string to uart 
//
// Arguments: pFmt - string address
//            
//////////////////////////////////////////////////////////////////////////////
void SerialDataOut(unsigned char count, char *p_formate)
{
    char i, ch_buf;


    for(i=0; i<count; i++)
    {
        ch_buf = *p_formate;   // get a character

        put_char(ch_buf); // put a character

        p_formate++; // next
    }
}
*/
#if(UART_RX_SUPPORT)
void SerialInDone(void)
{
    // clear all Uart parameters
    BYTE i;
    WORD delay = 100;

    UART_Buffer_Index = 0;
    for(i=0; i<UART_BUFFERSIZE; i++)
        UART_Buffer[i] = 0;

    UART_TimeOut_Flag = 0;          // Finish, stop count
    UART_TimeOut_Count = 0;
}

void SerialDataIn(void)
{
    BYTE i;

    if(UART_TimeOut_Count>= 6)      // wait for about 4.8ms
    {
        for(i=0; i<UART_Buffer_Index; i++)
        {
            put_char(UART_Buffer[i]);
        }
        SerialInDone();
    }
}
#endif            
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
