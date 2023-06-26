#ifndef _CTUSB_IAP2_H_
#define _CTUSB_IAP2_H_
#include "Bulk.h"

#define IAP2_SUPPORT_EA             0
#define IAP2_SUPPORT_POWER          1

#if(IAP2_SUPPORT_EA)
#define IAP2_SUPPORT_APP            1
#else
#define IAP2_SUPPORT_APP            0
#endif

#define IAP2_SESSION_COUNT          (1+IAP2_SUPPORT_EA)

#define PACKET_RECEIVE_LEN_MAX      1024 
#define PACKET_RETRANSMIT_TIMEOUT   1600    // 20-65535
#define PACKET_CUM_ACK_TIMEOUT      640     // 10-(PACKET_RETRANSMIT_TIMEOUT/2)

#define PACKET_MAX_RETRANSMIT       10  //30 
#define PACKET_MAX_OUTSTANDING      1   //1-127
#define PACKET_MAX_CUM_ACK          0   //0-127(<=PACKET_MAX_OUTSTANDING)

#define PACKET_SESSION_ID           0x11
#define SECOND_SESSION_ID           0x22

#define SESSION_TYPE_CONTROL        0x00    
#define SESSION_TYPE_EA             0x02    

enum
{
	STATE_ACC_IDLE					    =0x00,
	STATE_ACC_INIT					    =0x01,
	STATE_ACC_WAIT					    =0x02,
	STATE_ACC_PREPARE				    =0x03,
	STATE_ACC_CONNECT			        =0x04,
	STATE_ACC_RESET					    =0x08,
	STATE_ACC_RECONNECT			        =0x09,

	STATE_ACC_DETECT_SET  		        =0x0E,
	STATE_ACC_DETECT_SEND			    =0x0F,
	STATE_DEV_DETECT_WAIT			    =0x10,
	STATE_DEV_DETECT_GET			    =0x11,
	
    STATE_ACC_LINK_SET_SYN				=0x1E,
	STATE_ACC_LINK_SEND_SYN             =0x1F,
	STATE_DEV_LINK_WAIT_SYNACK          =0x20,
	STATE_DEV_LINK_GET_SYNACK           =0x21,
	STATE_ACC_LINK_SET_ACK              =0x2E,
	STATE_ACC_LINK_SEND_ACK             =0x2F,

//	STATE_DEV_AUTH_WAIT                 =0x20,
//	STATE_DEV_AUTH_GET                  =0x21,
//	STATE_ACC_AUTH_SET_ACK              =0x22,
//	STATE_ACC_AUTH_SEND                 =0x23,

	STATE_DEV_CONTROL_WAIT              =0x30,
	STATE_DEV_CONTROL_GET               =0x31,
	STATE_ACC_CONTROL_SET_ACK           =0x3E,
	STATE_ACC_CONTROL_SEND              =0x3F,

	STATE_ACC_AUTH_SET_CERT             =0x34,
	STATE_ACC_AUTH_CHALLENGE_SET        =0x36,
	STATE_ACC_AUTH_SET_RESPONSE         =0x38,

//	STATE_DEV_IDENT_WAIT                =0x40,
//	STATE_DEV_IDENT_GET                 =0x41,
//	STATE_ACC_IDENT_SET_ACK             =0x42,
//	STATE_ACC_IDENT_SEND                =0x43,

	STATE_ACC_IDENT_SET_INFO            =0x3A,


	STATE_DEV_MESSAGE_WAIT              =0x40,
	STATE_DEV_MESSAGE_GET               =0x41,
	STATE_ACC_MESSAGE_SET_ACK           =0x4E,
	STATE_ACC_MESSAGE_SEND              =0x4F,

	STATE_ACC_MESSAGE_SET_INFO          =0x44,
	
    STATE_ACC_MESSAGE_SET_EAACK         =0x52,
	STATE_ACC_MESSAGE_SET_EA            =0x54,

};

enum
{
    MSG_REQUEST_AUTH_CERTIFICATE        =0xAA00,    //dev
    MSG_AUTH_CERTIFICATE                =0xAA01,    //acc
    MSG_REQUEST_AUTH_CHALLENGE          =0xAA02,    //dev
    MSG_AUTH_RESPONSE                   =0xAA03,    //acc
    MSG_AUTH_FAILED                     =0xAA04,    //dev
    MSG_AUTH_SUCCEEDED                  =0xAA05,    //dev

    MSG_START_IDENTIFICATION            =0x1D00,    //dev
    MSG_IDENTIFICATION_INFO             =0x1D01,    //acc
    MSG_IDENTIFICATION_ACCEPTED         =0x1D02,    //dev
    MSG_IDENTIFICATION_REJECTED         =0x1D03,    //dev
    MSG_CANCEL_IDENTIFICATION           =0x1D04,    //dev
    MSG_IDENTIFICATION_INFO_UPDATE      =0x1D05,    //acc

    MSG_START_POWER_UPDATES             =0xAE00,    //acc
    MSG_POWER_UPDATE                    =0xAE01,    //dev
    MSG_STOP_POWER_UPDATES              =0xAE02,    //acc
    MSG_POWER_SOURCE_UPDATE             =0xAE03,    //acc

    MSG_START_EA_SESSION                =0xEA00,    //dev
    MSG_STOP_EA_SESSION                 =0xEA01,    //dev
    MSG_REQUEST_APP_LAUNCH              =0xEA02,    //acc
    MSG_STATUS_EA_SESSION               =0xEA03,    //acc

    MSG_NONE                            =0x0000,
    MSG_ERROR                           =0xFFFF,
};
enum
{
    ACC_PACKET_NONE         =0x00,
    ACC_PACKET_DETEC        =0x01,
    ACC_PACKET_SYN          =0x02,

    ACC_CONTROL_RESPONSE    =0x04,
    ACC_MESSAGE_REQUEST     =0x05,
    ACC_EA_STATUS           =0x06,

    ACC_PACKET_ACK          =0x80,
};
/*
typedef union _PACKET_PROPERTY_
{
    uint8_t bytes[4];
	struct
	{
        WORD id:        2;
        WORD new:       1;
        WORD authed:    1;
        WORD acked:     1;
        WORD msg_idx:   3;
		WORD type:      4;
		WORD flags:     4;
        
        BYTE state;
        BYTE psn;        
	};

} PACKET_PROPERTY;
*/
typedef struct IAP2_SESSION_INFO
{
    uint8_t     id;   
    uint8_t     type;   
    uint8_t     ver;   
} IAP2_SESSION_INFO, *PIAP2_SESSION_INFO;

typedef struct IAP2_LINK_SYN_INFO
{
    uint8_t    	version;
    uint8_t    	max_outstanding;
    uint16_t    max_recv_len;
    uint16_t    retransmit_timeout;
    uint16_t   	cum_ack_timeout;
    uint8_t    	max_retransmit;
    uint8_t    	max_cum_ack;
	IAP2_SESSION_INFO session[IAP2_SESSION_COUNT];
	uint8_t		p_chk;
} IAP2_LINK_SYN_INFO, *PIAP2_LINK_SYN_INFO;

typedef struct IAP2_CONTROL_MESSAGE
{
    uint16_t    som;				//0x40
//    uint8_t    	som2;				//0x40
    uint16_t    msg_len;
    uint16_t    msg_id;
    uint16_t    para_len;
    uint16_t   	para_id;
    uint8_t     para[1];
} IAP2_CONTROL_MESSAGE, *PIAP2_CONTROL_MESSAGE;

typedef struct IAP2_GOLBAL_INFO
{
    uint8_t         flags;
    uint8_t         state;
//    uint8_t         state_set;
//    uint8_t         first_psn;
//    uint8_t         first_ack;
//    uint8_t         prev_psn;
//    uint8_t         prev_ack;
    uint8_t         session_id;
    uint8_t         retry;
    uint8_t         reset_count;
//    uint8_t         bulkout_count;

//    uint8_t         cert_index;
//    uint16_t        bulkin_len;
    uint16_t        pkt_len;
    uint16_t        cert_len;
    uint16_t        ack_timer;
//    uint16_t        test_timer;
//    uint16_t        bo_time[16];
    
    uint8_t         msg_index;
    uint8_t         msg_count;
//    uint8_t         msg_queue[4];

     
    struct
    {
        BYTE reset:         1;
        BYTE count:         7;
        BYTE len[4];
        BYTE addr_offset[4];
        BYTE *data_ptr;            
    }bulkout;

	struct
	{
        BYTE detected:      1;
        BYTE synced:        1;
        BYTE auth_done:     1;
        BYTE info_done:     1;
        BYTE msg_done:      1;
        BYTE ea_start:      1;
        BYTE support_power: 1;
        BYTE support_ea:    1;
	}session;

	struct
	{
//        BYTE new:       1;
//        BYTE acked:     1;
//        BYTE authed:    1;
//        BYTE ea:        1;
//        BYTE power:     1;
//        BYTE update:    1;
//        BYTE reserved:  2;
        BYTE type;
//        BYTE id;
        BYTE state;
        BYTE psn;
        BYTE pan;
        BYTE prev_psn;        
	}pkt;                                                   
     
//    uint16_t        msg_id;
//    uint16_t        msg_dev;
    PIAP2_CONTROL_MESSAGE msg_out;
}IAP2_GOLBAL_INFO, *PIAP2_GOLBAL_INFO;	

#define IAP2_CP_SLAVE_ADDR          0x20

#define IAP2_FLAG_PLUGIN            0x01
#define IAP2_FLAG_SUSPEND           0x02
#define IAP2_FLAG_BULKOUT_READY     0x04
//#define IAP2_FLAG_TIMEOUT           0x08
          
//#define IAP2_FLAG_MSG_RESEND        0x10 
//#define IAP2_FLAG_NEW_MSG           0x20 
//#define IAP2_FLAG_NEW_EA            0x40 
//#define IAP2_FLAG_AUTH_RESET         0x80
#define IAP2_FLAG_SHARE_MEM           0x80 

#define IAP2_MSG_INDEX_START_POWER_UPDATE       0
#define IAP2_MSG_INDEX_POWER_SOURCE_UPDATE0     1
#define IAP2_MSG_INDEX_POWER_SOURCE_UPDATE1     2
#define IAP2_MSG_INDEX_STOP_POWER_UPDATE        3

#ifdef	_CTUSB_IAP2_C_
IAP2_GOLBAL_INFO iap2={0};
#else
extern IAP2_GOLBAL_INFO iap2;
#endif

void iAp2StateMachine(void);
void iAp2Init(void);
void iAp2Idle(void);
void iAp2Waiting(void);
void iAp2Prepare(void);

WORD iAp2CpGetCertLen(void);
BOOLEAN iAp2CpGetCertData(BYTE index, BYTE len, BYTE *buffer);
BOOLEAN iAp2CpSetChallengeLen(WORD len);
BOOLEAN iAp2CpSetChallengeData(WORD len, BYTE* buffer);
BOOLEAN iAp2CpSetAuthControl(BYTE control);

//WORD iAp2GetMsgId(BYTE index);
void iAp2PrepareMsg(void);
void iAp2MessageAckSet(void);
void iAp2MessageSet(void);

BYTE iAp2GetSynInfo(BYTE *buf);
WORD iAp2GetIdentInfoX(BYTE *buf);
BYTE iAp2GetActiveMsgCount(void);
BYTE iAp2GetMsgInfo(BYTE idx, BYTE *buf);

#endif	