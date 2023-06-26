#ifndef _CTUSB_SPI_H_
#define _CTUSB_SPI_H_

#define MCU_MAILBOX_BASE    0x90

#define MCU_COUNT_REG       (MCU_MAILBOX_BASE+0x0F)
#define MCU_MAILBOX_CMD     (MCU_MAILBOX_BASE+0x00)
#define MCU_MAILBOX_PARA1   (MCU_MAILBOX_BASE+0x01)
#define MCU_MAILBOX_PARA2   (MCU_MAILBOX_BASE+0x02)
#define MCU_MAILBOX_PARA3   (MCU_MAILBOX_BASE+0x03)

#define FLASH_READ_CHIPID           0x9F
#define FLASH_READ_STATUS           0x05

#define FLASH_WRITE_DISABLE         0x04
#define FLASH_WRITE_ENABLE          0x06
#define FLASH_WRITE_STATUS          0x01

#define FLASH_SECTOR_ERASE          0x20    //4K
#define FLASH_BLOCK_ERASE           0xD8
#define FLASH_CHIP_ERASE            0xC7
             
#define FLASH_READ_DATA             0x03

#define FLASH_WRITE_DATA            0x02    //Page Program

#define FLASH_READY_DELAY           50

#define FLASH_PAGE_PROGRAM_TIME     50          //10*80   =800us
#define FLASH_WRITE_STATUS_TIME     500         //200*80  =16000us=16ms
#define FLASH_SECTOR_EARSE_TIME     5000        //4000*80 =320000us=320ms

void SpiInit(WORD in_addr, WORD out_addr);
BYTE SpiSwitch(BYTE index);
void SpiWrite(WORD len);
void SpiRead(BYTE cmd_len, BYTE data_len);
BYTE SpiReadStatus(BYTE count);
BYTE* SpiReadId(void);
BYTE* SpiReadData(DWORD addr, BYTE len);

void SpiSetPageProgramTime(WORD time);
void SpiWriteCmd(BYTE cmd);
void SpiWriteStatus(BYTE status);
void SpiWriteData(DWORD addr, BYTE *data_buffer, BYTE len);
void SpiEarse(BYTE cmd, DWORD addr);
BYTE SpiIsDone(BYTE diff);
#endif  //__CTUSB_SPI_H__
