#include "global.h"
#include "format1.h"

#ifndef _CTUSB_UAC1_DESCRIPTOR_H_
#define _CTUSB_UAC1_DESCRIPTOR_H_

#ifdef  _CTUSB_DESCRIPTOR_C_

typedef struct _DESCRIPTOR_INFO_
{
    WORD    len;
    BYTE    *buffer;
}DESCRIPTOR_INFO, *PDESCRIPTOR_INFO;	

#define     MANUFACTURER_NAME           "Comtrue"               //"mCore"
#define     SERIAL_NUMBER               "221026A1"

#define     VID                         0x2FC6                  //0x1403  //0x2FC6      

#if (UAC_TYPE==TYPE_UAC1_FS)
#define     PRODUCT_NAME                "CT7702 UAC1/FS"        //"mCore Audio UBox"
#define     PID                         0x7021                   //0x3388  //0x6002 Test for Bulk
#else
#define     PRODUCT_NAME                "CT7702 UAC1/HS"        //"mCore Audio UBox"
#define     PID                         0x7022                  //0x3388  //0x8001
#endif

#define     PLAYBACK_NAME               "Playback"
#define     RECORD_NAME                 "Record"
#define     HID_NAME                    "Hid"
#define     IAP_NAME                    "iAP Interface"

#define     MANUFACTURER_NAME_LEN       (sizeof(MANUFACTURER_NAME)-1)
#define     PRODUCT_NAME_LEN            (sizeof(PRODUCT_NAME)-1)
#define     SERIAL_NUMBER_LEN           (sizeof(SERIAL_NUMBER)-1)

#define     PLAYBACK_NAME_LEN           (sizeof(PLAYBACK_NAME)-1)
#define     RECORD_NAME_LEN             (sizeof(RECORD_NAME)-1)
#define     HID_NAME_LEN                (sizeof(HID_NAME)-1)
#define     IAP_NAME_LEN                (sizeof(IAP_NAME)-1)
     
#define     CT_UAC1_DEVICE_DESC_ADDR    USB_DESC_DATA_BASE

#define     UAC1_PLAY0_ENABLE                   PLAYBACK_ENABLE
#define     UAC1_REC0_ENABLE                    RECORD_ENABLE
#define     UAC1_HID_ENABLE                     HID_ENABLE
#define     UAC1_BULK_ENABLE                    BULK_ENABLE

#define     UAC1_PLAY0_IF_INDEX                 IF_PLAYBACK
#define     UAC1_REC0_IF_INDEX                  IF_RECORD
#define     UAC1_HID_IF_INDEX                   IF_HID
#define     UAC1_BULK_IF_INDEX                  IF_BULK

#define     UAC1_HID_KEYS                       HID_GPIO_KEY
#define     UAC1_HIDINOUT_TEST                  0

// UAC1 Device Descriptor Data
//code BYTE uac1_device_desc_buffer[]={
//    0x12,0x01,0x10,0x01,0xEF,0x02,0x01,0x40,0xC6,0x2F,0x01,0x80,0x01,0x01,0x01,0x02,   // Base = 0x0600 - 0x0603
//    0x03,0x01,                                                               // Base = 0x0604 - 0x0604
//    };
/* USB Standard Device Descriptor */
code BYTE uac1_device_desc_buffer[] = {
    0x12,                               /* bLength */
    0x01,                               /* bDescriptorType */
//    LOBYTE(UAC1_TYPE),HIBYTE(UAC1_TYPE),/* bcdUSB (1.10) */
    LOBYTE(USB_TYPE),HIBYTE(USB_TYPE),  /* bcdUSB (1.10) */
    0x00,                               /* bDeviceClass */
    0x00,                               /* bDeviceSubClass */
    0x00,                               /* bDeviceProtocol */
    0x40,                               /* bMaxPacketSize0 (64) */
    LOBYTE(VID),HIBYTE(VID),            /* idVendor */
    LOBYTE(PID),HIBYTE(PID),            /* idProduct */
    0x01,0x01,                          /* bcdDevice (1.00) */
    0x01,                               /* iManufacturer */
    0x02,                               /* iProduct */
    0x03,                               /* iSerialNumber */
    0x01                                /* bNumConfigurations */
};
#define     CT_UAC1_DEVICE_DESC_LEN   sizeof(uac1_device_desc_buffer)


// UAC1 HID Report Descriptor Data
//code BYTE uac1_hid_report_buffer[CT_UAC1_HID_REPORT_DESC_LEN]={
 //    };
code BYTE uac1_hid_report_buffer[] = 
{  
    0x06, 0x00, 0xFF,               // USAGE_PAGE (Vendor Defined Page 1)
    0x09, 0x01,                     // USAGE (Vendor Usage 1)
    0xA1, 0x01,                     // COLLECTION (Application)
    	0xA1, 0x02,                     //   Collection (Logical)                                                                 
            0x15, 0x00,                     //      LOGICAL_MINIMUM (0)
            0x26, 0xFF, 0x00,               //      LOGICAL_MAXIMUM (255)
            0x75, 0x08,                     //      REPORT_SIZE (8)
            0x85, 0x01,                     //      REPORT_ID (1)
            0x95, 0x3F,                     //      REPORT_COUNT (63+1=64) 
            0x09, 0x00,                     //      USAGE (Undefined)
            0xB2, 0x02, 0x01,               //      FEATURE (Data,Var,Abs,Buf)
            0x85, 0x02,                     //      REPORT_ID (2)
            0x95, 0x8F,                     //      REPORT_COUNT (143+1=144)
            0x09, 0x00,                     //      USAGE (Undefined)
            0xB2, 0x02, 0x01,               //      FEATURE (Data,Var,Abs,Buf)
        0xC0,                           //   End Collection                                                                       
#if (UAC1_HIDINOUT_TEST ==1)
    //181227++ for HID I/O Test
        0xA1, 0x02,                     //   Collection (Logical)                                                                 
        	0x85, 0x03,                     //     Report ID (3)                                                                      
        	0x75, 0x08,                     //     Report Size (8)                                                                    
        	0x95, 0x2F,                     //     Report Count (47+1=48)                                                                  
        	0x09, 0x01,                     //     Usage (Pointer)                                                                    
        	0x81, 0x02,                     //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)               
        0xC0,                           //   End Collection                                                                       
        0xA1, 0x02,                     //   Collection (Logical)                                                                 
        	0x85, 0x04,                     //     Report ID (4)                                                                      
        	0x75, 0x08,                     //     Report Size (8)                                                                    
        	0x95, 0x2F,                     //     Report Count (47+1=48)                                                                  
        	0x09, 0x01,                     //     Usage (Pointer)                                                                    
        	0x91, 0x02,                     //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile) 
        0xC0,                           //   End Collection 
    //181227..
#endif                                                                      
    0xC0,                           // END_COLLECTION
#if(UAC1_HID_KEYS==1)
	//Consumer Page
	0x05,0x0C,	// USAGE_PAGE (ConsumerPage 1)
	0x09,0x01,	// USAGE (Vendor Usage 1)
	0xA1,0x01,	// COLLECTION (Application)    
		0x85,0x03,  // REPORT_ID (3)
		0x05,0x0C,	// USAGE_PAGE (ConsumerPage 1)
		0x15,0x00,	// LOGICAL_MINIMUM (0)
		0x25,0x01,	// LOGICAL_MAXIMUM (1)
		0x09,0xE9,	// USAGE (Volume Increment)
		0x09,0xEA,	// USAGE (Volume Decrement)
		0x09,0xCD,	// USAGE (Play/Stop)
		0x09,0xE2,	// USAGE (Mute)
		0x09,0xB7,	// USAGE (STOP)
		0x09,0xB5,	// USAGE (Scan Next Track)
		0x09,0xB6,	// USAGE (Scan Previous Track)
		0x75,0x01,	// REPORT Size(1)
		0x95,0x07,	// REPORT_COUNT (7) 
		0x81,0x02,	// Input (Data, Variable, abs)

		0x75,0x01,	// REPORT Size(1)
		0x95,0x01,	// REPORT_COUNT (1) 
		0x81,0x03,	// INPUT(Cnst,Var,Abs)
	0xC0, 			// End Collection*/
#endif
};
#define     CT_UAC1_HID_REPORT_DESC_LEN   sizeof(uac1_hid_report_buffer)

// UAC1 Device Qualify Descriptor Data
code BYTE uac1_dev_qualify_buffer[]={
    0x0A,0x06,0x10,0x01,0xEF,0x02,0x01,0x40,0x01,0x00,                                 // Base = 0x06E4 - 0x06E6
    };
#define     CT_UAC1_DEV_QUALIFY_DESC_LEN   sizeof(uac1_dev_qualify_buffer)

code DESCRIPTOR_INFO string_table[] = 
{
    { MANUFACTURER_NAME_LEN,    MANUFACTURER_NAME }, 
    { PRODUCT_NAME_LEN,         PRODUCT_NAME }, 
    { SERIAL_NUMBER_LEN,        SERIAL_NUMBER }, 
    { PLAYBACK_NAME_LEN,        PLAYBACK_NAME }, 
    { RECORD_NAME_LEN,          RECORD_NAME }, 
    { HID_NAME_LEN,             HID_NAME }, 
#if(BULK_ENABLE)
    { IAP_NAME_LEN,             IAP_NAME },
#endif
};

#define UAC1_STRINGS_MAX_COUNT          (sizeof(string_table)/sizeof(string_table[0]))     


#define     UAC1_PLAY0_FU_ENABLE                1
#define     UAC1_REC0_FU_ENABLE                 0
#define     UAC1_REC0_SU_ENABLE                 0
#define     UAC1_REC0_MX_ENABLE                 0

#define     HEADER_COLLECT_NUM                  (UAC1_PLAY0_ENABLE+UAC1_REC0_ENABLE)
#define     HEADER_COLLECT_INDEX                0x01,0x02

#define     UAC1_INTERFACE_NUM                  (1+UAC1_PLAY0_ENABLE+UAC1_REC0_ENABLE+UAC1_HID_ENABLE+UAC1_BULK_ENABLE)

#define     UAC1_PIT_ID                         0x01
#define     UAC1_PFU_ID                         0x03
#define     UAC1_POT_ID                         0x04
#define     UAC1_PFU_SRC                        UAC1_PIT_ID
#define     UAC1_POT_SRC                        UAC1_PFU_ID

#define     UAC1_RIT0_ID                        0x08
#define     UAC1_RIT1_ID                        0x09
#define     UAC1_RSU_ID                         0x0A
#define     UAC1_RMX_ID                         0x0C
#define     UAC1_RFU_ID                         0x0B
#define     UAC1_ROT_ID                         0x0D

#define     UAC1_RSU_SRC0                       UAC1_RIT0_ID
#define     UAC1_RSU_SRC1                       UAC1_RIT1_ID
#define     UAC1_RMX_SRC0                       UAC1_RIT0_ID
#define     UAC1_RMX_SRC1                       UAC1_RIT1_ID

#define     UAC1_RFU_SRC                        UAC1_RMX_ID
#define     UAC1_ROT_SRC                        UAC1_PFU_ID

//#define     UAC1_STREAM_INTERFACE_LEN           7
//#define     UAC1_STREAM_FORMAT_NO_FREQ_LEN      8
//#define     UAC1_STREAM_FORMAT_LEN              (8+3*2)//14
//#define     UAC1_STREAM_FORMAT_P0_LEN           (8+3*UAC1_PLAY0_FREQ_COUNT)//14
//#define     UAC1_STREAM_FORMAT_R0_LEN           (8+3*UAC1_REC0_FREQ_COUNT)//14

// Configuration Descriptor index 1
code BYTE uac1_audio_control_default[]={
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x00
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x00 (Default Control Pipe only)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x01 (Audio Control)
//bInterfaceProtocol       : 0x00
//iInterface               : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x09,0x04,0x00,0x00,0x00,0x01,0x01,0x00,0x00,
};
// Configuration Descriptor index 3
code BYTE uac1_audio_control[]={
//        ------- Audio Control Input Terminal Descriptor -------
//bLength                  : 0x0C (12 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Input Terminal)
//bTerminalID              : 0x01
//wTerminalType            : 0x0101 (USB streaming)
//bAssocTerminal           : 0x00
//bNrChannels              : 0x02
//wChannelConfig           : 0x0003
//iChannelNames            : 0x00 (No String Descriptor)
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x0C,0x24,0x02,0x01,0x01,0x01,0x00,0x02,0x03,0x00,0x00,0x00,
//
//        -------- Audio Control Feature Unit Descriptor --------
//bLength                  : 0x0A (10 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x06 (Feature Unit)
//bUnitID                  : 0x03 (3)
//bSourceID                : 0x01 (1)
//bControlSize             : 0x01 (1 byte per control)
//bmaControls[0]           : 0x01
// D0: Mute                : 1 (yes)
// D1: Volume              : 0 (no)
// D2: Bass                : 0 (no)
// D3: Mid                 : 0 (no)
// D4: Treble              : 0 (no)
// D5: Graphic Equalizer   : 0 (no)
// D6: Automatic Gain      : 0 (no)
// D7: Delay               : 0 (no)
//bmaControls[1]           : 0x02
// D0: Mute                : 0 (no)
// D1: Volume              : 1 (yes)
// D2: Bass                : 0 (no)
// D3: Mid                 : 0 (no)
// D4: Treble              : 0 (no)
// D5: Graphic Equalizer   : 0 (no)
// D6: Automatic Gain      : 0 (no)
// D7: Delay               : 0 (no)
//bmaControls[2]           : 0x02
// D0: Mute                : 0 (no)
// D1: Volume              : 1 (yes)
// D2: Bass                : 0 (no)
// D3: Mid                 : 0 (no)
// D4: Treble              : 0 (no)
// D5: Graphic Equalizer   : 0 (no)
// D6: Automatic Gain      : 0 (no)
// D7: Delay               : 0 (no)
//iFeature                 : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x0A,0x24,0x06,0x03,0x01,0x01,0x01,0x02,0x02,0x00,
//
//        ------- Audio Control Output Terminal Descriptor ------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x03 (Output Terminal)
//bTerminalID              : 0x04
//wTerminalType            : 0x0301 (Speaker)
//bAssocTerminal           : 0x00 (0)
//bSourceID                : 0x03 (3)
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x09,0x24,0x03,0x04,0x01,0x03,0x00,0x03,0x00,
//
#if (UAC1_REC0_ENABLE)
#if (UAC1_REC0_MX_ENABLE)
//        ------- Audio Control Input Terminal Descriptor -------
//bLength                  : 0x0C (12 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Input Terminal)
//bTerminalID              : 0x09
//wTerminalType            : 0x0201 (Microphone)
//bAssocTerminal           : 0x00
//bNrChannels              : 0x02
//wChannelConfig           : 0x0003
//iChannelNames            : 0x00 (No String Descriptor)
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x0C,0x24,0x02,0x09,0x01,0x02,0x00,0x02,0x03,0x00,0x00,0x00,
//        ------- Audio Control Input Terminal Descriptor -------
//bLength                  : 0x0C (12 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Input Terminal)
//bTerminalID              : 0x08
//wTerminalType            : 0x0603 (Line connector)
//bAssocTerminal           : 0x00
//bNrChannels              : 0x02
//wChannelConfig           : 0x0003
//iChannelNames            : 0x00 (No String Descriptor)
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x0C,0x24,0x02,0x08,0x03,0x06,0x00,0x02,0x03,0x00,0x00,0x00,
//
//        ------- Audio Control Selector Unit Descriptor --------
//bLength                  : 0x08 (8 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x05 (Selector Unit)
//bUnitID                  : 0x0C
//bNrInPins                : 0x02
//baSourceID[1]            : 0x09
//baSourceID[2]            : 0x08
//iSelector                : 0x00 (No String Descriptor)
//Data (HexDump)           : 
//    0x08,0x24,0x05,0x0C,0x02,0x09,0x08,0x00,
//        --------- Audio Control Mixer Unit Descriptor ---------
//bLength                  : 0x0D (13 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x04 (Mixer Unit)
//bUnitID                  : 0x0A
//bNrInPins                : 0x02
//baSourceID[1]            : 0x01
//baSourceID[2]            : 0x0D
//bNrChannels              : 0x02
//wChannelConfig           : 0x0003
//iChannelNames            : 0x00 (No String Descriptor)
//bmControls (HexDump)     : 00                                                .
//iMixer                   : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x0D,0x24,0x04,0x0C,0x02,0x08,0x09,0x04,0x33,0x00,0x00,0x00,0x00,
#else
//        ------- Audio Control Input Terminal Descriptor -------
//bLength                  : 0x0C (12 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Input Terminal)
//bTerminalID              : 0x08
//wTerminalType            : 0x0603 (Line connector)
//bAssocTerminal           : 0x00
//bNrChannels              : 0x02
//wChannelConfig           : 0x0003
//iChannelNames            : 0x00 (No String Descriptor)
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 
//    0x0C,0x24,0x02,0x08,0x03,0x06,0x00,0x01,0x01,0x00,0x00,0x00,
    0x0C,0x24,0x02,0x08,0x03,0x06,0x00,0x02,0x03,0x00,0x00,0x00,
//    0x0C,0x24,0x02,0x08,0x03,0x06,0x00,0x04,0x33,0x00,0x00,0x00,
#endif
#if (UAC1_REC0_FU_ENABLE)
//
//        -------- Audio Control Feature Unit Descriptor --------
//bLength                  : 0x0A (10 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x06 (Feature Unit)
//bUnitID                  : 0x0B (11)
//bSourceID                : 0x08 (8)
//bControlSize             : 0x01 (1 byte per control)
//bmaControls[0]           : 0x01
// D0: Mute                : 1 (yes)
// D1: Volume              : 0 (no)
// D2: Bass                : 0 (no)
// D3: Mid                 : 0 (no)
// D4: Treble              : 0 (no)
// D5: Graphic Equalizer   : 0 (no)
// D6: Automatic Gain      : 0 (no)
// D7: Delay               : 0 (no)
//bmaControls[1]           : 0x02
// D0: Mute                : 0 (no)
// D1: Volume              : 1 (yes)
// D2: Bass                : 0 (no)
// D3: Mid                 : 0 (no)
// D4: Treble              : 0 (no)
// D5: Graphic Equalizer   : 0 (no)
// D6: Automatic Gain      : 0 (no)
// D7: Delay               : 0 (no)
//bmaControls[2]           : 0x02
// D0: Mute                : 0 (no)
// D1: Volume              : 1 (yes)
// D2: Bass                : 0 (no)
// D3: Mid                 : 0 (no)
// D4: Treble              : 0 (no)
// D5: Graphic Equalizer   : 0 (no)
// D6: Automatic Gain      : 0 (no)
// D7: Delay               : 0 (no)
//iFeature                 : 0x00 (No String Descriptor)
//Data (HexDump)           : 
#if (UAC1_REC0_SU_ENABLE == 1)
    0x0A,0x24,0x06,0x0B,0x0C,0x01,0x03,0x00,0x00,0x00,
//    0x0C,0x24,0x06,0x0B,0x0A,0x01,0x01,0x02,0x02,0x02,0x02,0x00,
#else
    0x0A,0x24,0x06,0x0B,0x08,0x01,0x03,0x00,0x00,0x00,
//    0x0C,0x24,0x06,0x0B,0x08,0x01,0x01,0x02,0x02,0x02,0x02,0x00,
#endif  //UAC1_REC0_SU_ENABLE
#endif  //UAC1_REC0_FU_ENABLE
//
//        ------- Audio Control Output Terminal Descriptor ------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x03 (Output Terminal)
//bTerminalID              : 0x0D
//wTerminalType            : 0x0101 (USB streaming)
//bAssocTerminal           : 0x00 (0)
//bSourceID                : 0x0B (12)
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump) 
#if (UAC1_REC0_FU_ENABLE==1)
    0x09,0x24,0x03,0x0D,0x01,0x01,0x00,0x0B,0x00, 
#elif (UAC1_REC0_SU_ENABLE==1)          : 
    0x09,0x24,0x03,0x0D,0x01,0x01,0x00,0x0C,0x00,
#elif (UAC1_REC0_MX_ENABLE==1)          : 
    0x09,0x24,0x03,0x0D,0x01,0x01,0x00,0x0A,0x00,
#else
    0x09,0x24,0x03,0x0D,0x01,0x01,0x00,0x08,0x00,
#endif
#endif  //UAC1_REC0_ENABLE
};

// Configuration Descriptor index 4
code BYTE uac1_audio_streaming[]={
//          Interface 1 (Playback 0)                       
//        ---------------- Interface Descriptor (alt0)-----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x01
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x00 (Default Control Pipe only)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x02 (Audio Streaming)
//bInterfaceProtocol       : 0x00
//iInterface               : 0x04 (String Descriptor 4)
// Language 0x0409         : "Play Interface"
//Data (HexDump)           : 
    0x09,0x04,UAC1_PLAY0_IF_INDEX,0x00,0x00,0x01,0x02,0x00,0x04,
//
//        ---------------- Interface Descriptor (alt1)-----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x01
//bAlternateSetting        : 0x01
//bNumEndpoints            : 0x01 (1 Endpoint)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x02 (Audio Streaming)
//bInterfaceProtocol       : 0x00
//iInterface               : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x09,0x04,UAC1_PLAY0_IF_INDEX,0x01,0x01,0x01,0x02,0x00,0x00,
//
//        -------- Audio Streaming Interface Descriptor ---------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x01
//bTerminalLink            : 0x01
//bDelay                   : 0x01
//wFormatTag               : 0x0001 (PCM)
//Data (HexDump)           : 
    0x07,0x24,0x01,0x01,0x00,0x01,0x00,                         
//
//        ------- Audio Streaming Format Type Descriptor --------
//bLength                  : 0x0E (14 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Format Type)
//bFormatType              : 0x01
//bNrChannels              : 0x02 (2 channels)
//bSubframeSize            : 0x02 (2 bytes per subframe)
//bBitResolution           : 0x10 (16 bits per sample)
//bSamFreqType             : 0x02 (supports 2 sample frequencies)
//tSamFreq[1]              : 0x0BB80 (48000 Hz)
//tSamFreq[2]              : 0x17700 (96000 Hz)
//Data (HexDump)           : 
//    UAC1_STREAM_FORMAT_P0_LEN,0x24,0x02,0x01,0x02,0x02,0x10,UAC1_PLAY0_FREQ_COUNT,UAC1_PLAY0_FREQS, 
    UAC1_PLAY0_LEN_ALT1,0x24,0x02,0x01,UAC1_PLAY0_FORMAT_ALT1, 
//
//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x01 (Direction=OUT  EndpointID=1)
//bmAttributes             : 0x09 (TransferType=Isochronous  SyncType=Adaptive  EndpointType=Data)
//                           0x0D (TransferType=Isochronous  SyncType=Synchronous  EndpointType=Data)  
//wMaxPacketSize           : 0x0184 (388 bytes) (388 bytes)
//bInterval                : 0x01
//bSynchAddress            : 0x00
//Data (HexDump)           : 
//    0x09,0x05,0x01,0x09,UAC1_PLAY0_FREQ_MAX_SIZE,0x01,0x00,0x00,   
    0x09,0x05,0x01,0x09,LE_ARRAY(UAC1_PLAY0_MAX_SIZE_ALT1),UAC1_FRAME_INTERVAL,0x00,0x00,   
//
//        ----------- Audio Data Endpoint Descriptor ------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x25 (Audio Endpoint Descriptor)
//bDescriptorSubtype       : 0x01 (General)
//bmAttributes             : 0x01
//bLockDelayUnits          : 0x00
//wLockDelay               : 0x0001
//Data (HexDump)           : 
    0x07,0x25,0x01,0x01,0x00,0x00,0x00,                          
#if (UAC1_PLAY0_ALT_COUNT > 1)
//        ---------------- Interface Descriptor (alt2)-----------------
    0x09,0x04,UAC1_PLAY0_IF_INDEX,0x02,0x01,0x01,0x02,0x00,0x00,
//        -------- Audio Streaming Interface Descriptor ---------
    0x07,0x24,0x01,0x01,0x00,0x01,0x00,                         
//        ------- Audio Streaming Format Type Descriptor --------
//    0x0E,0x24,0x02,0x01,UAC1_PLAY0_FORMAT_2C24B,  
    UAC1_PLAY0_LEN_ALT2,0x24,0x02,0x01,UAC1_PLAY0_FORMAT_ALT2, 
//        ----------------- Endpoint Descriptor -----------------
//    0x09,0x05,0x01,0x09,LE_ARRAY((96+1)*2*3),0x01,0x00,0x00,   
//    0x09,0x05,0x01,0x09,(UAC1_PLAY0_MAX_SIZE_ALT2|(UAC1_MULTI_TRANSACTION<<11)),UAC1_FRAME_INTERVAL,0x00,0x00,   
//    0x09,0x05,0x01,0x09,0x06,0x0B,UAC1_FRAME_INTERVAL,0x00,0x00,   
    0x09,0x05,0x01,0x09,LE_ARRAY(UAC1_PLAY0_MAX_SIZE_ALT2),UAC1_FRAME_INTERVAL,0x00,0x00,   
//        ----------- Audio Data Endpoint Descriptor ------------
    0x07,0x25,0x01,0x01,0x00,0x00,0x00,
#endif

#if (UAC1_REC0_ENABLE)
//          Interface 2 (Record) 
//        ---------------- Interface Descriptor (alt0)-----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x02
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x00 (Default Control Pipe only)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x02 (Audio Streaming)
//bInterfaceProtocol       : 0x00
//iInterface               : 0x05 (String Descriptor 5)
// Language 0x0409         : "Record Interface"
//Data (HexDump)           : 
    0x09,0x04,UAC1_REC0_IF_INDEX,0x00,0x00,0x01,0x02,0x00,0x05,
//
//        ---------------- Interface Descriptor (alt1)-----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x02
//bAlternateSetting        : 0x01
//bNumEndpoints            : 0x01 (1 Endpoint)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x02 (Audio Streaming)
//bInterfaceProtocol       : 0x00
//iInterface               : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x09,0x04,UAC1_REC0_IF_INDEX,0x01,0x01,0x01,0x02,0x00,0x00,  
//
//        -------- Audio Streaming Interface Descriptor ---------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x01
//bTerminalLink            : 0x0D
//bDelay                   : 0x01
//wFormatTag               : 0x0001 (PCM)
//Data (HexDump)           : 
    0x07,0x24,0x01,0x0D,0x00,0x01,0x00,    
//
//        ------- Audio Streaming Format Type Descriptor --------
//bLength                  : 0x0E (14 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Format Type)
//bFormatType              : 0x01
//bNrChannels              : 0x02 (2 channels)
//bSubframeSize            : 0x02 (2 bytes per subframe)
//bBitResolution           : 0x10 (16 bits per sample)
//bSamFreqType             : 0x02 (supports 2 sample frequencies)
//tSamFreq[1]              : 0x0AC44 (44100 Hz)
//tSamFreq[2]              : 0x15888 (88200 Hz)
//Data (HexDump)           : 
//    0x0E,0x24,0x02,0x01,0x02,0x02,0x10,0x02,UAC1_REC0_FREQS,  
//    UAC1_REC0_LEN_ALT1,0x24,0x02,0x01,UAC1_REC0_FORMAT_ALT1,  
    UAC1_REC0_LEN_ALT1,0x24,0x02,0x01,UAC1_REC0_FORMAT_ALT1,  
//
//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x82 (Direction=IN  EndpointID=2)
//bmAttributes             : 0x05 (TransferType=Isochronous  SyncType=Asynchronous  EndpointType=Data)
//wMaxPacketSize           : 0x0168 (360 bytes) (360 bytes)
//bInterval                : 0x01
//bSynchAddress            : 0x00
//Data (HexDump)           : 
//    0x09,0x05,0x82,0x05,UAC1_REC0_FREQ_MAX_SIZE,0x01,0x00,0x00,   
    0x09,0x05,0x82,0x05,LE_ARRAY(UAC1_REC0_MAX_SIZE_ALT1),UAC1_FRAME_INTERVAL,0x00,0x00,   
//
//        ----------- Audio Data Endpoint Descriptor ------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x25 (Audio Endpoint Descriptor)
//bDescriptorSubtype       : 0x01 (General)
//bmAttributes             : 0x01
//bLockDelayUnits          : 0x00
//wLockDelay               : 0x0000
//Data (HexDump)           : 
    0x07,0x25,0x01,0x01,0x00,0x00,0x00, 
#if (UAC1_REC0_ALT_COUNT > 1)
//        ---------------- Interface Descriptor (alt2)-----------------
    0x09,0x04,UAC1_REC0_IF_INDEX,0x02,0x01,0x01,0x02,0x00,0x00,  
//        -------- Audio Streaming Interface Descriptor ---------
    0x07,0x24,0x01,0x0D,0x01,0x01,0x00,    
//        ------- Audio Streaming Format Type Descriptor --------
    UAC1_REC0_LEN_ALT2,0x24,0x02,0x01,UAC1_REC0_FORMAT_ALT2,  
//        ----------------- Endpoint Descriptor -----------------
//    0x09,0x05,0x82,0x05,(UAC1_REC0_MAX_SIZE_ALT2|(UAC1_MULTI_TRANSACTION<<11)),UAC1_FRAME_INTERVAL,0x00,0x00,   
//    0x09,0x05,0x82,0x05,0x06,0x0B,UAC1_FRAME_INTERVAL,0x00,0x00,   
    0x09,0x05,0x82,0x05,LE_ARRAY(UAC1_REC0_MAX_SIZE_ALT2),UAC1_FRAME_INTERVAL,0x00,0x00,   
//        ----------- Audio Data Endpoint Descriptor ------------
    0x07,0x25,0x01,0x01,0x00,0x00,0x00, 
#endif
#endif //UAC1_REC0_ENABLE
};


#if (UAC1_HID_ENABLE == 1) 
// Configuration Descriptor index 5
code BYTE uac1_hid_interface[]={
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x04
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x01 (1 Endpoint)
//bInterfaceClass          : 0x03 (HID - Human Interface Device)
//bInterfaceSubClass       : 0x00 (None)
//bInterfaceProtocol       : 0x00 (None)
//iInterface               : 0x07 (String Descriptor 7)
// Language 0x0409         : "HID Interface"
//Data (HexDump)           : 
     0x09,0x04,UAC1_HID_IF_INDEX,0x00,0x01,0x03,0x00,0x00,0x06,
//
//        ------------------- HID Descriptor --------------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x21 (HID Descriptor)
//bcdHID                   : 0x0111 (HID Version 1.11)
//bCountryCode             : 0x00 (00 = not localized)
//bNumDescriptors          : 0x01
//Data (HexDump)           : 
    0x09,0x21,0x11,0x01,0x00,0x01,0x22,CT_UAC1_HID_REPORT_DESC_LEN,0x00,                  
//Descriptor 1:
//bDescriptorType          : 0x22 (Class=Report)
//wDescriptorLength        : 0x002B (43 bytes)
//Error reading descriptor : ERROR_GEN_FAILURE
//
//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x86 (Direction=IN  EndpointID=6)
//bmAttributes             : 0x03 (TransferType=Interrupt)
//wMaxPacketSize           : 0x0004 (4 bytes) (4 bytes)
//bInterval                : 0x20 (32 ms)
//Data (HexDump)           : 
    0x07,0x05,0x86,0x03,0x04,0x00,0x20, 
};
#endif  //HID 
                        
#if (UAC1_BULK_ENABLE == 1) 
// Configuration Descriptor index 5
code BYTE uac1_bulk_interface[]={
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x00
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x03 (3 Endpoints)
//bInterfaceClass          : 0xFF (Vendor Specific)
//bInterfaceSubClass       : 0xF0
//bInterfaceProtocol       : 0x00
//iInterface               : 0x00 (No String Descriptor)
//Data (HexDump)           : 
    0x09,0x04,UAC1_BULK_IF_INDEX,0x00,0x02,0xFF,0xF0,0x00,0x07,   

//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x01 (Direction=OUT EndpointID=1)
//bmAttributes             : 0x02 (TransferType=Bulk)
//wMaxPacketSize           : 0x0040 (max 64 bytes)
//bInterval                : 0x00 (never NAKs)
//Data (HexDump)           : 07 05 01 02 40 00 00                              ....@..
    0x07,0x05,0x07,0x02,0x40,0x00,0x01, 

//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x81 (Direction=IN EndpointID=1)
//bmAttributes             : 0x02 (TransferType=Bulk)
//wMaxPacketSize           : 0x0040 (max 64 bytes)
//bInterval                : 0x00 (never NAKs)
//Data (HexDump)           : 07 05 81 02 40 00 00                              ....@..
    0x07,0x05,0x88,0x02,0x40,0x00,0x01, 
};
#endif  //BULK                        

#if(UAC1_HID_ENABLE == 1)
    #define     UAC1_HID_INTERFACE_LEN          (sizeof(uac1_hid_interface)) 
#else
    #define     UAC1_HID_INTERFACE_LEN          0
#endif 
   
#define     UAC1_HID_DESC_LEN               9

//#define     UAC1_BULK_INTERFACE_LEN         (sizeof(uac1_bulk_interface))    
#if(UAC1_BULK_ENABLE == 1)
    #define     UAC1_BULK_INTERFACE_LEN     (sizeof(uac1_bulk_interface)) 
#else
    #define     UAC1_BULK_INTERFACE_LEN     0
#endif 

#define     UAC1_AUDIO_CONTROL_HEADER_LEN   (8+HEADER_COLLECT_NUM)     
#define     UAC1_AUDIO_CONTROL_LEN          (sizeof(uac1_audio_control))    
#define     UAC1_AUDIO_STREAMING_LEN        (sizeof(uac1_audio_streaming))    

#define     UAC1_AUDIO_CONTROL_TOTAL_LEN    (UAC1_AUDIO_CONTROL_HEADER_LEN+UAC1_AUDIO_CONTROL_LEN) 
#define     UAC1_CONFIG_DESC_LEN            (9+9+UAC1_AUDIO_CONTROL_TOTAL_LEN+UAC1_AUDIO_STREAMING_LEN+UAC1_HID_INTERFACE_LEN+UAC1_BULK_INTERFACE_LEN)    


// Configuration Descriptor index 2
code BYTE uac1_audio_control_header[]={
//        ------ Audio Control Interface Header Descriptor ------
//bLength                  : 0x0B (11 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x01 (Header)
//bcdADC                   : 0x0100
//wTotalLength             : 0x0068 (104 bytes)
//bInCollection            : 0x03
//baInterfaceNr[1]         : 0x01
//baInterfaceNr[2]         : 0x02
//baInterfaceNr[3]         : 0x03
//Data (HexDump)           : 
    UAC1_AUDIO_CONTROL_HEADER_LEN,0x24,0x01,0x00,0x01,LE_ARRAY(UAC1_AUDIO_CONTROL_TOTAL_LEN),HEADER_COLLECT_NUM,HEADER_COLLECT_INDEX,
};

// Configuration Descriptor index 0
code BYTE uac1_config_header[]={
//      ---------------- Configuration Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x02 (Configuration Descriptor)
//wTotalLength             : 0x00E1 (225 bytes)
//bNumInterfaces           : 0x04
//bConfigurationValue      : 0x01
//iConfiguration           : 0x00 (No String Descriptor)
//bmAttributes             : 0xC0
// D7: Reserved, set 1     : 0x01
// D6: Self Powered        : 0x01 (yes)
// D5: Remote Wakeup       : 0x00 (no)
// D4..0: Reserved, set 0  : 0x00
//MaxPower                 : 0x14 (40 mA)
    0x09,0x02,LE_ARRAY(UAC1_CONFIG_DESC_LEN),UAC1_INTERFACE_NUM,0x01,0x00,POWER_CONFIG,
};	

code DESCRIPTOR_INFO config_desc_table[] = 
{
    { 9,    						    uac1_config_header }, 
    { 9,         					    uac1_audio_control_default }, 
    { UAC1_AUDIO_CONTROL_HEADER_LEN,    uac1_audio_control_header }, 
    { UAC1_AUDIO_CONTROL_LEN,           uac1_audio_control }, 
    { UAC1_AUDIO_STREAMING_LEN,   	    uac1_audio_streaming }, 
#if (UAC1_HID_ENABLE == 1)
    { UAC1_HID_INTERFACE_LEN,           uac1_hid_interface }, 
#endif
#if (UAC1_BULK_ENABLE == 1) 
    { UAC1_BULK_INTERFACE_LEN,          uac1_bulk_interface },
#endif 
};

#define UAC1_CONFIG_MAX_COUNT           (sizeof(config_desc_table)/sizeof(config_desc_table[0]))     


#endif //_CTUSB_DESCRIPTOR_C_
#endif //_CTUSB_UAC1_DESCRIPTOR_H_
