#ifndef _CTUSB_SETTINGS_H_
#define _CTUSB_SETTINGS_H_

#define     TYPE_UAC1_FS                        0x01
#define     TYPE_UAC1_HS                        0x02
#define     TYPE_UAC2                           0x04
#define     TYPE_UAC_DETECT                     (TYPE_UAC1_HS|TYPE_UAC2)

//================================================================================================================

#define     UAC_TYPE                            TYPE_UAC2                                //TYPE_UAC1_FS, TYPE_UAC1_HS, TYPE_UAC2, TYPE_UAC_DETECT  
#define     SELF_POWER                          0
#define     MAX_POWER                           100

//-----------------------------------------------------------------------------------------------------------------
#define     CONTROL_ENABLE                      1 
#define     PLAYBACK_ENABLE                     1 
#define     RECORD_ENABLE                       0
#define     HID_ENABLE                          1 
#define     BULK_ENABLE                         (SUPPORT_IAP2)

#define     IF_CONTROL                          0 
#define     IF_PLAYBACK                         1 
#define     IF_RECORD                           (IF_PLAYBACK+RECORD_ENABLE) 
#define     IF_HID                              (IF_RECORD+HID_ENABLE)  
#define     IF_BULK                             (IF_HID+BULK_ENABLE) 

// --------------------------------------------------------------
// Special application
// --------------------------------------------------------------
#define     USE_D_MIC                           0
#define     PLAY_EXTERNAL_MCLK                  0
#define     HID_GPIO_KEY                        1

//for playback
#define     PIT_ID                              0x01
#define     PFU_ID                              0x03
#define     POT_ID                              0x04
#define     PCS_ID                              0x05
#define     PMX_ID                              0x06
#define     SFU_ID                              0x07

//for record
#define     RIT0_ID                             0x08        //Mic
#define     RIT1_ID                             0x09        //LineIn
#define     RSU_ID                              0x0A
#define     RFU_ID                              0x0B
#define     FFU_ID                              0x0C
#define     ROT_ID                              0x0D
#define     RCS_ID                              0x0F

//------------------------------------------------------------------------
// for UAC2 only:
//-------------------------------------------------------------------------
#define     PLAYBACK_16B_ENABLE                 1 
#define     PLAYBACK_24B_ENABLE                 1 
#define     PLAYBACK_32B_ENABLE                 1 
#define     PLAYBACK_NDSD_ENABLE                1 
#define     PLAYBACK_DTS_ENABLE                 0 

#define     ALT_PLAYBACK_16B                    1       //alt:1 
#define     ALT_PLAYBACK_24B                    2       //alt:2
#define     ALT_PLAYBACK_32B                    3       //alt:3
#define     ALT_PLAYBACK_NDSD                   4       //alt:4
#define     ALT_PLAYBACK_DTS                    5       //alt:5
//------------------------------------------------------
#define     RECORD_16B_ENABLE                   1 
#define     RECORD_24B_ENABLE                   1 
#define     RECORD_32B_ENABLE                   1 

#define     ALT_RECORD_16B                      1       //alt:1 
#define     ALT_RECORD_24B                      2       //alt:2
#define     ALT_RECORD_32B                      3       //alt:3

#define     ALT_RECORD_16B_CH                   0x02 
// bmChannelConfig
#define     ALT_RECORD_16B_CH_CONFIG            0x03,0x00,0x00,0x00         
//------------------------------------------------------

#define     FREQ_PLAY_44K_ENABLE                1
#define     FREQ_PLAY_48K_ENABLE                1
#define     FREQ_PLAY_88K_ENABLE                1
#define     FREQ_PLAY_96K_ENABLE                1
#define     FREQ_PLAY_176K_ENABLE               1
#define     FREQ_PLAY_192K_ENABLE               1
#define     FREQ_PLAY_352K_ENABLE               1
#define     FREQ_PLAY_384K_ENABLE               1
#define     FREQ_PLAY_705K_ENABLE               1
#define     FREQ_PLAY_768K_ENABLE               1

#define     FREQ_REC_44K_ENABLE                 1
#define     FREQ_REC_48K_ENABLE                 1
#define     FREQ_REC_88K_ENABLE                 1
#define     FREQ_REC_96K_ENABLE                 1
#define     FREQ_REC_176K_ENABLE                1
#define     FREQ_REC_192K_ENABLE                1
#define     FREQ_REC_352K_ENABLE                1
#define     FREQ_REC_384K_ENABLE                1
#define     FREQ_REC_705K_ENABLE                1
#define     FREQ_REC_768K_ENABLE                1
//--------------------------------------------------------------------------------------

#define     PLAY0_ALT_VALID                     (0x1F)
#define     REC0_ALT_VALID                      (0x07)
#define     FREQ_MAX_COUNT                      (10)
 
#define     PLAY0_FREQ_COUNT            (FREQ_PLAY_44K_ENABLE+FREQ_PLAY_48K_ENABLE+FREQ_PLAY_88K_ENABLE+FREQ_PLAY_96K_ENABLE+FREQ_PLAY_176K_ENABLE+FREQ_PLAY_192K_ENABLE \
                                        +FREQ_PLAY_352K_ENABLE+FREQ_PLAY_384K_ENABLE+FREQ_PLAY_705K_ENABLE+FREQ_PLAY_768K_ENABLE)
#define     REC0_FREQ_COUNT             (FREQ_REC_44K_ENABLE+FREQ_REC_48K_ENABLE+FREQ_REC_88K_ENABLE+FREQ_REC_96K_ENABLE+FREQ_REC_176K_ENABLE+FREQ_REC_192K_ENABLE \
                                        +FREQ_REC_352K_ENABLE+FREQ_REC_384K_ENABLE+FREQ_REC_705K_ENABLE+FREQ_REC_768K_ENABLE)

#define     PLAY0_FREQ_FLAGS            (((U16)(FREQ_PLAY_44K_ENABLE))       |((U16)(FREQ_PLAY_48K_ENABLE)<<1)|               \
                                         ((U16)(FREQ_PLAY_88K_ENABLE)<<2)    |((U16)(FREQ_PLAY_96K_ENABLE)<<3)|               \
                                         ((U16)(FREQ_PLAY_176K_ENABLE)<<4)   |((U16)(FREQ_PLAY_192K_ENABLE)<<5)|              \
                                         ((U16)(FREQ_PLAY_352K_ENABLE)<<6)   |((U16)(FREQ_PLAY_384K_ENABLE)<<7)|              \
                                         ((U16)(FREQ_PLAY_705K_ENABLE)<<8)   |((U16)(FREQ_PLAY_768K_ENABLE)<<9))             

#define     REC0_FREQ_FLAGS             (((U16)(FREQ_REC_44K_ENABLE))       |((U16)(FREQ_REC_48K_ENABLE)<<1)|               \
                                         ((U16)(FREQ_REC_88K_ENABLE)<<2)    |((U16)(FREQ_REC_96K_ENABLE)<<3)|               \
                                         ((U16)(FREQ_REC_176K_ENABLE)<<4)   |((U16)(FREQ_REC_192K_ENABLE)<<5)|              \
                                         ((U16)(FREQ_REC_352K_ENABLE)<<6)   |((U16)(FREQ_REC_384K_ENABLE)<<7)|              \
                                         ((U16)(FREQ_REC_705K_ENABLE)<<8)   |((U16)(FREQ_REC_768K_ENABLE)<<9))             
//--------------------------------------------------------------------------------------

typedef struct _DESCRIPTOR_INFO_
{
    WORD    len;
    BYTE    *buffer;
}DESCRIPTOR_INFO, *PDESCRIPTOR_INFO;

#define     USB_TYPE                            (UAC_TYPE==TYPE_UAC1_FS)?0x0110:0x0200      

#if (SELF_POWER)
    #define POWER_CONFIG                        0xC0, 0
#else
    #define POWER_CONFIG                        0x80, (MAX_POWER/2)
#endif

#define     PLAYBACK_NAME                       "Playback"
#define     RECORD_NAME                         "Record"
#define     HID_NAME                            "Hid"
#define     IAP_NAME                            "iAP Interface"

#define     PLAYBACK_NAME_LEN                   (sizeof(PLAYBACK_NAME)-1)
#define     RECORD_NAME_LEN                     (sizeof(RECORD_NAME)-1)
#define     HID_NAME_LEN                        (sizeof(HID_NAME)-1)
#define     IAP_NAME_LEN                        (sizeof(IAP_NAME)-1)

#if (UAC_TYPE==TYPE_UAC_DETECT)
    #define		UAC2_PLAY0_FEEDBACK					1
    #include    "uac1.h"
    #include    "uac2.h"
#elif (UAC_TYPE==TYPE_UAC2)
#define		UAC2_PLAY0_FEEDBACK					1
#include    "uac2.h"
#else
#define		UAC2_PLAY0_FEEDBACK					0
#include    "uac1.h"
#endif

#endif //_CTUSB_SETTINGS_H_