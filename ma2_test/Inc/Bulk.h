#ifndef _CTUSB_BULK_H_
#define _CTUSB_BULK_H_

#define BULK_OUT_MODE       2   //0:[b3=1,b4=0,fw=off] 1:[1,0,on(0/1)] 2:[1,1(0),on(1)] 

#define	ENDPOINT_BLUK_OUT	7
#define	ENDPOINT_BLUK_IN	8

typedef struct BULK_DATA_INFO
{
    uint8_t *data_ptr;
    uint8_t len;
    uint8_t offset;
    uint8_t ctl;
    uint8_t seq;
    uint8_t ack;
    uint8_t sess;
    uint16_t msg_id;
}BULK_DATA_INFO, *PBULK_DATA_INFO;	
//	------------------------------------
//		Function Prototypes
//	------------------------------------
void BulkInit(BYTE type);
void BulkInSetAddr(WORD addr);
void BulkInStart(WORD len);
BYTE BulkInDataDone(void);
BYTE BulkOutDataReady(void);
BYTE BulkOutGetData(BYTE *buffer);
BYTE BulkOutGetInfo(PBULK_DATA_INFO ptr);
BYTE BulkOutGetInfoX(void);
void BulkOutAck(BYTE ack);

#endif  //__CTUSB_BULK_H__
