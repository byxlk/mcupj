#include "api_config.h"
#include	"PCA.h"

/* 16384 8.8ms */
/* 6.67 / (((1 * 12) / 22118400) * 1000) */
#define PHASE_TMAX (18432) 
#define PHASE_TMIN (7168) 

typedef struct {
	u8 pca_int_cnt; /* interrupt counter */
	u16 time_ab;
	u16 time_bc;
	u16 phaseSeq;
	u8 phaseALossFlag:1;
	u8 phaseBLossFlag:1;
	u8 phaseCLossFlag:1;
	u8 phaseSeqIsRight:1;
} PHASE_ALGO_S;

static PHASE_ALGO_S pas = { 0x0 };

bit		B_Capture0,B_Capture1,B_Capture2;
u8		PCA0_mode,PCA1_mode,PCA2_mode;
u16		CCAP0_tmp,PCA_Timer0;
u16		CCAP1_tmp,PCA_Timer1;
u16		CCAP2_tmp,PCA_Timer2;
u16     CCAP_tmp;

/*************	����˵��	**************


******************************************/

u16	PWM0_low;	//PWM����͵�ƽ��PCAʱ���������, �û��㲻�ɼ���
u16	PWM1_low;	//PWM����͵�ƽ��PCAʱ���������, �û��㲻�ɼ���
u16	PWM2_low;	//PWM����͵�ƽ��PCAʱ���������, �û��㲻�ɼ���

static u8 phaseSeqCheckCount = 0;

static bool phaseSeqALastFlag = 1;
static bool phaseSeqBLastFlag = 1;
static bool phaseSeqCLastFlag = 1;

//========================================================================
// ����: void PWMn_SetHighReg(unsigned int high)
// ����: ����ռ�ձ����ݡ�
// ����: high: 	ռ�ձ����ݣ���PWM����ߵ�ƽ��PCAʱ�����������
// ����: ��
// �汾: VER1.0
// ����: 2013-5-15
// ��ע:
//========================================================================
void PWMn_Update(u8 PCA_id, u16 high)
{
	if(PCA_id == PCA0)
	{
		if(high > PWM0_HIGH_MAX)	high = PWM0_HIGH_MAX;	//���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
		if(high < PWM0_HIGH_MIN)	high = PWM0_HIGH_MIN;	//���д��С����Сռ�ձ����ݣ��򷵻ش������2��
		CR = 0;							//ֹͣPCA��
		PCA_Timer0 = high;				//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM0_low = PWM0_DUTY - high;	//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;							//����PCA��
	}
	else if(PCA_id == PCA1)
	{
		if(high > PWM1_HIGH_MAX)	high = PWM1_HIGH_MAX;	//���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
		if(high < PWM1_HIGH_MIN)	high = PWM1_HIGH_MIN;	//���д��С����Сռ�ձ����ݣ��򷵻ش������2��
		CR = 0;							//ֹͣPCA��
		PCA_Timer1 = high;				//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM1_low = PWM1_DUTY - high;	//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;							//����PCA��
	}
	else if(PCA_id == PCA2)
	{
		if(high > PWM2_HIGH_MAX)		high = PWM2_HIGH_MAX;	//���д��������ռ�ձ����ݣ�ǿ��Ϊ���ռ�ձȡ�
		if(high < PWM2_HIGH_MIN)		high = PWM2_HIGH_MIN;	//���д��С����Сռ�ձ����ݣ��򷵻ش������2��
		CR = 0;						//ֹͣPCA��
		PCA_Timer2 = high;						//��������ȷ��Χ����װ��ռ�ձȼĴ�����
		PWM2_low = PWM2_DUTY - high;				//���㲢����PWM����͵�ƽ��PCAʱ�����������
		CR = 1;						//����PCA��
	}
}



//========================================================================
// ����: UpdatePwm(u8 PCA_id, u8 pwm_value)
// ����: ����PWMֵ.
// ����: PCA_id: PCA���. ȡֵ PCA0,PCA1,PCA2,PCA_Counter
//		 pwm_value: pwmֵ, ���ֵ������͵�ƽ��ʱ��.
// ����: none.
// �汾: V1.0, 2012-11-22
//========================================================================
/*
void	UpdatePwm(u8 PCA_id, u8 pwm_value)
{
	if(PCA_id == PCA0)		CCAP0H = pwm_value;
	else if(PCA_id == PCA1)	CCAP1H = pwm_value;
	else if(PCA_id == PCA2)	CCAP2H = pwm_value;
}
*/

//========================================================================
// ����: void	PCA_Init(PCA_id, PCA_InitTypeDef *PCAx)
// ����: PCA��ʼ������.
// ����: PCA_id: PCA���. ȡֵ PCA0,PCA1,PCA2,PCA_Counter
//		 PCAx: �ṹ����,��ο�PCA.h��Ķ���.
// ����: none.
// �汾: V1.0, 2012-11-22
//========================================================================
void	PCA_Init(u8 PCA_id, PCA_InitTypeDef *PCAx)
{
	if(PCA_id > PCA_Counter)	return;		//id����

	if(PCA_id == PCA_Counter)			//���ù���Counter
	{
		CR = 0;
		CH = 0;
		CL = 0;
		AUXR1 = (AUXR1 & ~(3<<4)) | PCAx->PCA_IoUse;			//�л�IO��
		CMOD  = (CMOD  & ~(7<<1)) | PCAx->PCA_Clock;			//ѡ��ʱ��Դ
		CMOD  = (CMOD  & ~1) | (PCAx->PCA_Interrupt_Mode & 1);	//ECF
		if(PCAx->PCA_Polity == PolityHigh)		PPCA = 1;	//�����ȼ��ж�
		else									PPCA = 0;	//�����ȼ��ж�
		if(PCAx->PCA_RUN == ENABLE)	CR = 1;
		return;
	}

	PCAx->PCA_Interrupt_Mode &= (3<<4) + 1;
	if(PCAx->PCA_Mode >= PCA_Mode_SoftTimer)	PCAx->PCA_Interrupt_Mode &= ~(3<<4);

	if(PCA_id == PCA0)
	{
		CCAPM0    = PCAx->PCA_Mode | PCAx->PCA_Interrupt_Mode;	//����ģʽ, �ж�ģʽ
		PCA_PWM0  = (PCA_PWM0 & ~(3<<6)) | PCAx->PCA_PWM_Wide;	//PWM����

		PCA_Timer0 = PCAx->PCA_Value;
		B_Capture0 = 0;
		PCA0_mode = PCAx->PCA_Mode;
		CCAP0_tmp = PCA_Timer0;
		CCAP0L = (u8)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP0H = (u8)(CCAP0_tmp >> 8);	//��дCCAP0H
	}
	if(PCA_id == PCA1)
	{
		CCAPM1    = PCAx->PCA_Mode | PCAx->PCA_Interrupt_Mode;
		PCA_PWM1  = (PCA_PWM1 & ~(3<<6)) | PCAx->PCA_PWM_Wide;

		PCA_Timer1 = PCAx->PCA_Value;
		B_Capture1 = 0;
		PCA1_mode = PCAx->PCA_Mode;
		CCAP1_tmp = PCA_Timer1;
		CCAP1L = (u8)CCAP1_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP1H = (u8)(CCAP1_tmp >> 8);	//��дCCAP0H
	}
	if(PCA_id == PCA2)
	{
		CCAPM2    = PCAx->PCA_Mode | PCAx->PCA_Interrupt_Mode;
		PCA_PWM2  = (PCA_PWM2 & ~(3<<6)) | PCAx->PCA_PWM_Wide;

		PCA_Timer2 = PCAx->PCA_Value;
		B_Capture2 = 0;
		PCA2_mode = PCAx->PCA_Mode;
		CCAP2_tmp = PCA_Timer2;
		CCAP2L = (u8)CCAP2_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		CCAP2H = (u8)(CCAP2_tmp >> 8);	//��дCCAP0H
	}

	pas.pca_int_cnt = 0x0;
	pas.time_ab = 0x0;
	pas.time_bc = 0x0;
	pas.phaseSeq = 0x0;
	pas.phaseALossFlag = 1;
	pas.phaseBLossFlag = 1;
	pas.phaseCLossFlag = 1;
	pas.phaseSeqIsRight = 0;
}


//========================================================================
// ����: void	PCA_Handler (void) interrupt PCA_VECTOR
// ����: PCA�жϴ�������.
// ����: None
// ����: none.
// �汾: V1.0, 2012-11-22
//========================================================================
#if 0
void	PCA_Handler (void) interrupt PCA_VECTOR
{
	if(CCF0)		//PCA模块0中断
	{
		CCF0 = 0;		//清PCA模块0中断标志
		if(PCA0_mode >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
			CCAP0_tmp += PCA_Timer0;
			CCAP0L = (u8)CCAP0_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
			CCAP0H = (u8)(CCAP0_tmp >> 8);	//后写CCAP0H
		}
		else if(PCA0_mode == PCA_Mode_Capture)
		{
			CCAP0_tmp = CCAP0H;	//读CCAP0H
			CCAP0_tmp = (CCAP0_tmp << 8) + CCAP0L;
			B_Capture0 = 1;
		}
	}

	if(CCF1)	//PCA模块1中断
	{
		CCF1 = 0;		//清PCA模块1中断标志
		if(PCA1_mode >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
			CCAP1_tmp += PCA_Timer1;
			CCAP1L = (u8)CCAP1_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
			CCAP1H = (u8)(CCAP1_tmp >> 8);	//后写CCAP0H
		}
		else if(PCA1_mode == PCA_Mode_Capture)
		{
			CCAP1_tmp = CCAP1H;	//读CCAP1H
			CCAP1_tmp = (CCAP1_tmp << 8) + CCAP1L;
			B_Capture1 = 1;
		}
	}

	if(CCF2)	//PCA模块2中断
	{
		CCF2 = 0;		//清PCA模块1中断标志
		if(PCA2_mode >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
			CCAP2_tmp += PCA_Timer2;
			CCAP2L = (u8)CCAP2_tmp;			//将影射寄存器写入捕获寄存器，先写CCAP0L
			CCAP2H = (u8)(CCAP2_tmp >> 8);	//后写CCAP0H
		}
		else if(PCA2_mode == PCA_Mode_Capture)
		{
			CCAP2_tmp = CCAP2H;	//读CCAP2H
			CCAP2_tmp = (CCAP2_tmp << 8) + CCAP2L;
			B_Capture2 = 1;
		}
	}

	if(CF)	//PCA溢出中断
	{
		CF = 0;			//清PCA溢出中断标志
	}

}
#endif

void	PCA_Handler (void) interrupt PCA_VECTOR
{

	if(CCF0)		//PCAģ��0�ж�
	{		
		CCF0 = 0;		//��PCAģ��0�жϱ�־
		CCAP0L = 0x0; CCAP0H = 0x0;

		CCAP1_tmp = CCAP1H;	//	
		CCAP1_tmp = (CCAP1_tmp << 8) + CCAP1L;

		B_Capture0 = 1;
		pas.phaseALossFlag = 0;
	}

	if(CCF1)	//PCAģ��1�ж�
	{
		CCF1 = 0;		//��PCAģ��1�жϱ�־
		CCAP1L = 0x0; CCAP1H = 0x0;

		CCAP2_tmp = CCAP2H;	//��CCAP1H
		CCAP2_tmp = (CCAP2_tmp << 8) + CCAP2L;

		CCAP_tmp = CCAP1H;	//��CCAP0H
		CCAP_tmp = (CCAP_tmp << 8) + CCAP1L;

		pas.time_ab  = CCAP_tmp - CCAP1_tmp;

		B_Capture1 = 1;
		pas.phaseBLossFlag = 0;
	}

	if(CCF2)	//PCAģ��2�ж�
	{
		CCF2 = 0;		//��PCAģ��1�жϱ�־
		CCAP2L = 0x0; CCAP2H = 0x0;

		CCAP_tmp = CCAP2H;	//��CCAP2H
		CCAP_tmp = (CCAP2_tmp << 8) + CCAP2L;

		pas.time_bc  = CCAP_tmp - CCAP2_tmp;
	
		pas.phaseCLossFlag = 0;
		B_Capture2 = 1;
	}

	if(CF)	//PCA����ж�
	{
		//ledDisplayCtrl(5,'c');

		//CCAP_tmp = CCAP0H;	//	
		//CCAP_tmp = (CCAP_tmp << 8) + CCAP0L;
		//if(CCAP_tmp == 0 || CCAP_tmp > 0xFFF0) pas.phaseALossFlag = 1;

		//CCAP_tmp = CCAP1H;	//void	PCA_Handler (void) interrupt PCA_VECTOR
		//CCAP_tmp = (CCAP_tmp << 8) + CCAP1L;
		//if(CCAP_tmp == 0 || CCAP_tmp > 0xFFF0) pas.phaseBLossFlag = 1; 

		//CCAP_tmp = CCAP2H;	//��CCAP0H
		//CCAP_tmp = (CCAP_tmp << 8) + CCAP2L;
		//if(CCAP_tmp == 0 || CCAP_tmp > 0xFFF0) pas.phaseCLossFlag = 1;

		CF = 0;			//清PCA溢出中断标志
	}

	/* 判断相序 */
	pas.phaseSeq &= (0xF << ((pas.pca_int_cnt % 3) * 4));
	if(B_Capture0) {
		B_Capture0 = 0;
		pas.phaseSeq |= (0xA << ((pas.pca_int_cnt % 3) * 4));
	} else if(B_Capture1) {
		B_Capture1 = 0;
		pas.phaseSeq |= (0xB << ((pas.pca_int_cnt % 3) * 4));
	} else if(B_Capture2) {
		B_Capture2 = 0;
		pas.phaseSeq |= (0xC << ((pas.pca_int_cnt % 3) * 4));
	}

	/* ABC phase interrupt were complete */
    if(pas.pca_int_cnt % 3 == 2) {

		if((pas.phaseSeq & 0x000F != 0xA) && (pas.phaseSeq & 0x00F0 != 0xA0) && (pas.phaseSeq & 0x0F00 != 0xA00))
			pas.phaseALossFlag = 1;
		else pas.phaseALossFlag = 0;
		if((pas.phaseSeq & 0x000F != 0xB) && (pas.phaseSeq & 0x00F0 != 0xB0) && (pas.phaseSeq & 0x0F00 != 0xB00)) 
			pas.phaseBLossFlag = 1;
		else pas.phaseBLossFlag = 0;
		if((pas.phaseSeq & 0x000F != 0xC) && (pas.phaseSeq & 0x00F0 != 0xC0) && (pas.phaseSeq & 0x0F00 != 0xC00))
			pas.phaseCLossFlag = 1;
		else pas.phaseCLossFlag = 0;

		if((pas.time_ab > PHASE_TMIN && pas.time_ab < PHASE_TMAX)
			&& (pas.time_bc > PHASE_TMIN && pas.time_bc < PHASE_TMAX)
			&& (pas.phaseSeq == 0xABC || pas.phaseSeq == 0xBCA || pas.phaseSeq == 0xCAB)) {
			pas.phaseSeqIsRight = 1;
		} else {
			pas.phaseSeqIsRight = 0;
		}

		/* Only for debug */
		if(pas.phaseALossFlag) MSR_LedFlashCtrl(MSR_LED_UP_INDICAT);
		else MSR_LedStatusCtrl(MSR_LED_UP_INDICAT, LED_OFF);

		if(pas.phaseBLossFlag) MSR_LedFlashCtrl(MSR_LED_OVER_LOADING);
		else MSR_LedStatusCtrl(MSR_LED_OVER_LOADING, LED_OFF);

		if(pas.phaseCLossFlag) MSR_LedFlashCtrl(MSR_LED_DOWN_INDICAT);
		else MSR_LedStatusCtrl(MSR_LED_DOWN_INDICAT, LED_OFF);

		if(pas.phaseSeqIsRight) ledDisplayCtrl(1,'y');
		else ledDisplayCtrl(1,'n');
	}

	pas.pca_int_cnt++;
}

u16 checkACPowerPhaseSequence(void)
{
    /* F:����  0������ ABC����Ӧ��P3.5 P3.6 P3.7 */
    u16 phaseSeq = 0xFABC;//0x0ACB

    if(pas.phaseALossFlag) phaseSeq &=  0x00FF;
    else { phaseSeq &=  0x00FF; phaseSeq |=  0x0AFF;}

    if(pas.phaseBLossFlag) phaseSeq &=  0x0F0F;
    else { phaseSeq &=  0x0F0F; phaseSeq |=  0x0FBF;}

    if(pas.phaseCLossFlag) phaseSeq &=  0x0FF0;
    else { phaseSeq &=  0x0FF0; phaseSeq |=  0x0FFC;}

    if(pas.phaseSeqIsRight) phaseSeq |= 0xF000;
    else {
        phaseSeq &=  0x0FFF;
        phaseSeq = (phaseSeq & 0xFF00) | ((phaseSeq & 0x000F) << 4) | ((phaseSeq & 0x00F0) >> 4);
    }
 
    return (phaseSeq);
}