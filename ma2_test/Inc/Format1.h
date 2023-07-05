#ifndef _CTUSB_UAC1_FORMATE_H_
#define _CTUSB_UAC1_FORMATE_H_

#define     UAC1_FREQ_44K                       SAMPLE_RATE_44K
#define     UAC1_FREQ_48K                       SAMPLE_RATE_48K
#define     UAC1_FREQ_88K                       SAMPLE_RATE_88K
#define     UAC1_FREQ_96K                       SAMPLE_RATE_96K
#define     UAC1_FREQ_176K                      SAMPLE_RATE_176K
#define     UAC1_FREQ_192K                      SAMPLE_RATE_192K
//================================================================================================================

#define     UAC1_PLAY0_ALT_COUNT                2
#define     UAC1_REC0_ALT_COUNT                 2

#if (UAC_TYPE&TYPE_UAC1_FS)
#define     UAC1_PLAY0_ALT1_CH                  2 
#define     UAC1_PLAY0_ALT1_BITS                16 
#define     UAC1_PLAY0_ALT1_FREQ_MAX            96
#define     UAC1_PLAY0_ALT1_FREQ_NUM            4 
#define     UAC1_PLAY0_ALT1_FREQ_SETS           LE_ARRAY3(UAC1_FREQ_44K),LE_ARRAY3(UAC1_FREQ_48K),LE_ARRAY3(UAC1_FREQ_88K),LE_ARRAY3(UAC1_FREQ_96K) 

#define     UAC1_PLAY0_ALT2_CH                  2 
#define     UAC1_PLAY0_ALT2_BITS                24 
#define     UAC1_PLAY0_ALT2_FREQ_MAX            48           
#define     UAC1_PLAY0_ALT2_FREQ_NUM            2 
#define     UAC1_PLAY0_ALT2_FREQ_SETS           LE_ARRAY3(UAC1_FREQ_44K),LE_ARRAY3(UAC1_FREQ_48K)

#define     UAC1_REC0_ALT1_CH                   2 
#define     UAC1_REC0_ALT1_BITS                 16 
#define     UAC1_REC0_ALT1_MAX_FREQ             96
#define     UAC1_REC0_ALT1_FREQ_NUM             4 
#define     UAC1_REC0_ALT1_FREQ_SETS            LE_ARRAY3(UAC1_FREQ_44K),LE_ARRAY3(UAC1_FREQ_48K),LE_ARRAY3(UAC1_FREQ_88K),LE_ARRAY3(UAC1_FREQ_96K) 
                             
#define     UAC1_REC0_ALT2_CH                   2 
#define     UAC1_REC0_ALT2_BITS                 24 
#define     UAC1_REC0_ALT2_MAX_FREQ             48                             
#define     UAC1_REC0_ALT2_FREQ_NUM             2 
#define     UAC1_REC0_ALT2_FREQ_SETS            LE_ARRAY3(UAC1_FREQ_44K),LE_ARRAY3(UAC1_FREQ_48K) 

#else

#define     UAC1_PLAY0_ALT1_CH                  2 
#define     UAC1_PLAY0_ALT1_BITS                16 
#define     UAC1_PLAY0_ALT1_FREQ_MAX            192
#define     UAC1_PLAY0_ALT1_FREQ_NUM            4 
#define     UAC1_PLAY0_ALT1_FREQ_SETS           LE_ARRAY3(UAC1_FREQ_44K),LE_ARRAY3(UAC1_FREQ_48K),LE_ARRAY3(UAC1_FREQ_96K),LE_ARRAY3(UAC1_FREQ_192K) 

#define     UAC1_PLAY0_ALT2_CH                  2            
#define     UAC1_PLAY0_ALT2_BITS                24 
#define     UAC1_PLAY0_ALT2_FREQ_MAX            96           
#define     UAC1_PLAY0_ALT2_FREQ_NUM            4 
#define     UAC1_PLAY0_ALT2_FREQ_SETS           LE_ARRAY3(UAC1_FREQ_44K),LE_ARRAY3(UAC1_FREQ_48K),LE_ARRAY3(UAC1_FREQ_88K),LE_ARRAY3(UAC1_FREQ_96K) 


#define     UAC1_REC0_ALT1_CH                   2 
#define     UAC1_REC0_ALT1_BITS                 16 
#define     UAC1_REC0_ALT1_MAX_FREQ             192
#define     UAC1_REC0_ALT1_FREQ_NUM             4 
#define     UAC1_REC0_ALT1_FREQ_SETS            LE_ARRAY3(UAC1_FREQ_44K),LE_ARRAY3(UAC1_FREQ_48K),LE_ARRAY3(UAC1_FREQ_96K),LE_ARRAY3(UAC1_FREQ_192K) 
                             
#define     UAC1_REC0_ALT2_CH                   2 
#define     UAC1_REC0_ALT2_BITS                 24 
#define     UAC1_REC0_ALT2_MAX_FREQ             96                            
#define     UAC1_REC0_ALT2_FREQ_NUM             4 
#define     UAC1_REC0_ALT2_FREQ_SETS            LE_ARRAY3(UAC1_FREQ_44K),LE_ARRAY3(UAC1_FREQ_48K),LE_ARRAY3(UAC1_FREQ_88K),LE_ARRAY3(UAC1_FREQ_96K) 
#endif //(UAC_TYPE==TYPE_UAC1_FS)
//========================================================================================================================================

//#define     USB_TYPE                            ((UAC_TYPE==TYPE_UAC1_FS)?USB_LOW_SPEED:USB_HIGH_SPEED)                                   
#define     UAC1_FRAME_INTERVAL                 ((UAC_TYPE&TYPE_UAC1_HS)?4:1)                                   // 1/4  2^(n-1)*(1 or 0.125)ms

#define     UAC1_MULTI_TRANSACTION              ((UAC_TYPE&TYPE_UAC1_HS)?1:0) 

#define     UAC1_PLAY0_LEN_ALT1                 (8+UAC1_PLAY0_ALT1_FREQ_NUM*3)         
#define     UAC1_PLAY0_MAX_SIZE_ALT1            ((UAC1_PLAY0_ALT1_FREQ_MAX+1)*UAC1_PLAY0_ALT1_CH*UAC1_PLAY0_ALT1_BITS/8)            
#define     UAC1_PLAY0_FORMAT_ALT1              UAC1_PLAY0_ALT1_CH, UAC1_PLAY0_ALT1_BITS/8, UAC1_PLAY0_ALT1_BITS,UAC1_PLAY0_ALT1_FREQ_NUM, UAC1_PLAY0_ALT1_FREQ_SETS

#define     UAC1_PLAY0_LEN_ALT2                 (8+UAC1_PLAY0_ALT2_FREQ_NUM*3)         
#define     UAC1_PLAY0_FORMAT_ALT2              UAC1_PLAY0_ALT2_CH, UAC1_PLAY0_ALT2_BITS/8, UAC1_PLAY0_ALT2_BITS,UAC1_PLAY0_ALT2_FREQ_NUM, UAC1_PLAY0_ALT2_FREQ_SETS
#define     UAC1_PLAY0_MAX_SIZE_ALT2            (UAC1_MULTI_TRANSACTION)?0x0B0C:((UAC1_PLAY0_ALT2_FREQ_MAX+1)*UAC1_PLAY0_ALT2_CH*UAC1_PLAY0_ALT2_BITS/8)

#define     UAC1_REC0_LEN_ALT1                  (8+UAC1_REC0_ALT1_FREQ_NUM*3)         
#define     UAC1_REC0_MAX_SIZE_ALT1             (UAC1_REC0_ALT1_MAX_FREQ+1)*UAC1_REC0_ALT1_CH*UAC1_REC0_ALT1_BITS/8            
#define     UAC1_REC0_FORMAT_ALT1               UAC1_REC0_ALT1_CH,UAC1_REC0_ALT1_BITS/8,UAC1_REC0_ALT1_BITS,UAC1_REC0_ALT1_FREQ_NUM,UAC1_REC0_ALT1_FREQ_SETS  

#define     UAC1_REC0_LEN_ALT2                  (8+UAC1_REC0_ALT2_FREQ_NUM*3)         
#define     UAC1_REC0_FORMAT_ALT2               UAC1_REC0_ALT2_CH,UAC1_REC0_ALT2_BITS/8,UAC1_REC0_ALT2_BITS,UAC1_REC0_ALT2_FREQ_NUM,UAC1_REC0_ALT2_FREQ_SETS  
//#define     UAC1_REC0_MAX_SIZE_ALT2             (UAC1_MULTI_TRANSACTION)?0x0B0C:((UAC1_REC0_ALT2_MAX_FREQ+1)*UAC1_REC0_ALT2_CH*UAC1_REC0_ALT2_BITS/8) 
#define     UAC1_REC0_MAX_SIZE_ALT2             (UAC1_MULTI_TRANSACTION)?0x0B0A:((UAC1_REC0_ALT2_MAX_FREQ+1)*UAC1_REC0_ALT2_CH*UAC1_REC0_ALT2_BITS/8) 

#endif //_CTUSB_UAC1_FORMATE_H_