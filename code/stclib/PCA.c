#include "api_config.h"
#include	"PCA.h"

bit		B_Capture0,B_Capture1,B_Capture2;
u8		PCA0_mode,PCA1_mode,PCA2_mode;
u16		CCAP0_tmp,PCA_Timer0;
u16		CCAP1_tmp,PCA_Timer1;
u16		CCAP2_tmp,PCA_Timer2;

/*************	����˵��	**************


******************************************/

u16	PWM0_low;	//PWM����͵�ƽ��PCAʱ���������, �û��㲻�ɼ���
u16	PWM1_low;	//PWM����͵�ƽ��PCAʱ���������, �û��㲻�ɼ���
u16	PWM2_low;	//PWM����͵�ƽ��PCAʱ���������, �û��㲻�ɼ���

static bool phaseSeqIsRight = 0;
static u8 phaseSeqCheckCount = 0;

static bool phaseALossFlag = 1;
static bool phaseBLossFlag = 1;
static bool phaseCLossFlag = 1;

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
}


//========================================================================
// ����: void	PCA_Handler (void) interrupt PCA_VECTOR
// ����: PCA�жϴ�������.
// ����: None
// ����: none.
// �汾: V1.0, 2012-11-22
//========================================================================
void	PCA_Handler (void) interrupt PCA_VECTOR
{
	if(CCF0)		//PCAģ��0�ж�
	{
		//CCF0 = 0;		//��PCAģ��0�жϱ�־
        if(PCA0_mode >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
		    //if(P25)	CCAP0_tmp += PCA_Timer0;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		    //else	CCAP0_tmp += PWM0_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
            CCAP0_tmp += PCA_Timer0;
		    CCAP0L = (u8)CCAP0_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		    CCAP0H = (u8)(CCAP0_tmp >> 8);	//��дCCAP0H
		}
		else if(PCA0_mode == PCA_Mode_Capture)
		{
			phaseALossFlag = 0;
			CCAP1_tmp = CCAP1H;	//��CCAP0H
			CCAP1_tmp = (CCAP1_tmp << 8) + CCAP1L;
			
			B_Capture0 = 1;
		}
	}

	if(CCF1)	//PCAģ��1�ж�
	{
		//CCF1 = 0;		//��PCAģ��1�жϱ�־
        if(PCA1_mode >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
		    //if(P26)	CCAP1_tmp += PCA_Timer1;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		    //else	CCAP1_tmp += PWM1_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
			CCAP1_tmp += PCA_Timer1;
		    CCAP1L = (u8)CCAP1_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		    CCAP1H = (u8)(CCAP1_tmp >> 8);	//��дCCAP0H
		}
		else if(PCA1_mode == PCA_Mode_Capture)
		{
			phaseBLossFlag = 0;
			if(B_Capture0) {
				CCAP2_tmp = CCAP2H;	//��CCAP1H
				CCAP2_tmp = (CCAP2_tmp << 8) + CCAP2L;
				
				B_Capture1 = 1;
			}
		}
	}

	if(CCF2)	//PCAģ��2�ж�
	{
		CCF2 = 0;		//��PCAģ��1�жϱ�־
        if(PCA2_mode >= PCA_Mode_SoftTimer)		//PCA_Mode_SoftTimer and PCA_Mode_HighPulseOutput
		{
		    //if(P27)	CCAP2_tmp += PCA_Timer2;	//���Ϊ�ߵ�ƽ�����Ӱ��Ĵ���װ�ظߵ�ƽʱ�䳤��
		    //else	CCAP2_tmp += PWM2_low;	//���Ϊ�͵�ƽ�����Ӱ��Ĵ���װ�ص͵�ƽʱ�䳤��
            CCAP2_tmp += PCA_Timer2;
		    CCAP2L = (u8)CCAP2_tmp;			//��Ӱ��Ĵ���д�벶��Ĵ�������дCCAP0L
		    CCAP2H = (u8)(CCAP2_tmp >> 8);	//��дCCAP0H
        }
		else if(PCA2_mode == PCA_Mode_Capture)
		{
			phaseCLossFlag = 0;
			if(B_Capture0 && B_Capture1) {
				//CCAP2_tmp = CCAP2H;	//��CCAP2H
				//CCAP2_tmp = (CCAP2_tmp << 8) + CCAP2L;
				B_Capture0 = 0;
				B_Capture1 = 0;
				B_Capture2 = 1;
			
				CCF0 = 0;		//��PCAģ��0�жϱ�־
				CCF1 = 0;		//��PCAģ��1�жϱ�־
			}
		}
	}

	if(CF)	//PCA����ж�
	{
		CF = 0;			//��PCA����жϱ�־
	}
}

u16 checkACPowerPhaseSequence(void)
{
    /* F:����  0������ ABC����Ӧ��P3.5 P3.6 P3.7 */
    u16 phaseSeq = 0xFABC;//0x0ACB

    //if(phaseALossFlag) phaseSeq &=  0x00FF;
    //else { phaseSeq &=  0x00FF; phaseSeq |=  0x0AFF;}

    //if(phaseBLossFlag) phaseSeq &=  0x0F0F;
    //else { phaseSeq &=  0x0F0F; phaseSeq |=  0x0FBF;}

    //if(phaseCLossFlag) phaseSeq &=  0x0FF0;
    //else { phaseSeq &=  0x0FF0; phaseSeq |=  0x0FFC;}

    //if(phaseSeqIsRight) phaseSeq |= 0xF000;
    //else {
    //    phaseSeq &=  0x0FFF;
    //    phaseSeq = (phaseSeq & 0xFF00) | ((phaseSeq & 0x000F) << 4) | ((phaseSeq & 0x00F0) >> 4);
    //}
 
    return (phaseSeq);
}