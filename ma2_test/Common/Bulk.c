//#include "Reg51.h"
#include "Mcu.h"
//#include "Global.h"
#include "Settings.h"
#include "Uart.h"
#include "Bulk.h"
#include "iap2.h"

#define     PRINT_BULK_COMMON              0

#if (PRINT_BULK_COMMON)
#define DBGPRINT_BULK_COMMON(_FMT_,_X_)           printf(_FMT_,_X_)
#else
#define DBGPRINT_BULK_COMMON(_FMT_,_X_)         
#endif

void BulkInit(BYTE uac_type)
{
	BYTE 	old_bank;
	old_bank = hw_regs[0x00];		
	McuWriteReg(0x00,BANK_BULK);
    //endpoint=0x07
	//McuWriteReg(0x01,0x10|ENDPOINT_BLUK_OUT);                              
	McuWriteReg(0x01,0x17); 
	//uac2:0x200(512) uac1:0x40(64)                             
    McuWriteReg(0x02,(uac_type==TYPE_UAC2)?0x00:0x40);	            
    McuWriteReg(0x03,(uac_type==TYPE_UAC2)?0x02:0x00);	            
	//out addr=0x700-0x77F
	McuWriteReg(0x05,HIBYTE(USB_BULK_OUT_DATA_ADDR));
	McuWriteReg(0x04,LOBYTE(USB_BULK_OUT_DATA_ADDR));
	McuWriteReg(0x07,HIBYTE(USB_BULK_OUT_DATA_ADDR_END));
	McuWriteReg(0x06,LOBYTE(USB_BULK_OUT_DATA_ADDR_END));
	McuWriteReg(0x08,0x01); //Set addr  //0x0C/0x0D

    //endpoint=0x08
	//McuWriteReg(0x10,0x10|ENDPOINT_BLUK_IN);
	McuWriteReg(0x10,0x18);
	// in addr	0x0680
	McuWriteReg(0x13,HIBYTE(USB_BULK_IN_DATA_ADDR));
	McuWriteReg(0x12,LOBYTE(USB_BULK_IN_DATA_ADDR));
	
    McuWriteReg(0x14,0x40);     //len=64

    //desker++ 221004 for bulk out ack/nak
#if (BULK_OUT_MODE>1)
	McuWriteRegMask(0x0E,0x18,0x18);    // auto clear ACK bit after sending ACK  
#else
	McuWriteRegMask(0x0E,0x08,0x18);    // auto clear ACK bit after sending ACK  
#endif
    McuWriteRegMask(0x0F,0x04,0x04);    // ACK timeout on (600ms)                              
    
    //                            
    // bulkout_data1 = usb_bulkout_data+USB_BULK_OUT_DATA_LEN/2;      
    // for test bulkin timeout
	// McuWriteReg(0x09,0x00);
	McuWriteReg(0x00,old_bank);
}
//void BulkInSetData(WORD addr, WORD len)
void BulkInSetAddr(WORD addr)
{
	BYTE 	old_bank;
	old_bank = hw_regs[0x00];		
	
	McuWriteReg(0x00,BANK_BULK);
	// in addr	0x0680
	McuWriteReg(0x13,HIBYTE(addr));
	McuWriteReg(0x12,LOBYTE(addr));

	McuWriteReg(0x00,old_bank);
}
void BulkInStart(WORD len)
{
	BYTE 	old_bank;
	old_bank = hw_regs[0x00];		
	
	McuWriteReg(0x00,BANK_BULK);
	// set data size
	McuWriteReg(0x15,HIBYTE(len));
	McuWriteReg(0x14,LOBYTE(len));
	// go
	McuWriteRegMask(0x16,0x01,0x01);

#if(SUPPORT_IAP2)
//            printf("[T1:%x",HIBYTE(iap2.ack_timer));
//            printf("%x]\r\n",LOBYTE(iap2.ack_timer));
#endif
	McuWriteReg(0x00,old_bank);
}
BYTE BulkInDataDone(void)
{
	BYTE 	old_bank, ret_value;

	old_bank = hw_regs[0x00];		
	McuWriteReg(0x00,BANK_BULK);
	ret_value = 0;
	if((McuReadReg(0x17)&0x01))
	{
		McuWriteRegMask(0x17,0x00,0x01);
		ret_value = 1;
	}
	McuWriteReg(0x00,old_bank);
	return ret_value;
}
/*
BYTE BulkOutGetInfoX(void)
{
    BYTE len,offset,count;

    count = iap2.bulkout.count;
    len = iap2.bulkout.len[count-1]; 
    offset=iap2.bulkout.addr_offset[count-1]; 
	
    iap2.bulkout.data_ptr = bulkout_data+offset;
    //ptr->len = iap2.bulkout.len;
	//ptr->data_ptr = bulkout_data1+iap2.bulkout.addr_offset;
    //if(len!=9) iap2.bulkout.reset = 1;
    DBGPRINT_BULK_COMMON("[off:%x,",offset);
    DBGPRINT_BULK_COMMON("len:%x,",len);
    DBGPRINT_BULK_COMMON("cnt:%x]\r\n",count);
    return len;
}
*/

BYTE BulkOutGetInfo(PBULK_DATA_INFO ptr)
{
	BYTE 	old_bank;
    WORD    addr, offset;

	old_bank = hw_regs[0x00];		
	McuWriteReg(0x00,BANK_BULK);
	ptr->len = McuReadReg(0x0A);
    addr =  BYTE_TO_WORD(McuReadReg(0x0D),McuReadReg(0x0C));
    offset = addr-USB_BULK_OUT_DATA_ADDR;
	ptr->data_ptr = usb_bulkout_data+(offset);
    //ptr->offset   = (BYTE)(offset);   
	
    if(ptr->len > 9 )  McuWriteReg(0x08,0x01);         // Reset to original Base if not ack
    //McuWriteReg(0x08,0x01);         // Reset to original Base if not ack
	McuWriteReg(0x00,old_bank);

    DBGPRINT_BULK_COMMON("[%x",HIBYTE(addr));
    DBGPRINT_BULK_COMMON("%x+",LOBYTE(addr));
    DBGPRINT_BULK_COMMON("%x]\r\n",ptr->len);
	return TRUE;
}

void BulkOutAck(BYTE ack)
{
/*
#if (BULK_OUT_MODE==0)
    ack=0;
    return;
#else
	BYTE 	old_bank, mode;
    mode = BULK_OUT_MODE;
    if(ack==0 && mode==2) return;

	old_bank = hw_regs[0x00];		
	McuWriteReg(0x00,BANK_BULK);
	McuWriteRegMask(0x0E,ack<<3,0x08);    //0:nak, 1:ack
	McuWriteReg(0x00,old_bank);
    return ;
#endif
*/

	BYTE 	old_bank;
	old_bank = hw_regs[0x00];		
	McuWriteReg(0x00,BANK_BULK);
	McuWriteRegMask(0x0E,ack<<3,0x08);    //0:nak, 1:ack
	McuWriteReg(0x00,old_bank);
    return ;

}

/*
void BulkInSetDataX(WORD len)
{
	BYTE 	old_bank;
	old_bank = usb_regs[0x00];		
	
	McuWriteReg(0x00,BANK_BULK);
	// in addr	0x0680
	McuWriteReg(0x13,HIBYTE(USB_P1_DATA_BASE));
	McuWriteReg(0x12,LOBYTE(USB_P1_DATA_BASE));
	// set data size
	McuWriteReg(0x15,(len>>8)&0xFF);
	McuWriteReg(0x14,len&0xFF);
	// go
	McuWriteRegMask(0x16,0x01,0x01);

	McuWriteReg(0x00,old_bank);
}
BYTE BulkOutDataReady(void)
{
    if(iap2.flags&IAP2_FLAG_BULKOUT_READY)
    {
        iap2.flags &= ~(IAP2_FLAG_BULKOUT_READY);
        return 1;
    }
    return 0;
}
BYTE BulkOutGetInfo(PBULK_DATA_INFO ptr)
{
	BYTE 	old_bank;
    WORD    offset;

	old_bank = usb_regs[0x00];		
	McuWriteReg(0x00,BANK_BULK);
	offset = BYTE_TO_WORD(McuReadReg(0x0D),McuReadReg(0x0C))-USB_BULKOUT_DATA_BASE;
	ptr->len = McuReadReg(0x0A);
	ptr->data_ptr = usb_bulkout_data+(offset*4);
    ptr->offset   = (BYTE)(offset*4);
    
    if(ptr->len > 9)  McuWriteReg(0x08,0x01);         // Reset to original Base if not ack
	McuWriteReg(0x00,old_bank);
	return TRUE;
}
*/
