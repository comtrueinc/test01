//---------------------------------------------------------------------------
#ifndef UserSetting
#define UserSetting

//===============================================================
// User Description
//===============================================================

#define _disable                    0
#define _enable                     1

// --------------------------------------------------------------
// General Configuration
// --------------------------------------------------------------

//  VID & PID
#define Descriptor_VID              0x0EA0
#define Descriptor_PID10            0x6001          // for UAC1
#define Descriptor_PID20            0x6002          // for UAC2

// System Power
#define SelfPower                   1               // 1: self power, 0: usb bus power
#define MaxPower                    50              // Max power consumption when bus pwer (n*2 mA)


// Chip Package setting
#define CHIP_48PIN                  0
#define CHIP_80PIN                  1
#define CHIP_PACKAGE_TYPE           CHIP_80PIN

// Input / Output Port
#if (CHIP_PACKAGE_TYPE == CHIP_48PIN)
    #define PrimaryPlayPortEnable       _enable
    #define PrimaryPlayPortCH           Support_2CH
    
    #define SecondaryPlayPortEnable     _disable
    #define SecondaryPlayPortEnable_U1  _disable
    
    #define RecordPortEnable            _enable
    #define RecordPortCH                Support_2CH
#else //(CHIP_PACKAGE_TYPE == CHIP_80PIN)
    #define PrimaryPlayPortEnable       _enable
    #define PrimaryPlayPortCH           Support_2CH
    
    #define SecondaryPlayPortEnable     _disable
    #define SecondaryPlayPortEnable_U1  _disable
    
    #define RecordPortEnable            _enable
    #define RecordPortCH                Support_2CH
#endif

// Feedback endpoint support
#define SupportFBEFunction          _disable        // Feedback 

// USB slot size control setting
#define Fixed_SlotSize              _disable

// HID Support 
#define SupportHIDFunction          _enable

// Power Saving
#define SupportPowerSuspend         _disable     

// bit Resolution
#define BitResolution_16            _enable
#define BitResolution_24            _enable
#define BitResolution_32            _enable

// Support Timing
#define Timing_32K                  _disable
#define Timing_441K                 _enable
#define Timing_48K                  _enable

#define Timing_64K                  _disable
#define Timing_882K                 _enable
#define Timing_96K                  _enable

#define Timing_128K                 _disable
#define Timing_1764K                _enable
#define Timing_192K                 _enable

#define Timing_256K                 _disable
#define Timing_3528K                _enable
#define Timing_384K                 _enable

#define Timing_512K                 _disable
#define Timing_7056K                _enable
#define Timing_768K                 _enable

#define SPDIF_RecordPort            _disable

// --------------------------------------------------------------
// Configuration for UAC1.0
// --------------------------------------------------------------

// Support Frequency
#define Freq441_UAC1                _enable
#define Freq480_UAC1                _enable
#define Freq882_UAC1                _disable
#define Freq960_UAC1                _disable

// Bit resolution
#define SupportBitResolution_U1     16


// --------------------------------------------------------------
// Configuration for UAC2.0
// --------------------------------------------------------------

#define Support_2CH                 1               // stereo
#define Support_6CH                 2               // 5.1 channels
#define Support_8CH                 3               // 7.1 channels




//===============================================================
// Do not change any text below
//===============================================================


    // UTMI clock systemENDPOINT 
    // 0: control
    // 1: play0
    // 2: record0
    // 3: play0 feedback
    // 4: play1
    // 5: play 1 feedback
    // 6: bulk
    // 7: hid

// Interface
#define Interface_Control           1

#if PrimaryPlayPortEnable
#define Interface_Play0             1
#define Clock_Play0                 0x02        // bit 1
#define UTMI_CLK_Play0              0x02        // bit 1
    #if SupportFBEFunction
        #define UTMI_CLK_Play0_FB               0x08        // bit 3
    #else
        #define UTMI_CLK_Play0_FB               0x00
    #endif
#else
#define Interface_Play0             0
#define Clock_Play0                 0x00
#define UTMI_CLK_Play0              0x00
#define UTMI_CLK_Play0_FB           0x00
#endif
#if RecordPortEnable
#define Interface_Record            1
#define Clock_Record                0x01
#define UTMI_CLK_Record             0x04        // bit 2
#else
#define Interface_Record            0
#define Clock_Record                0x00
#define UTMI_CLK_Record             0x00
#endif
#if SecondaryPlayPortEnable
#define Interface_Play1             1
#define Clock_Play1                 0x04        // bit 2
#define UTMI_CLK_Play1              0x10        // bit 4
    #if SupportFBEFunction
        #define UTMI_CLK_Play1_FB               0x20        // bit 5
    #else
        #define UTMI_CLK_Play1_FB               0x00
    #endif
#else
#define Interface_Play1             0
#define Clock_Play1                 0x00
#define UTMI_CLK_Play1              0x00
#define UTMI_CLK_Play1_FB           0x00
#endif

#if SupportIAPFunction
#define Interface_iAP               1
#define Len_iAP                     (9+7+7)                             // interface + bulk in + bulk out (need interrupt??)
#define UTMI_CLK_Bulk               0x40        // bit 6
#else
#define Interface_iAP               0
#define Len_iAP                     0
#define UTMI_CLK_Bulk               0x00
#endif
#if SupportHIDFunction
#define Interface_HID               1
#define Len_HID                     (9+9+7)
#define UTMI_CLK_HID                0x80        // bit 7
#else
#define Interface_HID               0
#define Len_HID                     0
#define UTMI_CLK_HID                0x00
#endif

#define IFIndex_Control                 0
#define IFIndex_Play0                   (IFIndex_Control + Interface_Control)
#define IFIndex_Record                  (IFIndex_Play0   + Interface_Play0)
#define IFIndex_Play1                   (IFIndex_Record  + Interface_Record)
#define IFIndex_HID                     (IFIndex_Play1   + Interface_Play1)
#define IFIndex_IAP                     (IFIndex_HID     + Interface_HID)
#define INTERFACE_NUM                   (IFIndex_IAP     + Interface_iAP)
#define INTERFACE_NUM_U1                (Interface_Control+Interface_Play0+Interface_Record+Interface_HID+Interface_iAP)

// Resolution
#if BitResolution_16
#define Support_B16                 1
#else
#define Support_B16                 0
#endif

#if BitResolution_24
#define Support_B24                 1
#else
#define Support_B24                 0
#endif

#if BitResolution_32
#define Support_B32                 1
#else
#define Support_B32                 0
#endif


// Timing
#if Timing_32K
#define Support_32K                 0x0001
#else
#define Support_32K                 0x0000
#endif

#if Timing_441K
#define Support_441K                0x0002
#else
#define Support_441K                0x0000
#endif

#if Timing_48K
#define Support_48K                 0x0004
#else
#define Support_48K                 0x0000
#endif

#if Timing_64K
#define Support_64K                 0x0008
#else
#define Support_64K                 0x0000
#endif

#if Timing_882K
#define Support_882K                0x0010
#else
#define Support_882K                0x0000
#endif

#if Timing_96K
#define Support_96K                 0x0020
#else
#define Support_96K                 0x0000
#endif

#if Timing_128K
#define Support_128K                0x0040
#else
#define Support_128K                0x0000
#endif

#if Timing_1764K
#define Support_1764K               0x0080
#else
#define Support_1764K               0x0000
#endif

#if Timing_192K
#define Support_192K                0x0100
#else
#define Support_192K                0x0000
#endif


#if Timing_256K
#define Support_256K                0x0200
#else
#define Support_256K                0x0000
#endif

#if Timing_3528K
#define Support_3528K               0x0400
#else
#define Support_3528K               0x0000
#endif

#if Timing_384K
#define Support_384K                0x0800
#else
#define Support_384K                0x0000
#endif


#if Timing_512K
#define Support_512K                0x1000
#else
#define Support_512K                0x0000
#endif

#if Timing_7056K
#define Support_7056K               0x2000
#else
#define Support_7056K               0x0000
#endif

#if Timing_768K
#define Support_768K                0x4000
#else
#define Support_768K                0x0000
#endif

#define SupportTiming               ((Support_768K | Support_7056K | Support_512K | Support_384K | Support_3528K | Support_256K) | \
                                     (Support_192K | Support_1764K | Support_128K | Support_96K | Support_882K | Support_64K) | \
                                     (Support_48K | Support_441K | Support_32K))

#define TotResolutionNum            (Support_B16+Support_B24+Support_B32)

#define ALT_2CH_START               1
#define ALT_6CH_START               (1+TotResolutionNum)
#define ALT_8CH_START               (1+TotResolutionNum*2)

#define TOTAL_ALT_NUM_P0            (PrimaryPlayPortCH*TotResolutionNum)
#define TOTAL_ALT_NUM_P1            (TotResolutionNum)
#define TOTAL_ALT_NUM_REC           (RecordPortCH*TotResolutionNum)

#define         ALT_16B_OFFSET      0

#if (BitResolution_16) 
    #define     ALT_24B_OFFSET      1
#else 
    #define     ALT_24B_OFFSET      0
#endif

#if     (BitResolution_16 && BitResolution_24)
    #define     ALT_32B_OFFSET      2
#elif   (BitResolution_16 || BitResolution_24)
    #define     ALT_32B_OFFSET      1
#else
    #define     ALT_32B_OFFSET      0
#endif

#if(PrimaryPlayPortCH == Support_2CH)
    #define Play0ChannelNum         2               // 2 channel
    #define Play0ChDescCount        1               // 1 descriptor block at config
    #define Play0ChannelConfig      0x03
    #define PrimaryPlayStereo       _enable         // 2 channel for stereo
    #define PrimaryPlay5_1          _disable        // 5.1 channel
    #define PrimaryPlay7_1          _disable        // 7.1 channel
#elif(PrimaryPlayPortCH == Support_6CH)
    #define Play0ChannelNum         6
    #define Play0ChDescCount        2               // 2 descriptor block at config
    #define Play0ChannelConfig      0x3F
    #define PrimaryPlayStereo       _enable         // 2 channel for stereo
    #define PrimaryPlay5_1          _enable         // 5.1 channel
    #define PrimaryPlay7_1          _disable        // 7.1 channel
#else // if(PrimaryPlayPortCH == Support_8CH)
    #define Play0ChannelNum         8
    #define Play0ChDescCount        3               // 3 descriptor block at config
    #define Play0ChannelConfig      0x0FF
    #define PrimaryPlayStereo       _enable         // 2 channel for stereo
    #define PrimaryPlay5_1          _enable         // 5.1 channel
    #define PrimaryPlay7_1          _enable         // 7.1 channel
#endif

#if(RecordPortCH == Support_2CH)
    #define RecordChannelNum        2
    #define RecordChDescCount       1               // 1 descriptor block at config
    #define RecordChannelConfig     0x03
    #define RecordStereo            _enable         // 2 channel for stereo
    #define Record5_1               _disable        // 5.1 channel
    #define Record7_1               _disable        // 7.1 channel
#elif(RecordPortCH == Support_6CH)
    #define RecordChannelNum        6
    #define RecordChDescCount       2               // 2 descriptor block at config
    #define RecordChannelConfig     0x3F
    #define RecordStereo            _enable         // 2 channel for stereo
    #define Record5_1               _enable         // 5.1 channel
    #define Record7_1               _disable        // 7.1 channel
#else //  if(RecordPortCH == Support_8CH)
    #define RecordChannelNum        8
    #define RecordChDescCount       3               // 3 descriptor block at config
    #define RecordChannelConfig     0xFF
    #define RecordStereo            _enable         // 2 channel for stereo
    #define Record5_1               _enable         // 5.1 channel
    #define Record7_1               _enable         // 7.1 channel
#endif

#define Play1ChannelNum             2               // 2 channel
#define Play1ChDescCount            1               // 1 descriptor block at config

#define AudioClockSystem            (Clock_Play0 | Clock_Play1 | Clock_Record)
#define UtmiClockCtrl               (UTMI_CLK_HID | UTMI_CLK_Bulk | UTMI_CLK_Play1_FB | UTMI_CLK_Play1 | UTMI_CLK_Play0_FB | UTMI_CLK_Record | UTMI_CLK_Play0)



#endif  // UserSetting
