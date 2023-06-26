#include "Reg51.h"
#include "Mcu.h"
#include "Uart.h"
#include "iAp2.h"

#define	IAP2_IDENT_ID00_NAME				    "Comtrue CT7602 UAC2 Device"					//48=4+(43)+1
#define	IAP2_IDENT_ID01_MODE_IDENT			    "CT7602-2112"						            //24=4+(19)+1
#define	IAP2_IDENT_ID02_MANUFACTURE			    "Comtrue Inc."						            //24=4+(19)+1
#define	IAP2_IDENT_ID03_SERIAL_NUMBER		    "CTUS211221"						            //24=4+(19)+1	
#define	IAP2_IDENT_ID04_FIRMWARE_VERSION	    "1.21.1221"						                //24=4+(19)+1	
#define	IAP2_IDENT_ID05_HARDWARE_VERSION	    "1.02.00"							            //24=4+(19)+1	
#define	IAP2_IDENT_ID0A_PROTOCOL			    "com.comtrue-inc.eaprotocol.data"	            //64=4+5*2+4+(45)+1	
#define	IAP2_IDENT_ID0B_TEAMID				    "J69739G944"						            //16=4+(11)+1	
#define	IAP2_IDENT_ID22_PPUID				    "f829e6d1674047a1"						        //16=4+(16)+1	
	
#define IAP2_IDENT_ID00_NAME_LEN                sizeof(IAP2_IDENT_ID00_NAME)                    //40+1		
#define	IAP2_IDENT_ID01_MODE_IDENT_LEN			sizeof(IAP2_IDENT_ID01_MODE_IDENT)              //16+1    
#define	IAP2_IDENT_ID02_MANUFACTURE_LEN         sizeof(IAP2_IDENT_ID02_MANUFACTURE)             //16
#define	IAP2_IDENT_ID03_SERIAL_NUMBERE_LEN	    sizeof(IAP2_IDENT_ID03_SERIAL_NUMBER)           //16
#define	IAP2_IDENT_ID04_FIRMWARE_VERSION_LEN	sizeof(IAP2_IDENT_ID04_FIRMWARE_VERSION)        //16
#define	IAP2_IDENT_ID05_HARDWARE_VERSION_LEN	sizeof(IAP2_IDENT_ID05_HARDWARE_VERSION)        //16
#define	IAP2_IDENT_ID0A_PROTOCOL_LEN            sizeof(IAP2_IDENT_ID0A_PROTOCOL)                //max:45	
#define	IAP2_IDENT_ID0B_TEAMID_LEN              sizeof(IAP2_IDENT_ID0B_TEAMID)                  //16	
#define	IAP2_IDENT_ID22_PPUID_LEN               sizeof(IAP2_IDENT_ID22_PPUID)                   //16	

#define IAP2_MSG_LAUNCH_APP					    "com.comtrue-inc.samplecode.EADemo"	            //56=6+4+(45)+1	
#define	IAP2_MSG_LAUNCH_APP_LEN                 sizeof(IAP2_MSG_LAUNCH_APP)                     //max:45	

#if(IAP2_SUPPORT_APP)
#define ID06_ACC_MESSAGE_COUNT				    4                       //4
#define ID06_ACC_MESSAGE_SETS                   {0xAE00,0xAE02,0xAE03,0xEA02}  //{0xAE00,0xAE02,0xAE03,0xEA02,}
#else
#define ID06_ACC_MESSAGE_COUNT				    3                       //4
#define ID06_ACC_MESSAGE_SETS                   {0xAE00,0xAE02,0xAE03}  //{0xAE00,0xAE02,0xAE03,0xEA02,}
#endif

#if(IAP2_SUPPORT_EA)
#define ID07_DEV_MESSAGE_COUNT				    3                       //3
#define ID07_DEV_MESSAGE_SETS                   {0xAE01,0xEA00,0xEA01}                //{0xAE01,0xEA00,0xEA01,}
#else
#define ID07_DEV_MESSAGE_COUNT				    1                       //3
#define ID07_DEV_MESSAGE_SETS                   {0xAE01}                //{0xAE01,0xEA00,0xEA01,}
#endif


typedef struct _IAP2_ID_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[1];
}IAP2_ID_INFO, *PIAP2_ID_INFO;

typedef struct _IAP2_ID00_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[IAP2_IDENT_ID00_NAME_LEN];
}IAP2_ID00_INFO, *PIAP2_ID00_INFO;

typedef struct _IAP2_ID01_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[IAP2_IDENT_ID01_MODE_IDENT_LEN];
}IAP2_ID01_INFO, *PIAP2_ID01_INFO;

typedef struct _IAP2_ID02_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[IAP2_IDENT_ID02_MANUFACTURE_LEN];
}IAP2_ID02_INFO, *PIAP2_ID02_INFO;

typedef struct _IAP2_ID03_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[IAP2_IDENT_ID03_SERIAL_NUMBERE_LEN];
}IAP2_ID03_INFO, *PIAP2_ID03_INFO;

typedef struct _IAP2_ID04_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[IAP2_IDENT_ID04_FIRMWARE_VERSION_LEN];
}IAP2_ID04_INFO, *PIAP2_ID04_INFO;

typedef struct _IAP2_ID05_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[IAP2_IDENT_ID05_HARDWARE_VERSION_LEN];
}IAP2_ID05_INFO, *PIAP2_ID05_INFO;

typedef struct _IAP2_ID06_INFO_
{
	uint16_t	len;
	uint16_t	id;
#if(ID06_ACC_MESSAGE_COUNT>0)
	uint16_t	message[ID06_ACC_MESSAGE_COUNT];
#endif
}IAP2_ID06_INFO, *PIAP2_ID06_INFO;

typedef struct _IAP2_ID07_INFO_
{
	uint16_t	len;
	uint16_t	id;
#if(ID07_DEV_MESSAGE_COUNT>0)
	uint16_t	message[ID07_DEV_MESSAGE_COUNT];
#endif
}IAP2_ID07_INFO, *PIAP2_ID07_INFO;

typedef struct _IAP2_ID08_INFO_
{
	uint16_t	len;
	uint16_t	id;
	uint8_t		capability;
}IAP2_ID08_INFO, *PIAP2_ID08_INFO;

typedef struct _IAP2_ID09_INFO_
{
	uint16_t	len;
	uint16_t	id;
	uint16_t	max_current;
}IAP2_ID09_INFO, *PIAP2_ID09_INFO;

typedef struct _IAP2_ID0A_INFO_
{
	uint16_t			ea_len;
	uint16_t			ea_id;
	uint16_t			id0_len;
	uint16_t			id0;
	uint8_t				value0;
	uint16_t			id1_len;
	uint16_t			id1;
	uint8_t				protocal_str[IAP2_IDENT_ID0A_PROTOCOL_LEN];
	uint16_t			id2_len;
	uint16_t			id2;
	uint8_t				value2;
}IAP2_ID0A_INFO, *PIAP2_ID0A_INFO;

typedef struct _IAP2_ID0B_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[IAP2_IDENT_ID0B_TEAMID_LEN];
}IAP2_ID0B_INFO, *PIAP2_ID0B_INFO;


typedef struct _IAP2_ID0C_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[3];
}IAP2_ID0C_INFO, *PIAP2_ID0C_INFO;

typedef struct _IAP2_ID0D_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[3];
}IAP2_ID0D_INFO, *PIAP2_ID0D_INFO;

typedef struct _IAP2_ID10_INFO_
{
	uint16_t			len;
	uint16_t			id;
	uint16_t			len0;
	uint16_t			id0;
	uint16_t			value0;
	uint16_t			len1;
	uint16_t			id1;
	uint8_t				str[5];
	uint16_t			len2;
	uint16_t			id02;	
}IAP2_ID10_INFO, *PIAP2_ID10_INFO;

typedef struct _IAP2_ID22_INFO_
{
	uint16_t	len;
	uint16_t	id;
	char		content[IAP2_IDENT_ID22_PPUID_LEN];
}IAP2_ID22_INFO, *PIAP2_ID22_INFO;

typedef struct _IAP2_LAUNCH_APP_MSG_
{
	uint16_t			msg_tag;
	uint16_t			msg_len;
	uint16_t			msg_id;
	uint16_t			len0;
	uint16_t			id0;
	char				app_str[IAP2_MSG_LAUNCH_APP_LEN];
}IAP2_LAUNCH_APP_MSG, *PIAP2_LAUNCH_APP_MSG;

typedef struct _IAP2_MSG_TYPE_
{
    uint16_t    tag;
	uint16_t	len;
	uint16_t	msg;
	uint16_t	info_len;
	uint16_t	id;
    char        info[1];
}IAP2_MSG_TYPE, *PIAP2_MSG_TYPE;

//=============================================================================================
code IAP2_ID00_INFO id00_name = {
	sizeof(IAP2_ID00_INFO), 0,
	IAP2_IDENT_ID00_NAME,
};
code IAP2_ID01_INFO id01_mode_ident = {
	sizeof(IAP2_ID01_INFO), 1,
	IAP2_IDENT_ID01_MODE_IDENT,
};
code IAP2_ID02_INFO id02_manufacture = {
	sizeof(IAP2_ID02_INFO), 2,
	IAP2_IDENT_ID02_MANUFACTURE,
};
code IAP2_ID03_INFO id03_serial_number = {
	sizeof(IAP2_ID03_INFO), 3,
	IAP2_IDENT_ID03_SERIAL_NUMBER,
};
code IAP2_ID04_INFO id04_firmware = {
	sizeof(IAP2_ID04_INFO), 4,
	IAP2_IDENT_ID04_FIRMWARE_VERSION,
};
code IAP2_ID05_INFO id05_hardware = {
	sizeof(IAP2_ID05_INFO), 5,
	IAP2_IDENT_ID05_HARDWARE_VERSION,
};
/*	//ID06_MESSAGE_ACCESSORY
    0x00,0x0C,0x00,0x06,
    0xAE,0x00,     //StartPowerUpdate
    0xAE,0x02,     //StopPowerUpdates
    0xAE,0x03,     //PowerSourceUpdate
    0xEA,0x02,     //RequestAppLaunch
	//ID07_MESSAGE_DEVICE
    0x00,0x0A,0x00,0x07,
    0xAE,0x01,     //PowerUpdate
    0xEA,0x00,     //StartEASession
    0xEA,0x01,     //StopEASession
*/
code IAP2_ID06_INFO id06_acc_message = {
	sizeof(IAP2_ID06_INFO), 6,
    ID06_ACC_MESSAGE_SETS
//	{0xAE00,0xAE02,0xAE03,0xEA02,},
};

code IAP2_ID07_INFO id07_dev_message = {
	sizeof(IAP2_ID07_INFO), 7,
    ID07_DEV_MESSAGE_SETS
//	{0xAE01,0xEA00,0xEA01,},
};

code IAP2_ID08_INFO id08_power_capability = {
	sizeof(IAP2_ID08_INFO), 8,
	0x02,
};

code IAP2_ID09_INFO id09_max_power_drawn = {
	sizeof(IAP2_ID09_INFO), 9,
	0,	
};

code IAP2_ID0A_INFO id0a_ea_protocol = {
	sizeof(IAP2_ID0A_INFO), 10,
	5,0,0,
	(IAP2_IDENT_ID0A_PROTOCOL_LEN+4),1,IAP2_IDENT_ID0A_PROTOCOL,
	5,2,1,
};

code IAP2_ID0B_INFO id0b_teamid = {
	sizeof(IAP2_ID0B_INFO), 11,
	IAP2_IDENT_ID0B_TEAMID,	
};

code IAP2_ID0C_INFO id0c_current_lang = {
	sizeof(IAP2_ID0C_INFO), 12,
	{'e','n',0,},	
};

code IAP2_ID0D_INFO id0d_support_lang = {
	sizeof(IAP2_ID0D_INFO), 13,
	{'e','n',0,},	
};

code IAP2_ID10_INFO id10_usbhost_comp = {
	sizeof(IAP2_ID10_INFO), 16,
	6,0,0,
	9,1,{'i','A','P','2',0},
	4,2,
};
code IAP2_ID22_INFO id22_ppuid = {
	sizeof(IAP2_ID22_INFO), 34,
	IAP2_IDENT_ID22_PPUID,	
};

code BYTE* ident_info[]={
	(BYTE*)&id00_name,
	(BYTE*)&id01_mode_ident,
	(BYTE*)&id02_manufacture,
	(BYTE*)&id03_serial_number,
	(BYTE*)&id04_firmware,
	(BYTE*)&id05_hardware,
	(BYTE*)&id06_acc_message,
	(BYTE*)&id07_dev_message,
	(BYTE*)&id08_power_capability,
	(BYTE*)&id09_max_power_drawn,
#if (IAP2_SUPPORT_EA)
	(BYTE*)&id0a_ea_protocol,
#endif
	(BYTE*)&id0b_teamid,
	(BYTE*)&id0c_current_lang,
	(BYTE*)&id0d_support_lang,
	(BYTE*)&id10_usbhost_comp,	
	(BYTE*)&id22_ppuid,	
};

#define IDENT_INFO_COUNT	(sizeof(ident_info)/sizeof(ident_info[0]))


code IAP2_LINK_SYN_INFO link_syn_info = {
    1, 1,
    PACKET_RECEIVE_LEN_MAX, PACKET_RETRANSMIT_TIMEOUT, PACKET_CUM_ACK_TIMEOUT,
    PACKET_MAX_RETRANSMIT, PACKET_MAX_CUM_ACK,
    {{PACKET_SESSION_ID, SESSION_TYPE_CONTROL, 1},
#if (IAP2_SUPPORT_EA)
    {SECOND_SESSION_ID, SESSION_TYPE_EA, 1},
#endif
    },
    0x00,
};

#define LINK_SYN_LEN        (sizeof(link_syn_info))

code BYTE msg_acc_start_power_update[]={
	//ACC_MSG_START_POWER_UPDATES
    0x40,0x40,0x00,0x0E,0xAE,0x00,
    0x00,0x04,0x00,0x00,
    0x00,0x04,0x00,0x01,	
};

code BYTE msg_acc_power_source_update0[]={
	//ACC_MSG_POWER_SOURCE_UPDATE (0)
    0x40,0x40,0x00,0x11,0xAE,0x03,
    0x00,0x06,0x00,0x00,0x00,0x00,              // 1000/3E8,2100/834,2400/960
    0x00,0x05,0x00,0x01,0x01,                   // Yes
};

code BYTE msg_acc_power_source_update1[]={
	//ACC_MSG_POWER_SOURCE_UPDATE (1)
    0x40,0x40,0x00,0x11,0xAE,0x03,
    0x00,0x06,0x00,0x00,0x03,0xE8,              // 1000/3E8,2100/834,2400/960
    0x00,0x05,0x00,0x01,0x01,                   // Yes
};

code BYTE msg_acc_stop_power_update[]={
	//ACC_MSG_STOP_POWER_UPDATES
    0x40,0x40,0x00,0x06,0xAE,0x02,
};

code IAP2_LAUNCH_APP_MSG msg_launch_app = {
	0x4040, sizeof(IAP2_LAUNCH_APP_MSG), 0xEA02,
	(IAP2_MSG_LAUNCH_APP_LEN+4), 0, IAP2_MSG_LAUNCH_APP,
};

code BYTE* active_msg_table[]={
	msg_acc_start_power_update,             //AE00
#if (IAP2_SUPPORT_POWER)
	msg_acc_power_source_update1,           //AE03/1A
#else
	msg_acc_power_source_update0,           //AE03/0A
#endif
#if (IAP2_SUPPORT_APP)
	(BYTE*)&msg_launch_app,
#endif
};

#define ACTIVE_MSG_COUNT    (sizeof(active_msg_table)/sizeof(active_msg_table[0]))
/*
code BYTE* msg_table[]={
	msg_acc_start_power_update,             //AE00
	msg_acc_power_source_update0,           //AE03/0.0A
	msg_acc_power_source_update1,           //AE03/1.0A
	msg_acc_stop_power_update,              //AE02
	(BYTE*)&msg_launch_app,
};

code BYTE msg_queue[]={0,2};

#define MESSAGE_COUNT	2
*/


BYTE iAp2GetSynInfo(BYTE *buf)
{
    BYTE i,*pbuf;
    pbuf = (BYTE*)&link_syn_info;
    for(i=0;i<LINK_SYN_LEN;i++)
    {
	    *buf++ = pbuf[i];
    }
    return i;
}

WORD iAp2GetIdentInfoX(BYTE *buffer)
{
    BYTE i,j,*pbuf;
	WORD len=0;
    for(i=0;i<IDENT_INFO_COUNT;i++)
    {
		pbuf  = ident_info[i];
        if(pbuf)
        {
            for(j=0;j<((PIAP2_ID_INFO)pbuf)->len;j++)
            {
                *buffer++ = pbuf[j];
                //printf("%x,",pbuf[j]);
            }
            //printf("(%d)\r\n",j);
			len += j;
        }		
    } 
    //printf("(len:%d)\r\n",len);
    return len;       
}

BYTE iAp2GetActiveMsgCount(void)
{
    return ACTIVE_MSG_COUNT;
}

BYTE iAp2GetMsgInfo(BYTE idx, BYTE *buffer)
{
    BYTE i,*pbuf;
	BYTE len=0;
	pbuf  = active_msg_table[idx];
	if(pbuf)
	{
		for(i=0;i<((PIAP2_MSG_TYPE)pbuf)->len;i++)
		{
			*buffer++ = pbuf[i];
            //printf("%x,",pbuf[i]);
		}
        //printf("(%d)\r\n",i);
		len += i;
	}		
	return len;
}

