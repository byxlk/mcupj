#include "api_config.h"

/* F:正序  0：反序 ABC：对应的P3.5 P3.6 P3.7 */
static bool phaseSeqIsRight = 0;
static unsigned char phaseSeqCheckCount = 0;

static bool phaseALossFlag = 1;
static bool phaseBLossFlag = 1;
static bool phaseCLossFlag = 1;

static bool phaseSeqALastFlag = 1;
static bool phaseSeqBLastFlag = 1;
static bool phaseSeqCLastFlag = 1;

static unsigned int phaseALossCount = 0;
static unsigned int phaseBLossCount = 0;
static unsigned int phaseCLossCount = 0;

static bool onlyOnce_ACPowerPhaseSequenceCheck = 1; // only exectue once

static void checkPhaseSeqALost(void)
{
    if(phaseSeqALastFlag != PWR_PHASE_PIN_A) {
        phaseSeqALastFlag = PWR_PHASE_PIN_A;
        phaseALossCount = 0;
        phaseALossFlag = 0;
    } else {
        // 超过100ms 电平没有变化认为phase丢失
        if((phaseALossCount++) > 100) {
            phaseALossFlag = 1;
        }
    }
}

static void checkPhaseSeqBLost(void)
{
    if(phaseSeqBLastFlag != PWR_PHASE_PIN_B) {
        phaseSeqBLastFlag = PWR_PHASE_PIN_B;
        phaseBLossCount = 0;
        phaseBLossFlag = 0;
    } else {
        // 超过100ms 电平没有变化认为phase丢失
        if((phaseBLossCount++) > 100) {
            phaseBLossFlag = 1;
        }
    }
}

static void checkPhaseSeqCLost(void)
{
    if(phaseSeqCLastFlag != PWR_PHASE_PIN_C) {
        phaseSeqCLastFlag = PWR_PHASE_PIN_C;
        phaseCLossCount = 0;
        phaseCLossFlag = 0;
    } else {
        // 超过100ms 电平没有变化认为phase丢失
        if((phaseCLossCount++) > 100) {
            phaseCLossFlag = 1;
        }
    }
}

unsigned int checkACPowerPhaseSequence(void)
{
    /* F:正序  0：反序 ABC：对应的P3.5 P3.6 P3.7 */
    unsigned short phaseSeq = 0xFABC;//0x0ACB

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

/********************* Timer1中断函数************************/
/* 用于做三相电相序检测 */
void timer1_int (void) interrupt TIMER1_VECTOR
{
    /* Check phase sequene lost */
    checkPhaseSeqALost();
    checkPhaseSeqBLost();
    checkPhaseSeqCLost();   
}

//sbit PWR_PHASE_PIN_A                = P3^5; //
//sbit PWR_PHASE_PIN_B                = P3^6; //INT2
//sbit PWR_PHASE_PIN_C                = P3^7; //INT3

/********************* INT3中断函数 *************************/
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