#ifndef _CTUSB_CODEC_H_
#define _CTUSB_CODEC_H_

#define DAC_CT7302          1
#define DAC_ES9XX8          2

#define DAC_TYPE            (DAC_CT7302)    

#if (DAC_TYPE==DAC_CT7302) 
    #define DAC_I2C_SLAVE_ADDR      0x22
    #define DAC_MUTE_REG            0x06
    #define DAC_MUTE_MASK           0x80
#elif (DAC_TYPE==DAC_ES9XX8)   
    #define DAC_I2C_SLAVE_ADDR      0x90
    #define DAC_MUTE_REG            0x07
    #define DAC_MUTE_MASK           0x03
    #define DAC_VOL1_REG            0x0F
    #define DAC_VOL2_REG            0x10
#endif

void CodecInit(BYTE slave, WORD in_addr, WORD out_addr);
void CodecSetMute(BYTE mute);
#endif  //__CTUSB_CODEC_H__
