#include "api_config.h"

#define TIMEROUT_VAL  (300)

u32 getHX710Count(u8 iNo)
{
    u32 iCount = 0;
    u8 i = 0;
    u8 iCounter = 0;
    
    if((iNo != HX710A1) && (iNo != HX710A2))
       return iCount;

    if(iNo == HX710A1) SLV_HX710A1_SDA = 1;
    else SLV_HX710A2_SDA = 1;
    
    if(iNo == HX710A1) SLV_HX710A1_SCK = 0;
    else SLV_HX710A2_SCK = 0;

    if(iNo == HX710A1) {
        while(SLV_HX710A1_SDA) {
            delay_ms(1);
            if((iCounter++) > TIMEROUT_VAL) {
                break;
            }
        }
    } else {
        while(SLV_HX710A2_SDA) {
            delay_ms(1);
            if((iCounter++) > TIMEROUT_VAL) {
                break;
            }
        }
    };
    
    for( i = 0; i < 24; i++) {
        if(iNo == HX710A1) SLV_HX710A1_SCK = 1;
        else SLV_HX710A2_SCK = 1;
        
        iCount = (iCount << 1);
        
        if(iNo == HX710A1) SLV_HX710A1_SCK = 0;
        else SLV_HX710A2_SCK = 0;
        
        if(iNo == HX710A1) {if(SLV_HX710A1_SDA) iCount++;}
        else {if(SLV_HX710A2_SDA) iCount++;}
    }

    if(iNo == HX710A1) SLV_HX710A1_SCK = 1;
    else SLV_HX710A2_SCK = 1;
    
    iCount = iCount^0x800000;

    if(iNo == HX710A1) SLV_HX710A1_SCK = 0;
    else SLV_HX710A2_SCK = 0;
    
    return (iCount);
}

#if TEST_MODE
void hx710_Test(void)
{

}

#endif
