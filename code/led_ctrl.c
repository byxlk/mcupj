#include "api_config.h"

#define FLASH_TIME (10)  /* 10x10 ms ��ƽ��ת */

/* 00�� Ϩ��  01/10 ��˸ 11������ */
struct ledStatsManage {
    unsigned char msr_dy:2;//��Դ
    unsigned char msr_lp:2; //ȱ��
    unsigned char msr_ss:2; //����
    unsigned char msr_cz:2; //����
    unsigned char msr_xj:2; //�½�
    unsigned char msr_qz:2; //Ƿ��
    unsigned char msr_tb:2; //ͬ��
    unsigned char msr_qx:2; //��б
    unsigned char msr_tx:2; //ͨѶ

    unsigned char slv_zcg:2; //�󳬸�
    unsigned char slv_lp:2; //ȱ��
    unsigned char slv_cz:2; //����
    unsigned char slv_ss:2; //����
    unsigned char slv_qz:2; //Ƿ��
    unsigned char slv_xj:2; //�½�
    unsigned char slv_sk:2; //�ܿ�
    unsigned char slv_tx:2; //ͨѶ
    unsigned char slv_ycg:2; //�ҳ���
    unsigned char slv_bj:2; //����
} ;
static struct ledStatsManage ledCtrl = {0};

void MSR_LedStatusCtrl(LEDCODE_DEF_E LedNo, bool StatusFlag)
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
void MSR_LedFlashCtrl(LEDCODE_DEF_E LedNo)
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

void SLV_LedStatusCtrl(LEDCODE_DEF_E LedNo, bool StatusFlag)
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

void SLV_LedFlashCtrl(LEDCODE_DEF_E LedNo)
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
        //��б
        if(ledCtrl.msr_qx == 0x0)      MSR_LED_Incline_Indicat = 1;
        else if(ledCtrl.msr_qx == 0x3) MSR_LED_Incline_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Incline_Indicat = ~MSR_LED_Incline_Indicat; }
        //��Դ
        if(ledCtrl.msr_dy == 0x0)      MSR_LED_Power_Start = 1;
        else if(ledCtrl.msr_dy == 0x3) MSR_LED_Power_Start = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Power_Start = ~MSR_LED_Power_Start; }
        //ȱ��
        if(ledCtrl.msr_lp == 0x0)      MSR_LED_Loss_Phase = 1;
        else if(ledCtrl.msr_lp == 0x3) MSR_LED_Loss_Phase = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Loss_Phase = ~MSR_LED_Loss_Phase; }
        //����
        if(ledCtrl.msr_ss == 0x0)      MSR_LED_Up_Indicat = 1;
        else if(ledCtrl.msr_ss == 0x3) MSR_LED_Up_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Up_Indicat = ~MSR_LED_Up_Indicat; }
        //����
        if(ledCtrl.msr_cz == 0x0)      MSR_LED_Over_Loading = 1;
        else if(ledCtrl.msr_cz == 0x3) MSR_LED_Over_Loading = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Over_Loading = ~MSR_LED_Over_Loading; }
        //�½�
        if(ledCtrl.msr_xj == 0x0)      MSR_LED_Down_Indicat = 1;
        else if(ledCtrl.msr_xj == 0x3) MSR_LED_Down_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Down_Indicat = ~MSR_LED_Down_Indicat; }
        //Ƿ��
        if(ledCtrl.msr_qz == 0x0)      MSR_LED_Under_Loading = 1;
        else if(ledCtrl.msr_qz == 0x3) MSR_LED_Under_Loading = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Under_Loading = ~MSR_LED_Under_Loading; }
        //ͬ��
        if(ledCtrl.msr_tb == 0x0)      MSR_LED_Synchro_Start = 1;
        else if(ledCtrl.msr_tb == 0x3) MSR_LED_Synchro_Start = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Synchro_Start = ~MSR_LED_Synchro_Start; }
        //ͨѶ
        if(ledCtrl.msr_tx == 0x0)      MSR_LED_Communicat_Indicat = 1;
        else if(ledCtrl.msr_tx == 0x3) MSR_LED_Communicat_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) MSR_LED_Communicat_Indicat = ~MSR_LED_Communicat_Indicat; }
    } else {
        //ȱ��
        if(ledCtrl.slv_lp == 0x0)      SLV_LED_Loss_Phase = 1;
        else if(ledCtrl.slv_lp == 0x3) SLV_LED_Loss_Phase = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Loss_Phase = ~SLV_LED_Loss_Phase; }
        //����
        if(ledCtrl.slv_cz == 0x0)      SLV_LED_Over_Loading = 1;
        else if(ledCtrl.slv_cz == 0x3) SLV_LED_Over_Loading = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Over_Loading = ~SLV_LED_Over_Loading; }
        //����
        if(ledCtrl.slv_ss == 0x0)      SLV_LED_Up_Indicat = 1;
        else if(ledCtrl.slv_ss == 0x3) SLV_LED_Up_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Up_Indicat = ~SLV_LED_Up_Indicat; }
        //Ƿ��
        if(ledCtrl.slv_qz == 0x0)      SLV_LED_Under_Loading = 1;
        else if(ledCtrl.slv_qz == 0x3) SLV_LED_Under_Loading = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Under_Loading = ~SLV_LED_Under_Loading; }
        //�½�
        if(ledCtrl.slv_xj == 0x0)      SLV_LED_Down_Indicat = 1;
        else if(ledCtrl.slv_xj == 0x3) SLV_LED_Down_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Down_Indicat = ~SLV_LED_Down_Indicat; }
        //�ܿ�
        if(ledCtrl.slv_sk == 0x0)      SLV_LED_SK_Indicat = 1;
        else if(ledCtrl.slv_sk == 0x3) SLV_LED_SK_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_SK_Indicat = ~SLV_LED_SK_Indicat; }
        //ͨѶ
        if(ledCtrl.slv_tx == 0x0)      SLV_LED_Communicat_Indicat = 1;
        else if(ledCtrl.slv_tx == 0x3) SLV_LED_Communicat_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Communicat_Indicat = ~SLV_LED_Communicat_Indicat; }
        //�ҳ���
        if(ledCtrl.slv_ycg == 0x0)      SLV_LED_YCG_Indicat = 1;
        else if(ledCtrl.slv_ycg == 0x3) SLV_LED_YCG_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_YCG_Indicat = ~SLV_LED_YCG_Indicat; }
        //�󳬸�
        if(ledCtrl.slv_zcg == 0x0)      SLV_LED_ZCG_Indicat = 1;
        else if(ledCtrl.slv_zcg == 0x3) SLV_LED_ZCG_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_ZCG_Indicat = ~SLV_LED_ZCG_Indicat; }
        //����
        if(ledCtrl.slv_bj == 0x0)      SLV_LED_Warn_Indicat = 1;
        else if(ledCtrl.slv_bj == 0x3) SLV_LED_Warn_Indicat = 0;
        else { if((iSec % FLASH_TIME) == 0) SLV_LED_Warn_Indicat = ~SLV_LED_Warn_Indicat; }
    }
}

#if TEST_MODE
void ledLight_Test(bool StatusFlag)
{
    if( isMasterDevice()) {
        MSR_LedStatusCtrl(MSR_LED_INCLINE_INDICAT,    StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_OVER_LOADING,       StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_UNDER_LOADING,      StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_LOSS_PHASE,         StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_SYNCHRO_START,      StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_UP_INDICAT,         StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_DOWN_INDICAT,       StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_COMMUNICAT_INDICAT, StatusFlag);
        MSR_LedStatusCtrl(MSR_LED_POWER_START,        StatusFlag);
    } else {
        MSR_LedStatusCtrl(MSR_LED_INCLINE_INDICAT,    StatusFlag);
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
