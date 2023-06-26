#include "Mcu.h"
#include "Uart.h"
#include "Entity.h"
#define _CTUSB_DESCRIPTOR_C_
#include "Audio.h"
#undef  _CTUSB_DESCRIPTOR_C_ 

#define     DESC_PRINT_ADDR         0
#define     DESC_PRINT_STRING       0
#define     DESC_PRINT_DATA         0

BYTE  xdata usb_desc_data[USB_DESC_DATA_ADDR_LEN]           _at_    (USB_DESC_DATA_ADDR);    // 0x0000;	

void DescriptDevice(void)
{
    int i;
    BYTE *buff; 
    WORD len,addr;

    addr = desc_curr_addr; 
#if(UAC_TYPE==TYPE_UAC2)   
    len  = CT_UAC2_DEVICE_DESC_LEN;
    buff = uac2_device_desc_buffer;
#else 
    len  = CT_UAC1_DEVICE_DESC_LEN;
    buff = uac1_device_desc_buffer;
#endif

    DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x(Dev),",LOBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"len:%x",HIBYTE(len));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(len));
    
    McuWriteReg(0x00,BANK_EP0);
    McuWriteReg(0x0A,LOBYTE(addr));	
    McuWriteRegMask(0x0B,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x0B,0x40,0xC0);

    for(i=0;i<len;i++)
    {
    	McuWriteMemS(addr+i,buff[i]);
    }
    desc_curr_addr += len;
}

void DescriptDevQualify(void)
{
#if (UAC_TYPE==TYPE_UAC2) 
    int i;
    BYTE *buff; 
    WORD len,addr;
    
    addr = desc_curr_addr;    
    len = CT_UAC2_DEV_QUALIFY_DESC_LEN;
    buff = uac2_dev_qualify_buffer;
    DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x(DevQ),",LOBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"len:%x",HIBYTE(len));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(len));

    McuWriteReg(0x00,BANK_EP0);	
    McuWriteReg(0x0C,LOBYTE(addr));	
    McuWriteRegMask(0x0D,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x0D,0x40,0xC0);
    for(i=0;i<len;i++)
    {
    	McuWriteMemS(addr+i,buff[i]);
    }
    desc_curr_addr += len;
#endif  //UAC_TYPE==TYPE_UAC2
}
 
void DescriptHidReport(void)
{
//    int i;
    BYTE *buff; 
    WORD len,addr;
    
    addr = desc_curr_addr; 
#if (UAC_TYPE==TYPE_UAC2)   
    len = CT_UAC2_HID_REPORT_DESC_LEN;
    buff = uac2_hid_report_buffer;
#else
    len = CT_UAC1_HID_REPORT_DESC_LEN;
    buff = uac1_hid_report_buffer;
#endif
    McuWriteReg(0x00,BANK_EP0);	
    McuWriteReg(0x1A,LOBYTE(addr));	
    McuWriteRegMask(0x1B,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x1B,0x40,0xC0);
    McuWriteReg(0x1C,LOBYTE(len));	
    McuWriteReg(0x1D,HIBYTE(len));	

    DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x(HidR),",LOBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"len:%x",HIBYTE(len));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(len));
    /*
    for(i=0;i<len;i++)
    {
    	McuWriteMemS(addr+i,buff[i]);
    }
    */
    McuWriteMem(addr,len,buff);
    desc_curr_addr += len;
}

void DescriptStringTable(void)
{
    BYTE i,j,max_count;
    BYTE reg, temp, *buff, string0[4]={0x04,0x03,0x09,0x04};
    WORD str_len,addr;        

#if (UAC_TYPE==TYPE_UAC2)   
    max_count = UAC2_STRINGS_MAX_COUNT;
#else
    max_count = UAC1_STRINGS_MAX_COUNT;
#endif

    McuWriteReg(0x00,BANK_EP0);	
    for(i=0;i<=max_count;i++)
    {
        addr = desc_curr_addr;    
        reg = 0x22+i*2;
        McuWriteReg(reg,LOBYTE(addr));	
        McuWriteRegMask(reg+1,HIBYTE(addr),0x3F);	
        McuWriteRegMask(reg+1,0x40,0xC0);
        //printf("addr:%x",HIBYTE(addr));	
        //printf("%x,",LOBYTE(addr));	
        //printf("(Str%x=",i);	
        DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
        DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(addr));
        DEBUG_PRINT(DESC_PRINT_STRING,"(Str%x=",i);
        if(i==0)
        {
            str_len = 1;
            for(j=0;j<4;j++)
            {
            	McuWriteMemS(addr++,string0[j]);
                //printf("%x,",string0[j]);	
                DEBUG_PRINT(DESC_PRINT_STRING,"%x,",string0[j]);
            }
            desc_curr_addr += 4;
        }
        else
        {
            str_len = string_table[i-1].len;
            buff = string_table[i-1].buffer;
            temp = 0x03;
            McuWriteMemS(addr++,str_len*2+2);
            McuWriteMemS(addr++,0x03);
            //printf("%x,03,",str_len*2+2);	
            DEBUG_PRINT(DESC_PRINT_STRING,"%x,03,",str_len*2+2);
            for(j=0;j<str_len;j++)
            {
            	McuWriteMemS(addr++,buff[j]);
            	McuWriteMemS(addr++,0);
                //printf("%x,00,",buff[j]);	
                DEBUG_PRINT(DESC_PRINT_STRING,"%x,00,",buff[j]);	
            }
            desc_curr_addr += str_len*2+2;
        }
        //printf("),len:%x",HIBYTE(str_len*2+2));	
        //printf("%x\r\n",LOBYTE(str_len*2+2));	
        DEBUG_PRINT(DESC_PRINT_STRING,"),len:%x",HIBYTE(str_len*2+2));
        DEBUG_PRINT(DESC_PRINT_STRING,"%x\r\n",LOBYTE(str_len*2+2));
    }
}
void DescriptConfiguration(void) 
{
    BYTE *buff, max_count; 
    WORD i,j,len,addr;
    
    addr = desc_curr_addr;    
#if (UAC_TYPE==TYPE_UAC2)   
    len = UAC2_CONFIG_DESC_LEN;
    max_count = UAC2_CONFIG_MAX_COUNT;
#else
    len = UAC1_CONFIG_DESC_LEN;
    max_count = UAC1_CONFIG_MAX_COUNT;
#endif

    DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x(Conf),",LOBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"len:%x",HIBYTE(len));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(len));

    McuWriteReg(0x00,BANK_EP0);	
    // for config 0
    McuWriteReg(0x0E,LOBYTE(addr));	
    McuWriteRegMask(0x0F,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x0F,0x40,0xC0);	
    // for config 1
    McuWriteReg(0x10,LOBYTE(addr));	
    McuWriteRegMask(0x11,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x11,0x40,0xC0);	
    for(i=0;i<max_count;i++)
    {
        buff = config_desc_table[i].buffer;
        len  = config_desc_table[i].len;
        DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
        DEBUG_PRINT(DESC_PRINT_ADDR,"%x,",LOBYTE(addr));
        DEBUG_PRINT(DESC_PRINT_ADDR,"[%x:",i);
        DEBUG_PRINT(DESC_PRINT_ADDR,"(%i),",len);
        DEBUG_PRINT(DESC_PRINT_ADDR,"%x],\r\n",buff[0]);
        for(j=0;j<len;j++)
        {
        	McuWriteMemS(addr+j,buff[j]);
            DEBUG_PRINT(DESC_PRINT_DATA,"%x,",buff[j]);
            if(j%16==15 || j==(len-1))
                DEBUG_PRINT(DESC_PRINT_DATA,"\r\n",0);
        }

        addr += len;    
    }
    desc_curr_addr = addr;
    // for hid descriptor
#if (UAC_TYPE==TYPE_UAC2)   
    addr = desc_curr_addr - UAC2_HID_INTERFACE_LEN - UAC2_BULK_INTERFACE_LEN + 9 ; 
    len  = UAC2_HID_DESC_LEN;
    //McuWriteReg(0x06,UAC2_HID_IF_INDEX);	
#else
    addr = desc_curr_addr - UAC1_HID_INTERFACE_LEN - UAC1_BULK_INTERFACE_LEN + 9 ; 
    len  = UAC1_HID_DESC_LEN;
    //McuWriteReg(0x06,UAC1_HID_IF_INDEX);	
#endif
    McuWriteReg(0x06,IF_HID);	
    McuWriteReg(0x18,LOBYTE(addr));	
    McuWriteRegMask(0x19,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x19,0x40,0xC0);
    DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x(Hid),",LOBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"len:%x",HIBYTE(len));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(len));

    // for Other Speed 
#if (UAC_TYPE==TYPE_UAC2)   
    addr = desc_curr_addr;    
    len  = CT_UAC2_OS_CONFIG_DESC_LEN;
    buff = uac2_os_config_buffer;

    // for other speed config 0
    McuWriteReg(0x12,LOBYTE(addr));	
    McuWriteRegMask(0x13,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x13,0x40,0xC0);	
    // for other speed config 1
    McuWriteReg(0x14,LOBYTE(addr));	
    McuWriteRegMask(0x15,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x15,0x40,0xC0);	

    McuWriteMem(addr,len,buff);
    DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x(OS),",LOBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"len:%x",HIBYTE(len));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(len));
#endif

    desc_curr_addr += len;

    DEBUG_PRINT(1,"desc addr end:%x",HIBYTE(desc_curr_addr));	
    DEBUG_PRINT(1,"%x\r\n",LOBYTE(desc_curr_addr));

}


/*
void DescriptStrings(BYTE index)
{
    int i;
    BYTE reg, *buff; 
    WORD str_len,addr;

    addr = desc_curr_addr;    
    reg = 0x22+index*2;
    switch(index)
    {
    // write lang id for string 0
    case 0:
        str_len = 4;
        buff = uac1_string00_desc_buffer;
        break;
    case 1:
        str_len = CT_UAC1_STRING01_DESC_LEN;
        buff    = uac1_string01_desc_buffer;
        break;
    case 2:
        str_len = CT_UAC1_STRING02_DESC_LEN;
        buff    = uac1_string02_desc_buffer;
        break;
    case 3:
        str_len = CT_UAC1_STRING03_DESC_LEN;
        buff    = uac1_string03_desc_buffer;
        break;
    case 4:
        str_len = CT_UAC1_STRING04_DESC_LEN;
        buff    = uac1_string04_desc_buffer;
        break;
    case 5:
        str_len = CT_UAC1_STRING05_DESC_LEN;
        buff    = uac1_string05_desc_buffer;
        break;
    case 6:
        str_len = CT_UAC1_STRING06_DESC_LEN;
        buff    = uac1_string06_desc_buffer;
        break;
    default:
        break;
    }

    printf("addr:%x",HIBYTE(addr));	
    printf("%x,",LOBYTE(addr));	
    printf("(Str%x),",index);	
    printf("len:%x",HIBYTE(str_len));	
    printf("%x\r\n",LOBYTE(str_len));	
    
    McuWriteReg(0x00,0x06);	        
    McuWriteReg(reg,LOBYTE(addr));	
    McuWriteRegMask(reg+1,HIBYTE(addr),0x3F);	
    McuWriteRegMask(reg+1,0x40,0xC0);

    for(i=0;i<str_len;i++)
    {
    	McuWriteMemS(addr+i,buff[i]);
    }
    desc_curr_addr += str_len;
}
void DescriptConfiguration(void) 
{
    BYTE *buff; 
    WORD i,len,addr;
    
    addr = desc_curr_addr;    
    len = UAC1_CONFIG_DESC_LEN;
    buff =uac1_config_desc_buffer;
    
    //printf("addr:%x",HIBYTE(addr));	
    //printf("%x(Conf),",LOBYTE(addr));	
    //printf("len:%x",HIBYTE(len));	
    //printf("%x\r\n",LOBYTE(len));	
    DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x(Conf),",LOBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"len:%x",HIBYTE(len));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(len));

    McuWriteReg(0x00,BANK_EP0);	
    // for config 0
    McuWriteReg(0x0E,LOBYTE(addr));	
    McuWriteRegMask(0x0F,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x0F,0x40,0xC0);	
    // for config 1
    McuWriteReg(0x10,LOBYTE(addr));	
    McuWriteRegMask(0x11,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x11,0x40,0xC0);	

    for(i=0;i<len;i++)
    {
    	McuWriteMemS(addr+i,buff[i]);
    }
    desc_curr_addr += len;

    // for hid descriptor
    len  = UAC1_HID_DESC_LEN;
    addr = desc_curr_addr-(UAC1_HID_DESC_LEN+UAC1_ENDPOINT_INT_LEN);    
    McuWriteReg(0x06,UAC1_HID_IF_INDEX);	
    McuWriteReg(0x18,LOBYTE(addr));	
    McuWriteRegMask(0x19,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x19,0x40,0xC0);
    //printf("addr:%x",HIBYTE(addr));	
    //printf("%x(Hid),",LOBYTE(addr));	
    //printf("len:%x",HIBYTE(len));	
    //printf("%x\r\n",LOBYTE(len));	
    DEBUG_PRINT(DESC_PRINT_ADDR,"addr:%x",HIBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x(Hid),",LOBYTE(addr));
    DEBUG_PRINT(DESC_PRINT_ADDR,"len:%x",HIBYTE(len));
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(len));


    //printf("addr end:%x",HIBYTE(desc_curr_addr));	
    //printf("%x\r\n",LOBYTE(desc_curr_addr));	
    DEBUG_PRINT(DESC_PRINT_ADDR,"addr end:%x",HIBYTE(desc_curr_addr));	
    DEBUG_PRINT(DESC_PRINT_ADDR,"%x\r\n",LOBYTE(desc_curr_addr));

    //addr = CT_OS_CONFIG_DESC_ADDR;
    addr = desc_curr_addr;    
    len  = CT_OS_CONFIG_DESC_LEN;
    buff = uac1_config_desc_buffer;
    printf("addr:%x",HIBYTE(addr));	
    printf("%x(OS),",LOBYTE(addr));	
    printf("len:%x",HIBYTE(len));	
    printf("%x\r\n",LOBYTE(len));	

    // for other speed 0	
    McuWriteReg(0x12,LOBYTE(addr));	
    McuWriteRegMask(0x13,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x13,0x40,0xC0);
    // for other speed 1	
    McuWriteReg(0x14,LOBYTE(addr));	
    McuWriteRegMask(0x15,HIBYTE(addr),0x3F);	
    McuWriteRegMask(0x15,0x40,0xC0);
    for(i=0;i<len;i++)
    {
    	McuWriteMemS(addr+i,buff[i]);
    }
    McuWriteMemS(addr+1,0x07);
    desc_curr_addr += len;
}
*/