#ifndef _CTUSB_GLOBAL_H_
#define _CTUSB_GLOBAL_H_

//#include "Rammap.h"

#define BIT0  0x0001
#define BIT1  0x0002
#define BIT2  0x0004
#define BIT3  0x0008
#define BIT4  0x0010
#define BIT5  0x0020
#define BIT6  0x0040
#define BIT7  0x0080
#define BIT8  0x0100
#define BIT9  0x0200
#define BIT10 0x0400
#define BIT11 0x0800
#define BIT12 0x1000
#define BIT13 0x2000
#define BIT14 0x4000
#define BIT15 0x8000

typedef unsigned long uint32_t;
typedef unsigned int  uint16_t;
typedef unsigned char uint8_t;

typedef unsigned long DWORD;
typedef unsigned int  WORD;
typedef unsigned char BYTE;
typedef bit BOOLEAN;
typedef bit bool;

enum
{
    false = 0,
    true
};

enum
{
    FALSE=0,
    TRUE=1,
};

// used with UU16
# define LSB 1
# define MSB 0

// used with UU32 (b0 is least-significant byte)
# define b0 3
# define b1 2
# define b2 1
# define b3 0



typedef unsigned char U8;
typedef unsigned int U16;
typedef unsigned long U32;

typedef signed char S8;
typedef signed int S16;
typedef signed long S32;

//typedef (unsigned char volatile xdata *)    XDATA_ADDR;

typedef union UU16
{
   U16 U16;
   S16 S16;
   U8 U8[2];
   S8 S8[2];
} UU16;

typedef union UU32
{
   U32 U32;
   S32 S32;
   UU16 UU16[2];
   U16 U16[2];
   S16 S16[2];
   U8 U8[4];
   S8 S8[4];
} UU32;

typedef union USB_AUDIO_CONTROL_PACKET
{
    BYTE  bytes[8];
    struct
    {
		//! Request value
        BYTE bmRequest;
		//! Request type value
		BYTE bAttribute; //curr:0x01, range:0x02
        BYTE bCN;
        BYTE bCS;
        BYTE bEntity;
        BYTE bInterface;
        BYTE bLen[2];
    }info;            
} USB_AUDIO_CONTROL_PACKET;
// Setup Packet Type Definition
typedef struct SETUP_BUFFER
{
   U8    bmRequestType;       // Request recipient, type, and dir.
   U8    bRequest;            // Specific standard request number
   UU16  wValue;              // varies according to request
   UU16  wIndex;              // varies according to request
   UU16  wLength;             // Number of bytes to transfer

} SETUP_BUFFER, *PSETUP_BUFFER;
 
typedef struct IRQ_USB_INFO
{
    U8              status;
    SETUP_BUFFER    packet;
} IRQ_USB_INFO, *PIRQ_USB_INFO;

//#define CLOCK_44K_SETTING       0x33053978          // 288,000,000*1024*128 / 44,100
//#define CLOCK_48K_SETTING       0x2EE00000          // 288,000,000*1024*128 / 48,000
//#define CLOCK_44K_SETTING       0x35257132          // 300,000,000*1024*128 / 44,100
//#define CLOCK_48K_SETTING       0x30D40000          // 300,000,000*1024*128 / 48,000

#define	CLOCK_32K_SETTING       0x34980000          // 240,000,000*1024*128 / 32,100
#define	CLOCK_44K_SETTING       0x2A845A8F          // 240,000,000*1024*128 / 44,100
#define CLOCK_48K_SETTING       0x27100000          // 240,000,000*1024*128 / 48,000


#define SAMPLE_RATE_32K         0x00007D00
#define SAMPLE_RATE_16K         (((U32)SAMPLE_RATE_32K)>>1)
#define SAMPLE_RATE_8K          (((U32)SAMPLE_RATE_32K)>>2)


#define SAMPLE_RATE_44K         0x0000AC44
#define SAMPLE_RATE_48K         0x0000BB80
#define SAMPLE_RATE_88K         (((U32)SAMPLE_RATE_44K)<<1)
#define SAMPLE_RATE_96K         (((U32)SAMPLE_RATE_48K)<<1)
#define SAMPLE_RATE_176K        (((U32)SAMPLE_RATE_44K)<<2)
#define SAMPLE_RATE_192K        (((U32)SAMPLE_RATE_48K)<<2)
#define SAMPLE_RATE_352K        (((U32)SAMPLE_RATE_44K)<<3)
#define SAMPLE_RATE_384K        (((U32)SAMPLE_RATE_48K)<<3)
#define SAMPLE_RATE_705K        (((U32)SAMPLE_RATE_44K)<<4)
#define SAMPLE_RATE_768K        (((U32)SAMPLE_RATE_48K)<<4)

#define BANK_MISC                   0x00    
#define BANK_CLOCK                  0x01    
#define BANK_MISC1                  0x02    
#define BANK_PLAY0                  0x03    
#define BANK_REC0                   0x05    
#define BANK_DAC0                   0x06    
#define BANK_ADC0                   0x07    

#define BANK_EP0                    0x10    
#define BANK_EP1                    0x11    
#define BANK_SYNTH1                 0x12    
#define BANK_HID                    0x13    
#define BANK_BULK                   0x14    
#define BANK_UTMI                   0x15
#define BANK_JACK                   0x16

#define BANK_SPI                    0x20
#define BANK_MI2C                   0x21
#define BANK_BIST                   0x22
#define BANK_UART                   0x23


#define I2C_IN_DATA_LEN                 32
#define SPI_IN_DATA_LEN                 24
#define USB_HID_IN_DATA_LEN             2
#define USB_JACK_IN_DATA_LEN            6
#define USB_CUSTOM_DATA_IN_LEN          (I2C_IN_DATA_LEN+SPI_IN_DATA_LEN+USB_HID_IN_DATA_LEN+USB_JACK_IN_DATA_LEN)

#define I2C_OUT_DATA_LEN                32
#define SPI_OUT_DATA_LEN                24
#define USB_HID_OUT_DATA_LEN            8

#define USB_CUSTOM_DATA_ADDR            0x0660
#define I2C_IN_DATA_ADDR                (USB_CUSTOM_DATA_ADDR)
#define SPI_IN_DATA_ADDR                (I2C_IN_DATA_ADDR+I2C_IN_DATA_LEN)
#define USB_HID_IN_DATA_ADDR            (SPI_IN_DATA_ADDR+SPI_IN_DATA_LEN) 
#define USB_JACK_IN_DATA_ADDR           (USB_HID_IN_DATA_ADDR+USB_HID_IN_DATA_LEN) 

#define I2C_OUT_DATA_ADDR               (USB_CUSTOM_DATA_ADDR+USB_CUSTOM_DATA_IN_LEN)
#define SPI_OUT_DATA_ADDR               (I2C_OUT_DATA_ADDR+I2C_OUT_DATA_LEN)
#define USB_HID_OUT_DATA_ADDR           (SPI_OUT_DATA_ADDR+SPI_OUT_DATA_LEN)


#define USB_HID_FEATUREIN_DATA_LEN      144
#define USB_HID_FEATUREOUT_DATA_LEN     144

#define USB_VENDOR_DATA_ADDR            0x06E0
#define USB_HID_FEATUREIN_DATA_ADDR     (USB_VENDOR_DATA_ADDR)
#define USB_HID_FEATUREOUT_DATA_ADDR    (USB_VENDOR_DATA_ADDR+USB_HID_FEATUREIN_DATA_LEN)


#define IAP2_CP_DATA_LEN                1024 
#define USB_BULK_IN_DATA_LEN            512 
#define USB_BULK_OUT_DATA_LEN           512

#define IAP2_CP_DATA_ADDR               0x0800
#define USB_BULK_IN_DATA_ADDR           (IAP2_CP_DATA_ADDR+IAP2_CP_DATA_LEN)    //0x0C00
#define USB_BULK_OUT_DATA_ADDR          (USB_BULK_IN_DATA_ADDR+USB_BULK_IN_DATA_LEN)
#define USB_BULK_OUT_DATA_ADDR_END      (USB_BULK_OUT_DATA_ADDR+USB_BULK_OUT_DATA_LEN-1)


#define UART_BUFFERSIZE                 16
#define USB_DATA_MAX_ADDR               0x1000

#ifdef	_CTUSB_MAIN_C_
    BYTE xdata iap2_cp_data[IAP2_CP_DATA_LEN]                           _at_    (IAP2_CP_DATA_ADDR);    
    BYTE xdata usb_bulkin_data[USB_BULK_IN_DATA_LEN]                    _at_    (USB_BULK_IN_DATA_ADDR);    
    BYTE xdata usb_bulkout_data[USB_BULK_OUT_DATA_LEN]                  _at_    (USB_BULK_OUT_DATA_ADDR);     
    BYTE xdata usb_hid_feature_datain[USB_HID_FEATUREIN_DATA_LEN]       _at_    (USB_HID_FEATUREIN_DATA_ADDR);     
    BYTE xdata usb_hid_feature_dataOUT[USB_HID_FEATUREOUT_DATA_LEN]     _at_    (USB_HID_FEATUREOUT_DATA_ADDR);     
    BYTE xdata usb_hid_in_data[USB_HID_IN_DATA_LEN]                     _at_    (USB_HID_IN_DATA_ADDR);    
    BYTE xdata usb_hid_out_data[USB_HID_OUT_DATA_LEN]                   _at_    (USB_HID_OUT_DATA_ADDR);     
    BYTE xdata usb_jack_in_data[USB_JACK_IN_DATA_LEN]                   _at_    (USB_JACK_IN_DATA_ADDR);     

    BYTE xdata i2c_out_data[I2C_OUT_DATA_LEN]                           _at_    (I2C_OUT_DATA_ADDR);     
    BYTE xdata i2c_in_data[I2C_IN_DATA_LEN]                             _at_    (I2C_IN_DATA_ADDR);     
    BYTE xdata spi_write_data[SPI_OUT_DATA_LEN]                         _at_    (SPI_OUT_DATA_ADDR);
    BYTE xdata spi_read_data[SPI_IN_DATA_LEN]                           _at_    (SPI_IN_DATA_ADDR);
    BYTE xdata hw_no_used_data[0x1000]                                  _at_    0x1000;
    BYTE xdata hw_regs[256] 		                                    _at_    0x2000; 	
    #define _CTUSB_MAIN_EXTERN_	
#else
    extern BYTE xdata iap2_cp_data[IAP2_CP_DATA_LEN];    
    extern BYTE xdata usb_bulkin_data[USB_BULK_IN_DATA_LEN];    
    extern BYTE xdata usb_bulkout_data[USB_BULK_OUT_DATA_LEN];     
    extern BYTE xdata usb_hid_feature_datain[USB_HID_FEATUREIN_DATA_LEN];     
    extern BYTE xdata usb_hid_feature_dataout[USB_HID_FEATUREOUT_DATA_LEN];     
    extern BYTE xdata usb_hid_in_data[USB_HID_IN_DATA_LEN];    
    extern BYTE xdata usb_hid_out_data[USB_HID_OUT_DATA_LEN];     
    extern BYTE xdata usb_jack_in_data[USB_JACK_IN_DATA_LEN];     

//    extern BYTE xdata usb_spi_temp_data[USB_SPI_TEMP_LEN];     
    extern BYTE xdata i2c_out_data[I2C_OUT_DATA_LEN];    
    extern BYTE xdata i2c_in_data[I2C_IN_DATA_LEN];    
    extern BYTE xdata spi_write_data[SPI_OUT_DATA_LEN];                        
    extern BYTE xdata spi_read_data[SPI_IN_DATA_LEN];                        
    extern BYTE xdata hw_regs[256];	
    #define _CTUSB_MAIN_EXTERN_  extern
#endif 

_CTUSB_MAIN_EXTERN_ WORD timer_count;
_CTUSB_MAIN_EXTERN_ BYTE irq_flags,hid_flags,test_data[8];

//_CTUSB_MAIN_EXTERN_ BYTE xdata *spi_write_data,*spi_read_data;
_CTUSB_MAIN_EXTERN_ BYTE mi2c_slaveaddr;
_CTUSB_MAIN_EXTERN_ BYTE UART_TimeOut_Flag, UART_TimeOut_Count;
_CTUSB_MAIN_EXTERN_ BYTE UART_Buffer[UART_BUFFERSIZE], UART_Buffer_Index;
_CTUSB_MAIN_EXTERN_ BYTE xdata *bulkout_data1;

//-----------------------------------------------------------------------------
// Macros
//-----------------------------------------------------------------------------

// Return the LSB given a U16
#define LOBYTE(w)       ((U8)(w))

// Return the MSB given a U16
#define HIBYTE(w)       ((U8)(((U16)(w) >> 8) & 0xFF))

// Return the MSB given a U32
#define H0BYTE(w)       ((U8)(((U32)(w) >> 0) & 0xFF))
#define H1BYTE(w)       ((U8)(((U32)(w) >> 8) & 0xFF))
#define H2BYTE(w)       ((U8)(((U32)(w) >> 16) & 0xFF))
#define H3BYTE(w)       ((U8)(((U32)(w) >> 24) & 0xFF))

#define LE_ARRAY4(w)      ((U8)(w)), ((U8)(((U32)(w) >> 8) & 0xFF)), ((U8)(((U32)(w) >> 16) & 0xFF), (U8)(((U32)(w) >> 24) & 0xFF))

#define LE_ARRAY3(w)      ((U8)(w)), ((U8)(((U32)(w) >> 8) & 0xFF)), ((U8)(((U32)(w) >> 16) & 0xFF))

// Define a little-endian multi-byte array initialization given a U16
#define LE_ARRAY(w)     LOBYTE(w), HIBYTE(w)

// Define a big-endian multi-byte array initialization given a U16
#define BE_ARRAY(w)     HIBYTE(w), LOBYTE(w)

#define BYTE_TO_WORD(_HBYTE_,_LBYTE_)	((((WORD)_HBYTE_)<<8)|((WORD)_LBYTE_))

#define BYTE_TO_DWORD(__X3__,__X2__,__X1__,__X0__)    ((((DWORD)__X3__)<<24)|(((DWORD)__X2__)<<16)|(((DWORD)__X1__)<<8)|((DWORD)__X0__))

#define GET_ADDR_DWORD(_A_)     ((((DWORD)*(_A_+3))<<24)|(((DWORD)*(_A_+2))<<16)|(((DWORD)*(_A_+1))<<8)|((DWORD)*(_A_)))
#define GET_ADDR_WORD(_A_)      ((((WORD)*(_A_+1))<<8)|((WORD)*(_A_)))
#define GET_ADDR_BYTE(_A_)      (*(_A_))

#define INDEX_10_TO_15(_A_)     ((_A_/2)*3+_A_%2+2)

#define FREQ_TO_INDEX(_F_)      (_F_%SAMPLE_RATE_44K)?((_F_/SAMPLE_RATE_44K-1)*2+1):((_F_/SAMPLE_RATE_44K-1)*2)

#define DEBUG_PRINT(_FLAG_,_STR_,_X_)       if (_FLAG_) printf(_STR_,_X_)
//#define DEBUG_PRINT(_FLAG_,_STR_,_X_)       printf((_FLAG_)?_STR_:"",_X_)


#endif  // _CTUSB_GLOBAL_H_
