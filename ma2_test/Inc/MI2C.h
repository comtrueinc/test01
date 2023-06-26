#ifndef __CTUSB_MI2C_H__
#define __CTUSB_MI2C_H__

//	------------------------------------
//		Function Prototypes
//	------------------------------------
void Mi2cInit(BYTE slave_addr, WORD in_addr, WORD out_addr);
BYTE Mi2cGetSlave(void);
void Mi2cSetSlave(BYTE slave);


BYTE Mi2cReadByte(BYTE addr);
void Mi2cWriteByte(BYTE addr, BYTE value);
void Mi2cWriteByteMask(BYTE addr, BYTE value, BYTE mask);
BYTE Mi2cReadData(BYTE addr, BYTE len, BYTE *buffer);
BYTE Mi2cWriteData(BYTE addr, BYTE len, BYTE *buffer);

bool MI2CWriteData0(BYTE slave_addr, int Addr, BYTE len, BYTE *buffer);
bool MI2CReadData0(BYTE slave_addr, int Addr, BYTE len, BYTE *buffer);
bool MI2CWriteByte0(int Addr, BYTE Data);
BYTE MI2CReadByte0(int Addr);
void MI2CWriteByteMask0(BYTE addr, BYTE value, BYTE mask);

#endif  //__CTUSB_MI2C_H__

