#include "Global.h"
#include "Mcu.h"
#include "mi2c.h"
#include "Codec.h"

void CodecInit(BYTE slave, WORD in_addr, WORD out_addr)
{
    
#if (DAC_TYPE==DAC_CT7302) 
    Mi2cInit(slave,in_addr,out_addr);

    Mi2cWriteByte(0x10,0xC0);
    Mi2cWriteByte(0x11,0x00);
    Mi2cWriteByte(0x12,0x08);
    Mi2cWriteByte(0x13,0x00);
    Mi2cWriteByte(0x14,0x40);

    Mi2cWriteByte(0x30,0x23);
    Mi2cWriteByte(0x31,0x19);
    Mi2cWriteByte(0x32,0x1E);

    Mi2cWriteByte(0x39,0xF3);
    Mi2cWriteByte(0x3B,0x77);
    Mi2cWriteByte(0x40,0x02);
    Mi2cWriteByte(0x45,0x00);
    Mi2cWriteByte(0x4E,0x77);
	Mi2cWriteByte(0x47,0xA4);
	Mi2cWriteByte(0x49,0x00);       // disable dpop timer
	Mi2cWriteByte(0x4A,0x42);       // freerun
    Mi2cWriteByte(0x4D,0x37);
    Mi2cWriteByte(0x59,0x2D);
    Mi2cWriteByte(0x5F,0x4B);
    Mi2cWriteByte(0x61,0x08);
    Mi2cWriteByte(0x62,0x01);

    Mi2cWriteByte(0x04,0x30);       // SRC mode 3, Input source SPIDF
    Mi2cWriteByte(0x05,0x09);       // Output Freq 192K
    Mi2cWriteByte(0x06,0x48);       // spdif out 24bits + unmute

    // test 220727
    //Mi2cSetSlave(0x26);
    //Mi2cWriteByte(0x04,0x36);       // SRC mode 3, Input source I2S1
    //Mi2cWriteByte(0x05,0x09);       // Output Freq 192K
    //Mi2cWriteByte(0x06,0x48);       // spdif out 24bits + unmute
    //Mi2cSetSlave(slave);
#endif 
    CodecSetMute(0);
}

void CodecSetMute(BYTE mute)
{
#if (DAC_TYPE==DAC_CT7302) 
    Mi2cWriteByteMask(0x06,mute?0x80:0x00,0x80); 
#elif (DAC_TYPE==DAC_ES9XX8)   
    Mi2cWriteByteMask(0x07,mute?0x03:0x00,0x03); 
#endif         
}

/*
#define Codec_CS42L42       0

#if Codec_CS42L42
#define CS42L42_Addr        0x90

void InitCS42L42Setting(void)
{
    mi2c_slaveaddr = CS42L42_Addr;
    MI2CWriteByte(0x00, 0x10);                  // set page 0x10
    MI2CWriteByte(0x09, 0x02);                  // MCLK Control: /256 mode DEF:0x02
    MI2CWriteByte(0x0A, 0xA4);                  // DSR_RATE to ? DEF:0xA4

    MI2CWriteByte(0x00, 0x12);                  // set page 0x12
    MI2CWriteByte(0x0C, 0x00);                  // SCLK_PREDIV = 00 DEF:0x00

    MI2CWriteByte(0x00, 0x15);                  // set page 0x15
    MI2CWriteByte(0x05, 0x40);                  // PLL_DIV_INT = 0x40  DEF:0x40
    MI2CWriteByte(0x02, 0x00);                  // PLL_DIV_FRAC = 0x000000 DEF:0x00
    MI2CWriteByte(0x03, 0x00);                  // PLL_DIV_FRAC = 0x000000 DEF:0x00
    MI2CWriteByte(0x04, 0x00);                  // PLL_DIV_FRAC = 0x000000 DEF:0x00
    MI2CWriteByte(0x1B, 0x03);                  // PLL_MODE = 11 (bypassed) DEF:0x03
    MI2CWriteByte(0x08, 0x10);                  // PLL_DIVOUT = 0x10 DEF:0x10
    MI2CWriteByte(0x0A, 0x80);                  // PLL_CAL_RATIO = 128 DEF:0x80
    MI2CWriteByte(0x01, 0x01);                  // power up PLL 

    MI2CWriteByte(0x00, 0x12);                  // set page 0x12
    MI2CWriteByte(0x01, 0x01);                  // MCLKint = internal PLL DEF:0x00

    MI2CWriteByte(0x00, 0x11);                  // set page 0x11
    MI2CWriteByte(0x01, 0x00);                  // power up ASP, Mixer, HP, and DAC DEF:0xFF

    MI2CWriteByte(0x00, 0x1B);                  // set page 0x1B
    MI2CWriteByte(0x75, 0x9F);                  // Latch_To_VP = 1 DEF:0xFF

    MI2CWriteByte(0x00, 0x11);                  // set page 0x11
    MI2CWriteByte(0x07, 0x01);                  // SCLK present

    MI2CWriteByte(0x00, 0x12);                  // set page 0x12
    MI2CWriteByte(0x03, 0x1F);                  // FSYNC High time LB. LRCK +Duty = 32 SCLKs
    MI2CWriteByte(0x04, 0x00);                  // FSYNC High time UB DEF:0x00
    MI2CWriteByte(0x05, 0x3F);                  // FSYNC Period LB. LRCK = 64 SCLKs
    MI2CWriteByte(0x06, 0x00);                  // FSYNC Period UB DEF:0x00
    MI2CWriteByte(0x07, 0x2C);                  // Enable ASP SCLK, LRCK input, SCPOL_IN (ADC and DAC) inverted
    MI2CWriteByte(0x08, 0x0A);                  // frame starts high to low, I2S mode, 1 SCLK FSD

    MI2CWriteByte(0x00, 0x1F);                  // set page 0x1F
    MI2CWriteByte(0x06, 0x02);                  // Dac Control 2: Default

    MI2CWriteByte(0x00, 0x20);                  // set page 0x20
    MI2CWriteByte(0x01, 0x01);                  // unmute analog A and B, not -6dB mode

    MI2CWriteByte(0x00, 0x23);                  // set page 0x23
    MI2CWriteByte(0x01, 0xFF);                  // CHA_Vol = MUTE
    MI2CWriteByte(0x03, 0xFF);                  // CHB_Vol = MUTE

    MI2CWriteByte(0x00, 0x2A);                  // set page 0x2A
    MI2CWriteByte(0x01, 0x0C);                  // Enable Ch1/2 DAI
    MI2CWriteByte(0x02, 0x02);                  // Ch1 low 24 bit
    MI2CWriteByte(0x03, 0x00);                  // Ch1 Bit Start UB
    MI2CWriteByte(0x04, 0x00);                  // Ch1 Bit Start LB = 00
    MI2CWriteByte(0x05, 0x42);                  // Ch2 high 24 bit
    MI2CWriteByte(0x06, 0x00);                  // Ch2 Bit Start UB
    MI2CWriteByte(0x07, 0x00);                  // Ch2 Bit Start LB = 00
    MI2CWriteByte(0x00, 0x26);                  // set page 0x26
    MI2CWriteByte(0x01, 0x00);                  // SRC in at don't know
    MI2CWriteByte(0x09, 0x00);                  // SRC out at don't know
    MI2CWriteByte(0x00, 0x1B);                  // set page 0x1B
    MI2CWriteByte(0x71, 0xC1);                  // Toggle WAKEB_CLEAR
    MI2CWriteByte(0x71, 0xC0);                  // Set WAKE back to normal
    MI2CWriteByte(0x75, 0x84);                  // Set LATCH_TO_VP

    MI2CWriteByte(0x00, 0x11);                  // set page 0x11
    MI2CWriteByte(0x29, 0x01);                  // headset clamp disable
    MI2CWriteByte(0x02, 0xA7);                  // Power Down Control 2

    MI2CWriteByte(0x00, 0x1B);                  // set page 0x1B
    MI2CWriteByte(0x74, 0xA7);                  // Misc

    MI2CWriteByte(0x00, 0x1B);                  // set page 0x1B
    MI2CWriteByte(0x75, 0x9F);                  // Latch_To_VP = 1
    MI2CWriteByte(0x00, 0x1B);                  // set page 0x1B
    MI2CWriteByte(0x73, 0xE2);                  // Write TIP_SENSE_CTRL for plug type
    MI2CWriteByte(0x71, 0xA0);                  // Unmask WAKEB
}

void CS42L42_Codec_PowerOn(void)
{
    mi2c_slaveaddr = CS42L42_Addr;
    MI2CWriteByte(0x00, 0x11);                  // set page 0x11
    MI2CWriteByte(0x02, 0xA4);                  //Release Discharge cap, FILT+
    MI2CWriteByte(0x02, 0xA7);                  //Power Down Control 2
    MI2CWriteByte(0x01, 0x00);                  //power up ASP, Mixer, HP, and DAC
    MI2CWriteByte(0x00, 0x12);                  // set page 0x12
    MI2CWriteByte(0x07, 0x2C);                  //Enable ASP SCLK, LRCK input, SCPOL_IN (ADC and DAC) inverted
    MI2CWriteByte(0x00, 0x2A);                  // set page 0x2A
    MI2CWriteByte(0x01, 0x0C);                  //Enable Ch1/2 DAI
    MI2CWriteByte(0x00, 0x20);                  // set page 0x20
    MI2CWriteByte(0x01, 0x01);                  //unmute analog A and B, not -6dB mode
//    DAC_EN = 1;	                                        	// reset flag
}

void CS42L42_ADC_PowerOn(void)
{
    mi2c_slaveaddr = CS42L42_Addr;

    // enable mic
    MI2CWriteByte(0x00, 0x1B);                  // set page 0x1B
    MI2CWriteByte(0x74, 0x07);                  // HSBIAS set to 2.7V mode

    CS42L42_Codec_PowerOn();
    // enable ASP
    MI2CWriteByte(0x00, 0x29);                  // set page 0x29
    MI2CWriteByte(0x04, 0x00);                  // Ch1 Bit Start UB
    MI2CWriteByte(0x05, 0x00);                  // Ch1 Bit Start LB = 00
    MI2CWriteByte(0x0A, 0x00);                  // Ch2 Bit Start UB
    MI2CWriteByte(0x0B, 0x00);                  // Ch2 Bit Start LB = 00
    MI2CWriteByte(0x02, 0x01);                  // enable ASP Transmit CH1
    MI2CWriteByte(0x03, 0x4A);                  // RES=24 bits, CH2 and CH1
    MI2CWriteByte(0x01, 0x01);                  // ASP_TX_EN
    MI2CWriteByte(0x02, 0x00);                  // disable ASP Transmit CH2 and CH1
    MI2CWriteByte(0x02, 0x03);                  // enable ASP Transmit CH2 and CH1
    // set volume
    MI2CWriteByte(0x00, 0x1C);                  // set page 0x1C
    MI2CWriteByte(0x03, 0xC3);                  // set HSBIAS_RAMP to slowest
    MI2CWriteByte(0x00, 0x1D);                  // set page 0x1D
    MI2CWriteByte(0x02, 0x06);                  // ADC soft-ramp enable
    MI2CWriteByte(0x03, 0x06);                  // ADC_VOL = 6dB,
    MI2CWriteByte(0x01, 0x01);                  // ADC_DIG_BOOST; +20dB, 
    MI2CWriteByte(0x00, 0x23);                  // set page 0x23
    MI2CWriteByte(0x02, 0x19);                  // Mixer ADC_Vol = -25dB, 
}

#if 0
void CS42L42_ADC_PowerOff(void)
{
    mi2c_slaveaddr = CS42L42_Addr;
    // disable ADC (if currently enabled)
    MI2CWriteByte(0x00, 0x1D);                  // set page 0x1D
    MI2CWriteByte(0x03, 0x9F);                  // ADC_VOL = Mute, 0X9F is Mute
    MI2CWriteByte(0x01, 0x00);                  // ADC_DIG_BOOST; 0x00 is no boost
    MI2CWriteByte(0x00, 0x23);                  // set page 0x23
    MI2CWriteByte(0x02, 0x3F);                  // Mixer ADC_Vol = Mute
    MI2CWriteByte(0x00, 0x1B);                  // set page 0x1B
//    if(HP_Only == 0)
//        MI2CWriteByte(0x74, 0x07);                  //Turn on HSBIAS until requested by ADC
//    else if(HP_Only == 1)
    MI2CWriteByte(0x74, 0x01);                  //Turn off HSBIAS until requested by ADC

//    ADC_EN = 0;                  	//reset flag
}

void CS42L42_Codec_PowerOff(void)
{
    mi2c_slaveaddr = CS42L42_Addr;
    MI2CWriteByte(0x00, 0x23);                  // set page 0x23
    MI2CWriteByte(0x01, 0x3F);                  // CHA_Vol = MUTE
    MI2CWriteByte(0x03, 0x3F);                  // CHB_Vol = MUTE
    MI2CWriteByte(0x02, 0xFF);                  // Mixer ADC Input = MUTE
    MI2CWriteByte(0x00, 0x20);                  // set page 0x20
    MI2CWriteByte(0x01, 0x0F);                  // CHA, CHB = MUTE, FS VOL
    MI2CWriteByte(0x00, 0x2A);                  // set page 0x2A
    MI2CWriteByte(0x01, 0x00);                  // Disable ASP_TX
    MI2CWriteByte(0x00, 0x12);                  // set page 0x12
    MI2CWriteByte(0x07, 0x00);                  // Disable SCLK
    MI2CWriteByte(0x00, 0x11);                  // set page 0x11
    MI2CWriteByte(0x01, 0xFE);                  // Power down HP amp
    MI2CWriteByte(0x02, 0x8C);                  // Power down ASP and SRC
    MI2CWriteByte(0x00, 0x13);                  // set page 0x13
//    wait_15ms ();                                    // delay codec power down

    // read data
    MI2CReadByte(0x08);

    MI2CWriteByte(0x00, 0x11);                  // set page 0x11
    MI2CWriteByte(0x02, 0x9C);                  //Discharge cap, FILT+
//    DAC_EN = 0;                                    // set flag
 //   ADC_EN = 1;	                                 // reset flag
}
#endif
#endif

void InitCT7302(BYTE slave_addr)
{
    BYTE temp_value, time_count;

    mi2c_slaveaddr = slave_addr;
	// Check for chip alive	
    MI2CWriteByteMask(0x2B, 0x08, 0x08);
    Delay100us(5);
    MI2CWriteByteMask(0x2B, 0x00, 0x08);

	time_count=0;
	temp_value = MI2CReadByte(0x00);
	while(temp_value==0 && time_count++<100);
	{
		Delay100us(5);
		temp_value = MI2CReadByte(0x00);
	}
	if(time_count<100)
	{
		MI2CWriteByte(0x00,temp_value);
	}

    MI2CWriteByte(0x06,0x48);
    MI2CWriteByte(0x10,0xC0);
    MI2CWriteByte(0x11,0x00);
    MI2CWriteByte(0x12,0x08);
    MI2CWriteByte(0x13,0x00);
    MI2CWriteByte(0x14,0x40);

    MI2CWriteByte(0x30,0x23);
    MI2CWriteByte(0x31,0x19);
    MI2CWriteByte(0x32,0x1E);

    MI2CWriteByte(0x39,0xF3);
    MI2CWriteByte(0x3B,0xFF);
    MI2CWriteByte(0x40,0x02);
    MI2CWriteByte(0x45,0x00);
    MI2CWriteByte(0x4E,0x72);
	MI2CWriteByte(0x47,0xA4);
    MI2CWriteByte(0x4D,0x37);                
    MI2CWriteByte(0x61,0x08);
    MI2CWriteByte(0x62,0x01);

    MI2CWriteByte(0x04,0x36);       // SRC mode 3, Input source I2S1
    MI2CWriteByte(0x05,0x09);       // Output Freq 192K
    if(slave_addr == 0x26)
    {
        // Record port
        MI2CWriteByte(0x1C,0x01);   // MCLK freq
        MI2CWriteByteMask(0x06, 0x00, 0x0C);        // 0x00:24 bit output, 0x0C:32 bit output
        MI2CWriteByteMask(0x59, 0x00, 0x10);        // 0x00:I2S output port, 0x10:DSD output port
        MI2CWriteByteMask(0x59, 0x00, 0x02);        // 0x00:PCM output, 0x02:DoP output
        MI2CWriteByte(0x04,0x36);                   // 0x36: mode 3, 0x06: mode 0 for slave mode
        MI2CWriteByteMask(0x4D,0x00, 0x40);         // 0x00: Master mode output, 0x40: slave mode
    }
    else
    {
        // Play port
        MI2CWriteByte(0x1C,0x03);
    }
}

void InitSRCPrimarySetting(void)
{
    mi2c_slaveaddr = 0x22;

    InitCT7302(mi2c_slaveaddr);
}

void InitSRCSecondarySetting(void)
{
    mi2c_slaveaddr = 0x24;

    InitCT7302(mi2c_slaveaddr);
}

void InitSRCRecordSetting(void)
{
    mi2c_slaveaddr = 0x26;

    InitCT7302(mi2c_slaveaddr);
}

void InitialPeripheralDevice(void)
{
#if Codec_CS42L42
    InitCS42L42Setting();
    CS42L42_ADC_PowerOn();
#endif

    InitSRCPrimarySetting();
    InitSRCSecondarySetting();
    InitSRCRecordSetting();
}

void SetADCRecordFreq(BYTE freq)
{
    mi2c_slaveaddr = 0x26;
    MI2CWriteByteMask(0x05, freq&0x0F, 0x0F);
}
*/