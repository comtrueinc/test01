
#include "Mcu.h"
#include "Uart.h"
#include "MI2C.h"
#include "Spiflash.h"
#include "Entity.h"

#define _CTUSB_HID_C_
#include "Hid.h"
#undef  _CTUSB_HID_C_

#define     HID_PRINT_INFO              1
#define     HID_PRINT_COMMAND           0
#define     HID_PRINT_DATA              0

#define     HIDIN_KEY_PID               3

#define     HID_USE_PARSE               0

void HidInit(void)
{
	BYTE 	old_bank;
	old_bank = hw_regs[0x00];		
	McuWriteReg(0x00,BANK_HID);
    //int out:5, int in:6
	McuWriteReg(0x01,0x56);
    //enable FI/FO/I/O                              
	McuWriteReg(0x02,0x0F);
    // Report len for HidIn                               
	McuWriteReg(0x03,0x02);     //for volume id(1)+report(1)=2
    // Report len for HidOut                               
	//McuWriteReg(0x04,0x01);
    // Max frame size for HidIn/Out
	McuWriteReg(0x06,0x10);
	//in addr=0x77C
	//McuWriteReg(0x05,HIBYTE(USB_HID_FEATURE_DATA_ADDR));
	//McuWriteReg(0x04,LOBYTE(USB_HID_FEATURE_DATA_ADDR));

	McuWriteReg(0x11,HIBYTE(USB_HID_IN_DATA_ADDR));
	McuWriteReg(0x10,LOBYTE(USB_HID_IN_DATA_ADDR));
	
    McuWriteReg(0x13,HIBYTE(USB_HID_OUT_DATA_ADDR));
	McuWriteReg(0x12,LOBYTE(USB_HID_OUT_DATA_ADDR));
	
    McuWriteReg(0x15,HIBYTE(USB_HID_FEATUREIN_DATA_ADDR));
	McuWriteReg(0x14,LOBYTE(USB_HID_FEATUREIN_DATA_ADDR));

    McuWriteReg(0x17,HIBYTE(USB_HID_FEATUREOUT_DATA_ADDR));
	McuWriteReg(0x16,LOBYTE(USB_HID_FEATUREOUT_DATA_ADDR));
	
    // for Special Command(HID set/get report
    /*
    McuWriteReg(0x00,BANK_EP0);	

    McuWriteReg(0xE0,0x21);     // bmRequst	
    McuWriteReg(0xE1,0x80);	    // only for 0x21/0xA1

    McuWriteReg(0xE2,0x09);     // RequestType	
    McuWriteReg(0xE3,0x08);	    // only for 0x01/0x09

    McuWriteReg(0xE4,LOBYTE(USB_HID_FEATURE_DATA_ADDR));	
    McuWriteReg(0xE5,HIBYTE(USB_HID_FEATURE_DATA_ADDR));	

    McuWriteReg(0xE6,0x03);
    */	
	McuWriteReg(0x00,old_bank);
}

BYTE HidFeatureParse(void)
{
#if (HID_USE_PARSE) 
    PHID_REPORT_INFO hid_info;
    BYTE i,reg,value,valid_len,total_len;
    WORD addr;   


    hid_info  = (PHID_REPORT_INFO) usb_hid_feature_data;
    if(hid_info->vendor!=0xC0)
    {
        if(hid_info->vendor==0xCE)
        {
            McuWriteReg(0x00,BANK_MISC);
            McuWriteRegMask(0x1C,0x20, 0x20);                     // mask HID
        }
        DEBUG_PRINT(HID_PRINT_INFO,"hid vendor=%x\r\n",hid_info->vendor);
        return 0;
    }
    valid_len = hid_info->header_len + hid_info->cmd_len;
    total_len = hid_info->header_len + hid_info->cmd_len + hid_info->data_len;

    DEBUG_PRINT(HID_PRINT_DATA,"before:(%x,",usb_hid_feature_data[0]);
    for(i=1;i<(total_len-1);i++)
    {
        DEBUG_PRINT(HID_PRINT_DATA,"%x,",usb_hid_feature_data[i]);
    } 
    DEBUG_PRINT(HID_PRINT_DATA,"%x)\r\n",usb_hid_feature_data[total_len-1]);

    reg = hid_info->buffer[0];
    addr = BYTE_TO_WORD(hid_info->buffer[0],hid_info->buffer[1]);
    //hid_info->command &=~(0x01);
    DEBUG_PRINT(HID_PRINT_COMMAND,"hid cmd=%x\r\n",hid_info->command);
    switch(hid_info->command)
    {
    case HID_CMD_FW_RESET:
        DEBUG_PRINT(HID_PRINT_COMMAND,"fwReset:[%x,",hid_info->buffer[0]);
        DEBUG_PRINT(HID_PRINT_COMMAND,"%x]\r\n",hid_info->buffer[1]);
        if(hid_info->buffer[1])
        {
            McuWriteReg(0x00,BANK_MISC);	    	
            McuWriteReg(0x53,0x02);     //Reset Watch Dog
            McuWriteReg(0x00,BANK_SPI);	
            McuWriteReg(0x38,hid_info->buffer[1]);	
        }
        if(hid_info->buffer[0])
        {
            McuWriteReg(0x00,BANK_MISC);	    	
            //McuWriteReg(0x30,0x00);     //FS disconnect
            McuWriteReg(0x04,0x87);     //USB reset
            Delay100us(500);
            McuWriteReg(0x43,0x02);     //Reset Watch Dog
            
            McuWriteReg(0x00,BANK_SPI);	
            McuWriteReg(0x30,hid_info->buffer[0]);	
        }
        break;

    case HID_CMD_REG_READ:
        for(i=0;i<hid_info->data_len;i++)
        {
            hid_info->buffer[1+i]=McuReadReg(reg+i);
        }
        DEBUG_PRINT(HID_PRINT_COMMAND,"regR:<%x,",reg);
        DEBUG_PRINT(HID_PRINT_COMMAND,"%x>\r\n",hid_info->buffer[1]);
        break;
    case HID_CMD_REG_WRITE:
        for(i=0;i<hid_info->data_len;i++)
        {
            value = hid_info->buffer[1+i];
            McuWriteReg(reg+i,value);
        }
        DEBUG_PRINT(HID_PRINT_COMMAND,"regW:<%x,",reg);
        DEBUG_PRINT(HID_PRINT_COMMAND,"%x>\r\n",hid_info->buffer[1]);
        break;
    case HID_CMD_RAM_READ:
        for(i=0;i<hid_info->data_len;i++)
        {
            hid_info->buffer[2+i]=McuReadMemS(addr+i);
        }
        DEBUG_PRINT(HID_PRINT_COMMAND,"ramR:<%x,",hid_info->buffer[0]);
        DEBUG_PRINT(HID_PRINT_COMMAND,"%x>\r\n",hid_info->buffer[1]);
        break;
    case HID_CMD_RAM_WRITE:
        for(i=0;i<hid_info->data_len;i++)
        {
            value = hid_info->buffer[2+i];
            McuWriteMemS(addr+i,value);
        }
        DEBUG_PRINT(HID_PRINT_COMMAND,"ramW:<%x,",hid_info->buffer[0]);
        DEBUG_PRINT(HID_PRINT_COMMAND,"%x>\r\n",hid_info->buffer[1]);
        break;
    case HID_CMD_I2C_READ:
        for(i=0;i<hid_info->data_len;i++)
        {
            MI2CReadData(hid_info->buffer[0], hid_info->buffer[1]+i, 1, &value);
            hid_info->buffer[2+i]=value;
        }
        DEBUG_PRINT(HID_PRINT_COMMAND,"i2cR:<%x,",hid_info->buffer[0]);
        DEBUG_PRINT(HID_PRINT_COMMAND,"%x>\r\n",hid_info->buffer[1]);
        break;
    case HID_CMD_I2C_WRITE:
        for(i=0;i<hid_info->data_len;i++)
        {
            value = hid_info->buffer[2+i];
            MI2CWriteData(hid_info->buffer[0], hid_info->buffer[1]+i, 1, &value);
        }
        DEBUG_PRINT(HID_PRINT_COMMAND,"i2cW:<%x,",hid_info->buffer[0]);
        DEBUG_PRINT(HID_PRINT_COMMAND,"%x>\r\n",hid_info->buffer[1]);
        break;
    case HID_CMD_SPI_READ:
    case HID_CMD_FLASH_READ:
        SpiRead(hid_info->cmd_len,hid_info->data_len);
        DEBUG_PRINT(HID_PRINT_COMMAND,"spiR:%x,",hid_info->buffer[0]);
        DEBUG_PRINT(HID_PRINT_COMMAND,"%x,",hid_info->buffer[hid_info->cmd_len]);
        DEBUG_PRINT(HID_PRINT_COMMAND,"len:%x\r\n",hid_info->data_len);
        break;
    case HID_CMD_SPI_WRITE:
        SpiWrite(hid_info->cmd_len+hid_info->data_len);
        DEBUG_PRINT(HID_PRINT_COMMAND,"spiW:%x,",hid_info->buffer[0]);
        DEBUG_PRINT(HID_PRINT_COMMAND,"len:%x\r\n",(hid_info->cmd_len+hid_info->data_len));
        break;
    case HID_CMD_FLASH_WRITE:
        spi_write_data = usb_spi_temp_data;
        SpiInit(SPI_IN_DATA_ADDR,SPI_OUT_TEMP_ADDR);
        SpiWriteStatus(0x00);
        
        spi_write_data = (usb_hid_feature_data+USB_HID_HEADER_LEN);
        SpiInit(SPI_IN_DATA_ADDR,SPI_OUT_DATA_ADDR);
        SpiWrite(hid_info->cmd_len+hid_info->data_len);
        printf("flashW:%x,",hid_info->buffer[0]);
        printf("",hid_info->buffer[1]);
        printf("len:%x\r\n",(hid_info->cmd_len+hid_info->data_len));
        break;
    default:
        break;
    }
    if(hid_info->command & 0x02) 
        hid_info->command |= 0x01; 
        //hid_info->command |= 0x0F; 

    DEBUG_PRINT(HID_PRINT_DATA,"after:(%x,",usb_hid_feature_data[0]);
    for(i=1;i<(total_len-1);i++)
    {
        DEBUG_PRINT(HID_PRINT_DATA,"%x,",usb_hid_feature_data[i]);
    } 
    DEBUG_PRINT(HID_PRINT_DATA,"%x)\r\n",usb_hid_feature_data[total_len-1]);
#endif        
    return 1;
}
// for HidIn data ready
void HidInSetStatus(BYTE value)
{
	BYTE 	old_bank;

	old_bank = hw_regs[0x00];
    usb_hid_in_data[0]=HIDIN_KEY_PID;		
    usb_hid_in_data[1]=value;		
	McuWriteReg(0x00,BANK_HID);
	McuWriteRegMask(0x07,0x01,0x01);
	McuWriteReg(0x00,old_bank);
}

BYTE HidInSendOK(void)
{
	BYTE 	old_bank, ret;

	old_bank = hw_regs[0x00];		
	McuWriteReg(0x00,BANK_HID);
	//temp = McuReadReg(0x08);
	if((McuReadReg(0x08)&0x01))
	{
		McuWriteRegMask(0x08,0x00,0x01);
		//McuWriteReg(0x00,old_bank);
		//return 1;
        ret=1;

	}
	else
	{
		//McuWriteReg(0x00,old_bank);
		//return 0;
        ret=0;
	}
	McuWriteReg(0x00,old_bank);
    return ret;
}
// Jack Detection Init
void JackInit(void)
{
	BYTE 	old_bank;
	old_bank = hw_regs[0x00];		
    // for Jack Detection 
	McuWriteReg(0x00,BANK_JACK);
    //HN:out:0, LN in:9
	McuWriteReg(0x01,0x09);
    //enable interrupt                             
	McuWriteReg(0x02,0x01);
    
    // Return len for In                               
	McuWriteReg(0x03,0x06);     
    // Max frame size for HidIn/Out
	McuWriteReg(0x04,0x10);

	McuWriteReg(0x08,HIBYTE(USB_JACK_IN_DATA_ADDR));
	McuWriteReg(0x07,LOBYTE(USB_JACK_IN_DATA_ADDR));

} 

// for JackDetect data ready
void JackSetChange(BYTE id)
{
	BYTE 	old_bank;
    //usb_jack_in_data[0]=(value)?0x02:0x00;		
    //usb_jack_in_data[1]=(value)?0x03:0x00;		
    usb_jack_in_data[0]=0x00;		
    usb_jack_in_data[1]=0x01;		
    usb_jack_in_data[2]=0x02;		
    usb_jack_in_data[3]=0x02;		
    usb_jack_in_data[4]=0x00;  //0x04:speaker, 0x0A:Mic		
    usb_jack_in_data[5]=(id);		

	old_bank = hw_regs[0x00];
	McuWriteReg(0x00,BANK_JACK);
	McuWriteRegMask(0x05,0x01,0x01);
	McuWriteReg(0x00,old_bank);
}

BYTE JackInSendOK(void)
{
	BYTE 	old_bank, ret;
	old_bank = hw_regs[0x00];		
	McuWriteReg(0x00,BANK_JACK);
	if((McuReadReg(0x06)&0x01))
	{
		McuWriteRegMask(0x06,0x00,0x01);
        ret=1;
	}
	else
	{
        ret=0;
	}
	McuWriteReg(0x00,old_bank);
    return ret;
}

void JackUpdateStatus(BYTE status)
{
    WORD base;
    BYTE id;
    id = (status&0x7F);
    switch(id)
    {
        case 0x04:      //Speaker
            base = JD01_CURR_BASE;
            break;
        case 0x0A:      //Mic
            base = JD02_CURR_BASE;
            break;
        default:
            break;
    }
    if(status&0x80)
    {
        McuWriteMemS(base,0x02);
        McuWriteMemS(base+1,0x03);    
    }
    else
    {
        McuWriteMemS(base,0x00);
        McuWriteMemS(base+1,0x00);    

    }
 }

