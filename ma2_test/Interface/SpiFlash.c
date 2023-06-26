//#include "Global.h"
#include "Mcu.h"
#include "Uart.h"
#include "Spiflash.h"

WORD    spi_in_addr, spi_out_addr;
void SpiInit(WORD in_addr, WORD out_addr)
{
    BYTE    old_bank;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_SPI);
    spi_in_addr = in_addr;
    spi_out_addr = out_addr;
    McuWriteReg(0x1A,LOBYTE(spi_in_addr));
    McuWriteReg(0x1B,HIBYTE(spi_in_addr));
    McuWriteReg(0x12,LOBYTE(spi_out_addr));
    McuWriteReg(0x13,HIBYTE(spi_out_addr));
    //switch spi to mcu
    //McuWriteReg(0x21,0x02);
    McuWriteReg(0x00,old_bank);
    //printf("spi in/out addr=[%x",HIBYTE(spi_in_addr));
    //printf("%x,",LOBYTE(spi_in_addr));
    //printf("%x",HIBYTE(spi_out_addr));
    //printf("%x]\r\n",LOBYTE(spi_out_addr));
}
/*
BYTE SpiSwitch(BYTE index)
{
    BYTE    old_bank,ret;;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_SPI);
    ret =(McuReadReg(0x20)&0x02)?0:1;
    if(ret) McuWriteRegMask(0x20,index&0x01,0x01);
    //switch spi to mcu
    McuWriteReg(0x21,0x02);
    McuWriteReg(0x00,old_bank);
    return ret;
}
*/
// unit: 10us
void SpiSetPageProgramTime(WORD time)
{
    BYTE    old_bank;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_SPI);
    McuWriteReg(0x23,LOBYTE(time));
    McuWriteReg(0x24,HIBYTE(time));
    //McuWriteRegMask(0x21,0x04,0x04);    //Set for MCU SPI Write
    McuWriteReg(0x00,old_bank);     
}
void SpiWrite(WORD len)
{
    BYTE    old_bank,i;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_SPI);

    McuWriteRegMask(0x21,0x04,0x04);    //Set for MCU SPI Write

    McuWriteReg(0x14,LOBYTE(len));
    McuWriteReg(0x15,HIBYTE(len));
    //fire
    McuWriteReg(0x10,1); 
    for(i=0;i<100;i++) { 
        while(McuReadReg(0x16)) break;
        Delay100us(1);
    }
    McuWriteReg(0x16,0x00);
    McuWriteReg(0x00,old_bank);
 }

void SpiRead(BYTE cmd_len, BYTE data_len)
{
    BYTE    old_bank,i;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_SPI);
    //McuWriteReg(0x1A,LOBYTE(SPI_IN_DATA_ADDR));
    //McuWriteReg(0x1B,HIBYTE(SPI_IN_DATA_ADDR));
    McuWriteReg(0x1C,data_len);     //low byte 
    McuWriteReg(0x1D,0);            //high byte
    McuWriteReg(0x19,cmd_len);
    //fire 
    McuWriteReg(0x18,1); 
    for(i=0;i<100;i++) { 
        while(McuReadReg(0x1E)) break;
        Delay100us(1);
    }
    //while(McuReadReg(0x1E)==0);
    McuWriteReg(0x1E,0x00);
    McuWriteReg(0x00,old_bank);
}

BYTE SpiReadStatus(BYTE count)
{
    spi_read_data[0] = FLASH_READ_STATUS;
    SpiRead(1,count);
    return spi_read_data[count];
}

BYTE* SpiReadId(void)
{
    spi_read_data[0] = FLASH_READ_CHIPID;
    SpiRead(1,3);
    //printf("id=[%x,",spi_read_data[1] );
    //printf("%x,",spi_read_data[2] );
    //printf("%x]\r\n",spi_read_data[3]);
    return (spi_read_data+1);
}

BYTE* SpiReadData(DWORD addr, BYTE len)
{
    BYTE i;
    for(i=0;i<128;i++) spi_read_data[i]=0;
    spi_read_data[0] = FLASH_READ_DATA;
    spi_read_data[1] = (BYTE)((addr>>16)&0xFF);
    spi_read_data[2] = (BYTE)((addr>>8)&0xFF);
    spi_read_data[3] = (BYTE)(addr&0xFF);
    SpiRead(4,len);
    return (spi_read_data+4);
}

void SpiWriteStatus(BYTE status)
{
    SpiSetPageProgramTime(1);
    spi_write_data[0] = FLASH_WRITE_ENABLE;
    SpiWrite(1);

    SpiSetPageProgramTime(FLASH_WRITE_STATUS_TIME);
    spi_write_data[0] = FLASH_WRITE_STATUS;
    spi_write_data[1] = status;
    SpiWrite(2);

    if(!SpiIsDone(1))  printf("spi timeout =%x\r\n",spi_write_data[0]);
}
void SpiEarse(BYTE cmd, DWORD addr)
{
    BYTE temp;
    SpiSetPageProgramTime(1);
    spi_write_data[0] = FLASH_WRITE_ENABLE;
    SpiWrite(1);

    SpiSetPageProgramTime(FLASH_SECTOR_EARSE_TIME);

    if(cmd==FLASH_CHIP_ERASE)
    {
        spi_write_data[0] = cmd;
        SpiWrite(1);
    }
    else
    {
        spi_write_data[0] = cmd;
        spi_write_data[1] = (BYTE)((addr>>16)&0xFF);
        spi_write_data[2] = (BYTE)((addr>>8)&0xFF);
        spi_write_data[3] = (BYTE)(addr&0xFF);
        SpiWrite(4);
    }
    temp = SpiIsDone(50);
    printf("spi earse time=%d (x50ms)\r\n",temp);
}
void SpiWriteData(DWORD addr,  BYTE *data_buffer, BYTE len)
{
    BYTE i;

    SpiSetPageProgramTime(1);
    spi_write_data[0] = FLASH_WRITE_ENABLE;
    SpiWrite(1);

    SpiSetPageProgramTime(FLASH_PAGE_PROGRAM_TIME);

    spi_write_data[0] = FLASH_WRITE_DATA;
    spi_write_data[1] = (BYTE)((addr>>16)&0xFF);
    spi_write_data[2] = (BYTE)((addr>>8)&0xFF);
    spi_write_data[3] = (BYTE)(addr&0xFF);

    for(i=0;i<len;i++)
        spi_write_data[i+4]=data_buffer[i];
    
    SpiWrite(len+4);
    i = SpiIsDone(1);
    printf("spi write time=%d(ms)\r\n",i);
}

BYTE SpiIsDone(BYTE diff)
{
    BYTE i;
    WORD delay_time;
    delay_time = (WORD)(diff)*10;
    for(i=1;i<250;i++)
    {
        Delay100us(delay_time);
        if((SpiReadStatus(1)&0x01)==0)
            break;
    }
    return i;
}
