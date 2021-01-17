#include "api_config.h"

#define FLASH_TIME (10)  /* 10x10 ms 电平翻转 */

/* 00： 熄灭  01/10 闪烁 11：点亮 */
struct ledStatsManage {
    unsigned char msr_dy:2;//电源
    unsigned char msr_lp:2; //缺相
    unsigned char msr_ss:2; //上升
    unsigned char msr_cz:2; //超载
    unsigned char msr_xj:2; //下降
    unsigned char msr_qz:2; //欠载
    unsigned char msr_tb:2; //同步
    unsigned char msr_qx:2; //倾斜
    unsigned char msr_tx:2; //通讯

    unsigned char slv_zcg:2; //左超高
    unsigned char slv_lp:2; //缺相
    unsigned char slv_cz:2; //超载
    unsigned char slv_ss:2; //上升
    unsigned char slv_qz:2; //欠载
    unsigned char slv_xj:2; //下降
    unsigned char slv_sk:2; //受控
    unsigned char slv_tx:2; //通讯
    unsigned char slv_ycg:2; //右超高
    unsigned char slv_bj:2; //报警
} ;
static struct ledStatsManage ledCtrl = {0};


void MSR_LedStatusCtrl(unsigned char LedNo, bool StatusFlag)
{
    switch(LedNo) {
        case MSR_LED_INCLINE_INDICAT:    ledCtrl.msr_qx  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_OVER_LOADING:       ledCtrl.msr_cz  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_UNDER_LOADING:      ledCtrl.msr_qz  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_LOSS_PHASE:         ledCtrl.msr_lp  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_SYNCHRO_START:      ledCtrl.msr_tb  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_UP_INDICAT:         ledCtrl.msr_ss  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_DOWN_INDICAT:       ledCtrl.msr_xj  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_COMMUNICAT_INDICAT: ledCtrl.msr_tx  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_POWER_START:        ledCtrl.msr_dy  = StatusFlag? 0x0:0x3; break;
        case MSR_LED_ALL:
            ledCtrl.msr_qx  = StatusFlag? 0x0:0x3; 
            ledCtrl.msr_cz  = StatusFlag? 0x0:0x3; 
            ledCtrl.msr_qz  = StatusFlag? 0x0:0x3; 
            ledCtrl.msr_lp  = StatusFlag? 0x0:0x3; 
            ledCtrl.msr_tb  = StatusFlag? 0x0:0x3; 
            ledCtrl.msr_ss  = StatusFlag? 0x0:0x3; 
            ledCtrl.msr_xj  = StatusFlag? 0x0:0x3; 
            ledCtrl.msr_tx  = StatusFlag? 0x0:0x3; 
            ledCtrl.msr_dy  = StatusFlag? 0x0:0x3;    
            break;
        default: break;
    }
}
void MSR_LedFlashCtrl(unsigned char LedNo)
{
        switch(LedNo) {
        case MSR_LED_INCLINE_INDICAT:    ledCtrl.msr_qx  = 0x1; break;
        case MSR_LED_OVER_LOADING:       ledCtrl.msr_cz  = 0x1; break;
        case MSR_LED_UNDER_LOADING:      ledCtrl.msr_qz  = 0x1; break;
        case MSR_LED_LOSS_PHASE:         ledCtrl.msr_lp  = 0x1; break;
        case MSR_LED_SYNCHRO_START:      ledCtrl.msr_tb  = 0x1; break;
        case MSR_LED_UP_INDICAT:         ledCtrl.msr_ss  = 0x1; break;
        case MSR_LED_DOWN_INDICAT:       ledCtrl.msr_xj  = 0x1; break;
        case MSR_LED_COMMUNICAT_INDICAT: ledCtrl.msr_tx  = 0x1; break;
        case MSR_LED_POWER_START:        ledCtrl.msr_dy  = 0x1; break;
        case MSR_LED_ALL:
            ledCtrl.msr_qx  = 0x1; 
            ledCtrl.msr_cz  = 0x1; 
            ledCtrl.msr_qz  = 0x1; 
            ledCtrl.msr_lp  = 0x1; 
            ledCtrl.msr_tb  = 0x1; 
            ledCtrl.msr_ss  = 0x1; 
            ledCtrl.msr_xj  = 0x1; 
            ledCtrl.msr_tx  = 0x1; 
            ledCtrl.msr_dy  = 0x1;    
            break;
        default: break;
    }
}

void SLV_LedStatusCtrl(unsigned char LedNo, bool StatusFlag)
{
    switch( LedNo ) {
        case SLV_LED_ZCG_INDICAT:        ledCtrl.slv_zcg = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_OVER_LOADING:       ledCtrl.slv_cz  = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_UNDER_LOADING:      ledCtrl.slv_qz  = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_LOSS_PHASE:         ledCtrl.slv_lp  = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_SK_INDICAT:         ledCtrl.slv_sk  = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_UP_INDICAT:         ledCtrl.slv_ss  = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_DOWN_INDICAT:       ledCtrl.slv_xj  = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_COMMUNICAT_INDICAT: ledCtrl.slv_tx  = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_YCG_INDICAT:        ledCtrl.slv_ycg = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_WARN_INDICAT:       ledCtrl.slv_bj  = (StatusFlag)? 0x0:0x3; break;
        case SLV_LED_ALL:
            ledCtrl.slv_zcg = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_ycg = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_lp  = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_bj  = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_sk  = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_tx  = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_xj  = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_qz  = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_ss  = (StatusFlag)? 0x0:0x3; 
            ledCtrl.slv_cz  = (StatusFlag)? 0x0:0x3; 
            break;
        default: break;
    }
}

void SLV_LedFlashCtrl(unsigned char LedNo)
{
    switch( LedNo ) {
        case SLV_LED_ZCG_INDICAT:        ledCtrl.slv_zcg = 0x1; break;
        case SLV_LED_OVER_LOADING:       ledCtrl.slv_cz  = 0x1; break;
        case SLV_LED_UNDER_LOADING:      ledCtrl.slv_qz  = 0x1; break;
        case SLV_LED_LOSS_PHASE:         ledCtrl.slv_lp  = 0x1; break;
        case SLV_LED_SK_INDICAT:         ledCtrl.slv_sk  = 0x1; break;
        case SLV_LED_UP_INDICAT:         ledCtrl.slv_ss  = 0x1; break;
        case SLV_LED_DOWN_INDICAT:       ledCtrl.slv_xj  = 0x1; break;
        case SLV_LED_COMMUNICAT_INDICAT: ledCtrl.slv_tx  = 0x1; break;
        case SLV_LED_YCG_INDICAT:        ledCtrl.slv_ycg = 0x1; break;
        case SLV_LED_WARN_INDICAT:       ledCtrl.slv_bj  = 0x1; break;
        case SLV_LED_ALL:
            ledCtrl.slv_zcg = 0x1; 
            ledCtrl.slv_ycg = 0x1; 
            ledCtrl.slv_lp  = 0x1; 
            ledCtrl.slv_bj  = 0x1; 
            ledCtrl.slv_sk  = 0x1; 
            ledCtrl.slv_tx  = 0x1; 
            ledCtrl.slv_xj  = 0x1; 
            ledCtrl.slv_qz  = 0x1; 
            ledCtrl.slv_ss  = 0x1; 
            ledCtrl.slv_cz  = 0x1; 
            break;
        default: break;
    }
}

void ledStatusManageService(unsigned int iSec)
{
    if( isMasterDevice()) {
        //倾斜
        if(ledCtrl.msr_qx == 0x0)      MSR_LED_Incline_Indicat = 1;
        else if(ledCtrl.msr_qx == 0x3) MSR_LED_Incline_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Incline_Indicat = ~MSR_LED_Incline_Indicat; }
        //电源
        if(ledCtrl.msr_dy == 0x0)      MSR_LED_Power_Start = 1;
        else if(ledCtrl.msr_dy == 0x3) MSR_LED_Power_Start = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Power_Start = ~MSR_LED_Power_Start; }
        //缺相
        if(ledCtrl.msr_lp == 0x0)      MSR_LED_Loss_Phase = 1;
        else if(ledCtrl.msr_lp == 0x3) MSR_LED_Loss_Phase = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Loss_Phase = ~MSR_LED_Loss_Phase; }
        //上升
        if(ledCtrl.msr_ss == 0x0)      MSR_LED_Up_Indicat = 1;
        else if(ledCtrl.msr_ss == 0x3) MSR_LED_Up_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Up_Indicat = ~MSR_LED_Up_Indicat; }
        //超载
        if(ledCtrl.msr_cz == 0x0)      MSR_LED_Over_Loading = 1;
        else if(ledCtrl.msr_cz == 0x3) MSR_LED_Over_Loading = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Over_Loading = ~MSR_LED_Over_Loading; }
        //下降
        if(ledCtrl.msr_xj == 0x0)      MSR_LED_Down_Indicat = 1;
        else if(ledCtrl.msr_xj == 0x3) MSR_LED_Down_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Down_Indicat = ~MSR_LED_Down_Indicat; }
        //欠载
        if(ledCtrl.msr_qz == 0x0)      MSR_LED_Under_Loading = 1;
        else if(ledCtrl.msr_qz == 0x3) MSR_LED_Under_Loading = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Under_Loading = ~MSR_LED_Under_Loading; }
        //同步
        if(ledCtrl.msr_tb == 0x0)      MSR_LED_Synchro_Start = 1;
        else if(ledCtrl.msr_tb == 0x3) MSR_LED_Synchro_Start = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Synchro_Start = ~MSR_LED_Synchro_Start; }
        //通讯
        if(ledCtrl.msr_tx == 0x0)      MSR_LED_Communicat_Indicat = 1;
        else if(ledCtrl.msr_tx == 0x3) MSR_LED_Communicat_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Communicat_Indicat = ~MSR_LED_Communicat_Indicat; }
    } else {
        //缺相
        if(ledCtrl.slv_lp == 0x0)      SLV_LED_Loss_Phase = 1;
        else if(ledCtrl.slv_lp == 0x3) SLV_LED_Loss_Phase = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Loss_Phase = ~SLV_LED_Loss_Phase; }
        //超载
        if(ledCtrl.slv_cz == 0x0)      SLV_LED_Over_Loading = 1;
        else if(ledCtrl.slv_cz == 0x3) SLV_LED_Over_Loading = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Over_Loading = ~SLV_LED_Over_Loading; }
        //上升
        if(ledCtrl.slv_ss == 0x0)      SLV_LED_Up_Indicat = 1;
        else if(ledCtrl.slv_ss == 0x3) SLV_LED_Up_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Up_Indicat = ~SLV_LED_Up_Indicat; }
        //欠载
        if(ledCtrl.slv_qz == 0x0)      SLV_LED_Under_Loading = 1;
        else if(ledCtrl.slv_qz == 0x3) SLV_LED_Under_Loading = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Under_Loading = ~SLV_LED_Under_Loading; }
        //下降
        if(ledCtrl.slv_xj == 0x0)      SLV_LED_Down_Indicat = 1;
        else if(ledCtrl.slv_xj == 0x3) SLV_LED_Down_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Down_Indicat = ~SLV_LED_Down_Indicat; }
        //受控
        if(ledCtrl.slv_sk == 0x0)      SLV_LED_SK_Indicat = 1;
        else if(ledCtrl.slv_sk == 0x3) SLV_LED_SK_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_SK_Indicat = ~SLV_LED_SK_Indicat; }
        //通讯
        if(ledCtrl.slv_tx == 0x0)      SLV_LED_Communicat_Indicat = 1;
        else if(ledCtrl.slv_tx == 0x3) SLV_LED_Communicat_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Communicat_Indicat = ~SLV_LED_Communicat_Indicat; }
        //右超高
        if(ledCtrl.slv_ycg == 0x0)      SLV_LED_YCG_Indicat = 1;
        else if(ledCtrl.slv_ycg == 0x3) SLV_LED_YCG_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_YCG_Indicat = ~SLV_LED_YCG_Indicat; }
        //左超高
        if(ledCtrl.slv_zcg == 0x0)      SLV_LED_ZCG_Indicat = 1;
        else if(ledCtrl.slv_zcg == 0x3) SLV_LED_ZCG_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_ZCG_Indicat = ~SLV_LED_ZCG_Indicat; }
        //报警
        if(ledCtrl.slv_bj == 0x0)      SLV_LED_Warn_Indicat = 1;
        else if(ledCtrl.slv_bj == 0x3) SLV_LED_Warn_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Warn_Indicat = ~SLV_LED_Warn_Indicat; }
    }
}

#if 0
void MSR_LedStatusCtrl(unsigned char LedNo, bool StatusFlag)
{
    switch(LedNo) {
        case MSR_LED_INCLINE_INDICAT:    MSR_LED_Incline_Indicat    = StatusFlag; break;
        case MSR_LED_OVER_LOADING:       MSR_LED_Over_Loading       = StatusFlag; break;
        case MSR_LED_UNDER_LOADING:      MSR_LED_Under_Loading      = StatusFlag; break;
        case MSR_LED_LOSS_PHASE:         MSR_LED_Loss_Phase         = StatusFlag; break;
        case MSR_LED_SYNCHRO_START:      MSR_LED_Synchro_Start      = StatusFlag; break;
        case MSR_LED_UP_INDICAT:         MSR_LED_Up_Indicat         = StatusFlag; break;
        case MSR_LED_DOWN_INDICAT:       MSR_LED_Down_Indicat       = StatusFlag; break;
        case MSR_LED_COMMUNICAT_INDICAT: MSR_LED_Communicat_Indicat = StatusFlag; break;
        case MSR_LED_POWER_START:        MSR_LED_Power_Start        = StatusFlag; break;
        case MSR_LED_ALL:
            MSR_LED_Incline_Indicat    = StatusFlag;
            MSR_LED_Over_Loading       = StatusFlag;
            MSR_LED_Under_Loading      = StatusFlag;
            MSR_LED_Loss_Phase         = StatusFlag;
            MSR_LED_Synchro_Start      = StatusFlag;
            MSR_LED_Up_Indicat         = StatusFlag;
            MSR_LED_Down_Indicat       = StatusFlag;
            MSR_LED_Communicat_Indicat = StatusFlag;
            MSR_LED_Power_Start        = StatusFlag;    
            break;
        default: break;
    }
}

void MSR_LedFlashCtrl(unsigned char LedNo, unsigned int iFlash)
{
    if((iFlash % 25) == 0) {
        switch(LedNo) {
            case MSR_LED_INCLINE_INDICAT:    MSR_LED_Incline_Indicat    = ~MSR_LED_Incline_Indicat;      break;
            case MSR_LED_OVER_LOADING:       MSR_LED_Over_Loading       = ~MSR_LED_Over_Loading;       break;
            case MSR_LED_UNDER_LOADING:      MSR_LED_Under_Loading      = ~MSR_LED_Under_Loading;      break;
            case MSR_LED_LOSS_PHASE:         MSR_LED_Loss_Phase         = ~MSR_LED_Loss_Phase;         break;
            case MSR_LED_SYNCHRO_START:      MSR_LED_Synchro_Start      = ~MSR_LED_Synchro_Start;      break;
            case MSR_LED_UP_INDICAT:         MSR_LED_Up_Indicat         = ~MSR_LED_Up_Indicat;         break;
            case MSR_LED_DOWN_INDICAT:       MSR_LED_Down_Indicat       = ~MSR_LED_Down_Indicat;       break;
            case MSR_LED_COMMUNICAT_INDICAT: MSR_LED_Communicat_Indicat = ~MSR_LED_Communicat_Indicat; break;
            case MSR_LED_POWER_START:        MSR_LED_Power_Start        = ~MSR_LED_Power_Start;        break;
            case MSR_LED_ALL:
                MSR_LedStatusCtrl(MSR_LED_ALL, LED_OFF);
                MSR_LED_Incline_Indicat    = ~MSR_LED_Incline_Indicat;
                MSR_LED_Over_Loading       = ~MSR_LED_Over_Loading;
                MSR_LED_Under_Loading      = ~MSR_LED_Under_Loading;
                MSR_LED_Loss_Phase         = ~MSR_LED_Loss_Phase;
                MSR_LED_Synchro_Start      = ~MSR_LED_Synchro_Start;
                MSR_LED_Up_Indicat         = ~MSR_LED_Up_Indicat;
                MSR_LED_Down_Indicat       = ~MSR_LED_Down_Indicat;
                MSR_LED_Communicat_Indicat = ~MSR_LED_Communicat_Indicat;
                MSR_LED_Power_Start        = ~MSR_LED_Power_Start;    
                break;
            default: break;
        }
    }
}

void SLV_LedStatusCtrl(unsigned char LedNo, bool StatusFlag)
{
    switch( LedNo ) {
        case SLV_LED_ZCG_INDICAT:        SLV_LED_ZCG_Indicat        = StatusFlag; break;
        case SLV_LED_OVER_LOADING:       SLV_LED_Over_Loading       = StatusFlag; break;
        case SLV_LED_UNDER_LOADING:      SLV_LED_Under_Loading      = StatusFlag; break;
        case SLV_LED_LOSS_PHASE:         SLV_LED_Loss_Phase         = StatusFlag; break;
        case SLV_LED_SK_INDICAT:         SLV_LED_SK_Indicat         = StatusFlag; break;
        case SLV_LED_UP_INDICAT:         SLV_LED_Up_Indicat         = StatusFlag; break;
        case SLV_LED_DOWN_INDICAT:       SLV_LED_Down_Indicat       = StatusFlag; break;
        case SLV_LED_COMMUNICAT_INDICAT: SLV_LED_Communicat_Indicat = StatusFlag; break;
        case SLV_LED_YCG_INDICAT:        SLV_LED_YCG_Indicat        = StatusFlag; break;
        case SLV_LED_WARN_INDICAT:       SLV_LED_Warn_Indicat       = StatusFlag; break;
        case SLV_LED_ALL:
            SLV_LED_ZCG_Indicat        = StatusFlag; 
            SLV_LED_Over_Loading       = StatusFlag; 
            SLV_LED_Under_Loading      = StatusFlag; 
            SLV_LED_Loss_Phase         = StatusFlag; 
            SLV_LED_SK_Indicat         = StatusFlag; 
            SLV_LED_Up_Indicat         = StatusFlag; 
            SLV_LED_Down_Indicat       = StatusFlag; 
            SLV_LED_Communicat_Indicat = StatusFlag; 
            SLV_LED_YCG_Indicat        = StatusFlag; 
            SLV_LED_Warn_Indicat       = StatusFlag; 
            break;
        default: break;
    }
}

void SLV_LedFlashCtrl(unsigned char LedNo, unsigned int iFlash)
{
    if((iFlash % 25) == 0) {
        switch( LedNo ) {
            case SLV_LED_ZCG_INDICAT:        SLV_LED_ZCG_Indicat        = ~SLV_LED_ZCG_Indicat; break;
            case SLV_LED_OVER_LOADING:       SLV_LED_Over_Loading       = ~SLV_LED_Over_Loading; break;
            case SLV_LED_UNDER_LOADING:      SLV_LED_Under_Loading      = ~SLV_LED_Under_Loading; break;
            case SLV_LED_LOSS_PHASE:         SLV_LED_Loss_Phase         = ~SLV_LED_Loss_Phase; break;
            case SLV_LED_SK_INDICAT:         SLV_LED_SK_Indicat         = ~SLV_LED_SK_Indicat; break;
            case SLV_LED_UP_INDICAT:         SLV_LED_Up_Indicat         = ~SLV_LED_Up_Indicat; break;
            case SLV_LED_DOWN_INDICAT:       SLV_LED_Down_Indicat       = ~SLV_LED_Down_Indicat; break;
            case SLV_LED_COMMUNICAT_INDICAT: SLV_LED_Communicat_Indicat = ~SLV_LED_Communicat_Indicat; break;
            case SLV_LED_YCG_INDICAT:        SLV_LED_YCG_Indicat        = ~SLV_LED_YCG_Indicat; break;
            case SLV_LED_WARN_INDICAT:       SLV_LED_Warn_Indicat       = ~SLV_LED_Warn_Indicat; break;
            case SLV_LED_ALL:
                SLV_LedStatusCtrl(SLV_LED_ALL, LED_OFF);
                SLV_LED_ZCG_Indicat        = ~SLV_LED_ZCG_Indicat; 
                SLV_LED_Over_Loading       = ~SLV_LED_Over_Loading; 
                SLV_LED_Under_Loading      = ~SLV_LED_Under_Loading; 
                SLV_LED_Loss_Phase         = ~SLV_LED_Loss_Phase; 
                SLV_LED_SK_Indicat         = ~SLV_LED_SK_Indicat; 
                SLV_LED_Up_Indicat         = ~SLV_LED_Up_Indicat; 
                SLV_LED_Down_Indicat       = ~SLV_LED_Down_Indicat; 
                SLV_LED_Communicat_Indicat = ~SLV_LED_Communicat_Indicat; 
                SLV_LED_YCG_Indicat        = ~SLV_LED_YCG_Indicat; 
                SLV_LED_Warn_Indicat       = ~SLV_LED_Warn_Indicat; 
                break;
            default: break;
        }
    }
}
#endif

#if TEST_MODE
void ledLight_Test(bool StatusFlag)
{
    if( isMasterDevice()) {
        MSR_LedStatusCtrl(MSR_LED_PHASE_INDICAT,      StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_OVER_LOADING,       StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_UNDER_LOADING,      StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_LOSS_PHASE,         StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_SYNCHRO_START,      StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_UP_INDICAT,         StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_DOWN_INDICAT,       StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_COMMUNICAT_INDICAT, StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_POWER_START,        StatusFlag);
    } else {
        MSR_LedStatusCtrl(MSR_LED_PHASE_INDICAT,      StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_OVER_LOADING,       StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_UNDER_LOADING,      StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_LOSS_PHASE,         StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_SYNCHRO_START,      StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_UP_INDICAT,         StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_DOWN_INDICAT,       StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_COMMUNICAT_INDICAT, StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_POWER_START,        StatusFlag);
    }
}
#endif
