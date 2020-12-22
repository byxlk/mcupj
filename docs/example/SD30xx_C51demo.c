
//************************************************************
// ʵʱʱ��SD30XX��дC51��ʾ����
// MCU: STC15W1K16S   @11.0592MHz
// E-mail:   FAE@whwave.com.cn
// TEL��		 0755-83114387
// Last update:                   2016/11/8
//************************************************************

#include <reg51.h>
#include <intrins.h>

/******************** ���ʹ�ܺ궨�� ********************/
#define		EN_CHARGE				0		// 0:���ó�繦��  1:ʹ�ܳ�繦��

/******************** RTC Device address ********************/
#define		RTC_Address			0x64

/******************** Alarm register ********************/
#define		Alarm_SC	0x07
#define		Alarm_MN	0x08
#define		Alarm_HR	0x09
#define		Alarm_WK	0x0A
#define		Alarm_DY	0x0B
#define		Alarm_MO	0x0C
#define		Alarm_YR	0x0D
#define		Alarm_EN	0x0E

/******************** Control register ********************/
#define		CTR1					0x0F
#define		CTR2					0x10
#define		CTR3					0x11

/***************** Timer Counter Register ***********************/
#define		Timer_Counter1	0x13
#define		Timer_Counter2	0x14
#define		Timer_Counter3	0x15

/******************** Battery register ********************/
#define		CHARGE_MG			0x18		//������Ĵ�����ַ
#define		Bat_bit8			0x1A		//�������λ�Ĵ�����ַ
#define		Bat_Low8			0x1B		//�����Ͱ�λ�Ĵ�����ַ

/**********************************************************/
#define		IDcode				0x72		//8�ֽ�ID����ʼ��ַ
#define		UserRam_30H		0x30		//�û�ram 30H��ַ

#define		TRUE  				1
#define 	FALSE 				0

//*********������IO�ڶ���*********
typedef unsigned char uchar;
typedef unsigned int uint;

sfr AUXR  = 0x8e;               //�����Ĵ���
sfr T2H   = 0xd6;               //��ʱ��2��8λ
sfr T2L   = 0xd7;               //��ʱ��2��8λ

sbit SDA	=	P1^5;
sbit SCL	=	P1^6;

uchar		Sram[8];	//ͨ�����ݻ�����
typedef	struct
{
	uchar second;
	uchar	minute;
	uchar	hour;
	uchar	week;
	uchar	day;
	uchar	month;
	uchar	year;
}S_Time;	

S_Time  RTC = {0x55,0x59,0x14,0x02,0x26,0x01,0x16};	//��ʼ��ʱ��ṹ�����������ʱ�䣺2016��1��26�� 14:59:55  ���ڶ���
//           55�� 59�� 14ʱ �ܶ� 26�� 1�� 16��

/********��������********/
uchar I2CReadOneByte(uchar add);//��һ�ֽ�
bit 	I2CWriteOneByte(uchar add, uchar date);//дһ�ֽ�
uchar I2CReadSerial(uchar Address, uchar length,uchar *ps);//������
uchar I2CWriteSerial(uchar Address, uchar length,uchar *ps);//����д
bit 	I2CWriteDate(S_Time	*SetRTC);//дʱ��	
bit 	I2CReadDate(S_Time	*psRTC);//��ʱ��
bit		WriteRTC_Enable(void);//д����
bit		WriteRTC_Disable(void);//д��ֹ

/*********I2C��ʱ4us***********/
void I2CWait(void)//4us  @11.0592MHz
{	
	unsigned char i;

	_nop_();
	_nop_();
	i = 8;
	while (--i);
}

/********����I2C����********/
bit I2CStart(void)
{
	SDA = 1;
	I2CWait();
	SCL = 1;
	I2CWait();
	if(!SDA)return FALSE;	//SDA��Ϊ�͵�ƽ������æ,�˳�
	SDA = 0;
	I2CWait();
	SCL = 0;
	I2CWait();
	return TRUE;
}

/********ֹͣI2C����*******/
void I2CStop(void)
{
	SDA = 0;
	I2CWait();
	SCL = 0;
	I2CWait();
	SCL = 1;
	I2CWait();
	SDA = 1;
}

/*********���� ACK*********/
void I2CAck(void)
{	
	SDA = 0;
	SCL = 0;
	I2CWait();
	SCL = 1;
	I2CWait();
	SCL = 0;
}

/*********����NO ACK*********/
void I2CNoAck(void)
{	
	SDA = 1;
	SCL = 0;
	I2CWait();
	SCL = 1;
	I2CWait();
	SCL = 0;
}

/*********��ȡACK�ź�*********/
bit I2CWaitAck(void) 	 //����Ϊ:1=��ACK,0=��ACK
{
	SCL = 0;
	SDA = 1;		//����SDAΪ���루�������͵ĵ�Ƭ����Ҫ����IO��������Ĵ�����
	I2CWait();
	SCL = 1;
	I2CWait();
	while(SDA)
	{
		SCL = 0;
		return FALSE;
	}
	SCL = 0;
	return TRUE;
}

/************I2C����8Bit*************/
void I2CSendByte(uchar demand) //���ݴӸ�λ����λ//
{
	uchar i = 8;                                                    
	
	while(i--)
	{
		SCL = 0;
		_nop_();
		SDA=(bit)(demand&0x80);
		demand <<= 1;
		I2CWait();
		SCL = 1;
		I2CWait();
	}
	SCL = 0;
}

/*********I2C����8Bit*********/
uchar I2CReceiveByte(void)      //���ݴӸ�λ����λ//
{
	uchar i = 8;
	uchar ddata = 0;
	SDA = 1;			//����SDAΪ���루�������͵ĵ�Ƭ����Ҫ����IO��������Ĵ�����
	while(i--)
	{
		ddata <<= 1;      //���ݴӸ�λ��ʼ��ȡ
		SCL = 0;
		I2CWait();
		SCL = 1;
		I2CWait();	//�Ӹ�λ��ʼ ddata|=SDA;ddata<<=1
		if(SDA)
		{
			ddata |= 0x01;
		}
	}
	SCL = 0;
	return ddata;
}

/******I2Cдһ���ֽ�******/
bit I2CWriteOneByte(uchar add, uchar date)
{		
	if(!I2CStart())return FALSE;
	I2CSendByte(RTC_Address);      
	if(!I2CWaitAck()){I2CStop(); return FALSE;} 
	I2CSendByte(add);		//����д��ַ      
	I2CWaitAck();	
	I2CSendByte(date);		//д����
	I2CWaitAck();	
	I2CStop(); 
	return	TRUE;
}

/******I2C��һ���ֽڳ���******/
uchar I2CReadOneByte(uchar add)
{		
	uchar dat;
	if(!I2CStart())return FALSE;
	I2CSendByte(RTC_Address);      
	if(!I2CWaitAck()){I2CStop(); return FALSE;}
  I2CSendByte(add);		//����Ҫ���ĵ�ַ
	I2CWaitAck();
	I2CStart();	
 	I2CSendByte(RTC_Address+1);	
	I2CWaitAck();	
	dat = I2CReceiveByte();		//������
	I2CNoAck();
	I2CStop(); 
	return  dat;
}

/******RTCд�������******/
bit WriteRTC_Enable(void)
{		
	if(!I2CWriteOneByte(CTR2,0x80))return FALSE;
	I2CWriteOneByte(CTR1,0x84);
	return	TRUE;
}

/******RTCд��ֹ����******/
bit WriteRTC_Disable(void)
{		
	if(!I2CWriteOneByte(CTR1,0))return FALSE;
	I2CWriteOneByte(CTR2,0);
	return	TRUE;
}

/******��RTCʵʱ���ݼĴ���******/
bit I2CReadDate(S_Time	*psRTC)
{
	
	if(!I2CStart())return FALSE;
	I2CSendByte(RTC_Address+1); 
  if(!I2CWaitAck()){I2CStop(); return FALSE;}
	psRTC->second = I2CReceiveByte();
	I2CAck();
	psRTC->minute = I2CReceiveByte();
	I2CAck();
	psRTC->hour = I2CReceiveByte() & 0x3f;
	I2CAck();
	psRTC->week = I2CReceiveByte();
	I2CAck();
	psRTC->day = I2CReceiveByte();
	I2CAck();
	psRTC->month = I2CReceiveByte();
	I2CAck();
	psRTC->year = I2CReceiveByte();
	I2CNoAck();		//��ʱ����ɣ�����NoAck
	I2CStop();
	return	TRUE;
}

/******дRTCʵʱ���ݼĴ���******/
bit I2CWriteDate(S_Time	*psRTC)	//дʱ�����Ҫ��һ�ζ�ʵʱʱ��Ĵ���(00H~06H)����д�룬
{                               //�����Ե�����7��ʱ�������е�ĳһλ����д����,������ܻ�����ʱ�����ݵĴ����λ. 
                                //Ҫ�޸�����ĳһ������ , Ӧһ����д��ȫ�� 7 ��ʵʱʱ������.

	WriteRTC_Enable();				//ʹ�ܣ�����
	
	I2CStart();
	I2CSendByte(RTC_Address); 
	if(!I2CWaitAck()){I2CStop(); return FALSE;}
	I2CSendByte(0x00);			//����д��ʼ��ַ      
	I2CWaitAck();	
	I2CSendByte(psRTC->second);		//second     
	I2CWaitAck();	
	I2CSendByte(psRTC->minute);		//minute      
	I2CWaitAck();	
	I2CSendByte(psRTC->hour|0x80);		//hour ,ͬʱ����Сʱ�Ĵ������λ��0��Ϊ12Сʱ�ƣ�1��Ϊ24Сʱ�ƣ�
	I2CWaitAck();	
	I2CSendByte(psRTC->week);		//week      
	I2CWaitAck();	
	I2CSendByte(psRTC->day);		//day      
	I2CWaitAck();	
	I2CSendByte(psRTC->month);		//month      
	I2CWaitAck();	
	I2CSendByte(psRTC->year);		//year      
	I2CWaitAck();	
	I2CStop();
	
	WriteRTC_Disable();				//ʹ�ܣ�����
	return	TRUE;
}


/******����RTC�����ж���ʾ������ʾ******/
void WriteALARM(void)				//���ñ���ʱ�䣺2016��2��14�� 8��00
{						
	WriteRTC_Enable();
	I2CWriteOneByte(Alarm_HR,0x08);	//8ʱ
	I2CWriteOneByte(Alarm_DY,0x14);	//14��
	I2CWriteOneByte(Alarm_MO,0x02);	//02��
	I2CWriteOneByte(Alarm_YR,0x16);	//16��
	I2CWriteOneByte(Alarm_EN,0x74);	//���ñ�������ʹ���ꡢ�¡��ա�Сʱ������
	I2CWriteOneByte(CTR2,0x92);	//����INT�ж�ѡͨ��INTS1��INTS0�����������ж�������λ��INTAE��
	WriteRTC_Disable();
}

/******����RTC����ʱ�ж���ʾ******/
void SetDjs(void)							//���õ���ʱ�ж�
{
	WriteRTC_Enable();
	I2CWriteOneByte(CTR2,0xF0);//���嵹��ʱ�ж�������λ��INTDE��
	I2CWriteOneByte(CTR2,0xF4);//�����������жϣ�IM=1��INT�ж�ѡͨ��INTS1��INTS0�������õ���ʱ�ж�������λ��INTDE��
	I2CWriteOneByte(CTR3,0x20);//ѡ��ʱ��Ƶ��Դ��TDS1��TDS0��Ϊ1HZ
	I2CWriteOneByte(Timer_Counter1,5);//����ʱ��ֵ�Ĵ���������8λ����ʱ������ֵ��5s��
	I2CWriteOneByte(Timer_Counter2,0);
	I2CWriteOneByte(Timer_Counter3,0);
	WriteRTC_Disable();
}

/******����RTCƵ���ж���ʾ******/
void SetFrq_2Hz(void)					
{
	WriteRTC_Enable();
	I2CWriteOneByte(CTR2,0xA1);	//ѡͨƵ���жϣ�INTS1��INTS0��������Ƶ���ж�������λ��INTFE��
	I2CWriteOneByte(CTR3,0x09);	//����2HzƵ���ж�
	WriteRTC_Disable();
}
/******��ֹRTC�ж�******/
void ClrINT(void)         
{
	WriteRTC_Enable();
	I2CWriteOneByte(CTR2,0x80);
	WriteRTC_Disable();
}
/************RTC����������ֽ�************/
uchar I2CReadSerial(uchar Address, uchar length,uchar *ps)
{
	uchar	i;
	I2CStart();
	I2CSendByte(RTC_Address);      
	if(!I2CWaitAck()){I2CStop(); return FALSE;}
	I2CSendByte(Address);			//����Ҫ���ĵ�ַ
	I2CWaitAck();
	I2CStart();	
 	I2CSendByte(RTC_Address+1);
	I2CWaitAck();
	for(i = 0;i<length-1;i++,ps++)
	{
		*ps = I2CReceiveByte();		//������
		I2CAck();
	}
	*ps = I2CReceiveByte();	
	I2CNoAck();
	I2CStop(); 
	return	TRUE;
}

//|******************RTC����д����ֽ�******************|
uchar RTCWriteSerial(uchar Address, uchar length,uchar *ps)
{
	uchar	i;
	WriteRTC_Enable();
	
	I2CStart();
	I2CSendByte(RTC_Address);      	//������ַ
	if(!I2CWaitAck()){I2CStop(); return FALSE;}
	I2CSendByte(Address);			//������ʼ��ַ
	I2CWaitAck();
	for(i = 0;i<length;i++)
	{ 	
		I2CSendByte(*(ps++));		
		I2CAck();			
	}
	I2CStop(); 
	
	WriteRTC_Disable();
	return	TRUE;
}

void UartInit(void)		//115200bps@11.0592MHz
{
	SCON = 0x50;		//8λ����,�ɱ䲨����
	AUXR |= 0x01;		//����1ѡ��ʱ��2Ϊ�����ʷ�����
	AUXR |= 0x04;		//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xE8;		//�趨��ʱ��ֵ
	T2H = 0xFF;		//�趨��ʱ��ֵ
	AUXR |= 0x10;		//������ʱ��2
}

void UartSend(uchar dat)
{
		SBUF = dat;
		while(!TI);
		TI = 0;	
}

void TxString(uchar *puts)		//���ڷ����ַ���
{
  for (; *puts != 0;	puts++)   	
	{
		SBUF = *puts;
		while(!TI);
		TI = 0;
	}
}

void RTC_display(void)
{
		TxString("\r\n\r\n");
		UartSend((RTC.hour >> 4) + '0');
		UartSend((RTC.hour & 0x0f) + '0');
		TxString("ʱ");
		UartSend((RTC.minute >> 4) + '0');
		UartSend((RTC.minute & 0x0f) + '0');
		TxString("��");
		UartSend((RTC.second >> 4) + '0');
		UartSend((RTC.second & 0x0f) + '0');
		TxString("��  ");

		UartSend((RTC.year >> 4) + '0');
		UartSend((RTC.year & 0x0f) + '0');
		TxString("��");
		UartSend((RTC.month >> 4) + '0');
		UartSend((RTC.month & 0x0f) + '0');
		TxString("��");
		UartSend((RTC.day >> 4) + '0');
		UartSend((RTC.day & 0x0f) + '0');
		TxString("��  ����");
		UartSend(RTC.week + '0');
}

/*********��ʱ�ӳ���*********/
void Delay1ms()		//@11.0592MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	_nop_();
	i = 11;
	j = 190;
	do
	{
		while (--j);
	} while (--i);
}

void Delayms(uint n)
{
	while(n--)
		Delay1ms();
}
//////*****��������ʾ*****//////
void main()
{
	uint		Bat;
	uchar  	Batbit8, Batbit0_7;
	
	UartInit();//115200bps@11.0592MHz
	I2CWriteDate(&RTC);	//����ʱ����ʾ��16��1��26��......
//	WriteALARM();    	//���ñ����ж�ʱ����ʾ
//	SetDjs();		 	//���õ���ʱ�ж���ʾ
	SetFrq_2Hz();			//����Ƶ���жϣ���INT�������������ʾ
	
	I2CReadSerial(IDcode,8,&Sram);		//���ڲ�8�ֽ�ID����ʾ
	RTCWriteSerial(UserRam_30H,8,&Sram);		//���ڲ�8�ֽڵ�ID��д���û�ͨ�ô洢����0x30-0x37��ַ��ʾ
	
	WriteRTC_Enable();	
#if EN_CHARGE		
	I2CWriteOneByte(CHARGE_MG,0x82);//��ӳ���ص�ʹ�ܳ�繦��
#else
	I2CWriteOneByte(CHARGE_MG,0);//���һ���Ե�صĽ�ֹ��繦�ܣ������𻵵��
#endif	
	WriteRTC_Disable();
	
	while(1)
	{
		I2CReadDate(&RTC);		//��ʱ����ʾ
		RTC_display();	//��ʱ�����ݴӴ��ڴ������ʾ

		Batbit8 = I2CReadOneByte(Bat_bit8);		//��SD25XX�ĵ�ص������λ
		Batbit0_7 = I2CReadOneByte(Bat_Low8);		//��SD25XX�ĵ�ص����Ͱ�λ
		Bat = (Batbit8>>7)*255+Batbit0_7;				//�����ص���ֵ��ʾ����Bat=285���ʾ2.85V
		TxString("  ��ص�ѹ:");
		UartSend(Bat/100 + '0');
		TxString(".");
		UartSend(Bat%100/10 + '0');
		UartSend(Bat%10 + '0');
		TxString("V\r\n");
		
		
		Delayms(1000);			//��ʱ1000ms��1s��1��
	}

}
//�ر����ѣ���дʵʱʱ������ʱ (00H~06H), �����Ե��� �� 7 ��ʱ�������е�ĳһλ����д���� ,
//������ܻ�����ʱ�����ݵĴ����λ , ����Ҫ�޸�����ĳһ������ , Ӧһ����д��ȫ�� 7 ��ʵʱʱ������ .