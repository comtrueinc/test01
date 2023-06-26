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

void SerialInDone(void)
{
    // clear all Uart parameters
    BYTE i;
    WORD delay = 100;

    UART_Buffer_Size = 0;
    UART_Input_First = 0;
    for(i=0; i<UART_BUFFERSIZE; i++)
        UART_Buffer[i] = 0;

    UART_TimeOut_Flag = 0;          // Finish, stop count
    UART_TimeOut_Count = 0;
}

void SerialDataIn(void)
{
    BYTE		i,temp=0xff,ret_len, buffer_len;
    BYTE		serial_len, serial_cmd, buffer[16]={0},ret_data[16]={0};
	WORD		addr;
//	BYTE  		out_data[16]={0};

	//for Comtrue test
    buffer_len =  (UART_Buffer_Size>0)?(UART_Buffer[0]+2):0;
    if (buffer_len>0 && UART_Buffer_Size==buffer_len && buffer_len<=16)
    {
        serial_len = UART_Buffer[0];
        serial_cmd = UART_Buffer[1];
        for(i=0;i<(buffer_len-2);i++)
        {
            buffer[i] =  UART_Buffer[2+i];
        }
		addr = BYTE_TO_WORD(buffer[0],buffer[1]);
		//addr = ((WORD)buffer[0])*256 + ((WORD)buffer[1]);
		//uart_timer_count = 0;
        switch (serial_cmd)											  
        {
        		
            case 0x00:
//                printf("test command...\r\n",0);
                break;
			case 0x02:			  
                printf("P1:%x, ",P1);
                printf("P2:%x, ",P2);
                printf("RCAP2H:%x, ",RCAP2H);
                printf("RCAP2L:%x, ",RCAP2L);
                printf("PCON:%x\r\n",PCON);
				break;
			case 0x10:
				//P2 = buffer[0];
				break;
			case 0x12:
				break;
            case 0x20:
                MI2CWriteData(buffer[0], buffer[1], 1, &buffer[2]);
                break;
            case 0x21:
				ret_len = buffer[2];
                MI2CReadData(buffer[0], buffer[1], 1, ret_data);
                SerialDataOut(ret_len, ret_data);
                break;
			case 0x50:
				McuWriteReg(buffer[0],buffer[1]);
				break;
			case 0x51:
				ret_len = buffer[1];
				for(i=0;i<ret_len;i++)
				{
                	ret_data[i]=McuReadReg(buffer[0]+i);
				}
				SerialDataOut(ret_len, ret_data); 
				break;
 			case 0x60:
				McuWriteMem(addr,serial_len-2,(buffer+2));
				break;
			case 0x61:
				ret_len = buffer[2];
				for(i=0;i<ret_len;i++)
				{
                	ret_data[i]=McuReadMemS(addr+i);
				} 
				SerialDataOut(ret_len, ret_data); 
			case 0x62:
				ret_len = buffer[2];
				for(i=0;i<ret_len;i++)
				{
                	ret_data[i]=McuReadMemS(addr+i);
				} 
				for(i=0;i<ret_len;i++)
				{
					printf("%x,",ret_data[i]);
					if(i%16==15) print_str("\r\n");
				}
				break;
			case 0x64:
				ret_len = buffer[2];
                McuReadMem(addr,ret_len,ret_data);
				for(i=0;i<ret_len;i++)
				{
					printf("%x,",ret_data[i]);
					if(i%16==15) print_str("\r\n");
				}
				break;
			case 0x88:
				g_debug_level = buffer[0]; 
                printf("debug level= %x...\r\n",g_debug_level);
				break;
			case 0x90:
				HidSetStatus(buffer[0]);
                printf("value=%x\r\n",buffer[0]);
				break;
			case 0x92:
				temp = HidSendOK();
                printf("ok=%x\r\n",temp);
				break;
            case 0xC0:
                MI2CReadData(buffer[0], buffer[1], buffer[2], ret_data);
 				for(i=0;i<buffer[2];i++)
				{
                	printf("%x,",ret_data[i]);
				}
                break;
            case 0xE0:
                SetGpioPortAOut(0,0);
                break;
            
            default:
                break;
        }
        SerialInDone();
    }
    if(UART_TimeOut_Count>= 6)      // wait for about 4.8ms
        SerialInDone();
}
*/
//-----------------------------------------------------------------------------
// End Of File
//-----------------------------------------------------------------------------
