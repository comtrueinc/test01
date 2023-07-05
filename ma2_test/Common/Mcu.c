#define __CTUSB_MCU_C__
#include "Reg51.h"
#include "Mcu.h"
#include "Uart.h"
#undef  __CTUSB_MCU_C__ 

#include "Entity.h"
#include "Audio.h"
#if(SUPPORT_IAP2)
#include "iap2.h"
#endif
//-----------------------------------------------------------------------------
// Global Variables
//-----------------------------------------------------------------------------
#define USB_MEM_START               ((unsigned char volatile xdata *) 0x0000)
#define USB_MEM_ADDR_LEN            (USB_DATA_MAX_ADDR)

static BYTE rx_data;

void ExternalInt_Init(void)
{
    // External int0
    IT0 = 0;    // 0:low-level 1:falling edge trigger
    IE0 = 0;	// clear int0 flag
    EX0 = 1;    // enable int0

    // External int1
    IT1 = 0;    // 0:low-level 1:falling edge trigger
    IE1 = 0;    // clear flag
    EX1 = 0;    // disable
}

void Timer0_Init(void)  //Initialize TIMER0
{

//    IE |= (ET2<<5)|(ET1<<3)|(ET0<<1); //IE=IEN0
//    ET0 = 0;
    TMOD = ((TMOD & 0xF0) | 0x01);  // Timer0 16 bits mode
    TH0 = TIMER0_100US_VALUE/256;	//MODE1 16bit 65536
    TL0 = TIMER0_100US_VALUE%256;
	ET0 = 1;
    TR0 = 1; //Timer0 Start bit
}

void Uart_Initial(void)
{
	WORD	temp;
 #if (UART_RX_SUPPORT)   
    BYTE old_bank;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MISC);
    // Enable Uart RX channel
    McuWriteRegMask(0x06,0x01, 0x01);
    McuWriteReg(0x00,old_bank);
 #endif   
    ES      = 0;                    				//Disable Serial interrupt
    //desker++ 211004 for new MCU design

    SCON   = 0x50;
    PCON  |= 0x40;    // enable ZR UART Mode
	temp = (WORD) ((SYSTEM_CLOCK/2+BAUDRATE/2)/(BAUDRATE));         //for modified 8051          
    THX    = HIBYTE((0xFFFF-temp+1));
    TLX    = LOBYTE((0xFFFF-temp+1));

    UART_TimeOut_Flag = 0;
    UART_TimeOut_Count = 0;
    UART_Buffer_Index = 0;

    ES      = 1;

}
void Mcu_Initial(void)
{
    EA = 0;
    ExternalInt_Init();
    Timer0_Init();	
    Uart_Initial();

    McuWriteReg(0x00,BANK_MISC);
    McuWriteReg(0x1C,0x00);                     // mask HID
    McuWriteReg(0x1D,~((1<<5)|(1<<2)));         // 2/10:bulk out, 5/13:suspend,
    McuWriteReg(0x1E,0xFF);
	//hw_regs[0x1C]=0x00;                        // no mask for usb packet irq flags 0:valid 1:invalid
	//hw_regs[0x1D]=~(1<<5);                     // 5(+8=13):suspend
	//hw_regs[0x1E]=0xFF;                        // mask all

    EA = 1;
}

void EXT0_ISR (void) interrupt INTERRUPT_INT0
{

	BYTE 	old_bank,iface,alternate,endpoint,entity,index,selector,number;
    BYTE    status0,status1,status2;
    BYTE    bm,type;

    EX0 = 0;
	old_bank = hw_regs[0x00];
	
    hw_regs[0x00]=0x00;
    status0=hw_regs[0x10];
    status1=hw_regs[0x11];
    status2=hw_regs[0x12];

	hw_regs[0x10]=status0;         // clear irq flags
	hw_regs[0x11]=status1;         // clear irq flags
	hw_regs[0x12]=status2;         // clear irq flags
    
if(status0&0x40)                        //Set Addr
    {
        irq_flags   |=0x01;
    }
    if(status0&0x08)                        //Set Config
    {
        irq_flags   |=0x02;
#if(SUPPORT_IAP2)
        iap2.flags  |=IAP2_FLAG_PLUGIN;
//        iap2.test_timer = 0;
#endif
//        au.flags    |=0x01;
        //power.flags |=0x01;
    }
    if(status0&0x10)                        //Set/Clr Feature
    {
        irq_flags   |=0x04;
//        test_flags  |=0x80;
    }
    if(status0&0x20)                        //Set HID Idle(bRequest=0x0A)/Report(0x09)
    {
        irq_flags   |=0x08;
//        hid_flags   |=0x08;
    }
    if(status0&0x80)                        //Special Command
    {
        irq_flags   |=0x10;
    }

    if(status1&0x20)                        //Suspend mode
    {
        irq_flags   |=0x20;
//        power.flags |=0x02;
    }
    //if(status1&0x40)
    //{
    //    irq_flags   |=0x40;
    //}            
    if(status1&0x04)                        //Bulk out 
    {

#if(SUPPORT_IAP2)
/*
        int i,len,offset; 	    
        hw_regs[0x00]       = BANK_BULK;
        
        i=iap2.bulkout.count;
        len = hw_regs[0x0A];
        offset = BYTE_TO_WORD(McuReadReg(0x0D),McuReadReg(0x0C))-USB_BULK_OUT_DATA_ADDR;  

        if(i<4)
        {
        iap2.bulkout.len[i]             = len;
        iap2.bulkout.addr_offset[i]     = (i>0)?offset:0;
        }
*/
        iap2.bulkout.count++;
        iap2.flags          |=IAP2_FLAG_BULKOUT_READY;
        //iap2.bo_time[iap2.bulkout_count]=iap2.test_timer;
        //iap2.bulkout_count++;
/*
        if(iap2.bulkout.reset)
        {
            iap2.bulkout.reset = 0;
            iap2.bulkout.count = 0;
            //iap2.bulkout.addr_offset[0]=0;
        }
        j=iap2.bulkout.count;
        if(j>0)
        {
            iap2.bulkout.addr_offset[j]    = iap2.bulkout.addr_offset[j-1]+iap2.bulkout.len[j-1];
        }
        //read bulkout data and reset......
 	    hw_regs[0x00]       = BANK_BULK;
        iap2.bulkout.len[j] = hw_regs[0x0A];
        hw_regs[0x08]       = 0x01;    

        iap2.bulkout.count++;

        len = iap2.bulkout.len[j];
        offset = iap2.bulkout.addr_offset[j]; 
        for(i=0;i<len;i++)
            bulkout_data1[offset+i]=usb_bulkout_data[i];
*/
#endif

        irq_flags   |=0x40;
    }
 	
    // for USB Control Packet
    hw_regs[0x00]=BANK_EP0;
    bm          = hw_regs[0xF0];
    type        = hw_regs[0xF1];

    alternate   = hw_regs[0xF2];    //wValue.L
    selector    = hw_regs[0xF3];    //wValue.H
    iface       = hw_regs[0xF4];    //wIndex.L
    index       = hw_regs[0xF5];    //wIndex.H

    number      = hw_regs[0xF2];
    endpoint    = hw_regs[0xF4];    //for uac1 freq notification
    entity      = hw_regs[0xF5];

    au.entity = entity;
    au.iface  = iface;
    /*
    // for special command 
    if(irq_flags&0x10)
    {
        if(bm==0x21 && type==0x09)  //set report
        {
            hid_flags |= 1<<selector;   //selector 1:input, 2:output, 3:feature
        }
        else if(bm==0xA1 && type==0x01) //get report
        {
            hid_flags |= 1<<(selector+4);
        } 
    }
    //
    if(irq_flags&0x08)
    {
        if(type==0x09)                  //set report, (set idle:0x0A)
        {
            hid_flags |= 1<<selector;   //selector 1:input, 2:output, 3:feature
        }
    }
    // for set feature
    if(test_flags&0x80)
    {
        if(index>0 && index<5)
        {
            test_flags |= 1<<(index-1);
        }
    }
    */
    // for Interface IRQ (alternate setting)
    if(status0&0x04)
    {
        if(alternate==0)            // alterenate==0
        {
            irq_flags   |=0x80;
        }
        else
        {
            switch(iface)
            {
                case IF_PLAYBACK: // playback
                    //au.format.p0_alt = alternate;
                    au.stream[0].alt = alternate;
                    au.flags |= AUDIO_FLAGS_IFACE1;
                    break;
#if(RECORD_ENABLE==1)
                case IF_RECORD: // record
                    //au.format.r0_alt = alternate;
                    au.stream[1].alt = alternate;
                    au.flags |= AUDIO_FLAGS_IFACE2;
                    break;
#endif
                default:
                    break;
            }
        }

    } 
    // for Freq/FU IRQ                     
    if(status0&0x02)
    {
        //au.entity = entity;
        switch(entity)
        {
            case FU01_ID:
                if(selector==1)
                {
                    au.flags |= AUDIO_FLAGS_MUTE1;
                }
                else if (number==0)
                {
                    au.flags |= AUDIO_FLAGS_VOLUME10;   // for msater vol
                }
                else
                {
                    au.flags |= AUDIO_FLAGS_VOLUME10;   // for others
                }

                break;
            case FU02_ID:
                if(selector==1)
                {
                    au.flags |= AUDIO_FLAGS_MUTE2;
                }
                else if (number==0)
                {
                    au.flags |= AUDIO_FLAGS_VOLUME20;   // for master vol (ch0)
                }
                else if (number>=1 && number<=4)        // (ch1-ch4)
                {
                    au.flags |= AUDIO_FLAGS_VOLUME20;   // for others
                }
                break;
            case FU03_ID:
                if(selector==1)
                {
                    au.flags |= AUDIO_FLAGS_MUTE3;
                }
                else if (number==0)
                {
                    au.flags |= AUDIO_FLAGS_VOLUME30;   // for master vol (ch0)
                }
                else if (number>=1 && number<=4)        // (ch1-ch4)
                {
                    au.flags |= AUDIO_FLAGS_VOLUME30;   // for others
                }
                break;
            case FU04_ID:
                if(selector==1)
                {
                    au.flags |= AUDIO_FLAGS_MUTE4;
                }
                else if (number==0)
                {
                    au.flags |= AUDIO_FLAGS_VOLUME40;   // for master vol (ch0)
                }
                else if (number>=1 && number<=4)        // (ch1-ch4)
                {
                    au.flags |= AUDIO_FLAGS_VOLUME40;   // for others
                }
                break;
//#if (UAC_TYPE == TYPE_UAC2) 
            // for UAC2
            case CS01_ID:
                au.flags |= AUDIO_FLAGS_FREQ1;
                break;
            case CS02_ID:
                au.flags |= AUDIO_FLAGS_FREQ2;
                break;
//#endif              
            default:
                break;
        }
        if (au.uac_type < TYPE_UAC2)
        { 
        // for UAC1
        switch(endpoint)
        {
            case 0x01:
                //au.entity   = CS01_ID;
                au.flags    |= AUDIO_FLAGS_FREQ1;
                break;
            case 0x82:
                //au.entity   = CS02_ID;
                au.flags    |= AUDIO_FLAGS_FREQ2;
                break;
            default:
                break;

        } 
        }              
        //if(au.irq_count<(AUDIO_IRQ_DATA_MAX_COUNT-1))   au.irq_count++;
    }
	hw_regs[0x00]=0x00;

	//hw_regs[0x10]=status0;         // clear irq flags
	//hw_regs[0x11]=status1;         // clear irq flags

    hw_regs[0x9E]= hw_regs[0x9E]+1;
	hw_regs[0x9C]= status0;         
	hw_regs[0x9D]= status1;         

	hw_regs[0x00]= old_bank;

    EX0 = 1;
//  220915 desker++ 
//  IE0:    External Interrupt 0 edge flag. 
//          Set by hardware when external interrupt edge is detected. 
//          Cleared by hardware when the interrupt is processed.
//  IE0 = 0;	                    // clear int0 flag  
}

void Timer0_ISR(void) interrupt INTERRUPT_TIMER0 
{

    TH0 = TIMER0_125US_VALUE/256;	//MODE1 16bit 65536
    TL0 = TIMER0_125US_VALUE%256;
#if (UART_RX_SUPPORT)
    if((timer_count++&0x0007)==0)
    {
        if(UART_TimeOut_Flag)
            UART_TimeOut_Count++;
    }
#endif
#if (SUPPORT_IAP2)
    if((timer_count&0x07)==0)
    {
        if(iap2.ack_timer<65535) 
            iap2.ack_timer++;
        //iap2.test_timer++;
    }
#endif
}	

void Uart0_ISR(void) interrupt INTERRUPT_UART0 
{
	EA=0;

	if (RI == 1)
   	{
      	RI = 0;

   	    UART_TimeOut_Flag = 1;          // Restart time out control when Uart receive command 
        UART_TimeOut_Count = 0;

      	rx_data = SBUF;                    // Read a character from UART

      	if (UART_Buffer_Index < UART_BUFFERSIZE)
      	{
         	UART_Buffer[UART_Buffer_Index] = rx_data; // Store in array

         	UART_Buffer_Index++;           // Update array's size
      	}
	}

    EA=1;
}
void Delay100us(WORD count)
{
    WORD i,j;           
    for(i=0;i<count;i++)
    {
        j=0;         
        while(j++<6);
    }
}

BYTE	McuReadReg(BYTE reg)
{
	return hw_regs[reg];
}

void McuWriteReg(BYTE reg, BYTE value)
{
    hw_regs[reg]=value;
}
void McuWriteRegMask(BYTE reg, BYTE value, BYTE mask)
{
	BYTE temp;
	temp = hw_regs[reg]&(~mask);
    hw_regs[reg]=temp|(value&mask);
}

void McuWriteMemS(WORD addr, BYTE value)
{
	if(addr>=USB_MEM_ADDR_LEN) return;
	
	*(USB_MEM_START+addr)=value;
}

void McuWriteMem2(WORD addr, WORD value)
{
	if(addr>=USB_MEM_ADDR_LEN) return;
	
	*(USB_MEM_START+addr)   = LOBYTE(value);
	*(USB_MEM_START+addr+1) = HIBYTE(value);
}


void McuWriteMem4(WORD addr, DWORD value)
{
	if(addr>=USB_MEM_ADDR_LEN) return;
	
	*(USB_MEM_START+addr)   = H0BYTE(value);
	*(USB_MEM_START+addr+1) = H1BYTE(value);
	*(USB_MEM_START+addr+2) = H2BYTE(value);
	*(USB_MEM_START+addr+3) = H3BYTE(value);
}
void McuWriteMem(WORD addr, BYTE len, BYTE *buffer)
{
	WORD	i;
	if(addr>=USB_MEM_ADDR_LEN) return;
	for(i=0;i<len;i++)
	{
		*(USB_MEM_START+addr+i)=buffer[i];
	}
}
BYTE McuReadMemS(WORD addr)
{
	BYTE	temp;
	temp = *(USB_MEM_START+addr);
	return temp;
}
WORD McuReadMem2(WORD addr)
{
	WORD	temp;
	temp = ((((WORD)*(USB_MEM_START+addr+1))<<8)|((WORD)*(USB_MEM_START+addr))); 
	return temp;
}
DWORD McuReadMem4(WORD addr)
{
	DWORD	temp;
	temp =  (((DWORD)*(USB_MEM_START+addr+3))<<24)|(((DWORD)*(USB_MEM_START+addr+2))<<16)|(((DWORD)*(USB_MEM_START+addr+1))<<8)|((DWORD)*(USB_MEM_START+addr));
	return temp;
}
/*
void McuReadMem(WORD addr, BYTE len, BYTE *buffer)
{
    BYTE i;
    for(i=0;i<len;i++)
    {
	    buffer[i] = *(USB_MEM_START+addr);
    }
}

void McuWriteMem(WORD addr, BYTE len, BYTE *buffer)
{
	WORD	i;
	if(addr>=USB_MEM_ADDR_LEN) return;
	for(i=0;i<len;i++)
	{
		*(USB_MEM_START+addr+i)=buffer[i];
	}
}

void Delay100us(BYTE count)
{
    delay100us_count = count;
    while(delay100us_count);
}
*/