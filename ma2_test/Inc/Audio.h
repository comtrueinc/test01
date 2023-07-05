#ifndef _CTUSB_AUDIO_H_
#define _CTUSB_AUDIO_H_

#ifdef	_CTUSB_AUDIO_C_
    #define _CTUSB_AUDIO_EXTERN_	
#else
    #define _CTUSB_AUDIO_EXTERN_  extern
#endif 
#include "Descriptor.h"

#define P0_CURR_FREQ_ADDR           ((unsigned char volatile xdata *) (CS01_FREQ_CURR_BASE))
#define R0_CURR_FREQ_ADDR           ((unsigned char volatile xdata *) (CS02_FREQ_CURR_BASE))

#define R0_CURR_INPUT_ADDR          ((unsigned char volatile xdata *) (SU02_CURR_BASE))

#define P0_CURR_MUTE_ADDR           ((unsigned char volatile xdata *) (FU01_MUTE_BASE))
#define R0_CURR_MUTE_ADDR           ((unsigned char volatile xdata *) (FU02_MUTE_BASE))
#define FB_CURR_MUTE_ADDR           ((unsigned char volatile xdata *) (FU03_MUTE_BASE))
#define SP_CURR_MUTE_ADDR           ((unsigned char volatile xdata *) (FU04_MUTE_BASE))

#define P0_CURR_VOL0_ADDR           ((unsigned char volatile xdata *) (FU01_VOLUME_BASE))
#define R0_CURR_VOL0_ADDR           ((unsigned char volatile xdata *) (FU02_VOLUME_BASE))
#define FB_CURR_VOL0_ADDR           ((unsigned char volatile xdata *) (FU03_VOLUME_BASE))
#define SP_CURR_VOL0_ADDR           ((unsigned char volatile xdata *) (FU04_VOLUME_BASE))

#define VOLUME_CH_OFFSET            10

enum
{
    DRIVER_CMD_DSD_PLAY0_START      = 0x01,
    DRIVER_CMD_DSD_PLAY0_STOP       = 0x02,
    DRIVER_CMD_PCM_PLAY0_START      = 0x03,
    DRIVER_CMD_PCM_PLAY0_STOP       = 0x04,

    DRIVER_CMD_DSD_REC0_START       = 0x05,
    DRIVER_CMD_DSD_REC0_STOP        = 0x06,
    DRIVER_CMD_PCM_REC0_START       = 0x07,
    DRIVER_CMD_PCM_REC0_STOP        = 0x08,

    DRIVER_CMD_DSD_PLAY1_START      = 0x11,
    DRIVER_CMD_DSD_PLAY1_STOP       = 0x12,
    DRIVER_CMD_PCM_PLAY1_START      = 0x13,
    DRIVER_CMD_PCM_PLAY1_STOP       = 0x14,

    DRIVER_CMD_ASIO_SWITCH_OFF      = 0x30,
    DRIVER_CMD_ASIO_SWITCH_ON       = 0x31,

    DRIVER_CMD_ASIO_PLAY0_OFF       = 0x80,
    DRIVER_CMD_ASIO_PLAY0_ON        = 0x81,
    DRIVER_CMD_NDSD_PLAY0_ON        = 0x83,
    DRIVER_CMD_ASIO_PLAY1_OFF       = 0x84,
    DRIVER_CMD_ASIO_PLAY1_ON        = 0x85,
    DRIVER_CMD_NDSD_PLAY1_ON        = 0x87,
    DRIVER_CMD_ASIO_RECORD_OFF      = 0x88,
    DRIVER_CMD_ASIO_RECORD_ON       = 0x89,
    DRIVER_CMD_NDSD_RECORD_ON       = 0x8B,
};
//#define INTERFACE_PLAYBACK          1
//#define INTERFACE_RECORD            2

typedef struct _AUDIO_GOLBAL_INFO_
{
    WORD                flags;
    BYTE                entity;
    BYTE                iface;
    BYTE                drv_cmd;
    BYTE                uac_type;
//    DWORD               p0_freq;
//    DWORD               r0_freq;
//    BYTE                ms_count;
//    BYTE                master_vol;
//    BYTE                rec_ch;
/*
    struct {
        WORD            p0_alt      :4;
        WORD            r0_alt      :4;
        WORD            freq        :4;
        WORD            none        :4;
    }format;
*/
    struct{
        WORD            flags       :4;
        WORD            alt         :4;
        WORD            freq        :4;
        WORD            ready       :1;
        WORD            dop         :1;
        WORD            type        :2;
//        WORD            ndsd        :1;
//        WORD            dts         :1;
    }stream[2];
}AUDIO_GOLBAL_INFO, *PAUDIO_GOLBAL_INFO;	


#define AUDIO_FLAGS_IFACE1                  0x0001
#define AUDIO_FLAGS_IFACE2                  0x0010
#define AUDIO_FLAGS_FREQ1                   0x0002
#define AUDIO_FLAGS_FREQ2               	0x0020
/*
#define AUDIO_FLAGS_VOLUME10                0x0100
#define AUDIO_FLAGS_VOLUME11                0x0200
#define AUDIO_FLAGS_MUTE1                   0x0800
#define AUDIO_FLAGS_VOLUME20                0x1000
#define AUDIO_FLAGS_VOLUME21                0x2000
#define AUDIO_FLAGS_MUTE2                   0x8000
#define AUDIO_FLAGS_VOLUME30                0x0400
#define AUDIO_FLAGS_MUTE3                   0x4000
*/
#define AUDIO_FLAGS_VOLUME10                0x0100
#define AUDIO_FLAGS_MUTE1                   0x0200
#define AUDIO_FLAGS_VOLUME20                0x0400
#define AUDIO_FLAGS_MUTE2                   0x0800
#define AUDIO_FLAGS_VOLUME30                0x1000
#define AUDIO_FLAGS_MUTE3                   0x2000
#define AUDIO_FLAGS_VOLUME40                0x4000
#define AUDIO_FLAGS_MUTE4                   0x8000

#define AUDIO_FLAGS_PLUGIN                  0x0008
#define AUDIO_FLAGS_INPUT                   0x0080

#define PLAYBACK_FLAGS_NOT_READY            0x01
#define PLAYBACK_FLAGS_NEW_FORMAT           0x02
#define PLAYBACK_FLAGS_DOP_CHANGE           0x04
#define PLAYBACK_FLAGS_READY                0x08

#ifdef	_CTUSB_AUDIO_C_
    AUDIO_GOLBAL_INFO           au={0};	
#else
    extern AUDIO_GOLBAL_INFO    au;	
#endif 


//		Function Prototypes
//	------------------------------------
void AudioInit(void);

void AudioSetForUsbSpeed(BYTE speed);
void AudioSetPlayClock(BYTE index);
void AudioSetRecClock(BYTE index);

WORD AudioConvertVol(WORD value0);
void AudioSetVolume(BYTE id);
void AudioSetMute(BYTE id);
//void AudioSetMute(BYTE index, BYTE value);
WORD AudioGetVolume(BYTE ch);
WORD AudioGetMute(void);
void AudioSetSelector(BYTE id);

void AudioSetEntityBase(void);
void AudioSetMuteBase(void);
void AudioSetVolumeBase(void);
void AudioSetFreqRange(BYTE index);
void AudioSetVolumeRange(BYTE index);

BYTE AudioGetFreqIndex(BYTE iface);

void AudioSetPlayback(BYTE bits);
void AudioSetRecord(BYTE fi, BYTE bits);
void AudioSetPlayType(BYTE index);

void AudioGetStatus(void);
void AudioSetOutput(void);

void AudioDetectSpeed(void);
void AudioDoPlugIn(void);
//void AudioDoAlternate(void);
void AudioDoAlternate(BYTE iface);
void AudioDoFreq(void);
void AudioDoInput(void);

void AudioDoFormat(BYTE iface);
void AudioDoVolume(BYTE type);
void AudioDoMute(BYTE index);


#endif  //__CTUSB_AUDIO_H__
