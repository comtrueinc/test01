#include "Reg51.h"
#include "Mcu.h"
#include "uart.h"
#include "mi2c.h"

#define     PRINT_MI2C_COMMON              0
#define     PRINT_MI2C_INFO                0

#if (PRINT_MI2C_COMMON)
#define DBGPRINT_MI2C_COMMON(_FMT_,_X_)           printf(_FMT_,_X_)
#else
#define DBGPRINT_MI2C_COMMON(_FMT_,_X_)         
#endif

#if (PRINT_MI2C_INFO)
#define DBGPRINT_MI2C_INFO(_FMT_,_X_)           printf(_FMT_,_X_)
#else
#define DBGPRINT_MI2C_INFO(_FMT_,_X_)         
#endif

WORD    i2c_inbuff_addr, i2c_outbuff_addr;
void Mi2cInit(BYTE slave_addr, WORD in_addr, WORD out_addr)
{
    McuWriteReg(0x00,BANK_MI2C);
    McuWriteReg(0x11,0x04);             // clk= 1.5MHz/(2^N)  1500/16 ~= 100KHz
    McuWriteReg(0x20,slave_addr);
    McuWriteReg(0x21,LOBYTE(out_addr));
    McuWriteReg(0x22,HIBYTE(out_addr));
    McuWriteReg(0x23,LOBYTE(in_addr));
    McuWriteReg(0x24,HIBYTE(in_addr));
    i2c_inbuff_addr = in_addr;
    i2c_outbuff_addr = out_addr;
}

BYTE Mi2cGetSlave(void)
{
    BYTE old_bank,value=0;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MI2C);
    value=McuReadReg(0x20);
    McuWriteReg(0x00,old_bank);
    return value;
}

void Mi2cSetSlave(BYTE slave)
{
    BYTE old_bank;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MI2C);
    McuWriteReg(0x20,slave);
    McuWriteReg(0x00,old_bank);
}

BYTE Mi2cReadByte(BYTE addr)
{
    BYTE old_bank,value,i;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MI2C);
    McuWriteReg(0x25,1);        //write addr
    McuWriteReg(0x26,1);        //read  data
    McuWriteMemS(i2c_outbuff_addr,addr);    
    McuWriteReg(0x27,1);
    for(i=0;i<50;i++)
    {
        if(McuReadReg(0x27)&0x02) break;
        Delay100us(1);
    }
    //Delay100us(10);
    McuWriteReg(0x27,0);                //b2:write 0 to clear
    if(i>=50) DBGPRINT_MI2C_COMMON("i2c read 1 timeout\r\n",0);
    value = McuReadMemS(i2c_inbuff_addr);
    McuWriteReg(0x00,old_bank);
    return  value;    
}

void Mi2cWriteByte(BYTE addr, BYTE value)
{
    BYTE old_bank,i;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MI2C);
    McuWriteReg(0x25,2);        // write len=2 (addr+data)
    McuWriteReg(0x26,0);        // read  len=0
    McuWriteMemS(i2c_outbuff_addr,addr);    
    McuWriteMemS(i2c_outbuff_addr+1,value);    
    McuWriteReg(0x27,1);        //start
    for(i=0;i<50;i++)
    {
        if(McuReadReg(0x27)&0x02) break;
        Delay100us(1);
    }
    //Delay100us(10);
    McuWriteReg(0x27,0);        //clear & stop
    if(i>=50)
    {
        DBGPRINT_MI2C_COMMON("i2c write 1 timeout(%x",McuReadReg(0x20));
        DBGPRINT_MI2C_COMMON("%x,",addr);
        DBGPRINT_MI2C_COMMON("%x,)\r\n",value);
    }
    McuWriteReg(0x00,old_bank);
}
void Mi2cWriteByteMask(BYTE addr, BYTE value, BYTE mask)
{
    BYTE new_value;

    new_value = (Mi2cReadByte(addr)&(~mask))|value;
    Mi2cWriteByte(addr, new_value);
}
/* CP3 access cannot work for restart procedure
BYTE Mi2cReadData(BYTE addr, BYTE len, BYTE *buffer)
{
    BYTE old_bank,i,j;
    old_bank = McuReadReg(0x00);
    // Write Register
    McuWriteReg(0x00,BANK_MI2C);
    McuWriteReg(0x25,1);            //write len
    McuWriteReg(0x26,len);          //read  len
    McuWriteMemS(i2c_outbuff_addr,addr);    
    for(i=0;i<len;i++)
    {
        McuWriteMemS(i2c_inbuff_addr+i,0);
    }    
    for(j=0;j<10;j++)
    {
        McuWriteReg(0x27,1);
        for(i=0;i<20;i++)
        {
            if(McuReadReg(0x27)&0x02) break;
            Delay100us(1);
        }
        McuWriteReg(0x27,0);                //b2:write 0 to clear
        if(i>=20) printf("i2c w/r timeout j=%x\r\n",j);
        if(!(McuReadReg(0x10)&0x08)) break; //0:Ack, 1:NAck (if Nack then retry)
        Delay100us(5);
    } 
    for(i=0;i<len;i++)
        buffer[i] = McuReadMemS(i2c_inbuff_addr+i);
    //McuWriteReg(0x00,old_bank);
    return i;    
}
*/
BYTE Mi2cReadData(BYTE addr, BYTE len, BYTE *buffer)
{
    BYTE old_bank,i,j;
    old_bank = McuReadReg(0x00);
    // Write Register
    McuWriteReg(0x00,BANK_MI2C);
    McuWriteReg(0x25,1);        //write len
    McuWriteReg(0x26,0);        //read  len
    for(i=0;i<len;i++)
    {
        McuWriteMemS(i2c_inbuff_addr+i,0);
    }    
    McuWriteMemS(i2c_outbuff_addr,addr);    
    for(j=0;j<10;j++)
    {
        McuWriteReg(0x27,1);
        for(i=0;i<5;i++)
        {
            if(McuReadReg(0x27)&0x02) break;    // end
            Delay100us(2);
        }
        McuWriteReg(0x27,0);                //b2:write 0 to clear
        if(i>=10) DBGPRINT_MI2C_COMMON("i2c write addr timeout(no done) addr=%x\r\n",addr);
        Delay100us(5);
        //if ack ;
        if(!(McuReadReg(0x10)&0x08)) break; //0:Ack, 1:NAck (if Nack then retry)
        //Delay100us(5);
    }
    if (j>=10)  //nack & timeout
    {
        DBGPRINT_MI2C_COMMON("i2c write addr timeout(no ack) addr=%x\r\n",addr);        
        return 0;
    } 
    // Read Data
    McuWriteReg(0x25,0);        //write len
    McuWriteReg(0x26,len);      //read  len
    for(j=0;j<10;j++)
    {
        McuWriteReg(0x27,1);
        for(i=0;i<(len+1);i++)
        {
            if(McuReadReg(0x27)&0x02) break;
            Delay100us(5);
        }
        if(i>len) DBGPRINT_MI2C_COMMON("i2c read data timeout(no done) len=%x\r\n",len);
        McuWriteReg(0x27,0);                //b2:write 0 to clear
        Delay100us(5);
        if(!(McuReadReg(0x10)&0x08)) break; //0:Ack, 1:NAck (if Nack then retry)
    }
    if (j>=10)  //nack & timeout
    {
        DBGPRINT_MI2C_COMMON("i2c read addr timeout(???) addr=%x\r\n",addr);        
        len=0;
    } 
    for(i=0;i<len;i++)
        buffer[i] = McuReadMemS(i2c_inbuff_addr+i);
    McuWriteReg(0x00,old_bank);
    return len;    
}

BYTE Mi2cWriteData(BYTE addr, BYTE len, BYTE *buffer)
{
    BYTE old_bank,i,j;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MI2C);
    McuWriteReg(0x25,len+1);        //write len
    McuWriteReg(0x26,0);            //read  len
    McuWriteMemS(i2c_outbuff_addr,addr); 
    DBGPRINT_MI2C_INFO("buff=%x",HIBYTE(i2c_outbuff_addr));   
    DBGPRINT_MI2C_INFO("%x,",LOBYTE(i2c_outbuff_addr)); 
    DBGPRINT_MI2C_INFO("addr=%x,",addr);
    DBGPRINT_MI2C_INFO("len=%x\r\n",len);  
    for(i=0;i<len;i++)
    {
        McuWriteMemS(i2c_outbuff_addr+i+1,buffer[i]);    
    }
    for(j=0;j<10;j++)
    {
        McuWriteReg(0x27,1);
        for(i=0;i<(len+1);i++)
        {
            if(McuReadReg(0x27)&0x02) break;    // finished
            Delay100us(5);
        }
        McuWriteReg(0x27,0);                    // b2:write 0 to clear
        if(i>len) DBGPRINT_MI2C_COMMON("i2c write data timeout(no done) addr=%x\r\n",addr);
        Delay100us(5);
        if(!(McuReadReg(0x10)&0x08)) break;     // 0:Ack, 1:NAck
    } 
    if (j>=10)  //nack & timeout
    {
        DBGPRINT_MI2C_COMMON("i2c write data timeout(no ack) addr=%x\r\n",addr);        
        return 0;
    } 
    return i;    
}

/*
void MI2CStart(void)
{
    McuWriteReg(0x10, 0xC0);        // Set bit6 to trigger Start command
//    McuWriteReg(0x10, 0x80);
}

void MI2CStop(void)
{
    McuWriteReg(0x10, 0xA0);        // Set bit5 to trigger Stop command
//    McuWriteReg(0x10, 0x80);
}

void MI2CClearFlag(void)
{
    McuWriteReg(0x14, 0x01);        // clear int_status Reg10[0]
}

// Check Master IIC engine Write or Read byte finish
// Reg10[0]   0:busy,  1:ready
bool MI2CCheckReady(void)
{
    BYTE Data;

    Data = McuReadReg(0x10);

    return Data&0x01;
}

// Check Ack signal from slave device
// Reg10[3]   0:ack,  1:nack
bool MI2CGetWriteAck(void)
{
    BYTE Data;

    Data = McuReadReg(0x10);

    return ((Data&0x08) == 0x00);
}

// Set Ack/Nack command to slave device when read byte
// Reg10[3]   0:ack,  1:nack
void MI2CSetReadAck(bool last)
{
    if(last)
        McuWriteReg(0x10, 0x90);    //NAck => last byte
    else
        McuWriteReg(0x10, 0x80);    //Ack  => ok => next byte
}

// Master I2C engine start to get data after trigger bit is set.
void MI2CGetByteTrigger(void)
{
    McuWriteReg(0x14, 0x03);
}

BYTE MI2CGetByte(bool Ack)
{
    WORD i;
    BYTE ret;

    MI2CSetReadAck(Ack);
    MI2CGetByteTrigger();
    for(i=0; i<1000; i++)
    {
        if(MI2CCheckReady())
            break;
    }
    ret = McuReadReg(0x13);

    return ret;
}

bool MI2CSetByte(BYTE Data)
{
    WORD i;

    MI2CClearFlag();
    McuWriteReg(0x12, Data);
    for(i=0; i<1000; i++)
    {
        if(MI2CCheckReady())
            break;
    }
    return MI2CGetWriteAck();
}

bool MI2CWriteByte(int Addr, BYTE Data)
{
    BYTE old_bank;
    bool success;

    old_bank = McuReadReg(0x00);
    success = 1;
    //MI2CInit();

    MI2CStart();
    MI2CSetByte(mi2c_slaveaddr);
    MI2CSetByte(Addr);
    MI2CSetByte(Data);
    MI2CStop();

    McuWriteReg(0x00, old_bank);
    return success;
}

BYTE MI2CReadByte(int Addr)
{
    BYTE old_bank, Data;

    old_bank = McuReadReg(0x00);
    //MI2CInit();

    MI2CStart();
    MI2CSetByte(mi2c_slaveaddr);
    MI2CSetByte(Addr);

    MI2CStart();
    MI2CSetByte(mi2c_slaveaddr|0x01);
    Data = MI2CGetByte(true);
    MI2CStop();

    McuWriteReg(0x00, old_bank);

    return Data;
}


bool MI2CReadData0(BYTE slave_addr, int Addr, BYTE len, BYTE *buffer)
{
    BYTE i, loop, old_bank;
    bool ack, success;

    success = 1;
    old_bank = McuReadReg(0x00);
    McuWriteReg(0x00,BANK_MI2C);
    //MI2CInit();

    for(loop=0; loop<10; loop++)
    {
        MI2CStart();
        if(MI2CSetByte(slave_addr))
            break;
        Delay100us(5);
    }

    if(loop == 10)
    {
        success = 0;
        MI2CStop();
        McuWriteReg(0x00, old_bank);
        return success;
    }
    MI2CSetByte(Addr);
    MI2CStop();
    Delay100us(1);

    for(loop=0; loop<10; loop++)
    {
        MI2CStart();
        if(MI2CSetByte(slave_addr|0x01))
            break;
        Delay100us(5);
    }

    if(loop == 10)
    {
        success = 0;
        MI2CStop();
        McuWriteReg(0x00, old_bank);
        return success;
    }

    for(i=0; i<len; i++)
    {
        if(i==(len-1))
            ack = 1;
        else
            ack = 0;
        *buffer++ = MI2CGetByte(ack);
    }

    MI2CStop();

    McuWriteReg(0x00, old_bank);

    return true;
}

bool MI2CWriteData(BYTE slave_addr, int Addr, BYTE len, BYTE *buffer)
{
    BYTE i, loop, old_bank;
    bool success;

    old_bank = McuReadReg(0x00);
    success = 1;
    //MI2CInit();
    for(loop=0; loop<10; loop++)
    {
        MI2CStart();
        if(MI2CSetByte(slave_addr))
            break;
        Delay100us(5);
    }

    if(loop == 10)
        success = 0;
    else
    {
        MI2CSetByte(Addr);

        for(i=0; i<len; i++)
        {
            MI2CSetByte(*(buffer++));
        }
    }
    MI2CStop();

    McuWriteReg(0x00, old_bank);
    return success;
}
*/