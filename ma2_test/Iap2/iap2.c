#include "Reg51.h"
#include "Mcu.h"
#include "Uart.h"
#include "Descriptor.h"
#include "Mi2c.h"
//#include "Power.h"
#include "audio.h"

#define _CTUSB_IAP2_C_
#include "iAp2.h"
#undef  _CTUSB_IAP2_C_

/*
******************************************************
**
** Packet structure:
**
** msb                             lsb
**  +---+---+---+---+---+---+---+---+
**  | 7 | 6 | 5 | 4 | 3 | 2 | 1 | 0 |
**  +---+---+---+---+---+---+---+---+
**  |   Start of Packet MSB (0xFF)  | byte 0
**  +-------------------------------+
**  |   Start of Packet LSB (0x5A)  | byte 1
**  +-------------------------------+
**  |       Packet Length MSB       | byte 2
**  +-------------------------------+
**  |       Packet Length LSB       | byte 3
**  +---+---+---+---+---+---+---+---+
**  |SYN|ACK|EAK|RST|SUS| 0 | 0 | 0 | byte 4
**  +---+---+---+---+---+---+---+---+
**  |     Packet Sequence Number    | byte 5
**  +-------------------------------+
**  | Packet Acknowledgement Number | byte 6
**  +-------------------------------+
**  |           Session ID          | byte 7
**  +-------------------------------+
**  |        Header Checksum        | byte 8
**  +-------------------------------+
**  |               .               |
**  |               .               |
**  |          Payload Data         | byte 9 to N - 1
**  |               .               |
**  |               .               |
**  +-------------------------------+
**  |        Payload Checksum       | byte N
**  +-------------------------------+
**
******************************************************
*/

enum IAP2PacketEnums
{
    /* Start of packet bytes */
    kIAP2PacketSYNC        = 0xFF,
    kIAP2PacketSOP         = 0x5A,
    kIAP2PacketSOPOrig     = 0x55,

    kIAP2PacketSOPLen      = 2,

    kiAP2PacketVersion     = 1,

    /* Packet field index */
    kIAP2PacketIndexSYNC   = 0,   /* Start of packet byte 1 */
    kIAP2PacketIndexSOP    = 1,   /* Start of packet byte 2 */
    kIAP2PacketIndexLEN1   = 2,   /* Packet length MSB */
    kIAP2PacketIndexLEN2   = 3,   /* Packet length LSB */
    kIAP2PacketIndexCTRL   = 4,   /* Control flags */
    kIAP2PacketIndexSEQ    = 5,   /* SEQ number */
    kIAP2PacketIndexACK    = 6,   /* ACK number */
    kIAP2PacketIndexSESSID = 7,   /* Session ID */
    kIAP2PacketIndexCHK    = 8,   /* Header checksum */

    /* iAP 1.0/2.0 packet detect: FF 55 02 00 EE 10 */
    kIAP2PacketDetectLEN    = 0x0200,
    kIAP2PacketDetectCTRL   = 0xEE,
    kIAP2PacketDetectSEQ    = 0x10,

    /* iAP 1.0/2.0 packet detect BAD ACK: FF 55 04 00 02 04 EE 08 */
    kIAP2PacketDetectNACKLEN    = 0x0400,
    kIAP2PacketDetectNACKCTRL   = 0x02,
    kIAP2PacketDetectNACKSEQ    = 0x04,
    kIAP2PacketDetectNACKACK    = 0xEE,
    kIAP2PacketDetectNACKSESSID = 0x08,

    /*
    ** Packet Header Len
    ** SOP(2) + LEN(2) + CONTROL(1) + SEQ(1) + ACK(1) + SESSID(1) + CHK(1)
    */
    kIAP2PacketHeaderLen = 9,
    kIAP2PacketChksumLen = 1,   /* checksum byte length */

    kiAP2PacketLenMax = 0xFFFF,
    kiAP2PacketMaxPayloadSize = (kiAP2PacketLenMax - kIAP2PacketHeaderLen - kIAP2PacketChksumLen),

    /* BitMask defines for bits in control byte */
    kIAP2PacketControlMaskSYN = 0x80,   /* synchronization */
    kIAP2PacketControlMaskACK = 0x40,   /* acknowledgement */
    kIAP2PacketControlMaskEAK = 0x20,   /* extended acknowledgement */
    kIAP2PacketControlMaskRST = 0x10,   /* reset */
    kIAP2PacketControlMaskSUS = 0x08,   /* suspend (sleep) */

    kIAP2PacketSynDataIdxVersion           = 0,
    kIAP2PacketSynDataIdxMaxOutstanding    = 1,
    kIAP2PacketSynDataIdxMaxPacketSize     = 2,
    kIAP2PacketSynDataIdxRetransmitTimeout = 4,
    kIAP2PacketSynDataIdxCumAckTimeout     = 6,
    kIAP2PacketSynDataIdxMaxRetransmit     = 8,
    kIAP2PacketSynDataIdxMaxCumACK         = 9,

    kIAP2PacketSynDataBaseLen              = 10,

    kIAP2PacketSynDataIdxSessionInfo       = 10,

    kIAP2PacketSynSessionIdxID             = 0,
    kIAP2PacketSynSessionIdxType           = 1,
    kIAP2PacketSynSessionIdxVersion        = 2,
    kIAP2PacketSynSessionSize              = 3,

    /* Session ID 0x0 shall not be used for any session types */
    kIAP2PacketReservedSessionID           = 0,

    kIAP2PacketMaxSessions                 = 10,

    kIAP2PacketSynOptionMaskLP             = 0x80,
    kIAP2PacketSynOptionMaskHP             = 0x40
};


typedef struct IAP2_PACKET_HEADER
{
    uint8_t sop1;   /* 0xFF */
    uint8_t sop2;   /* 0x5A */
    uint8_t len1;
    uint8_t len2;
    uint8_t ctl;
    uint8_t seq;
    uint8_t ack;
    uint8_t sess;
    uint8_t h_chk;

//} iAP2PacketHeader_t;
} IAP2_PACKET_HEADER, *PIAP2_PACKET_HEADER;
/*
typedef struct IAP2_SESSION_INFO
{
    uint8_t     id;   
    uint8_t     type;   
    uint8_t     ver;   
} IAP2_SESSION_INFO, *PIAP2_SESSION_INFO;
*/
typedef struct IAP2_LINK_SYN_PAYLOAD
{
    uint8_t    	version;
    uint8_t    	max_outstanding;
    uint8_t    	max_recv_len1;
    uint8_t    	max_recv_len2;
    uint8_t    	retransmit_timeout1;
    uint8_t		retransmit_timeout2;
    uint8_t   	cum_ack_timeout1;
    uint8_t    	cum_ack_timeout2;
    uint8_t    	max_retransmit;
    uint8_t    	max_cum_ack;
	IAP2_SESSION_INFO session[IAP2_SESSION_COUNT];
	uint8_t		p_chk;
} IAP2_LINK_SYN_PAYLOAD, *PIAP2_LINK_SYN_PAYLOAD;

typedef struct IAP2_MESSAGE_PARAMETER
{
    uint8_t    	para_len1;
    uint8_t    	para_len2;
    uint8_t    	para_id1;
    uint8_t		para_id2;
} IAP2_MESSAGE_PARAMETER, *PIAP2_MESSAGE_PARAMETER;

typedef struct IAP2_EA_SESSION
{
    uint16_t    ea_id;
    uint16_t    syn_ack;
    uint16_t    cmd;
    uint8_t     para[8];
} IAP2_EA_SESSION, *PIAP2_EA_SESSION;
/*
#define PACKET_RECEIVE_LEN_MAX      1024 
#define PACKET_RETRANSMIT_TIMEOUT   1200    // 20-65535
#define PACKET_CUM_ACK_TIMEOUT      100     // 10-(PACKET_RETRANSMIT_TIMEOUT/2)

#define PACKET_MAX_RETRANSMIT       10  //30 
#define PACKET_MAX_OUTSTANDING      1   //1-127
#define PACKET_MAX_CUM_ACK          0   //0-127(<=PACKET_MAX_OUTSTANDING)

#define PACKET_SESSION_ID           0x11
#define SECOND_SESSION_ID           0x22

#define SESSION_TYPE_CONTROL        0x00    
#define SESSION_TYPE_EA             0x02    
*/
#define PACKET_TYPE_DETECT          0x00
#define PACKET_TYPE_SYN             0x01
#define PACKET_TYPE_ACK             0x02
#define PACKET_TYPE_RESET           0x03
#define PACKET_TYPE_ACKED           0x04

#define PACKET_TYPE_MSG             0x05
#define PACKET_TYPE_EA              0x06
#define PACKET_TYPE_CERT            0x07

#define PACKET_FLAG_RESEND          0x01
#define PACKET_FLAG_NEW_MSG         0x02
#define PACKET_FLAG_NEW_EA          0x04
   
#define PACKET_ONE_SEC_PERIOD       1000
#define PACKET_CERT_DATA_LEN_MAX    128
#define PACKET_MESSAGE_BASE_LEN     6
#define PACKET_PARAMETER_BASE_LEN   4

#define AUTH_DATA_FIRST_OFFSET      19
#define AUTH_DATA_NORMAL_OFFSET     9

#define MSG_ID_OFFSET               13

//#define IAP2_ACK_TIMEOUT_MAX        (PACKET_RETRANSMIT_TIMEOUT+PACKET_RETRANSMIT_TIMEOUT/10)
#define IAP2_ACK_TIMEOUT_MAX        (PACKET_RETRANSMIT_TIMEOUT)
#define IAP2_RETRY_MAX              PACKET_MAX_RETRANSMIT

#define IAP2_AUTH_RESET_MAX         5

#define IAP2_EA_DATA_LEN            (sizeof(IAP2_EA_SESSION))

//#define RESPONSE_ACK                0x01    //for EA
//#define RESPONSE_SESSION            0x02    //for EA

#define IAP2_RESET_RECONNECT        1

//#define IAP2_SIM_MCORE_DSP          0

BULK_DATA_INFO     pktout_info={0};     

PIAP2_PACKET_HEADER     pkt_header = (PIAP2_PACKET_HEADER)usb_bulkin_data;

IAP2_EA_SESSION ea_data={0};
/*
#define     IAP2_PRINT_STATE        1
#define     IAP2_PRINT_INFO         1
#define     IAP2_PRINT_EA           0
#define     IAP2_PRINT_DATA         1
*/

#define     PRINT_IAP2_STATE        1
#define     PRINT_IAP2_INFO         1
#define     PRINT_IAP2_DATA         0
#define     PRINT_IAP2_EA           0

#if (PRINT_IAP2_STATE)
#define DBGPRINT_IAP2_STATE(_FMT_,_X_)             printf(_FMT_,_X_)
#else
#define DBGPRINT_IAP2_STATE(_FMT_,_X_)         
#endif

#if (PRINT_IAP2_INFO)
#define DBGPRINT_IAP2_INFO(_FMT_,_X_)             printf(_FMT_,_X_)
#else
#define DBGPRINT_IAP2_INFO(_FMT_,_X_)         
#endif

#if (PRINT_IAP2_DATA)
#define DBGPRINT_IAP2_DATA(_FMT_,_X_)             printf(_FMT_,_X_)
#else
#define DBGPRINT_IAP2_DATA(_FMT_,_X_)         
#endif

WORD iAp2CpGetCertLen(void)
{
    WORD len=0;
    BYTE buffer[2],old_addr;
    old_addr = Mi2cGetSlave();
    Mi2cSetSlave(IAP2_CP_SLAVE_ADDR);         //CP Slave Addr=0x20    
    if(Mi2cReadData(0x30,2,buffer))
    {
        len = BYTE_TO_WORD(buffer[0],buffer[1]);
    }
    Mi2cSetSlave(old_addr);         //DAC Slave Addr=DAC_I2C_SLAVE_ADDR   
    return len;
}

void iAp2Prepare(void)
{
    WORD    i,len,count;
    BYTE    old_addr;
    BYTE    old_bank;
    // switch to use playback/record buffer when authing
    iap2.flags |= IAP2_FLAG_SHARE_MEM;
    old_bank = hw_regs[0x00];
	McuWriteReg(0x00,BANK_MISC);
    McuWriteRegMask(0x06,0x18,0x18);
	hw_regs[0x00] = old_bank;

    old_addr = Mi2cGetSlave();
    len = iAp2CpGetCertLen();
    iap2.cert_len 	= len;

    Mi2cSetSlave(IAP2_CP_SLAVE_ADDR);         //CP Slave Addr=0x20    
	if(len>0 || iap2.retry++>3) 
    {
        count = len/128+1;;
        for(i=0;i<count;i++)
        {
            if(len<128)        
                Mi2cReadData(0x31+i,len,(iap2_cp_data+19+i*128));
            else
                Mi2cReadData(0x31+i,128,(iap2_cp_data+19+i*128));
            len = len-128;
        }
    }
    Mi2cSetSlave(old_addr);         //DAC Slave Addr=DAC_I2C_SLAVE_ADDR   
    iap2.state      = STATE_ACC_CONNECT; 
    DBGPRINT_IAP2_INFO("=>%x",iap2.state);
    DBGPRINT_IAP2_INFO("(len=%x",HIBYTE(iap2.cert_len));
    DBGPRINT_IAP2_INFO("%x),",LOBYTE(iap2.cert_len));
    DBGPRINT_IAP2_INFO("(r:%x)\r\n",iap2.retry);
}

BOOLEAN iAp2CpSetChallengeLen(WORD len)
{
    BYTE    buffer[2], ret_value;
//    BYTE    old_addr;
//    old_addr = Mi2cGetSlave();
//    Mi2cSetSlave(IAP2_CP_SLAVE_ADDR);         //CP Slave Addr=0x20    

    DBGPRINT_IAP2_INFO("iAp2CpSetChallengeLen(len=%x)\r\n",LOBYTE(len));
    buffer[0] = HIBYTE(len);
    buffer[1] = LOBYTE(len);
    ret_value = Mi2cWriteData(0x20,2,buffer);
//    Mi2cSetSlave(old_addr);         //DAC Slave Addr=DAC_I2C_SLAVE_ADDR   

    return ret_value;

}

BOOLEAN iAp2CpSetChallengeData(WORD len, BYTE *buffer)
{
//    BYTE    i;
    BOOLEAN ret;
//    BYTE    old_addr;
//    old_addr = Mi2cGetSlave();
//    Mi2cSetSlave(IAP2_CP_SLAVE_ADDR);         //CP Slave Addr=0x20    

    DBGPRINT_IAP2_INFO("iAp2CpSetChallengeData(len=%x)\r\n",LOBYTE(len));
    /*
    for(i=0;i<len;i++)
    {
        printf("%x,",buffer[i]);
    }
    printf("\r\n",0);
    */
    ret = Mi2cWriteData(0x21,len,buffer);

//    Mi2cSetSlave(old_addr);         //DAC Slave Addr=DAC_I2C_SLAVE_ADDR   
    return ret;
}
BOOLEAN iAp2CpSetAuthControl(BYTE control)
{
    BYTE buffer[2];
//    BYTE    old_addr;
//    old_addr = Mi2cGetSlave();
//    Mi2cSetSlave(IAP2_CP_SLAVE_ADDR);         //CP Slave Addr=0x20    

    DBGPRINT_IAP2_INFO("iAp2CpSetAuthControl(control=%x)\r\n",control);
    buffer[0]=control;
    //Mi2cWriteByte(0x10,control);
    Mi2cWriteData(0x10,1,buffer);

//    Mi2cSetSlave(old_addr);         //DAC Slave Addr=DAC_I2C_SLAVE_ADDR   
    return TRUE;
}
BYTE iAp2CpGetAuthStatus(void)
{
    BYTE status;
    BYTE    old_addr;
    old_addr = Mi2cGetSlave();
    Mi2cSetSlave(IAP2_CP_SLAVE_ADDR);         //CP Slave Addr=0x20    
    //status = Mi2cReadByte(0x10);
    Mi2cReadData(0x10,1,&status);
    Mi2cSetSlave(old_addr);         //DAC Slave Addr=DAC_I2C_SLAVE_ADDR   
    return status;
}
WORD iAp2CpGetResponseLen(void)
{
    WORD len=0;
    BYTE buffer[2];

    if(Mi2cReadData(0x11,2,buffer)) 
        len = BYTE_TO_WORD(buffer[0],buffer[1]);

    DBGPRINT_IAP2_INFO("iAp2CpGetResponseLen(len=%x)\r\n",LOBYTE(len));
    return len;
//    if(Mi2cReadData(0x11,2,buffer))
//    {
//        len = BYTE_TO_WORD(buffer[0],buffer[1]);
//        return len;
//    }
//    else
//        return 0;   
}
BOOLEAN iAp2CpGetResponeData(BYTE len, BYTE *buffer)
{
    BOOLEAN ret;
//    BYTE    i;

    ret = Mi2cReadData(0x12,len,buffer);

    DBGPRINT_IAP2_INFO("iAp2CpGetResponeData(len=%x)\r\n",LOBYTE(len));
    /*
    for(i=0;i<len;i++)
    {
        printf("%x,",buffer[i]);
    }
    printf("\r\n",0);
    */
    return ret;

//    if(Mi2cReadData(0x12,len,buffer))
//        return TRUE;
//    else
//        return FALSE;
}

/*
****************************************************************
**
**  iAP2PacketCalcChecksum
**
**  Input:
**      buffer:     Input buffer containing data to Checksum.
**      bufferLen:  Size of the Input buffer data.
**
**  Output:
**      None
**
**  Return:
**      uint8_t     Calculated checksum for provide buffer
**
****************************************************************
*/
BYTE iAp2PacketCalcChecksum (BYTE *buffer, WORD len)
{
    BYTE checksum = 0;

    if (len > 0)
    {
        BYTE* bufferEnd = buffer + len;

        while (buffer != bufferEnd)
        {
            checksum += *buffer; /* add to checksum */
            buffer++;
        }

        checksum = (BYTE)(0x100 - checksum); /* 2's complement negative checksum */
    }
    else
    {
        printf("checksume error!(%x)\r\n",len);
    }
    return checksum;
}

BYTE iAp2CheckDevData(BULK_DATA_INFO *p_out)
{
//    BYTE len;
//    printf("[T0:%x",HIBYTE(iap2.ack_timer));
//    printf("%x]\r\n",LOBYTE(iap2.ack_timer));
    iap2.flags &= ~(IAP2_FLAG_BULKOUT_READY);

    BulkOutGetInfo(p_out);

    if(p_out->len>=9)
    {
        p_out->ctl  = (p_out->data_ptr)[4];
        p_out->seq  = (p_out->data_ptr)[5];
        p_out->ack  = (p_out->data_ptr)[6];
        p_out->sess = (p_out->data_ptr)[7];
        DBGPRINT_IAP2_INFO("[ctl:%x,",p_out->ctl);
        DBGPRINT_IAP2_INFO("psn:%x,",p_out->seq);
        DBGPRINT_IAP2_INFO("pan:%x,",p_out->ack);
        DBGPRINT_IAP2_INFO("sess:%x]\r\n",p_out->sess);
    }
    return p_out->len;
}

void iAp2HeaderSet(BYTE session_id)
{
    pkt_header->sop1        = kIAP2PacketSYNC;
    pkt_header->sop2        = kIAP2PacketSOP;
    pkt_header->len1        = HIBYTE(iap2.pkt_len);
    pkt_header->len2        = LOBYTE(iap2.pkt_len);
    pkt_header->ctl         = kIAP2PacketControlMaskACK;
/*
    pkt_header->seq         = iap2.prev_psn;
    pkt_header->ack         = iap2.pkt.pan;
    pkt_header->sess        = session;
*/
    pkt_header->seq         = iap2.pkt.psn;
    pkt_header->ack         = iap2.pkt.pan;
    pkt_header->sess        = session_id;
    pkt_header->h_chk       = iAp2PacketCalcChecksum(usb_bulkin_data,kIAP2PacketHeaderLen-1);
}
void iAp2HeaderSetX(BYTE *pbuf, BYTE session_id)
{
    pbuf[0]        = kIAP2PacketSYNC;
    pbuf[1]        = kIAP2PacketSOP;
    pbuf[2]        = HIBYTE(iap2.pkt_len);
    pbuf[3]        = LOBYTE(iap2.pkt_len);
    pbuf[4]        = kIAP2PacketControlMaskACK;
    pbuf[5]        = iap2.pkt.psn;
    pbuf[6]        = iap2.pkt.pan;
    pbuf[7]        = session_id;
    pbuf[8]        = iAp2PacketCalcChecksum(pbuf,kIAP2PacketHeaderLen-1);
}
/*
void iAp2PrintTimer(void)
{
    DBGPRINT_IAP2_STATE("[t:%x",HIBYTE(iap2.ack_timer));
    DBGPRINT_IAP2_STATE("%x],",LOBYTE(iap2.ack_timer));
}
*/
void iAp2PrintState(BYTE prev_state)
{
    switch(prev_state&0x0F)
    {
    case 0x00: 
        //printf(">>:%x",iap2.state);
        DBGPRINT_IAP2_STATE(">>:%x",iap2.state);
        break;
    case 0x01: 
    //case 0x05: 
        //printf(">*:%x",iap2.state);
        DBGPRINT_IAP2_STATE(">*:%x",iap2.state);
        break;
    case 0x0E: 
        //printf("<*:%x",iap2.state);
        DBGPRINT_IAP2_STATE("<*:%x",iap2.state);
        break;
    case 0x0F: 
        //printf("<<:%x",iap2.state);
        DBGPRINT_IAP2_STATE("<<:%x",iap2.state);
        break;
    default:
        //printf("<**:%x",iap2.state);
        DBGPRINT_IAP2_STATE("**:%x",iap2.state);
        break;
    }
    //printf(",(%x,",iap2.pkt.prev_psn);
    //printf("%x,",iap2.retry);
    //printf("%x",HIBYTE(iap2.ack_timer));
    //printf("%x)\r\n",LOBYTE(iap2.ack_timer));
    if(prev_state>0x20 && iap2.ack_timer>1000) DBGPRINT_IAP2_STATE("--------------------------------",0);
    DBGPRINT_IAP2_STATE(",(%x,",iap2.pkt.psn);
    DBGPRINT_IAP2_STATE("%x,",iap2.pkt.pan);
    DBGPRINT_IAP2_STATE("%x,",iap2.retry);
    DBGPRINT_IAP2_STATE("%x",HIBYTE(iap2.ack_timer));
    DBGPRINT_IAP2_STATE("%x)\r\n",LOBYTE(iap2.ack_timer));
}
/*
void iAp2PrintPacket(BYTE* pbuf, BYTE len)
{
	BYTE i;
	for(i=0;i<len;i++)
	{
		if(i%16==15 || i==(len-1)) 
            DBGPRINT_IAP2_DATA("%x,\r\n",pbuf[i]);
        else
            DBGPRINT_IAP2_DATA("%x,",pbuf[i]);

	}

}
*/
void iAp2Init(void)
{

//	BulkInit();	
//    iap2.state      = STATE_ACC_PREPARE; 
    iAp2Prepare();
    //iap2.first_psn      = 0x50;
//    iap2.state      = STATE_ACC_IDLE;   // for 0x6002 
    iap2.pkt.psn        = 0x10;
    
    DBGPRINT_IAP2_STATE("init=>%x\r\n",iap2.state);
}

void iAp2Reset(void)
{
    BYTE i, *pbuf=&iap2;
    //old_bank = hw_regs[0x00];
	//McuWriteReg(0x00,BANK_MISC);
    //value = McuReadReg(0x30);    //bit1:connect/disconnect bit0:highspeed/fullspeed
    //McuWriteRegMask(0x30,0x00,0x02);
	//hw_regs[0x00] = old_bank;
    
    for(i=0;i<sizeof(IAP2_GOLBAL_INFO);i++)
    {
        pbuf[i]=0;
    }
    iAp2Prepare();
    iap2.pkt.psn    = 0x20;
        
    DBGPRINT_IAP2_STATE("reset=>%x\r\n",iap2.state);
    Delay100us(10000);
}

void iAp2Connect(void)
{
    BYTE old_bank, connect_value;
    old_bank = hw_regs[0x00];
	McuWriteReg(0x00,BANK_MISC);
    connect_value = McuReadReg(0x30);    //bit1:connect/disconnect bit0:highspeed/fullspeed
    McuWriteRegMask(0x30,0x02,0x02);     //[0]:HS/FS, [1]:Connect, [2]:Suspand
	hw_regs[0x00] = old_bank;

    iap2.state = (iap2.cert_len > 0)?STATE_ACC_DETECT_SET:STATE_ACC_WAIT;    

    DBGPRINT_IAP2_STATE("connect==>%x",iap2.state);
    DBGPRINT_IAP2_STATE("(cert_len=%x",HIBYTE(iap2.cert_len));
    DBGPRINT_IAP2_STATE("%x)\r\n",LOBYTE(iap2.cert_len));
}

void iAp2DetectSet(void)
{
    BYTE    prev_state;
    prev_state = iap2.state;

    pkt_header->sop1= kIAP2PacketSYNC;
    pkt_header->sop2= kIAP2PacketSOPOrig;
    pkt_header->len1= HIBYTE(kIAP2PacketDetectLEN);
    pkt_header->len2= LOBYTE(kIAP2PacketDetectLEN);
    pkt_header->ctl = kIAP2PacketDetectCTRL;
    pkt_header->seq = kIAP2PacketDetectSEQ;
    
    //iap2.retry          = 0;
    iap2.pkt_len        = 6;
    BulkInStart(iap2.pkt_len);  

    iap2.ack_timer      = 0;
    iap2.msg_count      = iAp2GetActiveMsgCount();
    iap2.session.msg_done = (iap2.msg_count)?0:1;
    iap2.state          = STATE_ACC_DETECT_SEND;
    iAp2PrintState(prev_state);
}
void iAp2DetectSend(void)
{

    BYTE    prev_state;
    prev_state = iap2.state;
    if(BulkInDataDone())
    {
        iap2.state  = STATE_DEV_DETECT_WAIT;
        //iAp2PrintPacket(usb_bulkin_data,iap2.pkt_len);
        iAp2PrintState(prev_state);
        //iap2.retry++;
        iap2.ack_timer      = 0;
    }
}
// no retries or retransmission are allowed
void iAp2DetectWait(void)
{
    BYTE    prev_state;
    prev_state = iap2.state;
    //if(BulkOutDataReady())
    if(iap2.flags&IAP2_FLAG_BULKOUT_READY)
    {
        iap2.state  = STATE_DEV_DETECT_GET;//iap2.state_get;
        iAp2PrintState(prev_state);
    }
    else if (iap2.ack_timer > PACKET_ONE_SEC_PERIOD)
    {
        BulkInStart(iap2.pkt_len);  
        iap2.ack_timer      = 0;
        iap2.state          = STATE_ACC_DETECT_SEND;//iap2.state_send;
        iAp2PrintState(prev_state);
    }
}

void iAp2DetectGet(void)
{
    PIAP2_PACKET_HEADER     pktout_header;     
    BYTE    prev_state;

    //BulkOutAck(0);
    iAp2CheckDevData(&pktout_info);
    BulkOutAck(1);

    iap2.session.detected = 1;
    //iAp2PrintPacket(pktout_info.data_ptr,pktout_info.len);
    
    prev_state = iap2.state;
    pktout_header   = (PIAP2_PACKET_HEADER) (pktout_info.data_ptr);

    if(pktout_header->sop1==0xFF)
        iap2.state  = STATE_ACC_LINK_SET_SYN;
    else 
        iap2.state  = STATE_DEV_DETECT_WAIT;
    iAp2PrintState(prev_state);
    //printf("[<:%x\r\n",iap2.state);
}
void iAp2SynSet(void)
{
    BYTE    prev_state;
    BYTE    syn_len, *pkt_syn;
    
    prev_state = iap2.state;
    pkt_syn = (usb_bulkin_data+kIAP2PacketHeaderLen); 

    syn_len = iAp2GetSynInfo(pkt_syn);

//    iap2.pkt_len        = kIAP2PacketHeaderLen+kIAP2PacketSynDataBaseLen+kIAP2PacketSynSessionSize*IAP2_SESSION_COUNT+1;
    iap2.pkt_len        = kIAP2PacketHeaderLen+syn_len;
    iap2.pkt.prev_psn   = iap2.pkt.psn; 
    
    pkt_header->sop1    = kIAP2PacketSYNC;
    pkt_header->sop2    = kIAP2PacketSOP;
    pkt_header->len1    = HIBYTE(iap2.pkt_len);                                 
    pkt_header->len2    = LOBYTE(iap2.pkt_len);
    pkt_header->ctl     = kIAP2PacketControlMaskSYN;
    pkt_header->seq     = iap2.pkt.psn;
    pkt_header->ack     = 0;
    pkt_header->sess    = 0;
    pkt_header->h_chk   = iAp2PacketCalcChecksum(usb_bulkin_data,kIAP2PacketHeaderLen-1);
/*
    pkt_syn->version            = 1;
    pkt_syn->max_outstanding    = 1;
    pkt_syn->max_recv_len1      = HIBYTE(PACKET_RECEIVE_LEN_MAX);
    pkt_syn->max_recv_len2      = LOBYTE(PACKET_RECEIVE_LEN_MAX);
    pkt_syn->retransmit_timeout1= HIBYTE(PACKET_RETRANSMIT_TIMEOUT);
    pkt_syn->retransmit_timeout2= LOBYTE(PACKET_RETRANSMIT_TIMEOUT);
    pkt_syn->cum_ack_timeout1   = HIBYTE(PACKET_CUM_ACK_TIMEOUT);
    pkt_syn->cum_ack_timeout2   = LOBYTE(PACKET_CUM_ACK_TIMEOUT);
    pkt_syn->max_retransmit     = PACKET_MAX_RETRANSMIT;
    pkt_syn->max_cum_ack        = PACKET_MAX_CUM_ACK;
    pkt_syn->session[0].id      = PACKET_SESSION_ID;
    pkt_syn->session[0].type    = SESSION_TYPE_CONTROL;
    pkt_syn->session[0].ver     = 1;
#if (IAP2_SUPPORT_EA)
    pkt_syn->session[1].id      = SECOND_SESSION_ID;                     
    pkt_syn->session[1].type    = SESSION_TYPE_EA;
    pkt_syn->session[1].ver     = 1;
#endif
    //pkt_syn->p_chk              = iAp2PacketCalcChecksum((BYTE*)pkt_syn,kIAP2PacketSynDataBaseLen+kIAP2PacketSynSessionSize*2);
*/
    usb_bulkin_data[iap2.pkt_len-1] = iAp2PacketCalcChecksum((BYTE*)pkt_syn, syn_len-1);
    
    BulkInStart(iap2.pkt_len);                                                             

    iap2.retry          = 0;                                                        
    iap2.ack_timer      = 0;
    iap2.state          = STATE_ACC_LINK_SEND_SYN;
    iAp2PrintState(prev_state);
}

void iAp2SynSend(void)
{
    BYTE    prev_state;
    prev_state = iap2.state;
    
    if(BulkInDataDone())
    {
        iap2.pkt.prev_psn = iap2.pkt.psn;
        iap2.state  = STATE_DEV_LINK_WAIT_SYNACK;//state_wait;//iap2.state_wait;
        //iAp2PrintPacket(usb_bulkin_data,iap2.pkt_len);
        iAp2PrintState(prev_state);
    }
    else if(iap2.ack_timer > PACKET_ONE_SEC_PERIOD)
    {
        //iap2.retry++;
        //iap2.ack_timer = 0;
        //BulkInSetData(iap2.pkt_len);  
        iap2.state  = STATE_DEV_LINK_WAIT_SYNACK;
    }
}
void iAp2SynAckWait(void)
{
    BYTE    prev_state;
    prev_state = iap2.state;
    if(iap2.flags&IAP2_FLAG_BULKOUT_READY)
    {
        iap2.state  = STATE_DEV_LINK_GET_SYNACK;//iap2.state_get;
        iAp2PrintState(prev_state);
    }
    else if(iap2.ack_timer > PACKET_ONE_SEC_PERIOD)
    {
        BulkInStart(iap2.pkt_len);  
        iap2.ack_timer      = 0;
        iap2.state  = STATE_ACC_LINK_SEND_SYN;//iap2.state_send;
        iAp2PrintState(prev_state);
    }
}
void iAp2SynAckGet(void)
{
    PIAP2_PACKET_HEADER     pktout_header;     
    BYTE    prev_state;
    
    //iAp2PrintTimer();
    prev_state = iap2.state;

    //BulkOutAck(0);
    iAp2CheckDevData(&pktout_info);
    BulkOutAck(1);
    //iAp2PrintPacket(pktout_info.data_ptr,pktout_info.len);
    pktout_header   = (PIAP2_PACKET_HEADER) (pktout_info.data_ptr);

    if(pktout_header->ack == iap2.pkt.prev_psn)
    {           
        //iap2.first_ack  = pktout_header->seq;
        iap2.session.synced = 1;
        iap2.pkt.pan        = pktout_header->seq;
        iap2.session_id     = pktout_header->sess;
        iap2.state          = STATE_ACC_LINK_SET_ACK;
        iAp2PrintState(prev_state);
    }
    else if (iap2.ack_timer > PACKET_ONE_SEC_PERIOD)
    {
        iap2.state      = STATE_DEV_LINK_WAIT_SYNACK;
        iAp2PrintState(prev_state);
    }
    //printf("<ack0:%x,",pktout_header->ack);
    //printf(",seq1:%x>\r\n",iap2.pkt.prev_psn);
    //printf("<offset:%x,",pktout_info.offset);
    //printf(",len:%x>\r\n",pktout_info.len);
}

void iAp2AckSet(void)
{
    BYTE    prev_state;

    prev_state = iap2.state;
    
    iap2.pkt_len     = kIAP2PacketHeaderLen;
    iAp2HeaderSet(0);
    BulkInStart(iap2.pkt_len);  

    iap2.state          = STATE_ACC_LINK_SEND_ACK;
    iAp2PrintState(prev_state);
}

void iAp2AuthSetCert(BYTE *buff)
{
    PIAP2_CONTROL_MESSAGE   pkt_msg; 
    BYTE    prev_state;

    prev_state = iap2.state;
    pkt_msg = (PIAP2_CONTROL_MESSAGE)(buff+kIAP2PacketHeaderLen); 

    iap2.pkt_len     = kIAP2PacketHeaderLen+PACKET_MESSAGE_BASE_LEN+PACKET_PARAMETER_BASE_LEN+iap2.cert_len+1; 
//    cert_buff = iAp2CpGetCertBuffer(iap2.cert_index);

    //iAp2HeaderSet(iap2.session);
    iAp2HeaderSetX(buff, iap2.session_id); 

//    pkt_msg->som1           = 0x40;       
//    pkt_msg->som2           = 0x40; 
    pkt_msg->som            = 0x4040;      
    pkt_msg->msg_len        = iap2.cert_len+PACKET_MESSAGE_BASE_LEN+PACKET_PARAMETER_BASE_LEN;       
    pkt_msg->msg_id         = MSG_AUTH_CERTIFICATE;
    pkt_msg->para_len       = iap2.cert_len+PACKET_PARAMETER_BASE_LEN;       
    pkt_msg->para_id        = 0;
    
    //iAp2CopyBuffer(128,temp_data, (usb_bulkin_data+AUTH_DATA_FIRST_OFFSET)); 
    //iAp2CopyBuffer(PACKET_CERT_DATA_LEN_MAX,cert_buff, (usb_bulkin_data+AUTH_DATA_FIRST_OFFSET)); 
    buff[iap2.pkt_len-1]  = iAp2PacketCalcChecksum((BYTE*)pkt_msg,iap2.pkt_len-kIAP2PacketHeaderLen-1);
    
    BulkInSetAddr(IAP2_CP_DATA_ADDR); //addr & len IAP2_CP_DATA_ADDR
    BulkInStart(iap2.pkt_len);  

    iap2.ack_timer          = 0;
    iap2.state              = STATE_ACC_CONTROL_SEND;
    iAp2PrintState(prev_state);
}

void iAp2AuthSetResponse(void)
{
    PIAP2_CONTROL_MESSAGE   pkt_msg;
    //BYTE    temp_data[128];
    BYTE    prev_state;
    WORD    response_len;
    prev_state = iap2.state;

    pkt_msg = (PIAP2_CONTROL_MESSAGE)(usb_bulkin_data+kIAP2PacketHeaderLen); 

    //iap2.response_len = iAp2CpGetResponseLen();
    //iAp2CpGetResponeData(iap2.response_len,(usb_bulkin_data+AUTH_DATA_FIRST_OFFSET));    
    response_len = iAp2CpGetResponseLen();
    iAp2CpGetResponeData(response_len,(usb_bulkin_data+AUTH_DATA_FIRST_OFFSET));    
    
    //iap2.pkt.prev_psn       = iap2.pkt.prev_psn+1;
    //iap2.pkt_len     = kIAP2PacketHeaderLen+PACKET_MESSAGE_BASE_LEN+PACKET_PARAMETER_BASE_LEN+iap2.response_len+1; 
    iap2.pkt_len     = kIAP2PacketHeaderLen+PACKET_MESSAGE_BASE_LEN+PACKET_PARAMETER_BASE_LEN+response_len+1; 

    iAp2HeaderSet(iap2.session_id);

    pkt_msg->som        = 0x4040;      
    pkt_msg->para_id    = 0;
    pkt_msg->para_len   = response_len+PACKET_PARAMETER_BASE_LEN;       
    pkt_msg->msg_id     = MSG_AUTH_RESPONSE;
    pkt_msg->msg_len    = pkt_msg->para_len+PACKET_MESSAGE_BASE_LEN;       
    //iAp2CopyBuffer(iap2.response_len,temp_data, (usb_bulkin_data+AUTH_DATA_FIRST_OFFSET)); 
    usb_bulkin_data[iap2.pkt_len-1]  = iAp2PacketCalcChecksum((BYTE*)pkt_msg,iap2.pkt_len-kIAP2PacketHeaderLen-1);
    
    BulkInSetAddr(USB_BULK_IN_DATA_ADDR);
    BulkInStart(iap2.pkt_len);  
    //BulkInSetData(iap2.pkt_len);

    iap2.ack_timer          = 0;
    iap2.state              = STATE_ACC_CONTROL_SEND;
    iAp2PrintState(prev_state);
}
void iAp2IdentSetInfo(void)
{
    PIAP2_CONTROL_MESSAGE   pkt_msg;
    BYTE    prev_state;
    prev_state = iap2.state;

    pkt_msg             = (PIAP2_CONTROL_MESSAGE)(usb_bulkin_data+kIAP2PacketHeaderLen); 
    iap2.pkt_len        = iAp2GetIdentInfoX(usb_bulkin_data+kIAP2PacketHeaderLen+PACKET_MESSAGE_BASE_LEN)+kIAP2PacketHeaderLen+PACKET_MESSAGE_BASE_LEN+1;

    iAp2HeaderSet(iap2.session_id);
    pkt_msg->som        = 0x4040;      
    pkt_msg->msg_len    = iap2.pkt_len-kIAP2PacketHeaderLen-1;       
    pkt_msg->msg_id     = MSG_IDENTIFICATION_INFO;
    
    usb_bulkin_data[iap2.pkt_len-1]  = iAp2PacketCalcChecksum((BYTE*)pkt_msg,pkt_msg->msg_len);

    BulkInStart(iap2.pkt_len);  
    //BulkInSetData(iap2.pkt_len);  

    iap2.ack_timer          = 0;
    iap2.state              = STATE_ACC_CONTROL_SEND;
    iAp2PrintState(prev_state);
}

void iAp2MessageWait(void)
{
    BYTE    prev_state;
    prev_state = iap2.state;

    if(iap2.flags&IAP2_FLAG_BULKOUT_READY)
    {
        iap2.state  = (iap2.session.info_done)?STATE_DEV_MESSAGE_GET:STATE_DEV_CONTROL_GET;
        iAp2PrintState(prev_state);
    }
    else if(iap2.ack_timer > IAP2_ACK_TIMEOUT_MAX)
    {
        /*
        if(!iap2.session.acked)
        {
            iap2.retry++;
            iap2.pkt.prev_psn       = iap2.pkt.psn;
            iap2.state          = iap2.pkt.state;
            iAp2PrintState(prev_state);
        }
        */
    }
}
void iAp2MessageGet(void)
{
    PIAP2_PACKET_HEADER     pktout_header; 
    BYTE    prev_state,*pbuf;
    prev_state = iap2.state;

    //BulkOutAck(0);
    iAp2CheckDevData(&pktout_info);
    BulkOutAck(1);

    //iAp2PrintPacket(pktout_info.data_ptr,pktout_info.len);
    pktout_header = (PIAP2_PACKET_HEADER)pktout_info.data_ptr;
    //DBGPRINT_IAP2_INFO("(%x,",pktout_header->seq);
    //DBGPRINT_IAP2_INFO("%x)#",pktout_header->ack);
    //DBGPRINT_IAP2_INFO("(%x,",iap2.pkt.prev_psn);
    //DBGPRINT_IAP2_INFO("%x)\r\n",iap2.pkt.pan);
    
    if(pktout_header->sess==0)  // for ack packet
    {
        if(pktout_header->ctl==0xC0)   //SYN+ACK
        {
            iap2.state = STATE_ACC_LINK_SET_ACK;
        }
        else if(pktout_header->ctl==kIAP2PacketControlMaskACK) //Ack Only
        {
            iap2.state   = (iap2.session.info_done)?STATE_DEV_MESSAGE_WAIT:STATE_DEV_CONTROL_WAIT;   ;
        }
        DBGPRINT_IAP2_INFO("[0:ctl=%x],",pktout_header->ctl);
        //if(iap2.pkt.authed)
        if(iap2.session.info_done && !iap2.session.msg_done)
        {
            iap2.pkt.psn    = iap2.pkt.prev_psn+1;
            iap2.pkt.type   = ACC_MESSAGE_REQUEST;
            //iap2.state      = STATE_ACC_MESSAGE_SET_INFO;
            iAp2MessageSet();
        }
        //BulkOutAck(1);
        //else
        //{
        //    iAp2PrintState(prev_state);
        //}
        iAp2PrintState(prev_state);
        return;
        //if(prev_state != iap2.state) return;
    }
    //Normal
    if(pktout_header->ack==iap2.pkt.prev_psn)
    {
        //iap2.pkt.acked = 1;
        iap2.ack_timer  = 0;
        if(pktout_header->sess==PACKET_SESSION_ID)
        {
            BYTE *pmsg;
    
            iap2.session_id = pktout_header->sess;
            iap2.pkt.pan    = pktout_header->seq;
            iap2.pkt.type   = ACC_PACKET_ACK;
            //iap2.pkt.prev_psn   = pktout_header->ack;
            iap2.msg_out    = (PIAP2_CONTROL_MESSAGE)(pktout_info.data_ptr+kIAP2PacketHeaderLen);
            iap2.state      = (iap2.session.info_done)?STATE_ACC_MESSAGE_SET_ACK:STATE_ACC_CONTROL_SET_ACK;
            pmsg = (BYTE *)iap2.msg_out;
            //printf("[2:msg=%x",HIBYTE(iap2.msg_out->msg_id));
            //printf("%x],",LOBYTE(iap2.msg_out->msg_id));
            DBGPRINT_IAP2_INFO("[2:msg=%x",HIBYTE(iap2.msg_out->msg_id));
            DBGPRINT_IAP2_INFO("%x,",LOBYTE(iap2.msg_out->msg_id));
            DBGPRINT_IAP2_INFO("done:%x],",iap2.session.info_done);
            // for responsing message
            switch(iap2.msg_out->msg_id)
            {
                case 0xAA00:    // Auth RequestCert
                    //iap2.pkt.id     = 1;
                    //iap2.pkt.new    = 1;
                    //iap2.pkt.acked  = 0;
                    //iap2.pkt.psn    = pktout_header->ack+1;
                    iap2.pkt.type   |= ACC_CONTROL_RESPONSE;
                    //iap2.pkt.psn    = iap2.pkt.prev_psn+1;
                    iap2.pkt.state  = STATE_ACC_AUTH_SET_CERT;
                    break;
                case 0xAA02:    // Auth Challenge
                    //iap2.pkt.id     = 1;
                    //iap2.pkt.new    = 1;
                    //iap2.pkt.acked  = 0;
                    //iap2.pkt.psn    = pktout_header->ack+1;
                    //iap2.pkt.psn    = iap2.pkt.prev_psn+1;
                    iap2.pkt.type   |= ACC_CONTROL_RESPONSE;
                    iap2.pkt.state  = STATE_ACC_AUTH_CHALLENGE_SET;
                    break;
                case 0xAA04:    // Auth Fail
                    //iap2.pkt.id     = 1;
                    //iap2.pkt.new    = 1;
                    //iap2.pkt.acked  = 0;
                    //iap2.pkt.psn    = pktout_header->ack+1;
                    //iap2.pkt.psn    = iap2.pkt.prev_psn+1;
                    iap2.pkt.type   |= ACC_CONTROL_RESPONSE;
                    iap2.pkt.state  = STATE_ACC_AUTH_SET_CERT;
                    break;
                case 0xAA05:    // Auth Succeeded
                    iap2.session.auth_done = 1;
                    break;
                case 0x1D00:    // StartIdent
                    //iap2.pkt.id     = 1;
                    //iap2.pkt.new    = 1;
                    //iap2.pkt.acked  = 0;
                    //iap2.pkt.psn    = pktout_header->ack+1;
                    //iap2.pkt.psn    = iap2.pkt.prev_psn+1;
                    iap2.pkt.type   |= ACC_CONTROL_RESPONSE;
                    iap2.pkt.state  = STATE_ACC_IDENT_SET_INFO;
                    break;
                case 0x1D02:    //IdentAccept 

                    if(iap2.flags&IAP2_FLAG_SHARE_MEM)
                    {
                        BYTE old_bank;
                        // switch to playback/record buffer when finish
                        iap2.flags &= ~IAP2_FLAG_SHARE_MEM;
                        old_bank = hw_regs[0x00];
                        McuWriteReg(0x00,BANK_MISC);
                        McuWriteRegMask(0x06,0x10,0x18);
                        hw_regs[0x00] = old_bank;
                        DBGPRINT_IAP2_INFO("[playback buffer on]\r\n",0);
                    }
                    //iap2.pkt.id     = 1;
       
                        //iap2.pkt.acked  = 0;
                    //iap2.pkt.psn    = pktout_header->ack+1;
                    //iap2.pkt.authed = 1;
                    iap2.session.info_done = 1;
                    /*
                    //iap2.pkt.psn    = iap2.pkt.prev_psn+1;
                    //iap2.pkt.new    = 1;
                    //iap2.msg_index  = iap2.msg_queue[0];
                    iap2.pkt.state  = STATE_ACC_MESSAGE_SET_INFO;
                    if(iap2.msg_count>0)
                    {
                        iap2.msg_count--;
                        for(i=0;i<iap2.msg_count;i++)
                            iap2.msg_queue[i] = iap2.msg_queue[i+1];
                        iap2.msg_queue[i] = 0;
                    }
                    */
                    
                    break;
                case 0xAE01:    // Power Update
                    /*
                    for(i=0;i<(iap2.msg_out->para_len-2);i++)
                    {
                        //printf("%x,",pmsg[8+i]);
                        DBGPRINT_IAP2_INFO("%x,",pmsg[8+i]);
                    }
                    */
                    break;
                case 0xEA00:    //Start EA
                    iap2.session.ea_start   = 1;
                    break;
                case 0xEA01:    //Stop EA
                    iap2.session.ea_start   = 0;
                    break;
                default:
                    break;  

            }
            //iAp2MessageAckSet();
                   
        }
        else if(pktout_header->sess==SECOND_SESSION_ID) 
        {
            //if(iap2.pkt.ea==0)
            //{
            //    iap2.pkt.ea = 1;
            //}
            iap2.session_id    = pktout_header->sess;
            iap2.pkt.pan   = pktout_header->seq;
            //iap2.pkt.prev_psn   = pktout_header->ack;
            iap2.msg_out    = (PIAP2_CONTROL_MESSAGE)(pktout_info.data_ptr+kIAP2PacketHeaderLen);
            iap2.state      = STATE_ACC_MESSAGE_SET_EAACK;
            // for response EA
            //iap2.pkt.id     = 2;
            //iap2.pkt.psn    = iap2.pkt.prev_psn+1;
            //iap2.pkt.state  = STATE_ACC_MESSAGE_SET_EA;
            //printf("[3:seq=%x],",pktout_header->seq);
            DBGPRINT_IAP2_INFO("[3:seq=%x],",pktout_header->seq);
        }
        //if(iap2.pkt.authed) iAp2CheckMsg();
        /*
        if(iap2.pkt.authed && iap2.msg_count<MESSAGE_COUNT)
        {
            iap2.pkt.new    = 1;
            if(iap2.pkt.psn == pktout_header->ack)
            { 
                iap2.pkt.psn    = iap2.pkt.prev_psn+1;
                iap2.msg_index  = msg_queue[iap2.msg_count++];
                
                for(i=0;i<iap2.msg_count;i++)
                    iap2.msg_queue[i] = iap2.msg_queue[i+1];
                iap2.msg_queue[i] = 0; 
                
            }
            iap2.pkt.state  = STATE_ACC_MESSAGE_SET_INFO;
            //printf("[1:index=%x],",iap2.msg_index);
            DBGPRINT_IAP2_INFO("[1:index=%x,",iap2.msg_index);
            DBGPRINT_IAP2_INFO("acc_psn=%x,",iap2.pkt.psn);
            DBGPRINT_IAP2_INFO("dev_ack=%x],",pktout_header->ack);
        }        
        */
    }
    // Retransmission
    else
    {
        if(pktout_header->sess==PACKET_SESSION_ID)
        {
            /*
            PIAP2_CONTROL_MESSAGE pmsg_out;
            pmsg_out = (PIAP2_CONTROL_MESSAGE)(pktout_info.data_ptr+kIAP2PacketHeaderLen);
            switch(pmsg_out->msg_id)
            {
                case 0xAE01:    // Power Update  
                case 0xAA00:    // Auth RequestCert
                case 0xAA02:    // Auth Challenge
                case 0xAA05:    // Auth Succeeded
                case 0x1D00:    // StartIdent
                case 0x1D02:    // IdentAccept
                    //iap2.session    = pktout_header->sess;
                    iap2.pkt.pan   = pktout_header->seq;
                    //iap2.pkt.prev_psn   = pktout_header->ack;
                    //iap2.pkt.prev_psn   = iap2.pkt.psn;
                    //iap2.msg_out    = (PIAP2_CONTROL_MESSAGE)(pktout_info.data_ptr+kIAP2PacketHeaderLen);
                    iap2.state      = STATE_ACC_MESSAGE_SET_ACK;
                    break;
                case 0xEA00:    // Start EA
                case 0xEA01:    // Stop EA
                default:
                    iap2.pkt.pan   = pktout_header->seq;
                    //iap2.state   = STATE_DEV_MESSAGE_WAIT;   ;
                    iap2.state      = STATE_ACC_MESSAGE_SET_ACK;
                    break;
            }
            //printf("[4:ack=%x],",pktout_header->ack);
            */
            iap2.pkt.pan    = pktout_header->seq;
            iap2.state      = (iap2.session.info_done)?STATE_ACC_MESSAGE_SET_ACK:STATE_ACC_CONTROL_SET_ACK;
            DBGPRINT_IAP2_INFO("[4:ack num=%x],",pktout_header->ack);
        }
        else
        {
            iap2.state   = (iap2.session.info_done)?STATE_DEV_MESSAGE_WAIT:STATE_DEV_CONTROL_WAIT;   ;
            //printf("[5:ack=%x],",pktout_header->ack);
            DBGPRINT_IAP2_INFO("[5:ack num=%x],",pktout_header->ack);
        }

    }
    //BulkOutAck(1);
    pbuf = (BYTE*)&iap2.pkt;
    //printf("[pkt[0]:%i,",pbuf[0]);
    //printf("psn:%x]\r\n",iap2.pkt.psn);
    DBGPRINT_IAP2_INFO("[pkt[0]:%x,",pbuf[0]);
    DBGPRINT_IAP2_INFO("psn:%x]\r\n",iap2.pkt.psn);
//    iap2.bulkout.reset = 1;
    iAp2PrintState(prev_state);
}
void iAp2MessageAckSet(void)
{
    BYTE    prev_state;
    
    prev_state = iap2.state;
    
    iap2.pkt_len     = kIAP2PacketHeaderLen;
    iAp2HeaderSet(0);
    BulkInSetAddr(USB_BULK_IN_DATA_ADDR);
    BulkInStart(iap2.pkt_len);  
    //BulkInSetData(iap2.pkt_len);  

//    iap2.pkt.type       = PACKET_TYPE_ACK;
    iap2.state          = (iap2.session.info_done)?STATE_ACC_MESSAGE_SEND:STATE_ACC_CONTROL_SEND;
    //printf("[ack:%x],",iap2.pkt.pan);
    //DBGPRINT_IAP2_INFO("[ack:%x],",iap2.pkt.pan);
    iAp2PrintState(prev_state);
//    iap2.ack_timer      = 0;
}

void iAp2MessageSet(void)
{
    BYTE*   pkt_msg;
    BYTE    prev_state, len;
    prev_state = iap2.state;

    //pkt_msg = (PIAP2_CONTROL_MESSAGE)iAp2GetMsgInfo(iap2.msg_index);
    pkt_msg =  usb_bulkin_data+kIAP2PacketHeaderLen;
    len = iAp2GetMsgInfo(iap2.msg_index, pkt_msg);
    //iAp2CopyBuffer(pkt_msg.msg_len, (BYTE*)&pkt_msg, usb_bulkin_data+kIAP2PacketHeaderLen);
    iap2.pkt_len     = len + kIAP2PacketHeaderLen+1;
    iAp2HeaderSet(iap2.session_id);
    usb_bulkin_data[iap2.pkt_len-1]  = iAp2PacketCalcChecksum((BYTE*)pkt_msg, len);

    BulkInStart(iap2.pkt_len);  
    //BulkInSetData(iap2.pkt_len);  

    iap2.ack_timer      = 0;
//    iap2.retry          = 0;
//    iap2.pkt.type       = PACKET_TYPE_MSG;
    iap2.state          = STATE_ACC_MESSAGE_SEND;
//    iap2.flags |=IAP2_FLAG_TIMEOUT;
    
    //printf("{msg:%x",HIBYTE(msg_id));
    //printf("%x,",LOBYTE(msg_id));
    //printf("psn:%x]",iap2.pkt.prev_psn);
    DBGPRINT_IAP2_INFO("[[msg_index:%x,",iap2.msg_index);
    DBGPRINT_IAP2_INFO("psn:%x]]\r\n",iap2.pkt.psn);
    if(++iap2.msg_index==iap2.msg_count)
        iap2.session.msg_done = 1;
//    iap2.msg_index++;

//    iAp2PrintPacket(usb_bulkin_data,iap2.pkt_len);
    iAp2PrintState(prev_state);
}

void iAp2MessageSend(void)
{
//    WORD    next_msg, curr_msg;
    BYTE    prev_state;
    prev_state = iap2.state;
    
    if(iap2.retry >= IAP2_RETRY_MAX)
    {
        iap2.state  = STATE_ACC_RESET;
    }
    else if(BulkInDataDone())
    {        
        iap2.pkt.prev_psn = iap2.pkt.psn;
        //DBGPRINT_IAP2_INFO("[[%x,",iap2.pkt.prev_psn);;
        //DBGPRINT_IAP2_INFO("%x]]\r\n",iap2.pkt.pan);
        if (iap2.pkt.type&ACC_PACKET_ACK)
        {
            iap2.pkt.type &=~(ACC_PACKET_ACK);
            if(iap2.pkt.type == ACC_CONTROL_RESPONSE)
            {
                iap2.pkt.psn        = iap2.pkt.prev_psn+1;
                iap2.state          = iap2.pkt.state;
            }
            else if (iap2.session.info_done && !iap2.session.msg_done)
            {
                iap2.pkt.psn        = iap2.pkt.prev_psn+1;
                iap2.pkt.type       = ACC_MESSAGE_REQUEST;
                //iap2.state          = STATE_ACC_MESSAGE_SET_INFO;
                iAp2MessageSet();
                return;
            }
            else
            {
                iap2.state   = (iap2.session.info_done)?STATE_DEV_MESSAGE_WAIT:STATE_DEV_CONTROL_WAIT;   ;
            }
        }
        else
        {
            iap2.state   = (iap2.session.info_done)?STATE_DEV_MESSAGE_WAIT:STATE_DEV_CONTROL_WAIT;   ;
        }
        
        /*
        //desker++ 220831
        if(iap2.pkt.new)
        {
            iap2.pkt.new        = 0;
            iap2.retry          = 0;
            iap2.pkt.acked      = 0;
            iap2.pkt.prev_psn       = iap2.pkt.psn;
            iap2.state          = iap2.pkt.state;
            iAp2PrintState(prev_state);
            //if(iap2.pkt.prev_psn==0x56) Delay500us(50);
        }
        else
        {
            iap2.state  = STATE_DEV_MESSAGE_WAIT;    //iap2.state_wait;
        }
        */
        //iAp2PrintPacket(usb_bulkin_data,iap2.pkt_len);
        iAp2PrintState(prev_state);
    }
    else if(iap2.ack_timer > IAP2_ACK_TIMEOUT_MAX)
    {
        //iap2.state  = STATE_DEV_MESSAGE_WAIT;    //iap2.state_wait;
        iap2.state   = (iap2.session.info_done)?STATE_DEV_MESSAGE_WAIT:STATE_DEV_CONTROL_WAIT;   ;
        iAp2PrintState(prev_state);
        //iap2.retry++;
        //iap2.ack_timer = 0;
        //BulkInSetData(iap2.pkt_len); 
    }

}

void iAp2StateMachine(void)
{
    /*
    if(iap2.flags&IAP2_FLAG_SUSPEND)     // suspand
    {
        BYTE old_bank, connect_value;
        iap2.flags = 0;
        old_bank = usb_regs[0x00];
    	McuWriteReg(0x00,BANK_MISC);
        connect_value = McuReadReg(0x30);    //bit1:connect/disconnect bit0:highspeed/fullspeed
    	usb_regs[0x00] = old_bank;
        if(connect_value&0x02)
        {
            printf("Clear reset count(%x),",iap2.reset_count);
            iap2.reset_count = 0;
        }
        iap2.state = STATE_ACC_WAIT;
        //printf("suspand=>%x...\r\n",iap2.state);
        DBGPRINT_IAP2_STATE("suspand=>(v=%x)...\r\n",connect_value);
    }
    */
    switch(iap2.state)
    {
 /*
        case STATE_ACC_INIT:
            iAp2Init();
            break;
        case STATE_ACC_WAIT:
            iAp2Waiting();
            break;

        case STATE_ACC_PREPARE:
            //iAp2Prepare();
            iAp2PrepareX();
            break;
        case STATE_ACC_RESET:
            iAp2Reset();
            break;
*/
        case STATE_ACC_WAIT:
            break;
        case STATE_ACC_CONNECT:
            iAp2Connect();
            break;
        case STATE_ACC_RESET:
            iAp2Reset();
            break;
//        case STATE_ACC_RECONNECT:
//            iAp2Reconnect();
//            break;
        case STATE_ACC_DETECT_SET:
            iAp2DetectSet();
            break;
        case STATE_ACC_DETECT_SEND:
            iAp2DetectSend();
            break;
        case STATE_DEV_DETECT_WAIT:
            iAp2DetectWait();
            //iAp2PacketWait(STATE_DEV_DETECT_GET,STATE_ACC_DETECT_SEND);
            break;
        case STATE_DEV_DETECT_GET:
            iAp2DetectGet();
            iap2.bulkout.reset = 1;
            break;

        case STATE_ACC_LINK_SET_SYN:
            iAp2SynSet();
            break;

        case STATE_ACC_LINK_SEND_SYN:
            iAp2SynSend();
            //iAp2PacketSend(STATE_DEV_LINK_WAIT_SYNACK);
            break;

        case STATE_DEV_LINK_WAIT_SYNACK:
            iAp2SynAckWait();
            //iAp2PacketWait(STATE_DEV_LINK_GET_SYNACK,STATE_ACC_LINK_SEND_SYN);
            break;

        case STATE_DEV_LINK_GET_SYNACK:
            iAp2SynAckGet();
            iap2.bulkout.reset = 1;
            break;

        case STATE_ACC_LINK_SET_ACK:
            iAp2AckSet();
            break;

        case STATE_ACC_LINK_SEND_ACK:
            iAp2MessageSend();
            //iAp2AuthSend();
            //iAp2PacketSend(STATE_DEV_AUTH_WAIT);
            break;

        case STATE_ACC_AUTH_SET_CERT:
            iAp2AuthSetCert(iap2_cp_data);
            break;
        case STATE_ACC_AUTH_CHALLENGE_SET:
        {
            /*            
            BYTE *buff,i;
            buff = (BYTE*)iap2.msg_out;
            for(i=0;i<32;i++)
            {
                printf("%x,",buff[i]);
            }
            printf("para_len=%x\r\n",iap2.msg_out->para_len);
            */
            Mi2cSetSlave(IAP2_CP_SLAVE_ADDR);
            iAp2CpSetChallengeLen(iap2.msg_out->para_len-4);
            iAp2CpSetChallengeData(iap2.msg_out->para_len-4,(BYTE*)iap2.msg_out+AUTH_DATA_FIRST_OFFSET-kIAP2PacketHeaderLen);
            iAp2CpSetAuthControl(0x01);
            iap2.state  = STATE_ACC_AUTH_SET_RESPONSE;
            //iap2.state  = STATE_ACC_IDLE;
            //printf("================>to state %x\r\n",iap2.state);
        //
        }
            break;
        case STATE_ACC_AUTH_SET_RESPONSE:
            if(iAp2CpGetAuthStatus()==0x10)
            {
                iAp2AuthSetResponse();
                Mi2cSetSlave(0x22);
            }
            break;

        case STATE_ACC_IDENT_SET_INFO:
            iAp2IdentSetInfo();
            break;
/*
        case STATE_DEV_CONTROL_WAIT:
            iAp2MessageWait();

        case STATE_DEV_CONTROL_GET:
            iAp2MessageGet();
            break;
        case STATE_ACC_CONTROL_SET_ACK:
            iAp2MessageAckSet();
            break;
        case STATE_ACC_CONTROL_SEND:
            iAp2MessageSend();
            break;
*/
        case STATE_ACC_MESSAGE_SET_INFO:
            iAp2MessageSet();
            break;

        case STATE_DEV_CONTROL_WAIT:
        case STATE_DEV_MESSAGE_WAIT:
            iAp2MessageWait();
//#if (IAP2_SUPPORT_POWER==1)
//            if(iap2.pkt.authed) iAp2MsgCheckPower();
//#endif
//            if(!iap2.pkt.update && iap2.pkt.ea) iAp2EACheckStatus();
            break;
        case STATE_DEV_CONTROL_GET:
        case STATE_DEV_MESSAGE_GET:
            iAp2MessageGet();
            //iap2.bulkout.reset = 1;
            break;
        case STATE_ACC_CONTROL_SET_ACK:
        case STATE_ACC_MESSAGE_SET_ACK:
            iAp2MessageAckSet();
            break;
        case STATE_ACC_CONTROL_SEND:
        case STATE_ACC_MESSAGE_SEND:
            iAp2MessageSend();
            break;
        case STATE_ACC_MESSAGE_SET_EA:
            //iAp2EADataSet();
            break;
        case STATE_ACC_MESSAGE_SET_EAACK:
            //iAp2EAAckSet();
            break;
        case STATE_ACC_IDLE:
        default:
            break;
    }
    
    if(iap2.flags&IAP2_FLAG_PLUGIN) 
    {    
        iap2.flags &=~IAP2_FLAG_PLUGIN;
        if(iap2.session.detected)
        {
            iap2.state = STATE_ACC_RESET;
        }
    }
    
}
