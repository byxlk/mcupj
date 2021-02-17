#include "api_config.h"

static u32 iSecondCounter = 0;

/* F:����  0������ ABC����Ӧ��P3.5 P3.6 P3.7 */
static bool phaseSeqIsRight = 0;
static u8 phaseSeqCheckCount = 0;

static bool phaseALossFlag = 1;
static bool phaseBLossFlag = 1;
static bool phaseCLossFlag = 1;

static bool phaseSeqALastFlag = 1;
static bool phaseSeqBLastFlag = 1;
static bool phaseSeqCLastFlag = 1;

static u32 phaseALossCount = 0;
static u32 phaseBLossCount = 0;
static u32 phaseCLossCount = 0;

static bool onlyOnce_ACPowerPhaseSequenceCheck = 1; // only exectue once

void checkPhaseSeqALost(void)
{
    if(phaseSeqALastFlag != PWR_PHASE_PIN_A) {
        phaseSeqALastFlag = PWR_PHASE_PIN_A;
        phaseALossCount = 0;
        phaseALossFlag = 0;
    } else {
        // ����100ms ��ƽû�б仯��Ϊphase��ʧ
        if((phaseALossCount++) > 100) {
            phaseALossFlag = 1;
        }
    }
}

void checkPhaseSeqBLost(void)
{
    if(phaseSeqBLastFlag != PWR_PHASE_PIN_B) {
        phaseSeqBLastFlag = PWR_PHASE_PIN_B;
        phaseBLossCount = 0;
        phaseBLossFlag = 0;
    } else {
        // ����100ms ��ƽû�б仯��Ϊphase��ʧ
        if((phaseBLossCount++) > 100) {
            phaseBLossFlag = 1;
        }
    }
}

void checkPhaseSeqCLost(void)
{
    if(phaseSeqCLastFlag != PWR_PHASE_PIN_C) {
        phaseSeqCLastFlag = PWR_PHASE_PIN_C;
        phaseCLossCount = 0;
        phaseCLossFlag = 0;
    } else {
        // ����100ms ��ƽû�б仯��Ϊphase��ʧ
        if((phaseCLossCount++) > 100) {
            phaseCLossFlag = 1;
        }
    }
}

u16 checkACPowerPhaseSequence(void)
{
    /* F:����  0������ ABC����Ӧ��P3.5 P3.6 P3.7 */
    u16 phaseSeq = 0xFABC;//0x0ACB

    if(phaseALossFlag) phaseSeq &=  0x00FF;
    else { phaseSeq &=  0x00FF; phaseSeq |=  0x0AFF;}

    if(phaseBLossFlag) phaseSeq &=  0x0F0F;
    else { phaseSeq &=  0x0F0F; phaseSeq |=  0x0FBF;}

    if(phaseCLossFlag) phaseSeq &=  0x0FF0;
    else { phaseSeq &=  0x0FF0; phaseSeq |=  0x0FFC;}

    if(phaseSeqIsRight) phaseSeq |= 0xF000;
    else {
        phaseSeq &=  0x0FFF;
        phaseSeq = (phaseSeq & 0xFF00) | ((phaseSeq & 0x000F) << 4) | ((phaseSeq & 0x00F0) >> 4);
    }
 
    return (phaseSeq);
}

#if TEST_MODE
void acPowerPhaseSequenceCheck_Test(void)
{
    u16 phaseSeq = checkACPowerPhaseSequence();
    if((phaseSeq & 0xF000) == 0) {/* ?? */
        LOGD("Phase Sequence is right\n");
    } else {
        LOGD("Phase Sequence is not right\n");
    }
    
    if((phaseSeq & 0x000F) == 0x0) {
        LOGD("Phase C is loss ...\n");
    }
    if((phaseSeq & 0x00F0) == 0x0) {
        LOGD("Phase B is loss ...\n");
    }
    if((phaseSeq & 0x0F00) == 0x0) {
        LOGD("Phase A is loss ...\n");
    }
}
#endif

u32 getSysTick(void)
{
    return iSecondCounter;
}

/********************* Timer0中断函数************************/
#define TIMER_VALUE (65536 - (MAIN_Fosc / (12 * 50)))
void timer0_int (void) interrupt TIMER0_VECTOR //1ms @22.1184MHz
{
	// process watch dog signal
	WDT_CONTR &= 0x7F;
    WDT_CONTR |= 0x10;

    /* Counter */
    iSecondCounter++;
    
    checkPhaseSeqALost();
    checkPhaseSeqBLost();
    checkPhaseSeqCLost();
    
    /* 减小定时器误差 */
	//TL0 = TIMER_VALUE % 256 - TL0;
	//TH0 = TIMER_VALUE / 256 - TH0;
}

/********************* INT3�жϺ��� *************************/
void Ext_INT3 (void) interrupt INT3_VECTOR
{
    /* Check phase sequene err */
    if((!phaseALossFlag) && (!phaseBLossFlag) && (!phaseCLossFlag)) {
        if((PWR_PHASE_PIN_B == 1) && (PWR_PHASE_PIN_A == 0)) {
            if((phaseSeqCheckCount++) > 10) {
                phaseSeqIsRight = 1;
            } else {
                phaseSeqIsRight = 0;
                phaseSeqCheckCount = 0;
            }
        } else {
            phaseSeqIsRight = 0;
            phaseSeqCheckCount = 0;
        }
    }    
}