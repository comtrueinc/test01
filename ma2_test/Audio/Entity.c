#include "Reg51.h"
#include "Mcu.h"
#include "Uart.h"
#include "Audio.h"
//#include "Entity.h"

#define _CTUSB_ENTITY_C_
#include "Entity.h"
#undef  _CTUSB_ENTITY_C_

BYTE xdata usb_entity_data[USB_ENTITY_DATA_ADDR_LEN]       _at_    (USB_ENTITY_DATA_BASE);      // 0x0400;	

#define     ENTITY_PRINT_REG            0
#define     ENTITY_PRINT_FREQ_RANGE     0

//WORD SUPPORT_TIMING = SupportTiming;

//code WORD FREQ_SYNC_SETTING[10]={441,480,882,960,1764,1920,3528,3840,7056,7680};
/*
DWORD uac2_play0_freq_sets[]={
    SAMPLE_RATE_44K,    SAMPLE_RATE_48K,
    SAMPLE_RATE_88K,    SAMPLE_RATE_96K,
    SAMPLE_RATE_176K,   SAMPLE_RATE_192K,
    SAMPLE_RATE_352K,   SAMPLE_RATE_384K,
    SAMPLE_RATE_705K,   SAMPLE_RATE_768K,
};
DWORD uac2_rec0_freq_sets[]={
    SAMPLE_RATE_44K,    SAMPLE_RATE_48K,
    SAMPLE_RATE_88K,    SAMPLE_RATE_96K,
    SAMPLE_RATE_176K,   SAMPLE_RATE_192K,
    SAMPLE_RATE_352K,   SAMPLE_RATE_384K,
    SAMPLE_RATE_705K,   SAMPLE_RATE_768K,
};
*/
void EntityInit(void)
{
    printf("EntityInit(base=%x",HIBYTE(USB_ENTITY_DATA_BASE));
    printf("%x)\r\n",LOBYTE(USB_ENTITY_DATA_BASE));
    EntitySetFreqBase();
#if (UAC_TYPE == TYPE_UAC2)
    EntitySetBase();
    EntitySetFreqRange(IF_PLAYBACK);        //Playback
    EntitySetFreqRange(IF_RECORD);          // Record
#endif
    
    EntitySetJackBase();

    EntitySetVolumeBase();
    EntitySetVolumeRange(FU01_ID);        //Playback
    EntitySetVolumeRange(FU02_ID);        //Record
    EntitySetVolumeRange(FU03_ID);        //Feedback
    EntitySetVolumeRange(FU04_ID);        //Speaker

}
void EntitySetFreqBase(void)
{
    BYTE    reg;
    WORD    base;//,addr;
    //DWORD   temp_freq; 

	McuWriteReg(0x00,BANK_EP0);

#if(UAC_TYPE == TYPE_UAC2)
    // for Playback Freq current
    reg         = CS01_FREQ_CURR_REG;    
    base        = CS01_FREQ_CURR_BASE;
    //temp_freq   = SAMPLE_RATE_48K;
    
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x0F);	
    //McuWriteRegMask(reg+1,0x20,0x20);   //ack handshake
    McuWriteRegMask(reg+1,0xE0,0xE0);   //RW+ack handshake
    McuWriteReg(reg+2,0x01);            //Curr : B[1:0]=01
    McuWriteReg(reg+3,0x01);	        //CS=1 (Control Selector/Freq)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,CS01_ID);	        //UnitID
    McuWriteReg(reg+6,IF_CONTROL);      //interface=0
    McuWriteReg(reg+7,0x04);            //len=4 (11:0) 
    //McuWriteRegMask(reg+8,0x00,0x3F);   //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)

    // Set current frequency
    //addr = base;
    //McuWriteMemS(addr++,(BYTE)(temp_freq&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>8)&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>16)&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>24)&0xFF));   
    McuWriteMem4(base,SAMPLE_RATE_48K);
    DEBUG_PRINT(ENTITY_PRINT_REG,"CS01_FREQ_CURR_REG:%x\r\n",reg);

    //for Record 
    reg     = CS02_FREQ_CURR_REG;    
    base    = CS02_FREQ_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    //McuWriteRegMask(reg+1,0x20,0x20);   //ack handshake
    McuWriteRegMask(reg+1,0xE0,0xE0);   //RW+ack handshake
    McuWriteReg(reg+2,0x01);            //B[1:0]=01 (1:Curr, 2:Range)
    McuWriteReg(reg+3,0x01);	        //CS=1 (Control Selector)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,CS02_ID);	        //UnitID
    //McuWriteReg(reg+6,0);               //Inteface index=0
    McuWriteReg(reg+6,IF_CONTROL);      //Inteface index=0
    McuWriteReg(reg+7,0x04);            //len=4
    //McuWriteRegMask(reg+8,0x00,0x3F);   //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    

    // Set current frequency    
    //addr = base;
    //McuWriteMemS(addr++,(BYTE)(temp_freq&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>8)&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>16)&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>24)&0xFF));    
    McuWriteMem4(base,SAMPLE_RATE_48K);
    DEBUG_PRINT(ENTITY_PRINT_REG,"CS02_FREQ_CURR_REG:%x\r\n",reg);


    //for Playbck Clock Valid
    reg     = CS01_FREQ_VALID_REG;
    base    = CS01_VALID_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x01);            //current : B[1:0]=01
    McuWriteReg(reg+3,0x02);	        //CS=2 (Control Selector/Vaild)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,CS01_ID);	        //UnitID
    //McuWriteReg(reg+6,0);               //Inteface=0
    McuWriteReg(reg+6,IF_CONTROL);      //Inteface=1
    McuWriteReg(reg+7,1);               //len=1
    //McuWriteRegMask(reg+8,0x00,0x3F);   //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    
    // Set current clock valid TRUE
    McuWriteMemS(base,1);
    DEBUG_PRINT(ENTITY_PRINT_REG,"CS01_FREQ_VALID_REG:%x\r\n",reg);

    //for Record Clock Valid
    reg = CS02_FREQ_VALID_REG;
    base = CS02_VALID_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x01);            //B[1:0]=01 (1:curr, 2:range)
    McuWriteReg(reg+3,0x02);	        //CS=2 (Control Selector/2:Valid)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,CS02_ID);	        //UnitID
    //McuWriteReg(reg+6,0);      //Inteface index=0
    McuWriteReg(reg+6,IF_CONTROL);      //Inteface index=0
    McuWriteReg(reg+7,1);               //len=1
    //McuWriteRegMask(reg+8,0x00,0x3F); //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    
    // Set current clock valid TRUE
    McuWriteMemS(base,1);
    DEBUG_PRINT(ENTITY_PRINT_REG,"CS02_FREQ_VALID_REG:%x\r\n",reg);

#else
    reg = CS01_FREQ_CURR_REG;    
    base = CS01_FREQ_CURR_BASE;
//    temp_freq = SAMPLE_RATE_48K;
    
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xE0,0xE0);   //RW
    McuWriteReg(reg+2,0x01);            //SetCurr : B[1:0]=01
    McuWriteReg(reg+3,0x01);	        //CS=1 (Control Selector)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,0x00);	        //UnitID
    McuWriteReg(reg+6,0x01);            //endpiont=0x01
    McuWriteReg(reg+7,0x04);            //len=4
    McuWriteRegMask(reg+8,0x00,0x3F);   //
    McuWriteRegMask(reg+8,0x80,0xC0);   //type:10(endpoint)

    reg += 9;        
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x81);            //GetCurr : B[1:0]=01
    McuWriteReg(reg+3,0x01);	        //CS=1 (Control Selector)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,0x00);	        //UnitID
    McuWriteReg(reg+6,0x01);            //endpiont=0x01
    McuWriteReg(reg+7,0x03);            //len=4
    McuWriteRegMask(reg+8,0x00,0x3F);   //
    McuWriteRegMask(reg+8,0x80,0xC0);   //type:10(endpoint)
   // Set current frequency
    
    //addr = base;
    //McuWriteMemS(addr++,(BYTE)(temp_freq&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>8)&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>16)&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>24)&0xFF));
    
    McuWriteMem4(base,SAMPLE_RATE_48K);

    reg = CS02_FREQ_CURR_REG;    
    base = CS02_FREQ_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x01);            //SetCurr : B[1:0]=01
    McuWriteReg(reg+3,0x01);	        //CS=1 (Control Selector)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,0x00);	        //UnitID
    McuWriteReg(reg+6,0x82);            //endpoint=2
    McuWriteReg(reg+7,0x04);            //len=4
    McuWriteRegMask(reg+8,0x00,0x3F);   //
    McuWriteRegMask(reg+8,0x80,0xC0);   //type:10(endpoint)    

    reg +=9;    
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x81);            //Get_Curr : B[1:0]=01
    McuWriteReg(reg+3,0x01);	        //CS=1 (Control Selector)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,0x00);	        //UnitID
    McuWriteReg(reg+6,0x82);            //endpoint=2
    McuWriteReg(reg+7,0x04);            //len=4
    McuWriteRegMask(reg+8,0x00,0x3F);   //
    McuWriteRegMask(reg+8,0x80,0xC0);   //type:10(endpoint)    
    // Set current frequency
    
    //addr = base;
    //McuWriteMemS(addr++,(BYTE)(temp_freq&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>8)&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>16)&0xFF));
    //McuWriteMemS(addr++,(BYTE)((temp_freq>>24)&0xFF));
    
    McuWriteMem4(base,SAMPLE_RATE_48K);
#endif

}

#if(UAC_TYPE == TYPE_UAC2)
void EntitySetFreqRange(BYTE iface)
{
    BYTE    reg,i,j,id;
    WORD    base,addr,freq_count, freq_flags;
    DWORD   temp_freq; 
    DWORD   active_freq_set[FREQ_MAX_COUNT]={0}; 

    switch(iface)
    {
    case IF_PLAYBACK:
        // for Playback Freq Range
        reg  = CS01_FREQ_RANGE_REG;    
        base = CS01_FREQ_RANGE_BASE;
        //freq_count = UAC2_PLAY0_FREQ_COUNT;
        freq_count = PLAY0_FREQ_COUNT;
        freq_flags = PLAY0_FREQ_FLAGS;
        id = CS01_ID; 
        break;
#if(RECORD_ENABLE==1)
    case IF_RECORD:
        // for Record Freq Range
        reg  = CS02_FREQ_RANGE_REG;    
        base = CS02_FREQ_RANGE_BASE;
        //freq_count = UAC2_REC0_FREQ_COUNT;
        freq_count = REC0_FREQ_COUNT;
        freq_flags = REC0_FREQ_FLAGS;
        id = CS02_ID; 
        break;
#endif
    default:
        break;
    }

    for(i=0,j=0;i<FREQ_MAX_COUNT;i++)
    {
        if(freq_flags&(1<<i)){
            active_freq_set[j++]= ((i%2)?(((U32)SAMPLE_RATE_48K)<<(i/2)):(((U32)SAMPLE_RATE_44K))<<(i/2));        
        }
        
    }
    
    DEBUG_PRINT(ENTITY_PRINT_FREQ_RANGE,"freq_flags:%x",HIBYTE(freq_flags));
    DEBUG_PRINT(ENTITY_PRINT_FREQ_RANGE,"%x\r\n",LOBYTE(freq_flags));
    addr = base;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x02);            //Range : B[1:0]=02
    McuWriteReg(reg+3,0x01);	        //CS=1 (Control Selector)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,id);	        //UnitID
    McuWriteReg(reg+6,IF_CONTROL);      //interface=0
    McuWriteReg(reg+7,2+12*freq_count); //len=4
    //McuWriteRegMask(reg+8,0x00,0x3F); //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    

    //McuWriteMemS(addr++,freq_count);
    //McuWriteMemS(addr++,0x00);
    //McuWriteMem2(addr,freq_count);
    addr +=2;
    //for(i=0,j=0;i<freq_count;i++)
    for(i=0,j=0;i<FREQ_MAX_COUNT;i++)
    {
        if(freq_flags&(1<<i)){
            temp_freq = ((i%2)?(((U32)SAMPLE_RATE_48K)<<(i/2)):(((U32)SAMPLE_RATE_44K))<<(i/2)); 
            j++;       
            //temp_freq = (index==1)?uac2_play0_freq_sets[i]:uac2_rec0_freq_sets[i];
            //temp_freq = active_freq_set[i];
        /*
        McuWriteMemS(addr++,(BYTE)(temp_freq&0xFF));
        McuWriteMemS(addr++,(BYTE)((temp_freq>>8)&0xFF));
        McuWriteMemS(addr++,(BYTE)((temp_freq>>16)&0xFF));
        McuWriteMemS(addr++,(BYTE)((temp_freq>>24)&0xFF));
        McuWriteMemS(addr++,(BYTE)(temp_freq&0xFF));
        McuWriteMemS(addr++,(BYTE)((temp_freq>>8)&0xFF));
        McuWriteMemS(addr++,(BYTE)((temp_freq>>16)&0xFF));
        McuWriteMemS(addr++,(BYTE)((temp_freq>>24)&0xFF));
        McuWriteMemS(addr++,H0BYTE(temp_freq));
        McuWriteMemS(addr++,H1BYTE(temp_freq));
        McuWriteMemS(addr++,H2BYTE(temp_freq));
        McuWriteMemS(addr++,H3BYTE(temp_freq));
        McuWriteMemS(addr++,H0BYTE(temp_freq));
        McuWriteMemS(addr++,H1BYTE(temp_freq));
        McuWriteMemS(addr++,H2BYTE(temp_freq));
        McuWriteMemS(addr++,H3BYTE(temp_freq));
        McuWriteMemS(addr++,0);
        McuWriteMemS(addr++,0);
        McuWriteMemS(addr++,0);
        McuWriteMemS(addr++,0);
        */

        McuWriteMem4(addr,      temp_freq);
        McuWriteMem4(addr+4,    temp_freq);
        McuWriteMem4(addr+8,    0);
        addr += 12;

        DEBUG_PRINT(ENTITY_PRINT_FREQ_RANGE,"freq %i: ",j);
        DEBUG_PRINT(ENTITY_PRINT_FREQ_RANGE,"%x",H3BYTE(temp_freq));
        DEBUG_PRINT(ENTITY_PRINT_FREQ_RANGE,"%x",H2BYTE(temp_freq));
        DEBUG_PRINT(ENTITY_PRINT_FREQ_RANGE,"%x",H1BYTE(temp_freq));
        DEBUG_PRINT(ENTITY_PRINT_FREQ_RANGE,"%x\r\n",H0BYTE(temp_freq));
    }
    }
    freq_count = j;
    McuWriteMem2(base,freq_count);
//#if (ENTITY_PRINT_REG)
//    printf("reg:%x\r\n",reg);
//#endif
    DEBUG_PRINT(ENTITY_PRINT_REG,"reg:%x\r\n",reg);
}

void EntitySetBase(void)
{
    BYTE    reg;
    WORD    base;

	McuWriteReg(0x00,BANK_EP0);
    
    //for Playbck Alternate Valid (Control Selector=2)
    //0x01:AS_ACT_ALT_SETTING_CONTROL
    //0x02:AS_VAL_ALT_SETTINGS_CONTROL
    //0x03:AS_AUDIO_DATA_FORMAT_CONTROL:0x03
    reg = AL01_CURR_VALID_REG;
    base = AL01_VALID_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x01);            //current : B[1:0]=02
    McuWriteReg(reg+3,0x02);	        //CS=2 (Control Selector/Vaild)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,0);	            //UnitID
    McuWriteReg(reg+6,IF_PLAYBACK);     //Inteface=1
    McuWriteReg(reg+7,2);               //len=2
    //McuWriteRegMask(reg+8,0x00,0x3F); //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    
    // Set current valid
    McuWriteMemS(base,1);
    //McuWriteMemS(base+1,UAC2_PLAY0_ALT_VALID);
    McuWriteMemS(base+1,PLAY0_ALT_VALID);

    //for Record Alternate Valid
    reg = AL02_CURR_VALID_REG;
    base = AL02_VALID_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x01);            //current : B[1:0]=02
    McuWriteReg(reg+3,0x02);	        //CS=2 (Control Selector/Vaild)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,0);	            //UnitID
    //McuWriteReg(reg+6,0x82);          //Inteface=2
    McuWriteReg(reg+6,IF_RECORD);       //Inteface=2
    McuWriteReg(reg+7,2);               //len=2
    //McuWriteRegMask(reg+8,0x00,0x3F); //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    
    // Set current valid
    McuWriteMemS(base,1);
    //McuWriteMemS(base+1,UAC2_REC0_ALT_VALID);
    McuWriteMemS(base+1,REC0_ALT_VALID);


    // for Mixer
    reg = MX01_CURR_REG;
    base = MX01_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0x40,0xC0);   //RW
    McuWriteReg(reg+2,0x01);            //current : B[1:0]=02
    McuWriteReg(reg+3,0x01);	        //CS=1 (Control Selector/Vaild)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,MX01_ID);	        //UnitID
    McuWriteReg(reg+6,0);               //Inteface=0
    McuWriteReg(reg+7,2);               //len=2
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    
    // Set current valid
    McuWriteMemS(base,0);
    McuWriteMemS(base+1,0);
    McuWriteReg(0xDF,0x07);             //only for mask/skip bit0-2 for offset 4
    DEBUG_PRINT(ENTITY_PRINT_REG,"reg:%x\r\n",reg);
}
#endif    

void EntitySetVolumeRange(BYTE id)
{
    BYTE i;
    WORD addr,vol_curr,vol_min,vol_max,vol_step;

    //for Mute 
    /*
    switch(id)
    {
    case FU01_ID:
        addr = FU01_MUTE_BASE;
        break;
    case FU02_ID:
        addr = FU02_MUTE_BASE;
        break;
    case FU03_ID:
        addr = FU03_MUTE_BASE;
        break;
    case FU04_ID:
        addr = FU04_MUTE_BASE;
        break;
    }
    //ch0-3 mute=0
    //McuWriteMem4(addr,0);
    //McuWriteMemS(addr++,0x00);
    //McuWriteMemS(addr++,0x00);
    //McuWriteMemS(addr++,0x00);
    //McuWriteMemS(addr++,0x00);

    //for Volume
    //vol_num = 1;
    //vol_step= 0x0010;   //(1/256)*16=1/16 dB
    //vol_min = 0x8010;   //-127.9375 
    
    //vol_curr= 0x0000;   //  0.0dB
    //vol_min = 0xC080;   //-63.5dB 
    //vol_max = 0x0000;   //  0.0dB
    //vol_step= 0x0080;   //  0.5dB
    */
    switch(id)
    {
    case FU01_ID:
        McuWriteMem4(FU01_MUTE_BASE,0);
        addr = FU01_VOLUME_BASE;
        vol_curr= 0x0000;   //  0.0dB
        vol_min = 0x8100;   // -127dB 
        vol_max = 0x0000;   //  0.0dB
        vol_step= 0x0080;   //  0.5dB
        break;
    case FU02_ID:
        McuWriteMem4(FU02_MUTE_BASE,0);
        addr = FU02_VOLUME_BASE;
        vol_curr= 0x0000;   //  0.0dB
        vol_min = 0x8100;   // -127dB R:0xC080
        vol_max = 0x0000;   //  0.0dB
        vol_step= 0x0080;   //  0.5dB R:0x0060
		break;
    case FU03_ID:
        McuWriteMem4(FU03_MUTE_BASE,1);
        addr = FU03_VOLUME_BASE;
        vol_curr= 0x0000;   //  0.0dB
        vol_min = 0x8100;   // -127dB 
        vol_max = 0x0000;   //  0.0dB
        vol_step= 0x0080;   //  0.5dB
        break;
    case FU04_ID:
        McuWriteMem4(FU04_MUTE_BASE,0);
        addr = FU04_VOLUME_BASE;
        vol_curr= 0x0000;   //  0.0dB
        vol_min = 0x8100;   // -127dB 
        vol_max = 0x0000;   //  0.0dB
        vol_step= 0x0080;   //  0.5dB
        break;
    }
    for(i=0;i<3;i++)
    {
        //addr=(index==1)?(FU01_VOLUME_BASE+i*10):(FU02_VOLUME_BASE+i*10); 
#if(UAC_TYPE == TYPE_UAC2)
        McuWriteMem2(addr,vol_curr);
        McuWriteMem2(addr+2,1);         // count=1
        McuWriteMem2(addr+4,vol_min);
        McuWriteMem2(addr+6,vol_max);
        McuWriteMem2(addr+8,vol_step);
        addr+= 10;
#else
        McuWriteMem2(addr,vol_curr);
        McuWriteMem2(addr+2,vol_min);
        McuWriteMem2(addr+4,vol_max);
        McuWriteMem2(addr+6,vol_step);
        McuWriteMem2(addr+8,0);
        addr+= 10;
#endif
    }
}
void EntitySetVolumeBase(void)
{
    BYTE    reg;
    WORD    base;

	McuWriteReg(0x00,BANK_EP0);
    // for Playbck Mute/Vol
    reg = 0xC0;
    base = FU01_MUTE_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteReg(reg+1,HIBYTE(base));
    //McuWriteRegMask(reg+1,(INTERFACE_PLAYBACK<<4),0xF0);	
    McuWriteReg(reg+2,FU01_ID); 
    // Mute R/W       	
    McuWriteReg(reg+3,0x07);    //R for ch0-ch2	
    McuWriteReg(reg+4,0x07);	//W for ch0-ch2
    // Vol R/W       	
    McuWriteReg(reg+5,0x07);	
    McuWriteReg(reg+6,0x07);	
    
    // for Record Mute/Vol
    reg +=8;
    base = FU02_MUTE_BASE;	
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteReg(reg+1,HIBYTE(base));	
    //McuWriteRegMask(reg+1,(INTERFACE_RECORD<<4),0xF0);	
    McuWriteReg(reg+2,FU02_ID); 
    // Mute R/W       	
    McuWriteReg(reg+3,0x07);	
    McuWriteReg(reg+4,0x07);	
    // Vol R/W       	
    McuWriteReg(reg+5,0x07);	
    McuWriteReg(reg+6,0x07);	

    // for Feedback Mute/Vol
    
    reg +=8;
    base = FU03_MUTE_BASE;	
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteReg(reg+1,HIBYTE(base));	
    McuWriteReg(reg+2,FU03_ID); 
    // Mute R/W       	
    McuWriteReg(reg+3,0x07);	
    McuWriteReg(reg+4,0x07);	
    // Vol R/W       	
    McuWriteReg(reg+5,0x07);	
    McuWriteReg(reg+6,0x07);	
    
    reg +=8;
    base = FU04_MUTE_BASE;	
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteReg(reg+1,HIBYTE(base));	
    McuWriteReg(reg+2,FU04_ID); 
    // Mute R/W       	
    McuWriteReg(reg+3,0x07);	
    McuWriteReg(reg+4,0x07);	
    // Vol R/W       	
    McuWriteReg(reg+5,0x07);	
    McuWriteReg(reg+6,0x07);	
}

void EntitySetJackBase(void)
{
    BYTE    reg;
    WORD    base;

	McuWriteReg(0x00,BANK_EP0);
    
    //for Playbck Alternate Valid (Control Selector=2)
    //0x01:AS_ACT_ALT_SETTING_CONTROL
    //0x02:AS_VAL_ALT_SETTINGS_CONTROL
    //0x03:AS_AUDIO_DATA_FORMAT_CONTROL:0x03
    reg = JD01_CURR_REG;
    base = JD01_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x01);            //(1:curr, 2:range)
    McuWriteReg(reg+3,0x02);	        //CS=2 (Control Selector/Vaild)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,0x04);	        //UnitID
    McuWriteReg(reg+6,IF_CONTROL);      //Inteface=0
    McuWriteReg(reg+7,6);               //len=6
    //McuWriteRegMask(reg+8,0x00,0x3F); //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    
    // Set current config
    McuWriteMemS(base,0x00);
    McuWriteMemS(base+1,0x00);

    //for Record Alternate Valid
    reg = JD02_CURR_REG;
    base = JD02_CURR_BASE;
    McuWriteReg(reg,LOBYTE(base));	
    McuWriteRegMask(reg+1,HIBYTE(base),0x3F);	
    McuWriteRegMask(reg+1,0xC0,0xC0);   //RW
    McuWriteReg(reg+2,0x01);            //(1:curr, 2:range)
    McuWriteReg(reg+3,0x02);	        //CS=2 (Control Selector/Vaild)
    McuWriteReg(reg+4,0x00);            //CN=0	
    McuWriteReg(reg+5,0x0A);	        //UnitID
    McuWriteReg(reg+6,IF_CONTROL);      //Inteface=0
    McuWriteReg(reg+7,6);               //len=2
    //McuWriteRegMask(reg+8,0x00,0x3F); //
    McuWriteRegMask(reg+8,0x40,0xC0);   //type:01(interface)    
    // Set current config
    McuWriteMemS(base,0x00);
    McuWriteMemS(base+1,0x00);
    DEBUG_PRINT(ENTITY_PRINT_REG,"reg:%x\r\n",reg);
}


