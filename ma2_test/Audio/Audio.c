#include "Reg51.h"
#include "Mcu.h"
#include "Uart.h"
//#include "UserSetting.h"
#include "entity.h"
#include "codec.h"

#define _CTUSB_AUDIO_C_
#include "Audio.h"
#undef  _CTUSB_AUDIO_C_

code WORD FREQ_SYNC_SETTING[10]={441,480,882,960,1764,1920,3528,3840,7056,7680};

//DWORD freq_sets[FREQ_MAX_COUNT]={ SAMPLE_RATE_44K, SAMPLE_RATE_48K, SAMPLE_RATE_88K, SAMPLE_RATE_96K, SAMPLE_RATE_176K, SAMPLE_RATE_192K,
//                        SAMPLE_RATE_352K, SAMPLE_RATE_384K, SAMPLE_RATE_705K, SAMPLE_RATE_768K,};
#define     PRINT_AUDIO_COMMON              1
#define     PRINT_AUDIO_FORMAT              0
#define     PRINT_AUDIO_MUTE                0
#define     PRINT_AUDIO_VOL                 0
#define     PRINT_AUDIO_VOL_INFO            0

#if (PRINT_AUDIO_COMMON)
#define DBGPRINT_AUDIO_COMMON(_FMT_,_X_)           printf(_FMT_,_X_)
#else
#define DBGPRINT_AUDIO_COMMON(_FMT_,_X_)         
#endif

#if (PRINT_AUDIO_FORMAT)
#define DBGPRINT_AUDIO_FORMAT(_FMT_,_X_)           printf(_FMT_,_X_)
#else
#define DBGPRINT_AUDIO_FORMAT(_FMT_,_X_)         
#endif

#if (PRINT_AUDIO_MUTE)
#define DBGPRINT_AUDIO_MUTE(_FMT_,_X_)             printf(_FMT_,_X_)
#else
#define DBGPRINT_AUDIO_MUTE(_FMT_,_X_)         
#endif

#if (PRINT_AUDIO_VOL)
#define DBGPRINT_AUDIO_VOL(_FMT_,_X_)              printf(_FMT_,_X_)
#else
#define DBGPRINT_AUDIO_VOL(_FMT_,_X_)         
#endif

#if (PRINT_AUDIO_VOL_INFO)
#define DBGPRINT_AUDIO_VOL_INFO(_FMT_,_X_)              printf(_FMT_,_X_)
#else
#define DBGPRINT_AUDIO_VOL_INFO(_FMT_,_X_)         
#endif

void AudioInit(void)
{                                            
	BYTE 	old_bank;
	old_bank = hw_regs[0x00];
    DBGPRINT_AUDIO_COMMON("AudioInit(type=%x)\r\n",au.uac_type);

	McuWriteReg(0x00,BANK_EP0);
	McuWriteReg(0x08,0xF0);                         //set usb ack timeout 85us*240=20ms
    //desker+- 211229
	//McuWriteReg(0x09,0x04);                         //set usb ack timeout 85us*1024=85ms
	McuWriteReg(0x09,0x20);                         //set usb ack timeout 85us*1024=85ms
	McuWriteRegMask(0x07,0x10,0x10);                //enable interface ack

	McuWriteReg(0x00,BANK_EP1);
    //McuWriteReg(0x04,0x00);	                    //ep1 sram start (high)
    //McuWriteReg(0x03,0xFF);	                    //ep1 sram start (low)
    //printf("0x05(%x)",hw_regs[0x05]);
    McuWriteReg(0x05,(au.uac_type == TYPE_UAC2)?0x40:0x80);	                        //playback ready timeout uac1:0x80(~2ms) uac2:0x10 (~0.25ms)
    McuWriteReg(0x24,(au.uac_type == TYPE_UAC2)?0x40:0x80);	                        //record ready timeout:  uac1:0x80(~2ms) uac2:0x10 (~0.25ms)
    McuWriteReg(0x0E,(au.uac_type == TYPE_UAC1_HS)?0x77:0x00);                         //interval value:   bit7-4: record, bit3-0: play [0: UAC1_FS/UAC2,  7:for UAC1_HS]   
    // for record
    //McuWriteReg(0x23,0x01);	                    //sram start (high)
    //McuWriteReg(0x22,0xFF);	                    //sram start (low)
    McuWriteRegMask(0x40,6,0x0F);                   //6:for high speed KP mode (only valid 

    //McuWriteRegMask(0x28,0x40,0x40);	        //auto correction

	McuWriteReg(0x00,BANK_SYNTH1);
    McuWriteRegMask(0x10,0x40,0x40);	        //sync to SOF
    McuWriteReg(0x02,(au.uac_type == TYPE_UAC2)?0x50:0x0A);	            //Clock Div only work on none-KP mode

    McuWriteRegMask(0x0D,(UAC2_PLAY0_FEEDBACK)?0x20:0x00,0x20);	        //Enbale KP mode

    //Enable FPGA Loopback Mpde 
	//McuWriteReg(0x00,BANK_ADC0);
    //McuWriteReg(0x15,0x09);	        //DAC->ADC
	//McuWriteReg(0x00,BANK_PLAY0);
    //McuWriteRegMask(0x02,0x08,0x08); //ADC->SPIDF
     

	//McuWriteRegMask(0x02,0x0A);                 //0x0A/0x50 for uac1/2
    //McuWriteRegMask(0x0D,0x20,0x20);	        //enable forced clock
    //AudioSetClock(1,0);

	//McuWriteReg(0x00,BANK_PLAY0);

    //McuWriteRegMask(0x02,0x00,0x90);    //internal MCLK
    //McuWriteRegMask(0x04,0x30,0x30);    //B4:DOP auto, B5:NDSD Auto  to PCM
    //McuWriteRegMask(0x3A,0x0C,0x0C);    //B2:DOP kernal off, B3:NDSD kernel off
        
//	McuWriteReg(0x00,BANK_DAC0);
//
//  McuWriteRegMask(0x04,0x30,0x30);    //B3:DOP auto, B2:NDSD Auto
 
 	//McuWriteReg(0x00,BANK_E2_PLAY0);
    //BK03_03[7]: 0:disable Depop 1:enable depop
    //McuWriteRegMask(0x03,0x00,0x80);
    //BK03_04[3]:0:skip data ready
    //BK03_04[4]:1:Set Play port auto output DSD format when input is DoP
    //McuWriteRegMask(0x04,0x10,0x18);
    //i2s support 6 channels 
    //McuWriteReg(0x1E,0x45);

	//DSD Mute Output Value
    //McuWriteReg(0x1D,0x00);

    //McuWriteReg(0x00,BANK_E2_REC0);
    //BK05_0F[3] = 0:spdif, 1:i2s/dsd
    //McuWriteRegMask(0x0F,0x08,0x08);


    //[2:0]:MPLL current, [6]:crystal follow suspend
    //McuWriteReg(0x00,BANK_E2_MISC);
    //McuWriteReg(0x18,0x07);     //47->07

    // Power saving setting
	//McuWriteReg(0x00,BANK_E2_MISC);
    // driving current set to minium
	//McuWriteReg(0x01,0x00);
	//McuWriteReg(0x02,0x00);
	//McuWriteReg(0x03,0x00);
	//McuWriteReg(0x04,0x00);
	//McuWriteReg(0x05,0x00);
	//McuWriteReg(0x06,0x40);

    //Play / Record port clock control.
	//McuWriteReg(0x00,BANK_CLOCK);
    //McuWriteRegMask(0x01, AudioClockSystem, 0x07);
	//McuWriteReg(0x09,UtmiClockCtrl);

    // disable ep0 state machine timeout 
    //McuWriteReg(0x00,BANK_EP0);
    //McuWriteRegMask(0x01,0x00,0x01);


    //High Speed Dynamic Power Saving
    //McuWriteReg(0x00,BANK_UTMI);
    //McuWriteRegMask(0x0B,0x00,0x08);   
    //McuWriteRegMask(0x23,0x20,0x60);   
    //McuWriteRegMask(0x2A,0x02,0x03);   
    //McuWriteReg(0x29,0x28);    //bit3=1
	hw_regs[0x00] = old_bank;
}
/* 
WORD AudioConvertVol(WORD value0)
{
    WORD value1;
    
    if(value0 > 0x8000)
        value1 = (0x10000-value0)<<1;
    else
        value1 = 0x0000;
    return value1;
}
*/
//void AudioSetMute(BYTE id,BYTE value)
void AudioSetMute(BYTE id)
{                                                           
    BYTE    old_bank,value[4],mute;
    
    value[0] = McuReadMemS(FU01_MUTE_BASE);
    value[1] = McuReadMemS(FU02_MUTE_BASE);
    value[2] = McuReadMemS(FU03_MUTE_BASE);
    value[3] = McuReadMemS(FU04_MUTE_BASE);
    old_bank = McuReadReg(0x00);
    //value = GET_ADDR_BYTE((iface==IF_PLAYBACK)?P0_CURR_MUTE_ADDR:R0_CURR_MUTE_ADDR);
    /*
    if(iface==1)
        value = GET_ADDR_BYTE(P0_CURR_MUTE_ADDR);
    else
        value = GET_ADDR_BYTE(R0_CURR_MUTE_ADDR);
    */
    switch (id)
    {
        case FU01_ID:
            mute = (value[0]+value[3])?0x01:0x00;
            McuWriteReg(0x00,BANK_DAC0);
            McuWriteRegMask(0x09,mute,0x01); 
            break;
        case FU02_ID:
            mute = (value[1])?0x01:0x00;
            McuWriteReg(0x00,BANK_ADC0);
            McuWriteRegMask(0x05,mute,0x01); 
            break;
        case FU03_ID:
            mute = (value[2]+value[3])?0x01:0x00;
            McuWriteReg(0x00,BANK_DAC0);
            McuWriteRegMask(0x19,mute,0x01); 
            break;
        case FU04_ID:
            McuWriteReg(0x00,BANK_DAC0);
            mute = (value[0]+value[3])?0x01:0x00;
            McuWriteRegMask(0x09,mute,0x01); 
            mute = (value[2]+value[3])?0x01:0x00;
            McuWriteRegMask(0x19,mute,0x01); 
            break;
        default:
            break;
    }
    McuWriteReg(0x00,old_bank);
    DBGPRINT_AUDIO_MUTE("M:[%x]\r\n",mute);
}  

WORD AudioGetVolValue(BYTE type)
{
    WORD    value[4],ret_value;
    DWORD   temp_value=0;
    switch(type)
    {
        case 0x11:
            value[0] = GET_ADDR_WORD(P0_CURR_VOL0_ADDR);
            value[1] = GET_ADDR_WORD(P0_CURR_VOL0_ADDR+10);
            value[2] = GET_ADDR_WORD(SP_CURR_VOL0_ADDR);
            value[3] = GET_ADDR_WORD(SP_CURR_VOL0_ADDR+10);
            //ret_value = (~value[0]+1)+(~value[1]+1)+(~value[2]+1)+(~value[3]+1);
            temp_value = (DWORD)(~value[0]+1)+(DWORD)(~value[1]+1)+(DWORD)(~value[2]+1)+(DWORD)(~value[3]+1);
            break;
        case 0x12:
            value[0] = GET_ADDR_WORD(P0_CURR_VOL0_ADDR);
            value[1] = GET_ADDR_WORD(P0_CURR_VOL0_ADDR+20);
            value[2] = GET_ADDR_WORD(SP_CURR_VOL0_ADDR);
            value[3] = GET_ADDR_WORD(SP_CURR_VOL0_ADDR+20);
            //ret_value = (~value[0]+1)+(~value[1]+1)+(~value[2]+1)+(~value[3]+1);
            temp_value = (DWORD)(~value[0]+1)+(DWORD)(~value[1]+1)+(DWORD)(~value[2]+1)+(DWORD)(~value[3]+1);
            break;
        case 0x21:
            value[0] = GET_ADDR_WORD(R0_CURR_VOL0_ADDR);
            value[1] = GET_ADDR_WORD(R0_CURR_VOL0_ADDR+10);
            //ret_value = (~value[0]+1)+(~value[1]+1);
            temp_value = (DWORD)(~value[0]+1)+(DWORD)(~value[1]+1);
            break;
        case 0x22:
            value[0] = GET_ADDR_WORD(R0_CURR_VOL0_ADDR);
            value[1] = GET_ADDR_WORD(R0_CURR_VOL0_ADDR+20);
            //ret_value = (~value[0]+1)+(~value[1]+1);
            temp_value = (DWORD)(~value[0]+1)+(DWORD)(~value[1]+1);
            break;
        case 0x31:
            value[0] = GET_ADDR_WORD(FB_CURR_VOL0_ADDR);
            value[1] = GET_ADDR_WORD(FB_CURR_VOL0_ADDR+10);
            value[2] = GET_ADDR_WORD(SP_CURR_VOL0_ADDR);
            value[3] = GET_ADDR_WORD(SP_CURR_VOL0_ADDR+10);
            //ret_value = (~value[0]+1)+(~value[1]+1)+(~value[2]+1)+(~value[3]+1);
            temp_value = (DWORD)(~value[0]+1)+(DWORD)(~value[1]+1)+(DWORD)(~value[2]+1)+(DWORD)(~value[3]+1);
            break;
        case 0x32:
            value[0] = GET_ADDR_WORD(FB_CURR_VOL0_ADDR);
            value[1] = GET_ADDR_WORD(FB_CURR_VOL0_ADDR+20);
            value[2] = GET_ADDR_WORD(SP_CURR_VOL0_ADDR);
            value[3] = GET_ADDR_WORD(SP_CURR_VOL0_ADDR+20);
            //ret_value = (~value[0]+1)+(~value[1]+1)+(~value[2]+1)+(~value[3]+1);
            temp_value = (DWORD)(~value[0]+1)+(DWORD)(~value[1]+1)+(DWORD)(~value[2]+1)+(DWORD)(~value[3]+1);
            break;
        default:
            break;
    }
    //ret_value = ~((ret_value>0x8000)?0x8000:ret_value)+1;
    //ret_value = (ret_value>0x7FF0)?0xFF00:(ret_value<<1);
    ret_value = (temp_value>0x7F00)?0xFF00:(WORD)(temp_value<<1);
    DBGPRINT_AUDIO_VOL("Vol:[%x,",type);
    DBGPRINT_AUDIO_VOL("%x",HIBYTE(value[0]));
    DBGPRINT_AUDIO_VOL("%x",LOBYTE(value[0]));
	DBGPRINT_AUDIO_VOL("+%x",HIBYTE(value[1]));
	DBGPRINT_AUDIO_VOL("%x",LOBYTE(value[1]));
    DBGPRINT_AUDIO_VOL("+%x",HIBYTE(value[2]));
    DBGPRINT_AUDIO_VOL("%x",LOBYTE(value[2]));
	DBGPRINT_AUDIO_VOL("+%x",HIBYTE(value[3]));
	DBGPRINT_AUDIO_VOL("%x",LOBYTE(value[3]));
    DBGPRINT_AUDIO_VOL(">>%x",HIBYTE(ret_value));
    DBGPRINT_AUDIO_VOL("%x]\r\n",LOBYTE(ret_value));
    return ret_value;
}
//void AudioSetVolume(BYTE type)
void AudioSetVolume(BYTE id)
{
    BYTE old_bank;
    WORD valueL1,valueL2,valueR1,valueR2;

    switch(id)
    {
        case FU01_ID:
            valueL1 = AudioGetVolValue(0x11);
            valueR1 = AudioGetVolValue(0x12);
            McuWriteReg(0x00,BANK_DAC0);
            McuWriteReg(0x06,HIBYTE(valueL1));
            McuWriteReg(0x05,LOBYTE(valueL1));
            McuWriteReg(0x08,HIBYTE(valueR1));
            McuWriteReg(0x07,LOBYTE(valueR1));
            break;
        case FU03_ID:
            valueL2 = AudioGetVolValue(0x31);
            valueR2 = AudioGetVolValue(0x32);

            McuWriteReg(0x00,BANK_DAC0);
            McuWriteReg(0x16,HIBYTE(valueL2));
            McuWriteReg(0x15,LOBYTE(valueL2));
            McuWriteReg(0x18,HIBYTE(valueR2));
            McuWriteReg(0x17,LOBYTE(valueR2));
            break;
          case FU04_ID:

            valueL1 = AudioGetVolValue(0x11);
            valueR1 = AudioGetVolValue(0x12);
            valueL2 = AudioGetVolValue(0x31);
            valueR2 = AudioGetVolValue(0x32);

            McuWriteReg(0x00,BANK_DAC0);
            McuWriteReg(0x06,HIBYTE(valueL1));
            McuWriteReg(0x05,LOBYTE(valueL1));
            McuWriteReg(0x08,HIBYTE(valueR1));
            McuWriteReg(0x07,LOBYTE(valueR1));
            McuWriteReg(0x16,HIBYTE(valueL2));
            McuWriteReg(0x15,LOBYTE(valueL2));
            McuWriteReg(0x18,HIBYTE(valueR2));
            McuWriteReg(0x17,LOBYTE(valueR2));
            break;
        case FU02_ID:
            valueL1 = AudioGetVolValue(0x21);
            valueR1 = AudioGetVolValue(0x22);
            McuWriteReg(0x00,BANK_ADC0);
            McuWriteReg(0x02,HIBYTE(valueL1));
            McuWriteReg(0x01,LOBYTE(valueL1));
            McuWriteReg(0x04,HIBYTE(valueR1));
            McuWriteReg(0x03,LOBYTE(valueR1));
            break;
        default:
            break;
    }
/*
    BYTE    old_bank;
    WORD    value,temp,value1,temp1;    
    BYTE    ch;

    ch = type&0x0F;
    old_bank = McuReadReg(0x00);
    switch(type&0xF0)
    {
        case 0x10:
        case 0x11:
            McuWriteReg(0x00,BANK_DAC0);
            value = GET_ADDR_WORD(P0_CURR_VOL0_ADDR);
            temp  = AudioConvertVol(value);
            
            value1 = GET_ADDR_WORD(P0_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*1);
            temp1 = AudioConvertVol(value1);
            McuWriteReg(0x06,HIBYTE(temp+temp1));
            McuWriteReg(0x05,LOBYTE(temp+temp1));
            
            value1 = GET_ADDR_WORD(P0_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*2);
            temp1 = AudioConvertVol(value1);
            McuWriteReg(0x08,HIBYTE(temp+temp1));
            McuWriteReg(0x07,LOBYTE(temp+temp1));
        break;
        case 0x20:
        case 0x21:
            McuWriteReg(0x00,BANK_ADC0);
            value = GET_ADDR_WORD(R0_CURR_VOL0_ADDR);
            temp  = AudioConvertVol(value);

            value1 = GET_ADDR_WORD(R0_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*1);
            temp1 = AudioConvertVol(value1);
            McuWriteReg(0x02,HIBYTE(temp+temp1));
            McuWriteReg(0x01,LOBYTE(temp+temp1));
            
            value1 = GET_ADDR_WORD(R0_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*2);
            temp1 = AudioConvertVol(value1);
            McuWriteReg(0x04,HIBYTE(temp+temp1));
            McuWriteReg(0x03,LOBYTE(temp+temp1));
        break;
        case 0x30:
            McuWriteReg(0x00,BANK_DAC0);
            value = GET_ADDR_WORD(FB_CURR_VOL0_ADDR);
            temp  = AudioConvertVol(value);
            
            value1 = GET_ADDR_WORD(FB_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*1);
            temp1 = AudioConvertVol(value1);
            McuWriteReg(0x16,HIBYTE(temp+temp1));
            McuWriteReg(0x15,LOBYTE(temp+temp1));
            
            value1 = GET_ADDR_WORD(FB_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*2);
            temp1 = AudioConvertVol(value1);
            McuWriteReg(0x18,HIBYTE(temp+temp1));
            McuWriteReg(0x17,LOBYTE(temp+temp1));
        break;
        default:
        break;
    }
    */
    /*
    if(type&0x10)
    {
        McuWriteReg(0x00,BANK_DAC0);
        value = GET_ADDR_WORD(P0_CURR_VOL0_ADDR);
        temp  = AudioConvertVol(value);
        
        value1 = GET_ADDR_WORD(P0_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*1);
        temp1 = AudioConvertVol(value1);
        McuWriteReg(0x06,HIBYTE(temp+temp1));
        McuWriteReg(0x05,LOBYTE(temp+temp1));
        
        value1 = GET_ADDR_WORD(P0_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*2);
        temp1 = AudioConvertVol(value1);
        McuWriteReg(0x08,HIBYTE(temp+temp1));
        McuWriteReg(0x07,LOBYTE(temp+temp1));
    }
    else if(type&0x20)
    {        
        McuWriteReg(0x00,BANK_ADC0);
        value = GET_ADDR_WORD(R0_CURR_VOL0_ADDR);
        temp  = AudioConvertVol(value);

        value1 = GET_ADDR_WORD(R0_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*1);
        temp1 = AudioConvertVol(value1);
        McuWriteReg(0x02,HIBYTE(temp+temp1));
        McuWriteReg(0x01,LOBYTE(temp+temp1));
        
        value1 = GET_ADDR_WORD(R0_CURR_VOL0_ADDR+VOLUME_CH_OFFSET*2);
        temp1 = AudioConvertVol(value1);
        McuWriteReg(0x04,HIBYTE(temp+temp1));
        McuWriteReg(0x03,LOBYTE(temp+temp1));

    }
    */

/*
    BYTE    old_bank,i;
    WORD    value,temp,value1,temp1;    
    old_bank = McuReadReg(0x00);
    value = GET_ADDR_WORD(P0_CURR_VOL0_ADDR+10*(WORD)ch);
    temp  = AudioConvertVol(value);
    McuWriteReg(0x00,BANK_E2_PLAY0);
                
    if(ch==0)
    {
    
        for(i=1;i<=2;i++)
        {
            value1 = GET_ADDR_WORD(P0_CURR_VOL0_ADDR+16*(WORD)i);
            temp1 = AudioConvertVol(value1);
            McuWriteReg(0x07+2*(i-1),HIBYTE(temp+temp1));
            McuWriteReg(0x06+2*(i-1),LOBYTE(temp+temp1));
        }
    
    }
    else
    {
        value1 = GET_ADDR_WORD(P0_CURR_VOL0_ADDR);
        temp1  = AudioConvertVol(value1);
        McuWriteReg(0x07+2*(ch-1),HIBYTE(temp+temp1));
        McuWriteReg(0x06+2*(ch-1),LOBYTE(temp+temp1));
    }
    McuWriteReg(0x00,old_bank);
*/
    /*
    printf("V:[%x,",ch);
    printf("%x",HIBYTE(value));
    printf("%x",LOBYTE(value));
	printf("+%x",HIBYTE(value1));
	printf("%x",LOBYTE(value1));
    printf(">>%x",HIBYTE(temp+temp1));
    printf("%x]\r\n",LOBYTE(temp+temp1));
    */

    McuWriteReg(0x00,old_bank);
    /*
    DBGPRINT_AUDIO_VOL("V:[%x,",ch);
    DBGPRINT_AUDIO_VOL("%x",HIBYTE(value));
    DBGPRINT_AUDIO_VOL("%x",LOBYTE(value));
	DBGPRINT_AUDIO_VOL("+%x",HIBYTE(value1));
	DBGPRINT_AUDIO_VOL("%x",LOBYTE(value1));
    DBGPRINT_AUDIO_VOL(">>%x",HIBYTE(temp+temp1));
    DBGPRINT_AUDIO_VOL("%x]\r\n",LOBYTE(temp+temp1));
    DBGPRINT_AUDIO_VOL("SetVol:[%x]\r\n",type);
    */
//    printf("SetVolume:[%x]\r\n",type);
}
void AudioSetPlayClock(BYTE index)
{
	BYTE 	old_bank, i, index_prev, old_value;
	DWORD	temp_value;
	old_bank = McuReadReg(0x00);

    McuWriteReg(0x00,BANK_SYNTH1);
	temp_value=(index&0x01)?CLOCK_48K_SETTING:CLOCK_44K_SETTING;
	temp_value >>= (index/2);
	for(i=0;i<4;i++)
	{
		McuWriteReg(0x08+i,temp_value&0xFF);
		temp_value >>= 8;	    
	}
    // for playback synchronous mode
    McuWriteReg(0x06,LOBYTE(FREQ_SYNC_SETTING[index]));
    McuWriteReg(0x07,HIBYTE(FREQ_SYNC_SETTING[index]));
    // preload clock
    //index_prev = au.format.freq;
    index_prev = au.stream[0].freq;
    if(index != index_prev)
    {
        //reload init value
        McuWriteRegMask(0x0C,0x80,0x80);
        DBGPRINT_AUDIO_FORMAT("clk:%i->",index_prev);
        DBGPRINT_AUDIO_FORMAT("%i\r\n",index);
        //try KP mode first;
        old_value = McuReadReg(0x0D);
        McuWriteRegMask(0x0D,0x20,0x20);
        McuWriteReg(0x0D,old_value);
    }
    McuWriteReg(0x00,old_bank);
    //DBGPRINT_AUDIO_FORMAT("clock:%x\r\n",index_prev);
}
void AudioSetRecClock(BYTE index)
{
	BYTE 	old_bank;
	old_bank = McuReadReg(0x00);

    McuWriteReg(0x00,BANK_REC0);
    McuWriteRegMask(0x43,INDEX_10_TO_15(index),0x0F);   //1:32K, 2:44.1K, 3:48K, ....
    
    McuWriteReg(0x00,old_bank);
}
void AudioSetPlayback(BYTE bits)
{
	BYTE 	old_bank;
    //WORD    ready_timeout;
//    WORD    end_base,buffer_len;
	                            
	old_bank = McuReadReg(0x00);	
    McuWriteReg(0x00,BANK_EP1);
    //printf("AudioSetPlayback(%x)\r\n",hw_regs[0x05]);

    //uac1: 0x80(~=2.1ms), uac2: 0x10~=0.273ms)
    //ready_timeout=0x0080;       
    //McuWriteReg(0x06,HIBYTE(ready_timeout));
    //McuWriteReg(0x05,LOBYTE(ready_timeout));
    //buffer_len = (bits==24)?252:256;
    //end_base = buffer_len -1;
    //McuWriteReg(0x04,HIBYTE(end_base));	        //ep1/4 sram start (high)
    //McuWriteReg(0x03,LOBYTE(end_base));	        //ep1/4 sram start (low)
	McuWriteRegMask(0x07,4-(bits/8),0x03);	    //sample bits{1:0] 16:2, 24:1, 32:0

    McuWriteReg(0x00,old_bank);
}
void AudioSetRecord(BYTE fi, BYTE bits)
{
	BYTE 	old_bank,ch;
	//WORD	end_base,buffer_len;
	WORD	min_len,max_len,pre_len,frame_num;

	old_bank = McuReadReg(0x00);	
	McuWriteReg(0x00,BANK_EP1);

    //buffer_len = (bits==24)?510:512;
    //end_base = buffer_len -1;
    //McuWriteReg(0x23,HIBYTE(end_base));	        //ep2 sram start (high)
    //McuWriteReg(0x22,LOBYTE(end_base));	        //ep2 sram start (low)
	McuWriteRegMask(0x28,4-(bits/8),0x03);	    //sample bits{1:0] 10:16, 01:24, 00:32

	//McuWriteRegMask(0x09,fifo_size,0x3F);	    //wrfifo size
    

    //frame_num = (au.hw.uac1)?10:80;
    /*
    if(au.uac_type == TYPE_UAC2)
    ch = (bits==16)?ALT_RECORD_16B_CH:2; //au.rec_ch;
    else
    ch = (bits==16)?UAC1_REC0_ALT1_CH:2; //au.rec_ch;
    */
    ch=2;
	McuWriteRegMask(0x2B,ch,0x0F);	            //max ch
    
    frame_num =  (au.uac_type == TYPE_UAC2)?80:10;
    // set min/max len, (samples per frame)*ch
    min_len = ((FREQ_SYNC_SETTING[fi]-1)/frame_num)*((WORD)ch)-1;
    max_len = ((FREQ_SYNC_SETTING[fi]+frame_num)/frame_num)*((WORD)ch)-1;
    //preload samples before first 
    pre_len = (FREQ_SYNC_SETTING[fi]/frame_num)*(bits/8)*((WORD)ch)/2-1;

    McuWriteReg(0x62,HIBYTE(min_len));	        //(min high)
    McuWriteReg(0x61,LOBYTE(min_len));	        //(min low)
    McuWriteReg(0x64,HIBYTE(max_len));	        //(max high)
    McuWriteReg(0x63,LOBYTE(max_len));	        //(max low)
	//McuWriteReg(0x2E,LOBYTE(pre_len));	
	//McuWriteReg(0x2F,HIBYTE(pre_len));
	//printf("r0:<%i,",ch);
    //printf("%i,",bits);
	//printf("fifo:%x>",fifo_size);
	//printf("<len:%x",(min_len>>8)&0xFF);
	//printf("%x,",min_len&0xFF);
	//printf("%x",(max_len>>8)&0xFF);
	//printf("%x,",max_len&0xFF);
	//printf("%x",(pre_len>>8)&0xFF);
	//printf("%x>\r\n",pre_len&0xFF);
	
    //McuWriteReg(0x00,BANK_E2_REC0);
 	//McuWriteRegMask(0x1A,(ch-1),0x07);	        //channel num for pll

    McuWriteReg(0x00,old_bank);
}
void AudioSetPlayType(BYTE index)
{
	BYTE 	old_bank;
    old_bank = McuReadReg(0x00);

//    if(au.drv_cmd==DRIVER_CMD_NDSD_PLAY0_ON || au.drv_cmd==DRIVER_CMD_DSD_PLAY0_START)
//        index = ALT_PLAYBACK_NDSD;
/*
    switch(index)
    {
        case ALT_PLAYBACK_NDSD:
            McuWriteRegMask(0x05,0xC0,0xE0);    //NDSD
            McuWriteRegMask(0x24,0x00,0x02);    //SPDIF CS:PCM
        au.stream[0].ndsd = 1;
            break;
        case ALT_PLAYBACK_DTS:
            McuWriteRegMask(0x05,0x20,0xE0);    //NPCM
            McuWriteRegMask(0x24,0x02,0x02);    //SPDIF CS:NPCM
            au.stream[0].ndsd = 0;
            break;
        default:
            McuWriteRegMask(0x05,0x00,0xE0);    //PCM
            McuWriteRegMask(0x24,0x00,0x02);    //SPDIF CS:PCM
            au.stream[0].ndsd = 0;
            break;
    }
*/
    
    McuWriteReg(0x00,BANK_PLAY0);
    if(ALT_PLAYBACK_NDSD==index){
        McuWriteRegMask(0x05,0xC0,0xE0);    //NDSD
        McuWriteRegMask(0x24,0x00,0x02);    //SPDIF CS:PCM
        au.stream[0].type = 3;              //B1:NPCM, B0:NDSD
    }
    else if(ALT_PLAYBACK_DTS==index){ 
        McuWriteRegMask(0x05,0x20,0xE0);    //NPCM
        McuWriteRegMask(0x24,0x02,0x02);    //SPDIF CS:NPCM
        au.stream[0].type = 2;              //B1:NPCM, B0:NDSD
    }
    else{
        McuWriteRegMask(0x05,0x00,0xE0);    //PCM
        McuWriteRegMask(0x24,0x00,0x02);    //SPDIF CS:PCM
        au.stream[0].type = 0;              //B1:NPCM, B0:NDSD
    }
    McuWriteReg(0x00,old_bank);
}
void AudioGetStatus(void)
{
	BYTE 	old_bank;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_PLAY0);
    if(McuReadReg(0x18)&0x08)
    {
        if(!au.stream[0].ready) au.stream[0].flags |= PLAYBACK_FLAGS_READY;
        au.stream[0].ready = 1;
    }
    else
    {
        if(au.stream[0].ready) au.stream[0].flags |= PLAYBACK_FLAGS_NOT_READY;
        au.stream[0].ready = 0;
    }
    if(McuReadReg(0x18)&0x02)
    {
        if(!au.stream[0].dop) au.stream[0].flags |= PLAYBACK_FLAGS_DOP_CHANGE;
        au.stream[0].dop = 1;
    }
    else
    {
        if(au.stream[0].dop) au.stream[0].flags |= PLAYBACK_FLAGS_DOP_CHANGE;
        au.stream[0].dop = 0;
    }
    McuWriteReg(0x00,old_bank);
}
/*
void AudioSetOutput(void)
{
	BYTE 	old_bank;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_PLAY0);

    //if(au.stream[0].ndsd || au.stream[0].dop)
    if((au.stream[0].type==3)||au.stream[0].dop)
    {
        McuWriteRegMask(0x03,0x10,0x10);      //DSD output
        //printf("DSD Out\r\n",0);
    }
    else
    {
        McuWriteRegMask(0x03,0x00,0x10);      //PCM output
        //printf("PCM Out\r\n",0);
    }
    McuWriteReg(0x00,old_bank);
}
*/
BYTE AudioGetFreqIndex(BYTE iface)
{

    BYTE    i;
    DWORD   freq;
    DWORD   freq_sets[FREQ_MAX_COUNT]={ SAMPLE_RATE_44K, SAMPLE_RATE_48K, SAMPLE_RATE_88K, SAMPLE_RATE_96K, SAMPLE_RATE_176K, SAMPLE_RATE_192K,
                        SAMPLE_RATE_352K, SAMPLE_RATE_384K, SAMPLE_RATE_705K, SAMPLE_RATE_768K,};

    freq = (IF_PLAYBACK==iface)?McuReadMem4(P0_CURR_FREQ_ADDR):McuReadMem4(R0_CURR_FREQ_ADDR);
    for(i=0;i<FREQ_MAX_COUNT;i++){
        if(freq==freq_sets[i]) break;
    }
	//for(i=0;i<FREQ_MAX_COUNT;i++){
    //    DWORD   check_freq;        
    //    check_freq = (i%2)?SAMPLE_RATE_48K:SAMPLE_RATE_44K;
    //    check_freq <<= i/2;
	//	if(freq == check_freq) break;
	//}
    //DBGPRINT_AUDIO_FORMAT("idx:%x\r\n",i);
    if(i>=10) i=1;
    return i;

/*
    DWORD check_freq,freq=0;
    BYTE i;
    switch(iface)
    {
        case IF_PLAYBACK:
            //freq   = GET_ADDR_DWORD(P0_CURR_FREQ_ADDR);
            freq   = McuReadMem4(P0_CURR_FREQ_ADDR);
            break;
        case IF_RECORD:
            //freq   = GET_ADDR_DWORD(R0_CURR_FREQ_ADDR);
            freq   = McuReadMem4(R0_CURR_FREQ_ADDR);
            break;
        default:
            break;
    }

	for(i=0;i<10;i++)
	{
        check_freq = (i%2)?SAMPLE_RATE_48K:SAMPLE_RATE_44K;
        check_freq <<= i/2;
		if(freq == check_freq)
		{
			break;
		}
	}
    if(i>=10) i=1;
    return i;
*/
}    
void AudioDoVolume(BYTE type)
{
    AudioSetVolume(type);
}
void AudioDoMute(BYTE id)
{
//    BYTE value;
    switch(id)
    {
        case FU01_ID:
            //value = McuReadMemS(FU01_MUTE_BASE);
            //AudioSetMute(id, value);
            AudioSetMute(id);
            //CodecSetMute(value);
            break;
        case FU02_ID:
            //value = McuReadMemS(FU02_MUTE_BASE);
            //AudioSetMute(id, value);
            AudioSetMute(id);
            break;
        case FU03_ID:
            //value = McuReadMemS(FU03_MUTE_BASE);
            //AudioSetMute(id, value);
            AudioSetMute(id);
            //CodecSetMute(value);
            break;
        case FU04_ID:
            //value = McuReadMemS(FU04_MUTE_BASE);
            //AudioSetMute(id, value);
            AudioSetMute(id);
            //CodecSetMute(value);
            break;
        default:
            break;
    }
    //printf("iface:%x",iface);
    //value = GET_ADDR_BYTE((iface==IF_PLAYBACK)?P0_CURR_MUTE_ADDR:R0_CURR_MUTE_ADDR);
    //value = McuReadMemS((iface==IF_PLAYBACK)?FU01_MUTE_BASE:FU02_MUTE_BASE);
    //printf("value:%x\r\n",value);
    //AudioSetMute(index, value);
    //AudioSetMute(1);
    //CodecSetMute(value);
}

void AudioDoFormat(BYTE iface)
{
    BYTE    alt_idx,freq_idx,bits;
    //BYTE    bits_table[]={16,24,32,32,32};
     
    freq_idx    = AudioGetFreqIndex(iface);
        
    if(IF_PLAYBACK==iface){
        alt_idx = au.stream[0].alt;
        if(ALT_PLAYBACK_16B==alt_idx){
            bits = 16;
        } 
        else if (ALT_PLAYBACK_24B==alt_idx){
            bits = 24;
        }
        else {
            bits = 32;
        }
        AudioSetPlayClock(freq_idx);    
        AudioSetPlayback(bits);
        AudioSetPlayType(alt_idx);
        au.stream[0].freq = freq_idx;
 		//printf("p0:[bits:%d]\r\n",bits);
    }
    else if(IF_RECORD==iface){ 
        alt_idx = au.stream[1].alt;
        if(ALT_RECORD_16B==alt_idx){
            bits = 16;
        } 
        else if (ALT_RECORD_24B==alt_idx){
            bits = 24;
        }
        else {
            bits = 32;
        }
        AudioSetRecClock(freq_idx);    
        AudioSetRecord(freq_idx,bits);
        au.stream[1].freq = freq_idx;
		//printf("r0:[bits:%d]\r\n",bits);
    }
    /*
    alt_idx     = (iface==IF_PLAYBACK)?au.stream[0].alt:au.stream[1].alt;
    bits        = bits_table[alt_idx-1];
    switch(iface)
    {
        case IF_PLAYBACK:
            //alt_idx     = au.format.p0_alt;
            //bits = play_info[alt_idx-1];  
            //bits = (alt_idx<3)?(alt_idx*8+8):32;  
            AudioSetPlayClock(freq_idx);    
            AudioSetPlayback(bits);
            AudioSetPlayType(alt_idx);
     		//printf("p0:[bits:%d]\r\n",bits);
            au.stream[0].freq = freq_idx;
            break;
        case IF_RECORD:
            //alt_idx     = au.format.r0_alt;
            //bits        = rec_info[alt_idx-1]; ;
            //au.rec_ch   = 2;  
            AudioSetRecClock(freq_idx);    
            AudioSetRecord(freq_idx,bits);
    		//printf("r0:[bits:%d]\r\n",bits);
            au.stream[1].freq = freq_idx;
            break;
        default:
     		printf("xx:[0]\r\n",0);
            break;
    }
    //au.format.freq = freq_idx;
    */
    DBGPRINT_AUDIO_FORMAT("if/fr/alt:[%x,",iface);
    DBGPRINT_AUDIO_FORMAT("%x,",freq_idx);
    DBGPRINT_AUDIO_FORMAT("%x]\r\n",alt_idx);
    //printf("if/fr/alt:[%x,",iface);
    //printf("%x,",freq_idx);
    //printf("%x]\r\n",alt_idx);
}

