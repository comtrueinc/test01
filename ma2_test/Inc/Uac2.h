#include "global.h"

#ifndef _CTUSB_UAC2_DESCRIPTOR_H_
#define _CTUSB_UAC2_DESCRIPTOR_H_

//#define		UAC2_PLAY0_FEEDBACK					1

//#define     UAC2_PLAY0_FREQ_COUNT               10
//#define     UAC2_REC0_FREQ_COUNT                10
/*
#define     UAC2_PLAY0_16BITS_ENABLE            1
#define     UAC2_PLAY0_24BITS_ENABLE            1
#define     UAC2_PLAY0_32BITS_ENABLE            1
#define     UAC2_PLAY0_NDSD_ENABLE              0
#define     UAC2_PLAY0_DTSS_ENABLE              0
#define     UAC2_PLAY0_ALT_VALID                (0x1F)      //5

#define     UAC2_PLAY0_16BITS_ALL_INDEX         1
#define     UAC2_PLAY0_24BITS_ALL_INDEX         2
#define     UAC2_PLAY0_32BITS_ALL_INDEX         3
#define     UAC2_PLAY0_NDSD_ALT_INDEX           4
#define     UAC2_PLAY0_DTS_ALT_INDEX            5


#define     UAC2_REC0_16BITS_ENABLE             1
#define     UAC2_REC0_24BITS_ENABLE             1
#define     UAC2_REC0_32BITS_ENABLE             1
#define     UAC2_REC0_ALT_VALID                 (0x07)      //3

#define     UAC2_REC0_16BITS_ALL_INDEX          1
#define     UAC2_REC0_24BITS_ALL_INDEX          2
#define     UAC2_REC0_32BITS_ALL_INDEX          3
*/
//#define _CTUSB_DESCRIPTOR_C_    1
#ifdef  _CTUSB_DESCRIPTOR_C_
typedef struct _DESCRIPTOR_INFO_
{
    WORD    len;
    BYTE    *buffer;
}DESCRIPTOR_INFO, *PDESCRIPTOR_INFO;

#define     MANUFACTURER_NAME                   "Comtrue"               //"mCore"
#define     PRODUCT_NAME                        "CT7702 UAC2"           //"mCore Audio UBox"
#define     SERIAL_NUMBER                       "221026A2"

#define     VID                                 0x2FC6                  //0x1403  //0x2FC6
#define     PID                                 0x7023                  //0x6002    //0x3388  //0x8001

#define     PLAYBACK_NAME                       "Playback"
#define     RECORD_NAME                         "Record"
#define     HID_NAME                            "Hid"
#define     IAP_NAME                            "iAP Interface"

#define     MANUFACTURER_NAME_LEN               (sizeof(MANUFACTURER_NAME)-1)
#define     PRODUCT_NAME_LEN                    (sizeof(PRODUCT_NAME)-1)
#define     SERIAL_NUMBER_LEN                   (sizeof(SERIAL_NUMBER)-1)

#define     PLAYBACK_NAME_LEN                   (sizeof(PLAYBACK_NAME)-1)
#define     RECORD_NAME_LEN                     (sizeof(RECORD_NAME)-1)
#define     HID_NAME_LEN                        (sizeof(HID_NAME)-1)
#define     IAP_NAME_LEN                        (sizeof(IAP_NAME)-1)

#define     CT_UAC2_DEVICE_DESC_ADDR            USB_DESC_DATA_BASE

#define		UAC2_ASYNC_MODE						0x05				//0x05:Async, 0x09:Adaptive 0x0D:Sync
#define		UAC2_ADATIVE_MODE					0x09				//0x05:Async, 0x09:Adaptive 0x0D:Sync
#define		UAC2_SYNC_MODE						0x0D				//0x05:Async, 0x09:Adaptive 0x0D:Sync

//=============================================================================================================================
#define     UAC2_CONTROL_ENABLE                 CONTROL_ENABLE
#define     UAC2_PLAY0_ENABLE                   PLAYBACK_ENABLE
#define     UAC2_REC0_ENABLE                    RECORD_ENABLE
#define     UAC2_HID_ENABLE                     HID_ENABLE
#define     UAC2_BULK_ENABLE                    BULK_ENABLE

#define		UAC2_PLAY0_ALT_16				    ALT_PLAYBACK_16B
#define		UAC2_PLAY0_ALT_24				    ALT_PLAYBACK_24B
#define		UAC2_PLAY0_ALT_32				    ALT_PLAYBACK_32B
#define		UAC2_PLAY0_ALT_NDSD					ALT_PLAYBACK_NDSD
#define		UAC2_PLAY0_ALT_DTS					ALT_PLAYBACK_DTS

#define     UAC2_HID_VOLUME_CONTROL             1
#define     UAC2_HIDINOUT_TEST                  0

#define     UAC2_PLAY0_FU_ENABLE                1
#define     UAC2_REC0_FU_ENABLE                 1
#define     UAC2_REC0_SU_ENABLE                 0
#define     UAC2_JACK_DETECTION                 0
#define     UAC2_FEEDBACK_MIXER                 0

#define     UAC2_CONTROL0_EP_NUM                UAC2_JACK_DETECTION
#define     UAC2_PLAY0_BMCONTROL                (UAC2_JACK_DETECTION?0x04:0x00)
#define     UAC2_REC0_BMCONTROL                 (UAC2_JACK_DETECTION?0x04:0x00)

#define     UAC2_INTERFACE_NUM                  (UAC2_CONTROL_ENABLE+UAC2_PLAY0_ENABLE+UAC2_REC0_ENABLE+UAC2_HID_ENABLE+UAC2_BULK_ENABLE)
#define     UAC2_IF_AUDIO_COUNT                 (CONTROL_ENABLE+PLAYBACK_ENABLE+RECORD_ENABLE)

#define     UAC2_PLAY0_IF_INDEX                 IF_PLAYBACK
#define     UAC2_REC0_IF_INDEX                  IF_RECORD
#define     UAC2_HID_IF_INDEX                   IF_HID
#define     UAC2_BULK_IF_INDEX                  IF_BULK

// USB Device Class Definition for Terminal Types
// Table 2-1 USB Terminal Types
#define     USB_UNDEFINED               0x0100
#define     USB_STREAMING               0x0101
#define     USB_VENDOR_SPECIFIC         0x01FF

// Table 2-2 Input Terminal Types
#define     INPUT_UDEFINED              0x0200
#define     IN_MICROPHONE               0x0201
#define     DESKTOP_MICROPHONE          0x0202
#define     PERSONAL_MICROPHONE         0x0203
#define     OMNIDIRECTIONAL_MICROPHONE  0x0204
#define     MICROPHONE_ARRAY            0x0205
#define     PROCESSING_MICROPHONE_ARRAY 0x0206

// Table 2-3 Output Terminal Types
#define     SPEAKER                     0x0301
#define     HEADPHONES                  0x0302
#define     HEAD_MOUNTED_DISPLAY        0x0303
#define     DESKTOPSPEAKER              0x0304
#define     ROOM_SPEAKER                0x0305
#define     COMMUNICATION_SPEAKER       0x0306
#define     LOW_FREQ_EFFECTS_SPEAKER    0x0307

// Table 2-6 External Terminal Types
#define     EXTERMAL_UNDEFINED          0x0600
#define     ANALOG_CONNECTOR            0x0601
#define     DIGITAL_AUDIO_INTERFACE     0x0602
#define     LINE_CONNECTOR              0x0603
#define     LEGACY_AUDIO_INTERFACE      0x0604
#define     SPDIF_INTERFACE             0x0605
#define     DA_1394_INTERFACE           0x0606
#define     DA_1394_ST_INTERFACE        0x0606

#define     PLAYBACK_OT0                HEADPHONES      //SPEAKER
#define     RECORD_IT0                  IN_MICROPHONE
#define     RECORD_IT1                  ANALOG_CONNECTOR


#define     UAC2_TYPE_I_UNDEFINED               0x00000000
#define     UAC2_TYPE_I_PCM                     0x00000001
#define     UAC2_TYPE_I_RAW_DATA                0x80000000

#define     UAC2_TYPE_III_DTS                   0x00001381  // ASUS D7

//#if (UAC2_PLAY0_FEEDBACK)
#define		UAC2_PLAY0_SYNC_TYPE				((UAC2_PLAY0_FEEDBACK)?UAC2_ASYNC_MODE:UAC2_ADATIVE_MODE)   //0x05:Async, 0x09:Adaptive
//#else
//#define		UAC2_PLAY0_SYNC_TYPE				UAC2_ADATIVE_MODE				//0x05:Async, 0x09:Adaptive
//#endif

//for playback
#define     UAC2_PIT_ID                         PIT_ID
#define     UAC2_PFU_ID                         PFU_ID
#define     UAC2_POT_ID                         POT_ID
#define     UAC2_PCS_ID                         PCS_ID
#define     UAC2_PMX_ID                         PMX_ID
#define     UAC2_SFU_ID                         SFU_ID

//for record
#define     UAC2_RIT0_ID                        RIT0_ID        //Mic
#define     UAC2_RIT1_ID                        RIT1_ID        //LineIn
#define     UAC2_RSU_ID                         RSU_ID
#define     UAC2_RFU_ID                         RFU_ID
#define     UAC2_FFU_ID                         FFU_ID
#define     UAC2_ROT_ID                         ROT_ID
#define     UAC2_RCS_ID                         RCS_ID

// IT --> FU1 -> MX -> FU4 --> OT
#define     UAC2_PMX_SRC0                       UAC2_PFU_ID 
#define     UAC2_PMX_SRC1                       UAC2_FFU_ID
#define     UAC2_PFU_SRC                        UAC2_PIT_ID
#define     UAC2_SFU_SRC                        UAC2_PMX_ID
#define     UAC2_POT_SRC                        ((UAC2_FEEDBACK_MIXER)?UAC2_SFU_ID:UAC2_PFU_ID)
// IT --> MX --> FU --> OT
//#define     UAC2_PMX_SRC0                       UAC2_PIT_ID 
//#define     UAC2_PFU_SRC                        ((UAC2_FEEDBACK_MIXER)?UAC2_PMX_ID:UAC2_PIT_ID)
//#define     UAC2_POT_SRC                        UAC2_PFU_ID
// IT --> FU --> MX --> OT
//#define     UAC2_PMX_SRC0                       UAC2_PFU_ID
//#define     UAC2_PFU_SRC                        UAC2_PIT_ID
//#define     UAC2_POT_SRC                        ((UAC2_FEEDBACK_MIXER)?UAC2_PMX_ID:UAC2_PFU_ID)

#define     UAC2_RSU_SRC0                       UAC2_RIT0_ID
#define     UAC2_RSU_SRC1                       UAC2_RIT1_ID
//#define     UAC2_RMX_SRC0                       UAC2_RIT0_ID
//#define     UAC2_RMX_SRC1                       UAC2_RIT1_ID

#define     UAC2_RFU_SRC                        ((UAC2_REC0_SU_ENABLE)?UAC2_RSU_ID:UAC2_RIT0_ID)
#define     UAC2_ROT_SRC                        ((UAC2_REC0_FU_ENABLE)?UAC2_RFU_ID:UAC2_RFU_SRC)

#define     UAC2_FFU_SRC                        UAC2_RIT0_ID
// UAC2 Device Descriptor Data

//code BYTE uac2_device_desc_buffer[]={
//    0x12,0x01,0x10,0x01,0xEF,0x02,0x01,0x40,0xC6,0x2F,0x01,0x80,0x01,0x01,0x01,0x02,   // Base = 0x0600 - 0x0603
//    0x03,0x01,                                                               // Base = 0x0604 - 0x0604
//    };
/* USB Standard Device Descriptor */
code BYTE uac2_device_desc_buffer[] = {
    0x12,                               /* bLength */
    0x01,                               /* bDescriptorType */
    LOBYTE(USB_TYPE),HIBYTE(USB_TYPE),  /* bcdUSB (1.10/2.00) */
    0xEF,                               /* bDeviceClass */
    0x02,                               /* bDeviceSubClass */
    0x01,                               /* bDeviceProtocol */
    0x40,                               /* bMaxPacketSize0 (64) */
    LOBYTE(VID),HIBYTE(VID),            /* idVendor */
    LOBYTE(PID),HIBYTE(PID),            /* idProduct */
    0x01,0x02,                          /* bcdDevice (1.00) */
    0x01,                               /* iManufacturer */
    0x02,                               /* iProduct */
    0x03,                               /* iSerialNumber */
    0x01                                /* bNumConfigurations */
};
#define     CT_UAC2_DEVICE_DESC_LEN   sizeof(uac2_device_desc_buffer)


code BYTE uac2_hid_report_buffer[] =
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
#if (UAC2_HIDINOUT_TEST ==1)
    //181227++ for HID I/O Test
    	0xA1, 0x02,                         //   Collection (Logical)
        	0x85, 0x04,                     //     Report ID (4)
        	0x75, 0x08,                     //     Report Size (8)
        	0x95, 0x2F,                     //     Report Count (47+1=48)
        	0x09, 0x01,                     //     Usage (Pointer)
        	0x81, 0x02,                     //     Input (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position)
        0xC0,                               //   End Collection
        0xA1, 0x02,                         //   Collection (Logical)
        	0x85, 0x05,                     //     Report ID (5)
        	0x75, 0x08,                     //     Report Size (8)
        	0x95, 0x2F,                     //     Report Count (47+1=48)
        	0x09, 0x01,                     //     Usage (Pointer)
        	0x91, 0x02,                     //     Output (Data,Var,Abs,No Wrap,Linear,Preferred State,No Null Position,Non-volatile)
    	0xC0,                           //   End Collection
    //181227..
#endif
    0xC0,                           // END_COLLECTION
#if(UAC2_HID_VOLUME_CONTROL==1)
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
		0x09,0xE2,	// USAGE (Mute)
		0x09,0xB7,	// USAGE (STOP)
		0x09,0xCD,	// USAGE (Play/Stop)
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
#define     CT_UAC2_HID_REPORT_DESC_LEN   sizeof(uac2_hid_report_buffer)

// UAC2 Device Qualify Descriptor Data
code BYTE uac2_dev_qualify_buffer[]={
    0x0A,0x06,0x00,0x02,0xEF,0x02,0x01,0x40,0x01,0x00,                                 // Base = 0x06E4 - 0x06E6
};
#define     CT_UAC2_DEV_QUALIFY_DESC_LEN   sizeof(uac2_dev_qualify_buffer)

// UAC2 Other Speed Config Descriptor Data
/*
code BYTE uac2_os_config_buffer[]={
    0x09, 0x07, 0xBE, 0x00, 0x03, 0x01, 0x00, 0xC0, 0x00, 
    0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 
    0x0A, 0x24, 0x01, 0x00, 0x01, 0x3E, 0x00, 0x02, 0x01, 0x02, 
    0x0C, 0x24, 0x02, 0x01, 0x01, 0x01, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 
    0x0A, 0x24, 0x06, 0x03, 0x01, 0x01, 0x03, 0x02, 0x02, 0x00, 
    0x09, 0x24, 0x03, 0x04, 0x01, 0x03, 0x00, 0x03, 0x00, 
    0x0C, 0x24, 0x02, 0x0A, 0x01, 0x02, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 
    0x09, 0x24, 0x03, 0x0D, 0x01, 0x01, 0x00, 0x0A, 0x00,
    0x09, 0x04, 0x01, 0x00, 0x00, 0x01, 0x02, 0x00, 0x04, 
    0x09, 0x04, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 
    0x07, 0x24, 0x01, 0x01, 0x01, 0x01, 0x00, 
    0x0E, 0x24, 0x02, 0x01, 0x02, 0x02, 0x10, 0x02, 0x44, 0xAC, 0x00, 0x80, 0xBB, 0x00, 
    0x09, 0x05, 0x01, 0x09, 0xF0, 0x00, 0x01, 0x00, 0x00,
    0x07, 0x25, 0x01, 0x01, 0x00, 0x01, 0x00, 
    0x09, 0x04, 0x02, 0x00, 0x00, 0x01, 0x02, 0x00, 0x05,
    0x09, 0x04, 0x02, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 
    0x07, 0x24, 0x01, 0x0D, 0x01, 0x01, 0x00,
    0x0E, 0x24, 0x02, 0x01, 0x02, 0x02, 0x10, 0x01, 0x44, 0xAC, 0x00, 0x80, 0xBB, 0x00, 
    0x09, 0x05, 0x82, 0x09, 0x84, 0x01, 0x01, 0x00, 0x00, 
    0x07, 0x25, 0x01, 0x01, 0x00, 0x00, 0x00,      
};
*/
code BYTE uac2_os_config_buffer[]={
//    0x09, 0x07, 0xBE, 0x00, 0x03, 0x01, 0x00, 0xC0, 0x00, 
    0x09, 0x07, 0x71, 0x00, 0x03, 0x01, 0x00, 0xC0, 0x00, 
    0x09, 0x04, 0x00, 0x00, 0x00, 0x01, 0x01, 0x00, 0x00, 
//    0x0A, 0x24, 0x01, 0x00, 0x01, 0x3E, 0x00, 0x02, 0x01, 0x02, 
    0x09, 0x24, 0x01, 0x00, 0x01, 0x28, 0x00, 0x01, 0x01, 
    0x0C, 0x24, 0x02, 0x01, 0x01, 0x01, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 
    0x0A, 0x24, 0x06, 0x03, 0x01, 0x01, 0x03, 0x02, 0x02, 0x00, 
    0x09, 0x24, 0x03, 0x04, 0x01, 0x03, 0x00, 0x03, 0x00, 
//    0x0C, 0x24, 0x02, 0x0A, 0x01, 0x02, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 
//    0x09, 0x24, 0x03, 0x0D, 0x01, 0x01, 0x00, 0x0A, 0x00,
    0x09, 0x04, 0x01, 0x00, 0x00, 0x01, 0x02, 0x00, 0x04, 
    0x09, 0x04, 0x01, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 
    0x07, 0x24, 0x01, 0x01, 0x01, 0x01, 0x00, 
    0x0E, 0x24, 0x02, 0x01, 0x02, 0x02, 0x10, 0x02, 0x44, 0xAC, 0x00, 0x80, 0xBB, 0x00, 
    0x09, 0x05, 0x01, 0x09, 0xF0, 0x00, 0x01, 0x00, 0x00,
    0x07, 0x25, 0x01, 0x01, 0x00, 0x01, 0x00, 
//    0x09, 0x04, 0x02, 0x00, 0x00, 0x01, 0x02, 0x00, 0x05,
//    0x09, 0x04, 0x02, 0x01, 0x01, 0x01, 0x02, 0x00, 0x00, 
//    0x07, 0x24, 0x01, 0x0D, 0x01, 0x01, 0x00,
//    0x0E, 0x24, 0x02, 0x01, 0x02, 0x02, 0x10, 0x01, 0x44, 0xAC, 0x00, 0x80, 0xBB, 0x00, 
//    0x09, 0x05, 0x82, 0x09, 0x84, 0x01, 0x01, 0x00, 0x00, 
//    0x07, 0x25, 0x01, 0x01, 0x00, 0x00, 0x00,      
};
#define     CT_UAC2_OS_CONFIG_DESC_LEN   sizeof(uac2_os_config_buffer)

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

#define UAC2_STRINGS_MAX_COUNT          (sizeof(string_table)/sizeof(string_table[0]))

//
//        ------------------- IAD Descriptor --------------------
code BYTE uac2_iad_descriptor[]={
//bLength                  : 0x08 (8 bytes)
//bDescriptorType          : 0x0B
//bFirstInterface          : 0x00
//bInterfaceCount          : 0x03
//bFunctionClass           : 0x01 (Audio)
//bFunctionSubClass        : 0x00 (undefined)
//bFunctionProtocol        : 0x20 (AF 2.0)
//iFunction                : 0x00 (No String Descriptor)
//Data (HexDump)           : 08 0B 00 03 01 00 20 00                           ...... .
    0x08,0x0B,IF_CONTROL,UAC2_IF_AUDIO_COUNT,0x01,0x00,0x20,0x00,
};
//        ---------------- Interface Descriptor -----------------
code BYTE uac2_audio_control_if0[]={
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x00
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x00 (Default Control Pipe only)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x01 (Audio Control)
//bInterfaceProtocol       : 0x20 (Device Protocol Version 2.0)
//iInterface               : 0x00 (No String Descriptor)
//Data (HexDump)           :
    0x09,0x04,IF_CONTROL,0x00,UAC2_CONTROL0_EP_NUM,0x01,0x01,0x20,0x00,
};
//        ---- Audio Control Interface Header Descriptor 2.0 ----
//        ==> see bottom
code BYTE uac2_audio_control[]={
//        --- Audio Control Clock Source Unit Descriptor 2.0 ----
//bLength                  : 0x08 (8 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x0A (Clock Source 2.0)
//bClockID                 : 0x05
//bmAttributes             : 0x03
// D1..0: Clock Type       : 0x03
// D2   : Sync to SOF      : 0x00
// D7..3: Reserved         : 0x00
//bmControls               : 0x07
// D1..0: Clock Frequency  : 0x03 (host programmable)
// D3..2: Clock Validity   : 0x01 (read only)
// D7..4: Reserved         : 0x00
//bAssocTerminal           : 0x00
//iClockSource             : 0x00 (No String Descriptor)
//Data (HexDump)           : 08 24 0A 05 03 07 00 00                           .$......
//
//        ----- Audio Control Input Terminal Descriptor 2.0 -----
//bLength                  : 0x11 (17 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Input Terminal 2.0)
//bTerminalID              : 0x01
//wTerminalType            : 0x0101 (USB streaming)
//bAssocTerminal           : 0x00
//bCSourceID               : 0x05 (5)
//bNrChannels              : 0x02 (2 Channels)
//bmChannelConfig          : 0x00000003 (FL, FR)
//iChannelNames            : 0x00 (No String Descriptor)
//bmControls               : 0x0000
// D1..0  : Copy Protect   : 0x00 (not present)
// D3..2  : Connector      : 0x00 (not present)
// D5..4  : Overload       : 0x00 (not present)
// D7..6  : Cluster        : 0x00 (not present)
// D9..8  : Underflow      : 0x00 (not present)
// D11..10: Overflow       : 0x00 (not present)
// D15..12: Reserved       : 0x00
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 11 24 02 01 01 01 00 05 02 03 00 00 00 00 00 00   .$..............
//                           00                                                .
//
//        ------ Audio Control Feature Unit Descriptor 2.0 ------
//bLength                  : 0x12 (18 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x06 (Feature Unit 2.0)
//bUnitID                  : 0x03 (3)
//bSourceID                : 0x01 (1)
//bmaControls[0]           : 0x0F, 0x00, 0x00, 0x00
// D1..0  : Mute            : 0x03 (host programmable)
// D3..2  : Volume          : 0x03 (host programmable)
// D5..4  : Bass            : 0x00 (not present)
// D7..6  : Mid             : 0x00 (not present)
// D9..8  : Treble          : 0x00 (not present)
// D11..10: Graph Equalizer : 0x00 (not present)
// D13..12: Automatic Gain  : 0x00 (not present)
// D15..14: Delay           : 0x00 (not present)
// D17..16: Bass Boost      : 0x00 (not present)
// D19..18: Loudness        : 0x00 (not present)
// D21..20: Input Gain      : 0x00 (not present)
// D23..22: Input Gain Pad  : 0x00 (not present)
// D25..24: Phase Inverter  : 0x00 (not present)
// D27..26: Underflow       : 0x00 (not present)
// D29..28: Overflow        : 0x00 (not present)
// D31..30: reserved        : 0x00 (not present)
//bmaControls[1]           : 0x0C, 0x00, 0x00, 0x00
// D1..0  : Mute            : 0x00 (not present)
// D3..2  : Volume          : 0x03 (host programmable)
// D5..4  : Bass            : 0x00 (not present)
// D7..6  : Mid             : 0x00 (not present)
// D9..8  : Treble          : 0x00 (not present)
// D11..10: Graph Equalizer : 0x00 (not present)
// D13..12: Automatic Gain  : 0x00 (not present)
// D15..14: Delay           : 0x00 (not present)
// D17..16: Bass Boost      : 0x00 (not present)
// D19..18: Loudness        : 0x00 (not present)
// D21..20: Input Gain      : 0x00 (not present)
// D23..22: Input Gain Pad  : 0x00 (not present)
// D25..24: Phase Inverter  : 0x00 (not present)
// D27..26: Underflow       : 0x00 (not present)
// D29..28: Overflow        : 0x00 (not present)
// D31..30: reserved        : 0x00 (not present)
//bmaControls[2]           : 0x0C, 0x00, 0x00, 0x00
// D1..0  : Mute            : 0x00 (not present)
// D3..2  : Volume          : 0x03 (host programmable)
// D5..4  : Bass            : 0x00 (not present)
// D7..6  : Mid             : 0x00 (not present)
// D9..8  : Treble          : 0x00 (not present)
// D11..10: Graph Equalizer : 0x00 (not present)
// D13..12: Automatic Gain  : 0x00 (not present)
// D15..14: Delay           : 0x00 (not present)
// D17..16: Bass Boost      : 0x00 (not present)
// D19..18: Loudness        : 0x00 (not present)
// D21..20: Input Gain      : 0x00 (not present)
// D23..22: Input Gain Pad  : 0x00 (not present)
// D25..24: Phase Inverter  : 0x00 (not present)
// D27..26: Underflow       : 0x00 (not present)
// D29..28: Overflow        : 0x00 (not present)
// D31..30: reserved        : 0x00 (not present)
//iFeature                 : 0x00 (No String Descriptor)
//Data (HexDump)           : 12 24 06 03 01 0F 00 00 00 0C 00 00 00 0C 00 00   .$..............
//                           00 00                                             ..
//
//        ----- Audio Control Output Terminal Descriptor 2.0 ----
//bLength                  : 0x0C (12 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x03 (Output Terminal 2.0)
//bTerminalID              : 0x04
//wTerminalType            : 0x0301 (Speaker)
//bAssocTerminal           : 0x00 (0)
//bSourceID                : 0x03 (3)
//bCSourceID               : 0x05 (5)
//bmControls               : 0x0004
// D1..0  : Copy Protect   : 0x00 (not present)
// D3..2  : Connector      : 0x01 (read only)
// D5..4  : Overload       : 0x00 (not present)
// D7..6  : Cluster        : 0x00 (not present)
// D9..8  : Underflow      : 0x00 (not present)
// D11..10: Overflow       : 0x00 (not present)
// D15..12: Reserved       : 0x00
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 0C 24 03 04 01 03 00 03 05 04 00 00               .$..........
//
//        --- Audio Control Clock Source Unit Descriptor 2.0 ----
//bLength                  : 0x08 (8 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x0A (Clock Source 2.0)
//bClockID                 : 0x0F
//bmAttributes             : 0x03
// D1..0: Clock Type       : 0x03
// D2   : Sync to SOF      : 0x00
// D7..3: Reserved         : 0x00
//bmControls               : 0x07
// D1..0: Clock Frequency  : 0x03 (host programmable)
// D3..2: Clock Validity   : 0x01 (read only)
// D7..4: Reserved         : 0x00
//bAssocTerminal           : 0x00
//iClockSource             : 0x00 (No String Descriptor)
//Data (HexDump)           : 08 24 0A 0F 03 07 00 00                           .$......
//
//        ----- Audio Control Input Terminal Descriptor 2.0 -----
//bLength                  : 0x11 (17 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Input Terminal 2.0)
//bTerminalID              : 0x0A
//wTerminalType            : 0x0201 (Microphone)
//bAssocTerminal           : 0x00
//bCSourceID               : 0x0F (15)
//bNrChannels              : 0x02 (2 Channels)
//bmChannelConfig          : 0x00000003 (FL, FR)
//iChannelNames            : 0x00 (No String Descriptor)
//bmControls               : 0x0004
// D1..0  : Copy Protect   : 0x00 (not present)
// D3..2  : Connector      : 0x01 (Ready only)
// D5..4  : Overload       : 0x00 (not present)
// D7..6  : Cluster        : 0x00 (not present)
// D9..8  : Underflow      : 0x00 (not present)
// D11..10: Overflow       : 0x00 (not present)
// D15..12: Reserved       : 0x00
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 11 24 02 0A 01 02 00 0F 02 03 00 00 00 00 04 00   .$..............
//                           00                                                .
//
//        ----- Audio Control Output Terminal Descriptor 2.0 ----
//bLength                  : 0x0C (12 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x03 (Output Terminal 2.0)
//bTerminalID              : 0x0D
//wTerminalType            : 0x0101 (USB streaming)
//bAssocTerminal           : 0x00 (0)
//bSourceID                : 0x0A (10)
//bCSourceID               : 0x0F (15)
//iTerminal                : 0x00 (No String Descriptor)
//Data (HexDump)           : 0C 24 03 0D 01 01 00 0A 0F 00 00 00               .$..........
//
#if (UAC2_PLAY0_ENABLE)
//        --- Audio Control Clock Source Unit Descriptor 2.0 ----
0x08, 0x24, 0x0A, UAC2_PCS_ID, 0x03, 0x07, 0x00, 0x00, 
//        ----- Audio Control Input Terminal Descriptor 2.0 -----
0x11, 0x24, 0x02, UAC2_PIT_ID, 0x01, 0x01, 0x00, UAC2_PCS_ID, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//        ------ Audio Control Feature Unit Descriptor 2.0 ------
0x12, 0x24, 0x06, UAC2_PFU_ID, UAC2_PFU_SRC, 0x03, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,                          
//        ----- Audio Control Output Terminal Descriptor 2.0 ----
//0x0C, 0x24, 0x03, UAC2_POT_ID, 0x01, 0x03, 0x00, UAC2_POT_SRC, UAC2_PCS_ID, UAC2_PLAY0_BMCONTROL, 0x00, 0x00,
//0x0C, 0x24, 0x03, UAC2_POT_ID, 0x01, 0x03, 0x01, UAC2_POT_SRC, UAC2_PCS_ID, UAC2_PLAY0_BMCONTROL, 0x00, 0x00,
0x0C, 0x24, 0x03, UAC2_POT_ID, LE_ARRAY(PLAYBACK_OT0), 0x01, UAC2_POT_SRC, UAC2_PCS_ID, UAC2_PLAY0_BMCONTROL, 0x00, 0x00,
#endif
#if (UAC2_FEEDBACK_MIXER)
//        ------- Audio Control Mixer Unit Descriptor 2.0 -------
//bLength                  : 0x10 (16 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x04 (Mixer Unit 2.0)
//bUnitID                  : 0x24 (36)
//bNrInPins                : 0x02 (2)
//baSourceID[1]            : 0x04
//baSourceID[2]            : 0x23
//bNrChannels              : 0x02 (2 channels)
//bmChannelConfig          : 0x00000003 (FL, FR)
//iChannelNames            : 0x00 (No String Descriptor)
//bmMixerControls (HexDump): 00 00                                             ..
//bmControls               : 0x00
// D1..0: Cluster          : 0x00 (not present)
// D3..2: Underflow        : 0x00 (not present)
// D5..4: Overflow         : 0x00 (not present)
// D7..6: Reserved         : 0x0
//iMixer                   : 0x10 (String Descriptor 16)
// Language 0x0409         : "RTK Usb Audio"
//Data (HexDump)           : 10 24 04 24 02 04 23 02 03 00 00 00 00 00 00 00   .$.$..#.........
0x10, 0x24, 0x04, UAC2_PMX_ID, 0x02, UAC2_PMX_SRC0, UAC2_PMX_SRC1, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
//        ------ Audio Control Feature Unit Descriptor 2.0 ------
0x12, 0x24, 0x06, UAC2_FFU_ID, UAC2_FFU_SRC, 0x03, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,                          
//        ------ Audio Control Feature Unit Descriptor 2.0 ------
0x12, 0x24, 0x06, UAC2_SFU_ID, UAC2_SFU_SRC, 0x03, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,                          
#endif
#if (UAC2_REC0_ENABLE)
//        --- Audio Control Clock Source Unit Descriptor 2.0 ----
0x08, 0x24, 0x0A, UAC2_RCS_ID, 0x03, 0x07, 0x00, 0x00,
//        ----- Audio Control Input Terminal Descriptor 2.0 -----
//0x11, 0x24, 0x02, UAC2_RIT0_ID, 0x01, 0x02, 0x00, UAC2_RCS_ID, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, UAC2_REC0_BMCONTROL, 0x00, 0x00, 
0x11, 0x24, 0x02, UAC2_RIT0_ID, LE_ARRAY(RECORD_IT0), 0x00, UAC2_RCS_ID, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, UAC2_REC0_BMCONTROL, 0x00, 0x00, 
#if (UAC2_REC0_FU_ENABLE)
//        ------ Audio Control Feature Unit Descriptor 2.0 ------
//0x12, 0x24, 0x06, UAC2_RFU_ID, UAC2_RFU_SRC, 0x0F, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,  
0x12, 0x24, 0x06, UAC2_RFU_ID, UAC2_RFU_SRC, 0x03, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00,  
#endif                        
//        ----- Audio Control Output Terminal Descriptor 2.0 ----
//0x0C, 0x24, 0x03, UAC2_ROT_ID, 0x01, 0x01, 0x00, UAC2_ROT_SRC, UAC2_RCS_ID, 0x00, 0x00, 0x00,
0x0C, 0x24, 0x03, UAC2_ROT_ID, 0x01, 0x01, 0x08, UAC2_ROT_SRC, UAC2_RCS_ID, 0x00, 0x00, 0x00,
#endif
#if (UAC2_JACK_DETECTION)
//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x89 (Direction=IN EndpointID=9)
//bmAttributes             : 0x03 (TransferType=Interrupt)
//wMaxPacketSize           : 0x0010
// Bits 15..13             : 0x00 (reserved, must be zero)
// Bits 12..11             : 0x00 (0 additional transactions per microframe -> allows 1..1024 bytes per packet)
// Bits 10..0              : 0x10 (16 bytes per packet)
//bInterval                : 0x08 (8 ms)
0x07, 0x05, 0x89, 0x03, 0x10, 0x00, 0x08,  
#endif 
};
//
#if(UAC2_PLAY0_ENABLE)
// Configuration Descriptor index 4 (Playback)
code BYTE uac2_audio_streaming_if1[]={
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x01
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x00 (Default Control Pipe only)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x02 (Audio Streaming)
//bInterfaceProtocol       : 0x20 (Device Protocol Version 2.0)
//iInterface               : 0x04 (String Descriptor 4)
// Language 0x0409         : "Primary Play Interface"
//Data (HexDump)           : 09 04 01 00 00 01 02 20 04                        ....... .
0x09, 0x04, IF_PLAYBACK, 0x00, 0x00, 0x01, 0x02, 0x20, 0x04,
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x01
//bAlternateSetting        : 0x01
//bNumEndpoints            : 0x02 (2 Endpoints)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x02 (Audio Streaming)
//bInterfaceProtocol       : 0x20 (Device Protocol Version 2.0)
//iInterface               : 0x00 (No String Descriptor)
//Data (HexDump)           : 09 04 01 01 02 01 02 20 00                        ....... .
//
//        ------ Audio Streaming Interface Descriptor 2.0 -------
//bLength                  : 0x10 (16 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x01 (AS General)
//bTerminalLink            : 0x01 (1)
//bmControls               : 0x05
// D1..0: Active Alt Settng: 0x01 (read only)
// D3..2: Valid Alt Settng : 0x01 (read only)
// D7..4: Reserved         : 0x00
//bFormatType              : 0x01 (FORMAT_TYPE_I)
//bmFormats                : 0x00000001 (PCM)
//bNrChannels              : 0x02 (2 channels)
//bmChannelConfig          : 0x00000003 (FL, FR)
//iChannelNames            : 0x00 (No String Descriptor)
//Data (HexDump)           : 10 24 01 01 05 01 01 00 00 00 02 03 00 00 00 00   .$..............
//
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
//bLength                  : 0x06 (6 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Format Type)
//bFormatType              : 0x01 (FORMAT_TYPE_I)
//bSubslotSize             : 0x02 (2 bytes)
//bBitResolution           : 0x10 (16 bits)
//Data (HexDump)           : 06 24 02 01 02 10                                 .$....
//
//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x01 (Direction=OUT EndpointID=1)
//bmAttributes             : 0x05 (TransferType=Isochronous  SyncType=Asynchronous  EndpointType=Data)
//wMaxPacketSize           : 0x0308
// Bits 15..13             : 0x00 (reserved, must be zero)
// Bits 12..11             : 0x00 (0 additional transactions per microframe -> allows 1..1024 bytes per packet)
// Bits 10..0              : 0x308 (776 bytes per packet)
//bInterval                : 0x01 (1 ms)
//Data (HexDump)           : 07 05 01 05 08 03 01                              .......
//
//        ----------- Audio Data Endpoint Descriptor ------------
//bLength                  : 0x08 (8 bytes)
//bDescriptorType          : 0x25 (Audio Endpoint Descriptor)
//bDescriptorSubtype       : 0x01 (General)
//bmAttributes             : 0x00
//bLockDelayUnits          : 0x00
//wLockDelay               : 0x0000
//Data (HexDump)           : 08 25 01 00 00 00 00 00                           .%......
//
//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x81 (Direction=IN EndpointID=1)
//bmAttributes             : 0x11 (TransferType=Isochronous  SyncType=None  EndpointType=Feedback)
//wMaxPacketSize           : 0x0004
// Bits 15..13             : 0x00 (reserved, must be zero)
// Bits 12..11             : 0x00 (0 additional transactions per microframe -> allows 1..1024 bytes per packet)
// Bits 10..0              : 0x04 (4 bytes per packet)
//bInterval                : 0x04 (4 ms)
//Data (HexDump)           : 07 05 81 11 04 00 04                              .......
//
#if(PLAYBACK_16B_ENABLE)
//        ---------------- Interface Descriptor -----------------
0x09, 0x04, IF_PLAYBACK, ALT_PLAYBACK_16B, (1+UAC2_PLAY0_FEEDBACK), 0x01, 0x02, 0x20, 0x00,
//        ------ Audio Streaming Interface Descriptor 2.0 -------
0x10, 0x24, 0x01, 0x01, 0x05, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
0x06, 0x24, 0x02, 0x01, 2, 16,
//        ----------------- Endpoint Descriptor -----------------
0x07, 0x05, 0x01, UAC2_PLAY0_SYNC_TYPE, 0x08, 0x03, 0x01,
//        ----------- Audio Data Endpoint Descriptor ------------
0x08, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
//        ----------------- Endpoint Descriptor -----------------
#if(UAC2_PLAY0_FEEDBACK)
0x07, 0x05, 0x81, 0x11, 0x04, 0x00, 0x04,
#endif
#endif //PLAYBACK_16B_ENABLE
#if(PLAYBACK_24B_ENABLE)
//        ---------------- Interface Descriptor -----------------
0x09, 0x04, IF_PLAYBACK, ALT_PLAYBACK_24B, (1+UAC2_PLAY0_FEEDBACK), 0x01, 0x02, 0x20, 0x00,
//        ------ Audio Streaming Interface Descriptor 2.0 -------
0x10, 0x24, 0x01, 0x01, 0x05, 1, 0x01, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
0x06, 0x24, 0x02, 0x01, 3, 24,
//        ----------------- Endpoint Descriptor -----------------
0x07, 0x05, 0x01, UAC2_PLAY0_SYNC_TYPE, 0x08, 0x03, 0x01,
//        ----------- Audio Data Endpoint Descriptor ------------
0x08, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
//        ----------------- Endpoint Descriptor -----------------
#if(UAC2_PLAY0_FEEDBACK)
0x07, 0x05, 0x81, 0x11, 0x04, 0x00, 0x04,
#endif
#endif //PLAYBACK_24B_ENABLE

#if(PLAYBACK_32B_ENABLE)
//        ---------------- Interface Descriptor -----------------
0x09, 0x04, IF_PLAYBACK, ALT_PLAYBACK_32B, (1+UAC2_PLAY0_FEEDBACK), 0x01, 0x02, 0x20, 0x00,
//        ------ Audio Streaming Interface Descriptor 2.0 -------
0x10, 0x24, 0x01, 0x01, 0x05, 1, 0x01, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
0x06, 0x24, 0x02, 0x01, 4, 32,
//        ----------------- Endpoint Descriptor -----------------
0x07, 0x05, 0x01, UAC2_PLAY0_SYNC_TYPE, 0x08, 0x03, 0x01,
//        ----------- Audio Data Endpoint Descriptor ------------
0x08, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
//        ----------------- Endpoint Descriptor -----------------
#if(UAC2_PLAY0_FEEDBACK)
0x07, 0x05, 0x81, 0x11, 0x04, 0x00, 0x04,
#endif
#endif  //PLAYBACK_32B_ENABLE 

#if(PLAYBACK_NDSD_ENABLE)
//        ---------------- Interface Descriptor -----------------
0x09, 0x04, IF_PLAYBACK, ALT_PLAYBACK_NDSD, (1+UAC2_PLAY0_FEEDBACK), 0x01, 0x02, 0x20, 0x00,
//        ------ Audio Streaming Interface Descriptor 2.0 -------
0x10, 0x24, 0x01, 0x01, 0x05, 1, 0x00, 0x00, 0x00, 0x80, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, //6-9: 0x80000000 for NDSD
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
0x06, 0x24, 0x02, 0x01, 4, 32,
//        ----------------- Endpoint Descriptor -----------------
0x07, 0x05, 0x01, UAC2_PLAY0_SYNC_TYPE, 0x08, 0x03, 0x01,
//        ----------- Audio Data Endpoint Descriptor ------------
0x08, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
//        ----------------- Endpoint Descriptor -----------------
#if(UAC2_PLAY0_FEEDBACK)
0x07, 0x05, 0x81, 0x11, 0x04, 0x00, 0x04,
#endif
#endif  //PLAYBACK_32B_ENABLE 

#if(PLAYBACK_DTS_ENABLE)
//https://docs.microsoft.com/en-us/windows-hardware/drivers/audio/usb-2-0-audio-drivers
/*
Type III formats (FMT-2 2.3.3 and A.2.3):

IEC61937_AC-3                   //D0
IEC61937_MPEG-2_AAC_ADTS        //D4
IEC61937_DTS-I                  //D7
IEC61937_DTS-II                 //D8
IEC61937_DTS-III                //D9
TYPE_III_WMA                    //D12
*/
//        ---------------- Interface Descriptor -----------------
0x09, 0x04, IF_PLAYBACK, ALT_PLAYBACK_DTS, (1+UAC2_PLAY0_FEEDBACK), 0x01, 0x02, 0x20, 0x00,
//        ------ Audio Streaming Interface Descriptor 2.0 -------
0x10, 0x24, 0x01, 0x01, 0x05, 3, 0x81, 0x13, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00, //6-9: 0x00001381 for Driver Support
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
0x06, 0x24, 0x02, 0x01, 4, 32,
//        ----------------- Endpoint Descriptor -----------------
0x07, 0x05, 0x01, UAC2_PLAY0_SYNC_TYPE, 0x08, 0x03, 0x01,
//        ----------- Audio Data Endpoint Descriptor ------------
0x08, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
//        ----------------- Endpoint Descriptor -----------------
#if(UAC2_PLAY0_FEEDBACK)
0x07, 0x05, 0x81, 0x11, 0x04, 0x00, 0x04,
#endif
#endif  //PLAYBACK_32B_ENABLE 
};
#endif  // UAC2_PLAY0_ENABLE

#if (UAC2_REC0_ENABLE)
code BYTE uac2_audio_streaming_if2[]={
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x02
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x00 (Default Control Pipe only)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x02 (Audio Streaming)
//bInterfaceProtocol       : 0x20 (Device Protocol Version 2.0)
//iInterface               : 0x05 (String Descriptor 5)
// Language 0x0409         : "Record Interface"
//Data (HexDump)           : 09 04 02 00 00 01 02 20 05                        ....... .
0x09, 0x04, IF_RECORD, 0x00, 0x00, 0x01, 0x02, 0x20, 0x05,
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x02
//bAlternateSetting        : 0x01
//bNumEndpoints            : 0x01 (1 Endpoint)
//bInterfaceClass          : 0x01 (Audio)
//bInterfaceSubClass       : 0x02 (Audio Streaming)
//bInterfaceProtocol       : 0x20 (Device Protocol Version 2.0)
//iInterface               : 0x00 (No String Descriptor)
//Data (HexDump)           : 09 04 02 01 01 01 02 20 00                        ....... .
//
//        ------ Audio Streaming Interface Descriptor 2.0 -------
//bLength                  : 0x10 (16 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x01 (AS General)
//bTerminalLink            : 0x0D (13)
//bmControls               : 0x05
// D1..0: Active Alt Settng: 0x01 (read only)
// D3..2: Valid Alt Settng : 0x01 (read only)
// D7..4: Reserved         : 0x00
//bFormatType              : 0x01 (FORMAT_TYPE_I)
//bmFormats                : 0x00000001 (PCM)
//bNrChannels              : 0x02 (2 channels)
//bmChannelConfig          : 0x00000003 (FL, FR)
//iChannelNames            : 0x00 (No String Descriptor)
//Data (HexDump)           : 10 24 01 0D 05 01 01 00 00 00 02 03 00 00 00 00   .$..............
//
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
//bLength                  : 0x06 (6 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x02 (Format Type)
//bFormatType              : 0x01 (FORMAT_TYPE_I)
//bSubslotSize             : 0x02 (2 bytes)
//bBitResolution           : 0x10 (16 bits)
//Data (HexDump)           : 06 24 02 01 02 10                                 .$....
//
//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x82 (Direction=IN EndpointID=2)
//bmAttributes             : 0x05 (TransferType=Isochronous  SyncType=Asynchronous  EndpointType=Data)
//wMaxPacketSize           : 0x0308
// Bits 15..13             : 0x00 (reserved, must be zero)
// Bits 12..11             : 0x00 (0 additional transactions per microframe -> allows 1..1024 bytes per packet)
// Bits 10..0              : 0x308 (776 bytes per packet)
//bInterval                : 0x01 (1 ms)
//Data (HexDump)           : 07 05 82 05 08 03 01                              .......
//
//        ----------- Audio Data Endpoint Descriptor ------------
//bLength                  : 0x08 (8 bytes)
//bDescriptorType          : 0x25 (Audio Endpoint Descriptor)
//bDescriptorSubtype       : 0x01 (General)
//bmAttributes             : 0x00
//bLockDelayUnits          : 0x00
//wLockDelay               : 0x0000
//Data (HexDump)           : 08 25 01 00 00 00 00 00                           .%......
//
#if(RECORD_16B_ENABLE)
//        ---------------- Interface Descriptor -----------------
0x09, 0x04, IF_RECORD, ALT_RECORD_16B, 0x01, 0x01, 0x02, 0x20, 0x00,
//        ------ Audio Streaming Interface Descriptor 2.0 -------
0x10, 0x24, 0x01, 0x0D, 0x05, 0x01, 0x01, 0x00, 0x00, 0x00, ALT_RECORD_16B_CH, ALT_RECORD_16B_CH_CONFIG, 0x00,
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
0x06, 0x24, 0x02, 0x01, 2, 16,
//        ----------------- Endpoint Descriptor -----------------
0x07, 0x05, 0x82, 0x05, 0x08, 0x03, 0x01,
//        ----------- Audio Data Endpoint Descriptor ------------
0x08, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif //RECORD_16B_ENABLE
#if(RECORD_24B_ENABLE)
//        ---------------- Interface Descriptor -----------------
0x09, 0x04, IF_RECORD, ALT_RECORD_24B, 0x01, 0x01, 0x02, 0x20, 0x00,
//        ------ Audio Streaming Interface Descriptor 2.0 -------
0x10, 0x24, 0x01, 0x0D, 0x05, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
0x06, 0x24, 0x02, 0x01, 3, 24,
//        ----------------- Endpoint Descriptor -----------------
0x07, 0x05, 0x82, 0x05, 0x08, 0x03, 0x01,
//        ----------- Audio Data Endpoint Descriptor ------------
0x08, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif //RECORD_24B_ENABLE
#if(RECORD_32B_ENABLE)
//        ---------------- Interface Descriptor -----------------
0x09, 0x04, IF_RECORD, ALT_RECORD_32B, 0x01, 0x01, 0x02, 0x20, 0x00,
//        ------ Audio Streaming Interface Descriptor 2.0 -------
0x10, 0x24, 0x01, 0x0D, 0x05, 0x01, 0x01, 0x00, 0x00, 0x00, 0x02, 0x03, 0x00, 0x00, 0x00, 0x00,
//        ----- Audio Streaming Format Type Descriptor 2.0 ------
0x06, 0x24, 0x02, 0x01, 4, 32,
//        ----------------- Endpoint Descriptor -----------------
0x07, 0x05, 0x82, 0x05, 0x08, 0x03, 0x01,
//        ----------- Audio Data Endpoint Descriptor ------------
0x08, 0x25, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00,
#endif  //RECORD_32B_ENABLE
};
#endif  // UAC2_PLAY0_ENABLE
// Configuration Descriptor index 5
code BYTE uac2_hid_interface[]={
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x03
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x01 (1 Endpoint)
//bInterfaceClass          : 0x03 (HID - Human Interface Device)
//bInterfaceSubClass       : 0x00 (None)
//bInterfaceProtocol       : 0x00 (None)
//iInterface               : 0x07 (String Descriptor 7)
// Language 0x0409         : "HID Interface"
//Data (HexDump)           :
     0x09,0x04,IF_HID,0x00,0x01,0x03,0x00,0x00,0x06,
//
//        ------------------- HID Descriptor --------------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x21 (HID Descriptor)
//bcdHID                   : 0x0111 (HID Version 1.11)
//bCountryCode             : 0x00 (00 = not localized)
//bNumDescriptors          : 0x01
//Data (HexDump)           :
    0x09,0x21,0x11,0x01,0x00,0x01,0x22,CT_UAC2_HID_REPORT_DESC_LEN,0x00,
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
//wMaxPacketSize           : 0x0004
// Bits 15..13             : 0x00 (reserved, must be zero)
// Bits 12..11             : 0x00 (0 additional transactions per microframe -> allows 1..1024 bytes per packet)
// Bits 10..0              : 0x04 (4 bytes per packet)
//bInterval                : 0x10 (16 ms) 0-16 is acceptable
//Data (HexDump)           :
    0x07,0x05,0x86,0x03,0x04,0x00,0x10,
};

#if (UAC2_BULK_ENABLE == 1)
// Configuration Descriptor index 5
code BYTE uac2_bulk_interface[]={
//        ---------------- Interface Descriptor -----------------
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x04 (Interface Descriptor)
//bInterfaceNumber         : 0x00
//bAlternateSetting        : 0x00
//bNumEndpoints            : 0x03 (3 Endpoints)
//bInterfaceClass          : 0xFF (Vendor Specific)
//bInterfaceSubClass       : 0xFF
//bInterfaceProtocol       : 0xFF
//iInterface               : 0x00 (No String Descriptor)
//Data (HexDump)           :
    0x09,0x04,UAC2_BULK_IF_INDEX,0x00,0x02,0xFF,0xF0,0x00,0x07,

//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x01 (Direction=OUT EndpointID=1)
//bmAttributes             : 0x02 (TransferType=Bulk)
//wMaxPacketSize           : 0x0040 (max 64 bytes)
//bInterval                : 0x00 (never NAKs)
//Data (HexDump)           : 07 05 01 02 40 00 00                              ....@..
    0x07,0x05,0x07,0x02,0x00,0x02,0x01,

//        ----------------- Endpoint Descriptor -----------------
//bLength                  : 0x07 (7 bytes)
//bDescriptorType          : 0x05 (Endpoint Descriptor)
//bEndpointAddress         : 0x81 (Direction=IN EndpointID=1)
//bmAttributes             : 0x02 (TransferType=Bulk)
//wMaxPacketSize           : 0x0040 (max 64 bytes)
//bInterval                : 0x00 (never NAKs)
//Data (HexDump)           : 07 05 81 02 40 00 00                              ....@..
    0x07,0x05,0x88,0x02,0x00,0x02,0x01,
};
#endif  //BULK

#if(UAC2_HID_ENABLE)
    #define     UAC2_HID_INTERFACE_LEN      (sizeof(uac2_hid_interface))
#else                                       
   #define     UAC2_HID_INTERFACE_LEN       0
#endif

#define     UAC2_HID_DESC_LEN               9

#if(UAC2_BULK_ENABLE)
    #define     UAC2_BULK_INTERFACE_LEN     (sizeof(uac2_bulk_interface))
#else
    #define     UAC2_BULK_INTERFACE_LEN     0
#endif

#define     UAC2_AUDIO_CONTROL_HEADER_LEN   (9)
#define     UAC2_AUDIO_CONTROL_LEN          (sizeof(uac2_audio_control))

#if(UAC2_PLAY0_ENABLE)
#define     UAC2_AUDIO_STREAMING1_LEN       (sizeof(uac2_audio_streaming_if1))
#else
#define     UAC2_AUDIO_STREAMING1_LEN       (0)
#endif

#if(UAC2_REC0_ENABLE)
#define     UAC2_AUDIO_STREAMING2_LEN       (sizeof(uac2_audio_streaming_if2))
#else
#define     UAC2_AUDIO_STREAMING2_LEN       (0)
#endif

//#define     UAC2_AUDIO_STREAMING2_LEN       (UAC2_REC0_ENABLE)?(sizeof(uac2_audio_streaming_if2)):0

#define     UAC2_AUDIO_CONTROL_TOTAL_LEN    (UAC2_AUDIO_CONTROL_HEADER_LEN+UAC2_AUDIO_CONTROL_LEN)
#define     UAC2_AUDIO_STREAMING_LEN        (UAC2_AUDIO_STREAMING1_LEN+UAC2_AUDIO_STREAMING2_LEN)
#define     UAC2_CONFIG_DESC_LEN            (9+8+9+UAC2_AUDIO_CONTROL_TOTAL_LEN+UAC2_AUDIO_STREAMING_LEN+UAC2_HID_INTERFACE_LEN+UAC2_BULK_INTERFACE_LEN)


// Audio Control Interface Header Descriptor 2.0 (3)
code BYTE uac2_audio_control_header[]={
//bLength                  : 0x09 (9 bytes)
//bDescriptorType          : 0x24 (Audio Interface Descriptor)
//bDescriptorSubtype       : 0x01 (Header 2.0)
//bcdADC                   : 0x0200 (2.0)
//bCategory                : 0x04 (headset)
//wTotalLength             : 0x0065 (101 bytes)
//bmControls               : 0x00
// D1..0: Latency          : 0x00 (not present)
// D7..2: Reserved         : 0x00
//Data (HexDump)           : 09 24 01 00 02 04 65 00
    0x09,0x24,0x01,0x00,0x02,0x04,LE_ARRAY(UAC2_AUDIO_CONTROL_TOTAL_LEN),0x00,
};

// Configuration Descriptor index 0
code BYTE uac2_config_header[]={
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
    0x09,0x02,LE_ARRAY(UAC2_CONFIG_DESC_LEN),UAC2_INTERFACE_NUM,0x01,0x00,POWER_CONFIG,
};

code DESCRIPTOR_INFO config_desc_table[] =
{
    { 9,                                uac2_config_header },           //Configuration Descriptor
    { 8,                                uac2_iad_descriptor },          //IAD Descriptor
    { 9,                                uac2_audio_control_if0 },       //Interface Descriptor
    { UAC2_AUDIO_CONTROL_HEADER_LEN,    uac2_audio_control_header },
    { UAC2_AUDIO_CONTROL_LEN,           uac2_audio_control },
#if (UAC2_PLAY0_ENABLE)
    { UAC2_AUDIO_STREAMING1_LEN,   	    uac2_audio_streaming_if1 },
#endif
#if (UAC2_REC0_ENABLE)
    { UAC2_AUDIO_STREAMING2_LEN,   	    uac2_audio_streaming_if2 },
#endif
#if (UAC2_HID_ENABLE)
    { UAC2_HID_INTERFACE_LEN,           uac2_hid_interface },
#endif
#if (UAC2_BULK_ENABLE)
    { UAC2_BULK_INTERFACE_LEN,          uac2_bulk_interface },
#endif
};

#define UAC2_CONFIG_MAX_COUNT           (sizeof(config_desc_table)/sizeof(config_desc_table[0]))


#endif //_CTUSB_DESCRIPTOR_C_
#endif //_CTUSB_UAC2_DESCRIPTOR_H_
