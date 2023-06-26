#ifndef _CTUSB_ENTITY_H_
#define _CTUSB_ENTITY_H_

#ifdef	_CTUSB_ENTITY_C_
    #define _CTUSB_ENTITY_EXTERN_	
#else
    #define _CTUSB_ENTITY_EXTERN_  extern
#endif 

//#include "rammap.h"
#define USB_DESC_DATA_BASE          0x0000
#define USB_DESC_DATA_ADDR          (USB_DESC_DATA_BASE)
#define USB_DESC_DATA_ADDR_LEN      0x3B0       //mixer: 3e0/3b0

#define USB_ENTITY_DATA_BASE        (USB_DESC_DATA_BASE+USB_DESC_DATA_ADDR_LEN)
#define USB_ENTITY_DATA_ADDR        (USB_ENTITY_DATA_BASE)
#define USB_ENTITY_DATA_ADDR_LEN    0x01A0

#define USB_ENTITY_END_ADDR         (USB_ENTITY_DATA_BASE+USB_ENTITY_DATA_ADDR_LEN-1)

/*//for playback
#define     UAC2_PIT_ID                         0x01
#define     UAC2_PFU_ID                         0x03
#define     UAC2_POT_ID                         0x04
#define     UAC2_PCS_ID                         0x05
#define     UAC2_PMX_ID                         0x06

//for record
#define     UAC2_RIT0_ID                        0x08        //Mic
#define     UAC2_RIT1_ID                        0x09        //LineIn
#define     UAC2_RSU_ID                         0x0A
#define     UAC2_RFU_ID                         0x0B
#define     UAC2_FFU_ID                         0x0C
#define     UAC2_ROT_ID                         0x0D
#define     UAC2_RMX_ID                         0x0E
#define     UAC2_RCS_ID                         0x0F
*/
enum
{
    IT01_ID     =0x01,
    FU01_ID     =0x03,
    OT01_ID     =0x04,
    CS01_ID     =0x05,
    MX01_ID     =0x06,
    FU04_ID     =0x07,

    IT02_ID     =0x08,
    IT12_ID     =0x09,
    SU02_ID     =0x0A,
    FU02_ID     =0x0B,
    FU03_ID     =0x0C,
    OT02_ID     =0x0D,
    CS02_ID     =0x0F,
};

enum
{
    CS01_FREQ_CURR_REG      =0x40,
    CS01_FREQ_RANGE_REG     =0x40+9,        //0x49    

    CS02_FREQ_CURR_REG      =0x40+18,       //0x52     
    CS02_FREQ_RANGE_REG     =0x40+27,       //0x5B

    CS01_FREQ_VALID_REG     =0x40+36,       //0x64      
    CS02_FREQ_VALID_REG     =0x40+45,       //0x6D    

    AL01_CURR_VALID_REG     =0x40+54,       //0x76
    AL02_CURR_VALID_REG     =0x40+63,       //0x7F

    JD01_CURR_REG           =0x40+72,       //0x88
    JD02_CURR_REG           =0x40+81,       //0x91

    SU02_CURR_REG           =0x40+90,       //0x9A

    MX01_CURR_REG           =0xB5,          //0xB5
 //   MX02_CURR_REG           =0x40+108,      //0xAC

};

enum
{

    FU01_MUTE_BASE          =(USB_ENTITY_DATA_BASE+0x00),   //3*1
    AL01_VALID_CURR_BASE    =(USB_ENTITY_DATA_BASE+0x04),   //len=2
    JD01_CURR_BASE          =(USB_ENTITY_DATA_BASE+0x06),   //len=2
    FU01_VOLUME_BASE        =(USB_ENTITY_DATA_BASE+0x08),   //3*10 =30

    FU02_MUTE_BASE          =(USB_ENTITY_DATA_BASE+0x28),   //3*1
    AL02_VALID_CURR_BASE    =(USB_ENTITY_DATA_BASE+0x2C),   //len=2
    JD02_CURR_BASE          =(USB_ENTITY_DATA_BASE+0x2E),   //len=2
    FU02_VOLUME_BASE        =(USB_ENTITY_DATA_BASE+0x30),   //3*10=30 3:ch0-2, 10:curr(2)+range(8:count+min+max+res)
    SU02_CURR_BASE          =(USB_ENTITY_DATA_BASE+0x4E),   //len=1

    CS01_FREQ_CURR_BASE     =(USB_ENTITY_DATA_BASE+0x50),   //4
    CS01_VALID_CURR_BASE    =(USB_ENTITY_DATA_BASE+0x54),   //len=1
    CS01_FREQ_RANGE_BASE    =(USB_ENTITY_DATA_BASE+0x56),   //2+12*10=122=0x7A for UAC2

    CS02_FREQ_CURR_BASE     =(USB_ENTITY_DATA_BASE+0xD0),   //4
    CS02_VALID_CURR_BASE    =(USB_ENTITY_DATA_BASE+0xD4),   //len=1
    CS02_FREQ_RANGE_BASE    =(USB_ENTITY_DATA_BASE+0xD6),   //2+12*10=122=0x7A for UAC2

    FU03_MUTE_BASE          =(USB_ENTITY_DATA_BASE+0x150),   //3*1
    MX01_CURR_BASE          =(USB_ENTITY_DATA_BASE+0x154),   //len=2
    MX02_CURR_BASE          =(USB_ENTITY_DATA_BASE+0x156),   //len=2
    FU03_VOLUME_BASE        =(USB_ENTITY_DATA_BASE+0x158),   //3*10=30 3:ch0-2, 10:curr(2)+range(8:count+min+max+res)

    FU04_MUTE_BASE          =(USB_ENTITY_DATA_BASE+0x178),   //3*1
    FU04_VOLUME_BASE        =(USB_ENTITY_DATA_BASE+0x180),   //3*10=30 3:ch0-2, 10:curr(2)+range(8:count+min+max+res)

/*
    FU01_MUTE_BASE          =(USB_ENTITY_DATA_BASE+0x00),   //8*1(fixed)
    AL01_VALID_CURR_BASE    =(USB_ENTITY_DATA_BASE+0x06),   //len=2
    FU01_VOLUME_BASE        =(USB_ENTITY_DATA_BASE+0x08),   //3*10 =30

    FU02_MUTE_BASE          =(USB_ENTITY_DATA_BASE+0x28),  //8*1
    AL02_VALID_CURR_BASE    =(USB_ENTITY_DATA_BASE+0x2E),  //len=2
    FU02_VOLUME_BASE        =(USB_ENTITY_DATA_BASE+0x30),  //3*10=30 10:curr(2)+range(8:count+min+max+res)

    CS01_FREQ_CURR_BASE     =(USB_ENTITY_DATA_BASE+0x50),
    CS02_FREQ_CURR_BASE     =(USB_ENTITY_DATA_BASE+0x54),
    CS01_VALID_CURR_BASE    =(USB_ENTITY_DATA_BASE+0x58),   //len=1
    CS02_VALID_CURR_BASE    =(USB_ENTITY_DATA_BASE+0x5A),   //len=1
    SU02_CURR_BASE          =(USB_ENTITY_DATA_BASE+0x5C),   //len=1

    CS01_FREQ_RANGE_BASE    =(USB_ENTITY_DATA_BASE+0x60),   //2+12*10=122=0x7A for UAC2
    CS02_FREQ_RANGE_BASE    =(USB_ENTITY_DATA_BASE+0xE0),   //2+12*10=122=0x7A for UAC2
 
    JD01_CURR_BASE          =(USB_ENTITY_DATA_BASE+0xDA),   //len=2
    JD02_CURR_BASE          =(USB_ENTITY_DATA_BASE+0x15A),  //len=2
*/
};

//	------------------------------------
//		Function Prototypes
//	------------------------------------
void EntityInit(void);
void EntityCheckFreqRange(void);
void EntitySetVolumeBase(void);
void EntitySetVolumeRange(BYTE id);
void EntitySetFreqBase(void);
void EntitySetFreqRange(BYTE index);
void EntitySetBase(void);
void EntitySetJackBase(void);

#endif  //__CTUSB_AUDIO_H__
