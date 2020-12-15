#include "api_config.h"

void MSR_LedStatusCtrl(unsigned char LedNo, bool StatusFlag)
{
    switch(LedNo) {
        case MSR_LED_PHASE_INDICAT:      MSR_LED_Phase_Indicat      = StatusFlag; break;
        case MSR_LED_OVER_LOADING:       MSR_LED_Over_Loading       = StatusFlag; break;
        case MSR_LED_UNDER_LOADING:      MSR_LED_Under_Loading      = StatusFlag; break;
        case MSR_LED_LOSS_PHASE:         MSR_LED_Loss_Phase         = StatusFlag; break;
        case MSR_LED_SYNCHRO_START:      MSR_LED_Synchro_Start      = StatusFlag; break;
        case MSR_LED_UP_INDICAT:         MSR_LED_Up_Indicat         = StatusFlag; break;
        case MSR_LED_DOWN_INDICAT:       MSR_LED_Down_Indicat       = StatusFlag; break;
        case MSR_LED_COMMUNICAT_INDICAT: MSR_LED_Communicat_Indicat = StatusFlag; break;
        case MSR_LED_POWER_START:        MSR_LED_Power_Start        = StatusFlag; break;
        default: break;
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
        default: break;
    }
}
