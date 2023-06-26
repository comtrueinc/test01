#ifndef _CTUSB_DESCRIPTOR_H_
#define _CTUSB_DESCRIPTOR_H_


//#include "Rammap.h"
#include "settings.h" 
//#include "Uac1_test.h" 
#ifdef	_CTUSB_DESCRIPTOR_C_
    WORD    desc_curr_addr;	
#else
    extern WORD    desc_curr_addr;	
#endif 

void DescriptDevice(void);
void DescriptDevQualify(void);
void DescriptHidReport(void);
void DescriptStrings(BYTE index);
void DescriptStringTable(void);
void DescriptConfiguration();
#endif  //_CTUSB_DESCRIPTOR_H_
