#define _CTUSB_MAIN_C_
#include "Reg51.h"
#include "Mcu.h"
//#include <stdlib.h>
#undef  _CTUSB_MAIN_C_ 

#include "Uart.h"
#include "Entity.h"
#include "Audio.h"
#include "Mi2c.h"
#include "Spiflash.h"
#include "Gpio.h"
#include "Hid.h"
#include "Bulk.h"
#include "Codec.h"
#include "iap2.h"


#define     PRINT_FLAGS_AUDIO           0
#define     PRINT_FLAGS_PLAYBACK        0

#if (PRINT_FLAGS_AUDIO)
#define DBGPRINT_FLAGS_AUDIO(_FMT_,_X_)             printf(_FMT_,_X_)
#else
#define DBGPRINT_FLAGS_AUDIO(_FMT_,_X_)         
#endif

#if (PRINT_FLAGS_PLAYBACK)
#define DBGPRINT_FLAGS_PLAYBACK(_FMT_,_X_)          printf(_FMT_,_X_)
#else
#define DBGPRINT_FLAGS_PLAYBACK(_FMT_,_X_)         
#endif

#define     PRINT_HID_COMMON            0
#if (PRINT_HID_COMMON)
#define DBGPRINT_HID_COMMON(_FMT_,_X_)          printf(_FMT_,_X_)
#else
#define DBGPRINT_HID_COMMON(_FMT_,_X_)         
#endif


#define REBOOT_BY_PAGEID                0   
#define MAGIC_CODE_ADDR                 0x0000F000
#define MAGIC_CODE_VALUE                0x54538020

BYTE    debug_flags;
//BYTE    hid_status=0;
//BYTE    hid_usage=0;
BYTE    hid_state=0;
BYTE    jd_state=0,jd_status=0;


#if (REBOOT_BY_PAGEID)
void    CheckMagic(void)
{
    BYTE    *buffer,curr_page, next_page;
    DWORD   magic_code;
    SpiInit(SPI_IN_DATA_ADDR,SPI_OUT_DATA_ADDR);
    buffer = SpiReadData(MAGIC_CODE_ADDR,6);
    magic_code = BYTE_TO_DWORD(buffer[0],buffer[1],buffer[2],buffer[3]);
    if(magic_code==MAGIC_CODE_VALUE)
    {
        McuWriteReg(0x00,BANK_SPI);
        curr_page = McuReadReg(0x0F);
        next_page = buffer[4];
        printf("Magic Match...(%x)\r\n",curr_page);
        if(curr_page!=next_page && next_page>0 && next_page<4)
        {
            printf("Reboot...(%x)\r\n",next_page);
            McuWriteReg(0x0F,next_page);
            McuWriteReg(0x0E,0x01);
        }
        McuWriteReg(0x00,BANK_MISC);
    }
}
#endif

void    UsbCmdDone(void)
{
    BYTE old_bank;           
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_EP0);
    McuWriteRegMask(0x07,0x01,0x01);
    McuWriteReg(0x00,old_bank);
}

void GetUacType(void)
{
#if(UAC_TYPE==TYPE_UAC_DETECT)
    au.uac_type = TYPE_UAC2;
#else
    au.uac_type= UAC_TYPE;
#endif
}
void AudioHandler(void)
{
    // for USB control packet

    AudioGetStatus();
    if(au.flags&AUDIO_FLAGS_PLUGIN)
    {
        //printf("flags(plugin):%x\r\n",au.flags);
        DBGPRINT_FLAGS_AUDIO("flags(plugin):%x\r\n",au.flags);
        //AudioDoPlugIn();
        au.flags &= ~(AUDIO_FLAGS_PLUGIN);
    }
    if(au.flags&AUDIO_FLAGS_INPUT)
    {
        //printf("flags(input):%x\r\n",au.flags);
        DBGPRINT_FLAGS_AUDIO("flags(input):%x\r\n",au.flags);
        //AudioDoInput();
        au.flags &= ~(AUDIO_FLAGS_INPUT);
    }

    if(au.flags&AUDIO_FLAGS_IFACE1)
    {
        //AudioSetMute(1,1);
        AudioSetMute(FU01_ID);
        //CodecSetMute(1);
        DBGPRINT_FLAGS_AUDIO("flags(if1:alt=%x)\r\n",au.stream[0].alt);
#if (SUPPORT_IAP2)
        if(iap2.flags&IAP2_FLAG_SHARE_MEM)
        {
            BYTE old_bank;
            // switch to playback/record buffer when finish
            iap2.flags &= ~IAP2_FLAG_SHARE_MEM;
            old_bank = hw_regs[0x00];
            McuWriteReg(0x00,BANK_MISC);
            McuWriteRegMask(0x06,0x10,0x18);
            hw_regs[0x00] = old_bank;
            DBGPRINT_FLAGS_AUDIO("[iap2 cp buffer return...]\r\n",0);
        }
#endif
        if(au.stream[0].ready)  DBGPRINT_FLAGS_AUDIO("ready=%x\r\n",au.stream[0].ready);
        AudioDoFormat(IF_PLAYBACK);
        //AudioSetOutput();
        UsbCmdDone();
        au.flags &= ~(AUDIO_FLAGS_IFACE1);
        au.stream[0].flags |= PLAYBACK_FLAGS_NEW_FORMAT;
    }
    if(au.flags&AUDIO_FLAGS_IFACE2)
    {
        //printf("flags(if2:alt=%x\r\n",au.format.r0_alt);
        DBGPRINT_FLAGS_AUDIO("flags(if2:alt=%x)\r\n",au.stream[1].alt);
        AudioDoFormat(IF_RECORD);
        UsbCmdDone();
        au.flags &= ~(AUDIO_FLAGS_IFACE2);
    }
	
    if(au.flags&AUDIO_FLAGS_FREQ1)
    {
        //AudioSetMute(1,1);
        AudioSetMute(FU01_ID);
        //CodecSetMute(1);
        DBGPRINT_FLAGS_AUDIO("flags(freq1:%x)\r\n",McuReadMemS(CS01_FREQ_CURR_BASE+1));
        if(au.stream[0].ready)  DBGPRINT_FLAGS_AUDIO("ready=%x\r\n",au.stream[0].ready);
        AudioDoFormat(IF_PLAYBACK);
        //AudioSetOutput();
        //UsbCmdDone();
        DBGPRINT_FLAGS_AUDIO(".............ready=%x\r\n",au.stream[0].ready);
        //Delay100us(1500);
        Delay100us(10);
        UsbCmdDone();
        au.flags &= ~(AUDIO_FLAGS_FREQ1);
        au.stream[0].flags |= PLAYBACK_FLAGS_NEW_FORMAT;
    }
    if(au.flags&AUDIO_FLAGS_FREQ2)
    {
        DBGPRINT_FLAGS_AUDIO("flags(freq2:%x)\r\n",McuReadMemS(CS02_FREQ_CURR_BASE));
        AudioDoFormat(IF_RECORD);
        UsbCmdDone();
        au.flags &= ~(AUDIO_FLAGS_FREQ2);
    }

    // for FU control
    if(au.flags&AUDIO_FLAGS_MUTE1)
    {
        DBGPRINT_FLAGS_AUDIO("flags(mute1):%x\r\n",HIBYTE(au.flags));
        AudioDoMute(FU01_ID);
        au.flags &= ~(AUDIO_FLAGS_MUTE1);
    }
    if(au.flags&AUDIO_FLAGS_MUTE2)
    {
        DBGPRINT_FLAGS_AUDIO("flags(mute2):%x\r\n",HIBYTE(au.flags));
        AudioDoMute(FU02_ID);
        au.flags &= ~(AUDIO_FLAGS_MUTE2);
    }
    if(au.flags&AUDIO_FLAGS_VOLUME10)    // for playback
    {
        DBGPRINT_FLAGS_AUDIO("flags(volume10):%x\r\n",HIBYTE(au.flags));
        AudioDoVolume(FU01_ID);
        au.flags &= ~(AUDIO_FLAGS_VOLUME10);
    }
    /*
    if(au.flags&AUDIO_FLAGS_VOLUME11)    // for playback
    {
        DBGPRINT_FLAGS_AUDIO("flags(volume11):%x\r\n",HIBYTE(au.flags));
        AudioDoVolume(0x11);
        au.flags &= ~(AUDIO_FLAGS_VOLUME11);
    }
    */
    if(au.flags&AUDIO_FLAGS_VOLUME20)    // for record 
    {
        DBGPRINT_FLAGS_AUDIO("flags(volume20):%x\r\n",HIBYTE(au.flags));
        AudioDoVolume(FU02_ID);
        au.flags &= ~(AUDIO_FLAGS_VOLUME20);
    }
    /*
    if(au.flags&AUDIO_FLAGS_VOLUME21)    // for record 
    {
        DBGPRINT_FLAGS_AUDIO("flags(volume21):%x\r\n",HIBYTE(au.flags));
        AudioDoVolume(0x21);
        au.flags &= ~(AUDIO_FLAGS_VOLUME21);
    }
    */
    if(au.flags&AUDIO_FLAGS_MUTE3)
    {
        DBGPRINT_FLAGS_AUDIO("flags(mute3):%x\r\n",HIBYTE(au.flags));
        AudioDoMute(FU03_ID);
        au.flags &= ~(AUDIO_FLAGS_MUTE3);
    }
    if(au.flags&AUDIO_FLAGS_VOLUME30)    // for playback
    {
        DBGPRINT_FLAGS_AUDIO("flags(volume30):%x\r\n",HIBYTE(au.flags));
        AudioDoVolume(FU03_ID);
        au.flags &= ~(AUDIO_FLAGS_VOLUME30);
    }
    if(au.flags&AUDIO_FLAGS_MUTE4)
    {
        DBGPRINT_FLAGS_AUDIO("flags(mute4):%x\r\n",HIBYTE(au.flags));
        AudioDoMute(FU04_ID);
        au.flags &= ~(AUDIO_FLAGS_MUTE4);
    }
    if(au.flags&AUDIO_FLAGS_VOLUME40)    // for playback
    {
        DBGPRINT_FLAGS_AUDIO("flags(volume40):%x\r\n",HIBYTE(au.flags));
        AudioDoVolume(FU04_ID);
        au.flags &= ~(AUDIO_FLAGS_VOLUME40);
    }

    // for playback flags    
    if(au.stream[0].flags&PLAYBACK_FLAGS_NOT_READY)
    {
        DBGPRINT_FLAGS_PLAYBACK("p0(nready:%x)\r\n",au.stream[0].type);
        //AudioSetMute(1,1);
        AudioSetMute(FU01_ID);
        //CodecSetMute(1);
        //Mi2cWriteByte(0x06,0xC8);    //set 7302 mute
        au.stream[0].flags &= ~(PLAYBACK_FLAGS_NOT_READY);
        DBGPRINT_FLAGS_PLAYBACK("{nready/%x}\r\n",au.stream[0].ready);
    }
    if(au.stream[0].flags&PLAYBACK_FLAGS_NEW_FORMAT)
    {
        //AudioSetMute(1);
        //CodecSetMute(1);
        //Mi2cWriteByte(0x06,0xC8);    //set 7302 mute
        //AudioDoFormat(IF_PLAYBACK);
        //AudioSetOutput();
        //Delay100us(1500);
        //UsbCmdDone();
        if(au.drv_cmd==DRIVER_CMD_NDSD_PLAY0_ON || au.drv_cmd==DRIVER_CMD_DSD_PLAY0_START)
            AudioSetPlayType(ALT_PLAYBACK_NDSD);
        au.stream[0].flags &= ~(PLAYBACK_FLAGS_NEW_FORMAT);
        DBGPRINT_FLAGS_PLAYBACK("(type:%x)",au.stream[0].type);
    }
    if(au.stream[0].flags&PLAYBACK_FLAGS_DOP_CHANGE)
    {
        DBGPRINT_FLAGS_PLAYBACK("p0(dop:%x)\r\n",au.stream[0].dop);
        //AudioSetMute(1,1);
        AudioSetMute(FU01_ID);
        //CodecSetMute(1);
        //Mi2cWriteByte(0x06,0xC8);    //set 7302 mute
        //AudioSetOutput();
        au.stream[0].flags &= ~(PLAYBACK_FLAGS_DOP_CHANGE);
    }
    if(au.stream[0].flags&PLAYBACK_FLAGS_READY)
    {
        DBGPRINT_FLAGS_PLAYBACK("p0(ready:%x)\r\n",au.stream[0].type);
        //Delay100us(1000);
        AudioDoMute(IF_PLAYBACK);
        //Mi2cWriteByte(0x06,0x48);    //set 7302 mute
        au.stream[0].flags &= ~(PLAYBACK_FLAGS_READY);
    }
           
}
void HidHandler(void)
{
#if (HID_GPIO_KEY)
    GpioMonitor();
    //hid_status=gpio.hid_status;
    //gpio.hid_status=0;
#endif
	switch(hid.state)
	{
		case STATE_HID_WAIT_TRIGGER:
			if(hid.status)
			{
                //if(hid_usage)
                //{
                //    hid_state = STATE_HID_SET_STATUS;
                //    DBGPRINT_GPIO_COMMON(">> HID_SET_STATUS (state=%x,",hid_state);
                //}
                hid.usage = hid.status;
                hid.state = STATE_HID_SET_STATUS;
                DBGPRINT_HID_COMMON(">> HID_SET_STATUS (state=%x,",hid.state);
                DBGPRINT_HID_COMMON("hid_status=%x)\r\n",hid.status);
                hid.status = 0;
            }    			
			break;
        case STATE_HID_SET_STATUS:
			HidInSetStatus(hid.usage);
            hid.state = STATE_HID_WAIT_SEND;
            DBGPRINT_HID_COMMON(">> HID_WAIT_SEND(state=%x,",hid.state);
            DBGPRINT_HID_COMMON("usage=%x)\r\n",hid.usage);
            break;
		case STATE_HID_WAIT_SEND:
			if(HidInSendOK())
			{
                hid.state = STATE_HID_SET_NORMAL;
                DBGPRINT_HID_COMMON(">> HID_SET_NORMAL (state=%x)\r\n",hid.state);
			}
			break;
		case STATE_HID_SET_NORMAL:
			HidInSetStatus(0x00);
            hid.state = STATE_HID_WAIT_FINISH;
            DBGPRINT_HID_COMMON(">> HID_WAIT_FINISH (state=%x)\r\n",hid.state);
            break;
		case STATE_HID_WAIT_FINISH:
			if(HidInSendOK())
			{
                hid.state = STATE_HID_WAIT_TRIGGER;
                DBGPRINT_HID_COMMON(">> HID_WAIT_TRIGGER (state=%x)\r\n",hid.state);
			}						  
			break;
		default:
			break;
	}
}
void JackDetectHandler(void)
{
    BYTE jd_change;
    jd_change = (jd_status&0x7F);
 	switch(jd_state)
	{
		case STATE_JD_WAIT_TRIGGER:
			if(jd_change)
			{
			    JackSetChange(jd_change);
                jd_state = STATE_JD_WAIT_SEND;
                //jd_state = STATE_JD_SET_STATUS;
                printf("jd_state=>%x\r\n",jd_state);
            }    			
			break;
        /*
        case STATE_JD_SET_STATUS:
            jd_change = (jd_status&0x7F);
			JackSetChange(jd_change);
            jd_state = STATE_JD_WAIT_SEND;
            printf("jd_state=>",jd_state);
             break;
        */
		case STATE_JD_WAIT_SEND:
			if(JackInSendOK())
			{
                JackUpdateStatus(jd_status);
                jd_status=0;
                jd_state = STATE_JD_WAIT_TRIGGER;
                printf("jd_state=>%x\r\n",jd_state);
 			}
			break;
    }
}
#if (SUPPORT_IAP2)
void iAP2Handler(void)
{
    iAp2StateMachine();
}
#endif
/*
void AdcKeyNoise(WORD ms_count)
{
    BYTE old_bank;
    WORD i;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,0x25);
    for(i=0;i<ms_count;i++)
    {
        McuWriteReg(0x15,0x00);    //active low
        Delay100us(2);
        McuWriteReg(0x15,0x0F);    //normal high
        Delay100us(3);
        McuWriteReg(0x15,0x00);    //active low
        Delay100us(1);
        McuWriteReg(0x15,0x0F);    //normal high
        Delay100us(4);
    }                
    McuWriteReg(0x00,old_bank);
}
void AdcKeyActive(WORD ms_count)
{
    BYTE old_bank;
    WORD i;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,0x25);
    for(i=0;i<ms_count;i++)
    {
        McuWriteReg(0x15,0x00);    //active low
        Delay100us(10);
    }                
    McuWriteReg(0x00,old_bank);
}
void AdcKeyNormal(WORD ms_count)
{
    BYTE old_bank;
    WORD i;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,0x25);
    for(i=0;i<ms_count;i++)
    {
        McuWriteReg(0x15,0x0F);    //active low
        Delay100us(10);
    }                
    McuWriteReg(0x00,old_bank);
}
//void AdcKeyNoiseX(BYTE value, WORD ms)
void AdcKeyNoiseX(WORD ms)
{
    WORD total, max_count;
    BYTE old_bank;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,0x25);
    total=0;
    max_count = 10*ms + rand()%100;
    while(total<max_count)
    {
        WORD temp;
        BYTE value;
        value=rand()%16;
        temp=rand()%49+1;
        McuWriteReg(0x15,value);    //active low
        Delay100us(temp);
        total += temp;
        value=rand()%16;
        temp=rand()%49+1;
        McuWriteReg(0x15,0x0F);    //normal high
        Delay100us(temp);
        total += temp;
    }
}

void AdcKeySetX(BYTE value, WORD ms)
{
    BYTE old_bank;
    WORD i,max_count;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,0x25);
    max_count = ms+rand()%10;
    for(i=0;i<max_count;i++)
    {
        McuWriteReg(0x15,value);    //active low
        Delay100us(10);
    }                
    McuWriteReg(0x00,old_bank);
}
*/
void DebugHandler(void)
{
    BYTE old_bank,temp;           
    BYTE debug_cmd,para1,para2,para3,ret_data,len;
    BYTE i,buffer[8],*ret_buffer;
    WORD addr;

    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MISC);
    temp = McuReadReg(MCU_COUNT_REG);
    McuWriteRegMask(MCU_COUNT_REG,temp+1,0xFF);
    debug_cmd = McuReadReg(MCU_MAILBOX_CMD);
    //debounding_max = McuReadReg(0x98)+8; 
    if(debug_cmd==0x00)
    {
        McuWriteReg(0x00,old_bank);
        Delay100us(10);
        return;
    }
    
    para1 = McuReadReg(MCU_MAILBOX_PARA1);
    para2 = McuReadReg(MCU_MAILBOX_PARA2);
    para3 = McuReadReg(MCU_MAILBOX_PARA3);
    temp  = McuReadReg(0x94);
    
    switch(debug_cmd)
    {
        case 0x38:
            // reserve for Driver control
            if(para1 == 0xF0)
            {
                // driver control protocal version.
                McuWriteReg(0x92,0x01);             //[4]1:asio special mode, [3:0]:version 
                McuWriteReg(0x91,0x00);
            }
            else
            {
                McuWriteReg(0x93,para1);
                au.drv_cmd = para1; // ASIO command
                switch(au.drv_cmd)
                {
                    case DRIVER_CMD_DSD_PLAY0_START:
                    case DRIVER_CMD_NDSD_PLAY0_ON:
                        au.stream[0].flags |= PLAYBACK_FLAGS_NEW_FORMAT;
                        break;
                    default:
                        break;
            }
                //if(au.drv_cmd==DRIVER_CMD_DSD_PLAY0_START)
                    //au.flags |= AUDIO_FLAGS_FREQ1;
            }
            debug_cmd=0x39;
            McuWriteReg(0x90,debug_cmd);
            McuWriteReg(0x91,0x00);
            //McuWriteReg(0x92,0x00);
            //DBGPRINT_FLAGS_PLAYBACK("<%x/",hw_regs[0x90]);
            DBGPRINT_FLAGS_PLAYBACK("<%x>,",para1);            
            break;
        case 0x10:
            //Mi2cGetSlave();
            Mi2cSetSlave(para1);
            Mi2cWriteByte(para2,para3); 
            Mi2cSetSlave(DAC_I2C_SLAVE_ADDR);
            printf("i2c wdata:%x\r\n",para3);
            break;
        case 0x12:
            Mi2cSetSlave(para1);
            ret_data = Mi2cReadByte(para2); 
            Mi2cSetSlave(DAC_I2C_SLAVE_ADDR);
            McuWriteReg(0x00,BANK_MISC);
            McuWriteReg(0xA0, ret_data);
            printf("i2c rdata:%x\r\n",ret_data);
            break;

        case 0x14:
            len = (para3>8)?8:para3;
            for(i=0;i<len;i++)
            {
                buffer[i]= McuReadReg(0x94+i);
            }
            Mi2cSetSlave(para1);
            ret_data = Mi2cWriteData(para2,len,buffer);
            Mi2cSetSlave(DAC_I2C_SLAVE_ADDR);
            McuWriteReg(0x00,BANK_MISC);
            McuWriteReg(0xA0,ret_data);
            for(i=0;i<len;i++)
            {
                McuWriteReg(0xA1+i, buffer[i]);
                printf("%x",buffer[i]);
            }
            printf("(W)\r\n",0);
            break;
        case 0x16:
            temp=Mi2cGetSlave();
            len = para3;
            Mi2cSetSlave(para1);
            ret_data = Mi2cReadData(para2,len,iap2_cp_data+19);
            for(i=0;i<len;i++)
            {
                if(i%16==15 || i==(len-1))
                    printf("%x\r\n",iap2_cp_data[19+i]);
                else
                    printf("%x,",iap2_cp_data[19+i]);
            }
            //Mi2cGetSlave();
            Mi2cSetSlave(temp);
            McuWriteReg(0x00,BANK_MISC);
            McuWriteReg(0xA0,ret_data);
            printf("(R=%x)\r\n",ret_data);
            break;
/*
       case 0x16:
            //McuWriteReg(0x00,BANK_MI2C);
            ret_data = MI2CReadData0(para3,(int)para1,para2,buffer);
            McuWriteReg(0x00,BANK_MISC);
            McuWriteReg(0xA0, ret_data);
            for(i=0;i<para2;i++)
            {
                McuWriteReg(0xA1+i, buffer[i]);
                printf("%x",buffer[i]);
            }
            printf("\r\n",0);
            break;
        case 0x18:
            Mi2cSetSlave(0x20);
            //McuWriteReg(0x00,BANK_MI2C);
            //McuWriteReg(0x20,0x20); //salve addr=0x20
            buffer[0]=0x11;
            buffer[1]=0x22;
            ret_data = Mi2cWriteData(para1,para2,buffer);
            //McuWriteReg(0x20,DAC_I2C_SLAVE_ADDR);              
            Mi2cSetSlave(DAC_I2C_SLAVE_ADDR);
            McuWriteReg(0x00,BANK_MISC);
            McuWriteReg(0xA0,ret_data);
            for(i=0;i<para2;i++)
            {
                McuWriteReg(0xA1+i, buffer[i]);
                printf("%x",buffer[i]);
            }
            printf("\r\n",0);
            break;
       case 0x1A:
            //McuWriteReg(0x00,BANK_MI2C);
            //McuWriteReg(0x20,0x20); //salve addr=0x20
            Mi2cSetSlave(0x20);
            ret_data = Mi2cReadData(para1,para2,buffer);
            //McuWriteReg(0x20,DAC_I2C_SLAVE_ADDR);              
            Mi2cSetSlave(DAC_I2C_SLAVE_ADDR);
            McuWriteReg(0x00,BANK_MISC);
            McuWriteReg(0xA0,ret_data);
            for(i=0;i<para2;i++)
            {
                McuWriteReg(0xA1+i, buffer[i]);
                printf("%x",buffer[i]);
            }
            printf("\r\n",0);
            break;
*/
        case 0x40:
            HidInSetStatus(para1);
            if(HidInSendOK())
                printf("hid in done\r\n",0);                
            break;
        case 0x42:
            HidFeatureParse();
            break;
        case 0x44:
            hid.usage = para1;
            hid.state = STATE_HID_SET_STATUS;
            DBGPRINT_HID_COMMON(">> HID_SET_STATUS (state=%x),",hid.state);
            break;
        case 0x50:
            temp = SpiReadStatus(1);
            McuWriteReg(0xA0,temp);
            break;
        case 0x52:
            ret_buffer = SpiReadId();
            McuWriteReg(0xA0,ret_buffer[0]);
            McuWriteReg(0xA1,ret_buffer[1]);
            McuWriteReg(0xA2,ret_buffer[2]);
            break;
        case 0x54:
            ret_buffer = SpiReadData(BYTE_TO_DWORD(0,para1,para2,para3),4);
            McuWriteReg(0xA0,ret_buffer[0]);
            McuWriteReg(0xA1,ret_buffer[1]);
            McuWriteReg(0xA2,ret_buffer[2]);
            McuWriteReg(0xA3,ret_buffer[3]);
            break;

        case 0x56:
            /*
            ret_buffer = SpiReadData(BYTE_TO_DWORD(0,para1,para2,para3),64);
            for(i=0;i<64;i++){
                if(i%16==15) printf("%x,\r\n",ret_buffer[i]);
                else  printf("%x,",ret_buffer[i]);
            }
            */
            SpiWriteStatus(0x00);
            break;
        case 0x58:  //Earse
            //SpiWriteStatus(0x00);
            SpiEarse(FLASH_SECTOR_ERASE,BYTE_TO_DWORD(0,para1,para2,para3));
            break;
        case 0x5A:  //Erase+write
            //SpiWriteStatus(0x00);
            //SpiEarse(FLASH_SECTOR_ERASE,BYTE_TO_DWORD(0,para1,para2,para3)); 
            for(i=0;i<4;i++)
                buffer[i]=McuReadReg(MCU_MAILBOX_PARA3+1+i);
            SpiWriteData(BYTE_TO_DWORD(0,para1,para2,para3),buffer,4);
            break;
        case 0x5C:  //Write only
        {
            BYTE    test_data[64];
            DWORD   faddr;
            faddr = BYTE_TO_DWORD(0,para1,para2,para3); 
            printf("addr:%x",H2BYTE(faddr));
            printf("%x\r\n",H1BYTE(faddr));
            for(i=0;i<64;i++) test_data[i]=i*2;

            //temp1 = ((WORD)McuReadReg(0x94))*5;
            //temp2 = ((WORD)McuReadReg(0x95))*5;
            //temp3 = ((WORD)McuReadReg(0x96))*5;

            SpiWriteStatus(0x00);
            SpiEarse(FLASH_SECTOR_ERASE,faddr); 
            SpiWriteData(faddr,test_data,64);
            break;
        }
        case 0x5E:
        {
            BYTE    *ret_buf;
            BYTE    j,test_data[64];
            WORD    fi;
            DWORD   faddr;

            for(i=0;i<64;i++) test_data[i]=i*2+1;
            faddr = BYTE_TO_DWORD(0,para1,para2,0); 

            SpiWriteStatus(0x00);
            SpiEarse(FLASH_SECTOR_ERASE,faddr);

            for(i=0;i<(para3+1);i++)
            {
                fi =64*(WORD)(i);
                SpiWriteData(faddr+fi,test_data,64);
            }
            for(i=0;i<(para3+1);i++)
            {
                fi =64*(WORD)(i);
                ret_buf = SpiReadData(faddr+fi,128);
                for(j=0;j<64;j++)
                {
                    if(ret_buf[j]!=test_data[j])
                        break;
                }
                if(j>=64)   printf("ok:%d*64,\r\n",i);
                else        printf("ng:%d*64,\r\n",i);    
                
            }
            break;
        }
#if (SUPPORT_IAP2)
        case 0xA0:
        {
            int idx;
            iAp2Prepare();
            for(idx=0;idx<iap2.cert_len;idx++)
            {
                if(idx%16==15)
                    printf("%x\r\n",iap2_cp_data[19+idx]);
                else
                    printf("%x,",iap2_cp_data[19+idx]);
            }
        }
            break;
        case 0xA8:
            iAp2GetIdentInfoX(usb_bulkin_data);
            break;
        case 0xAA:
            iAp2GetMsgInfo(para1,usb_bulkin_data);
            break;
        case 0xAE:
        {
            BYTE buffer[32]={
            0x03,0x87,0x98,0xC9,0x2C,0xD6,0x14,0xFB,0xFF,0xD6,0x97,0xF8,0xF4,0xF3,0xC9,0xA7,
            0x62,0xBF,0xAA,0x08,0x58,0x50,0xD3,0x5A,0x0A,0x11,0x71,0xCB,0x0A,0x52,0x26,0xF6,
            };
            printf("test start...\r\n",0);
            iAp2CpSetChallengeLen(32);
            iAp2CpSetChallengeData(32,buffer);
            iAp2CpSetAuthControl(01);
            printf("test end...\r\n",0);
        }
            break;
#endif
#if (BULK_ENABLE)
        case 0xB0:
            BulkInSetAddr(USB_BULK_IN_DATA_ADDR); //addr & len
            BulkInStart(8);
            BulkInDataDone();
            break;
#endif
        case 0xD0:
            McuWriteMemS(BYTE_TO_WORD(para1,para2),para3);
            break;
        case 0xD2:
            addr = BYTE_TO_WORD(para1,para2);
            //para3 = McuReadMemS(BYTE_TO_WORD(para1,para2));
            McuWriteReg(0xA0,McuReadMemS(addr++));    
            McuWriteReg(0xA1,McuReadMemS(addr++));    
            McuWriteReg(0xA2,McuReadMemS(addr++));    
            McuWriteReg(0xA3,McuReadMemS(addr++));    
            McuWriteReg(0xA4,McuReadMemS(addr++));    
            McuWriteReg(0xA5,McuReadMemS(addr++));    
            McuWriteReg(0xA6,McuReadMemS(addr++));    
            McuWriteReg(0xA7,McuReadMemS(addr++));    
            break;
        case 0xDE:
            McuReadMem2(0x0300);
            McuReadMem4(0x0300);
            break;
        case 0xE0:
        {
            BYTE prev,curr,i=0;
            prev=hw_regs[0x32];
            while(i<20)
            {
                curr = hw_regs[0x32];
                if(prev != curr)
                {
                    spi_read_data[i++]=prev;
                    prev=curr;
                }
            }
            spi_read_data[i]=prev;
            printf("Test 230508..(%x)\r\n",i);
        }
            break;
        case 0xEC:
            printf("Echo..(%x)\r\n",debug_cmd);
            McuWriteReg(0xA0,debug_cmd);
            break;
        /*
        case 0xF0:
            AdcKeyNoise(5);
            AdcKeyActive(400);
            AdcKeyNoise(10);
            break;
        case 0xF2:
            AdcKeyNoise(para1);
            AdcKeyActive(para2);
            AdcKeyNoise(para3);
            break;
        case 0xF4:
            AdcKeyNoise(para1);
            AdcKeyActive(para2);
            AdcKeyNoise(para1);
            AdcKeyNormal(para3);
            AdcKeyNoise(para1);
            AdcKeyActive(para2);
            AdcKeyNoise(para1);
            break;
        case 0xF8:
            AdcKeyNoiseX(para1);
            AdcKeySetX(temp, 5*(WORD)para2);
            AdcKeyNoiseX(para3);
            break;
        case 0xFA:
        {
            BYTE para4;
            para4=hw_regs[0x95];
            AdcKeyNoiseX(para1);
            AdcKeySetX(temp, 5*(WORD)para2);
            AdcKeyNoiseX(para1);
            AdcKeySetX(0x0F, 5*(WORD)para3);
            AdcKeyNoiseX(para1);
            AdcKeySetX(temp, 5*(WORD)para4);
            AdcKeyNoiseX(para1);
        }
            break;
        */
        case 0xF0:
            temp = ((~para1+1)+(~para2+1)+(~para3+1)+(~hw_regs[0x94]+1));
            temp =~(temp>128?128:temp)+1;
            McuWriteReg(0xA0,temp);
            break;
        case 0xFC:       
            JackSetChange(para1&0x7F);
            for(i=0;i<200;i++)
            {
                if(JackInSendOK()) break;
                Delay100us(100);
            }
            McuWriteReg(0xA0,i);
            if(i<200)
            {
                JackUpdateStatus(para1);
            }
            break;
        case 0xFE:
            jd_status=para1;
            printf("jd_status=%x\r\n",jd_status);
            break;
        default:
            break;
    }
    
    McuWriteReg(0x00,BANK_MISC);
    //if((debug_cmd != 0) && (debug_cmd&0x01)==0)    McuWriteReg(MCU_MAILBOX_CMD,(debug_cmd|0x01));
    if(!(debug_cmd&0x01))    McuWriteReg(MCU_MAILBOX_CMD,(debug_cmd|0x01));
    
    McuWriteReg(0x00,old_bank);
    Delay100us(10);
}

void LoadDescriptor(void)
{
    desc_curr_addr = USB_DESC_DATA_ADDR;

    DescriptDevice();
    if(au.uac_type==TYPE_UAC2)  DescriptDevQualify();
    DescriptHidReport();
    DescriptStringTable();
    DescriptConfiguration();
}
/*
void DacInit(void)
{
    Mi2cInit(0x20,USB_I2C_RDATA_ADDR,USB_I2C_WDATA_ADDR);
    
    Mi2cWriteByte(0x10,0xC0);
    Mi2cWriteByte(0x11,0x00);
    Mi2cWriteByte(0x12,0x08);
    Mi2cWriteByte(0x13,0x00);
    Mi2cWriteByte(0x14,0x40);

    Mi2cWriteByte(0x30,0x23);
    Mi2cWriteByte(0x31,0x19);
    Mi2cWriteByte(0x32,0x1E);

    Mi2cWriteByte(0x39,0xF3);
    Mi2cWriteByte(0x3B,0x77);
    Mi2cWriteByte(0x40,0x02);
    Mi2cWriteByte(0x45,0x00);
    Mi2cWriteByte(0x4E,0x77);
	Mi2cWriteByte(0x47,0xA4);
	Mi2cWriteByte(0x49,0x00);       // disable dpop timer
	Mi2cWriteByte(0x4A,0x42);       // freerun
    Mi2cWriteByte(0x4D,0x37);
    Mi2cWriteByte(0x59,0x2D);
    Mi2cWriteByte(0x5F,0x4B);
    Mi2cWriteByte(0x61,0x08);
    Mi2cWriteByte(0x62,0x01);

    Mi2cWriteByte(0x06,0x48);       // spdif out 24bits
    Mi2cWriteByte(0x04,0x36);       // SRC mode 3, Input source I2S1
    Mi2cWriteByte(0x05,0x09);       // Output Freq 192K
}
*/
void SystemInit(void)
{
//    BYTE old_bank;

    irq_flags = timer_count = hid_flags = 0;
    debug_flags = 0;
    
	//McuWriteReg(0x00,BANK_CLOCK);
	//McuWriteRegMask(0x08,0x20,0x30);    //XRAM Clock 60Mhz->80Mhz
	//McuWriteReg(0x0A,0x8D);             //RIU 6Mhz->12Mhz
    
    //McuWriteReg(0x00,BANK_UTMI);
    //McuWriteRegMask(0x0B,0x0A,0x0A); // test utmi  R0B.3=1 R0B.1=1
    //McuWriteRegMask(0x29,0x04,0x04); // test utmi  R29.2=1;
    //McuWriteRegMask(0x2D,0x01,0x01); // Tx enhance x2

    SpiInit(SPI_IN_DATA_ADDR,SPI_OUT_DATA_ADDR);

    GpioInit();
    HidInit();
    JackInit();
    hid_state = STATE_HID_WAIT_TRIGGER;
    jd_state = STATE_JD_WAIT_TRIGGER;

#if (BULK_ENABLE)
    BulkInit(au.uac_type);
#endif

    CodecInit(DAC_I2C_SLAVE_ADDR,I2C_IN_DATA_ADDR,I2C_OUT_DATA_ADDR);

}

void main(void)
{
    Mcu_Initial();
    McuWriteReg(0x00,BANK_MISC);            
    McuWriteRegMask(0x30,0x00,0x07);    //[0]:HS, [1]:Connect, [2]:Suspand
    McuWriteReg(0x43,0x01);             //[1]:Reset Watch Dog
#if (REBOOT_BY_PAGEID)
    CheckMagic();
#endif
    GetUacType();
//============================================================================
    McuWriteReg(0xAF,0x5A);            
    printf("CT7602 v0.23.0705(%x)\r\n",hw_regs[0xAF]);
    
    AudioInit();
    EntityInit(); 
    LoadDescriptor();
    SystemInit();

    McuWriteReg(0x00,BANK_MISC);
    McuWriteReg(0x43,0x02);             //[1]:Stop Watch Dog
#if(SUPPORT_IAP2)
    iAp2Init();
#endif
    Delay100us(2000);                                                                           

    McuWriteReg(0x00,BANK_MISC);
    McuWriteRegMask(0x30,(au.uac_type==TYPE_UAC1_FS)?0x02:0x03,0x07);    //[0]:HS, [1]:Connect, [2]:Suspand

    while(1)
    {
        DebugHandler();
        HidHandler();
        JackDetectHandler();
        AudioHandler();
#if(SUPPORT_IAP2)
        iAP2Handler();
#endif
#if(UART_RX_SUPPORT)
        SerialDataIn();
#endif
        if(irq_flags)
        {
            if(irq_flags!=0x08) //skip hid flags 
            printf("irq_flags:%x\r\n",irq_flags);
            irq_flags = 0;
        }
        
    }
}

