#include "api_config.h"

unsigned long getHX710Count(unsigned char iNo)
{
    unsigned long iCount = 0;
    unsigned char i;
    
    if((iNo != HX710A1) && (iNo != HX710A2))
       return iCount;

    if(iNo == HX710A1) SLV_HE710A1_SDA = 1;
    else SLV_HE710A2_SDA = 1;
    
    if(iNo == HX710A1) SLV_HX710A1_SCK = 0;
    else SLV_HX710A2_SCK = 0;
    
    if(iNo == HX710A1) while(SLV_HE710A1_SDA);
    else while(SLV_HE710A2_SDA);
    
    for( i = 0; i < 24; i++) {
        if(iNo == HX710A1) SLV_HX710A1_SCK = 1;
        else SLV_HX710A2_SCK = 1;
        
        iCount = (iCount << 1);
        
        if(iNo == HX710A1) SLV_HX710A1_SCK = 0;
        else SLV_HX710A2_SCK = 0;
        
        if(iNo == HX710A1) {if(SLV_HE710A1_SDA) iCount++;}
        else {if(SLV_HE710A2_SDA) iCount++;}
    }

    if(iNo == HX710A1) SLV_HX710A1_SCK = 1;
    else SLV_HX710A2_SCK = 1;
    
    iCount = iCount^0x800000;

    if(iNo == HX710A1) SLV_HX710A1_SCK = 0;
    else SLV_HX710A2_SCK = 0;
    
    return (iCount);
}