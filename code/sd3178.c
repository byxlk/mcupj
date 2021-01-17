#pragma code
#include <intrins.h>
#include "api_config.h"

/******************** 充电使能宏定义 ********************/
#define		EN_CHARGE			(0)	// 0:禁用充电功能  1:使能充电功能

/******************** RTC Device address ********************/
#define		RTC_Address			(0x64)

/******************** Alarm register ********************/
#define		Alarm_SC	        (0x07)
#define		Alarm_MN	        (0x08)
#define		Alarm_HR	        (0x09)
#define		Alarm_WK	        (0x0A)
#define		Alarm_DY	        (0x0B)
#define		Alarm_MO	        (0x0C)
#define		Alarm_YR	        (0x0D)
#define		Alarm_EN	        (0x0E)

/******************** Control register ********************/
#define		CTR1				(0x0F)
#define		CTR2				(0x10)
#define		CTR3				(0x11)

/***************** Timer Counter Register ***********************/
#define		Timer_Counter1	    (0x13)
#define		Timer_Counter2	    (0x14)
#define		Timer_Counter3	    (0x15)

/******************** Battery register ********************/
#define		CHARGE_MG			(0x18)		//充电管理寄存器地址
#define		Bat_bit8			(0x1A)		//电量最高位寄存器地址
#define		Bat_Low8			(0x1B)		//电量低八位寄存器地址

/**********************************************************/
#define		IDcode				(0x72)		//8字节ID号起始地址
#define		UserRam_30H		    (0x30)		//用户ram 30H地址

//*********变量及IO口定义*********
unsigned char		Sram[8];	//通用数据缓存器
typedef	struct
{
	unsigned char   second;
	unsigned char	minute;
	unsigned char	hour;
	unsigned char	week;
	unsigned char	day;
	unsigned char	month;
	unsigned char	year;
}S_Time;	

//初始化时间结构体变量（设置时间：2016年1月26日 14:59:55  星期二）
//           55秒 59分 14时 周二 26日 1月 16年
S_Time  RTC = {0x55,0x59,0x14,0x02,0x26,0x01,0x16};	

/********函数声明********/
//unsigned char I2CReadOneByte(unsigned char add);//读一字节
//unsigned char I2CReadSerial(unsigned char Address, unsigned char length,unsigned char *ps);//连续读
//unsigned char I2CWriteSerial(unsigned char Address, unsigned char length,unsigned char *ps);//连续写
//bool I2CWriteDate(S_Time	*SetRTC);//写时间	
//bool I2CReadDate(S_Time	*psRTC);//读时间
//bool WriteRTC_Enable(void);//写允许
//bool WriteRTC_Disable(void);//写禁止
//bool I2CWriteOneByte(unsigned char add, unsigned char date);//写一字节

/*********I2C延时4us***********/
void I2CWait(void)//4us  @22.1184MHz
{	
	unsigned char i;

	_nop_();
	_nop_();
	i = 19;
	while (--i);
}

/********开启I2C总线********/
static bool I2CStart(void)
{
	MSR_SD3178_SDA = 1;
	I2CWait();
	MSR_SD3178_SCK = 1;
	I2CWait();

	if(!MSR_SD3178_SDA) return FALSE;	//SDA线为低电平则总线忙,退出

	MSR_SD3178_SDA = 0;
	I2CWait();
	MSR_SD3178_SCK = 0;
	I2CWait();
	return TRUE;
}

/********停止I2C总线*******/
static void I2CStop(void)
{
	MSR_SD3178_SDA = 0;
	I2CWait();
	MSR_SD3178_SCK = 0;
	I2CWait();
	MSR_SD3178_SCK = 1;
	I2CWait();
	MSR_SD3178_SDA = 1;
}

/*********发送 ACK*********/
static void I2CAck(void)
{	
	MSR_SD3178_SDA = 0;
	MSR_SD3178_SCK = 0;
	I2CWait();
	MSR_SD3178_SCK = 1;
	I2CWait();
	MSR_SD3178_SCK = 0;
}

/*********发送NO ACK*********/
static void I2CNoAck(void)
{	
	MSR_SD3178_SDA = 1;
	MSR_SD3178_SCK = 0;
	I2CWait();
	MSR_SD3178_SCK = 1;
	I2CWait();
	MSR_SD3178_SCK = 0;
}

/*********读取ACK信号*********/
static bool I2CWaitAck(void) 	 //返回为:1=有ACK,0=无ACK
{
	MSR_SD3178_SCK = 0;
	MSR_SD3178_SDA = 1;		//设置SDA为输入（其它类型的单片机需要配置IO输入输出寄存器）
	I2CWait();
	MSR_SD3178_SCK = 1;
	I2CWait();
	while(MSR_SD3178_SDA)
	{
		MSR_SD3178_SCK = 0;
		return FALSE;
	}
	MSR_SD3178_SCK = 0;
	return TRUE;
}

/************I2C发送8Bit*************/
static void I2CSendByte(unsigned char demand) //数据从高位到低位//
{
	unsigned char i = 8;                                                    
	
	while(i--)
	{
		MSR_SD3178_SCK = 0;
		_nop_();
		MSR_SD3178_SDA=(bool)(demand&0x80);
		demand <<= 1;
		I2CWait();
		MSR_SD3178_SCK = 1;
		I2CWait();
	}
	MSR_SD3178_SCK = 0;
}

/*********I2C读入8Bit*********/
static unsigned char I2CReceiveByte(void)      //数据从高位到低位//
{
	unsigned char i = 8;
	unsigned char ddata = 0;
	MSR_SD3178_SDA = 1;			//设置SDA为输入（其它类型的单片机需要配置IO输入输出寄存器）
	while(i--)
	{
		ddata <<= 1;      //数据从高位开始读取
		MSR_SD3178_SCK = 0;
		I2CWait();
		MSR_SD3178_SCK = 1;
		I2CWait();	//从高位开始 ddata|=MSR_SD3178_SDA;ddata<<=1
		if(MSR_SD3178_SDA)
		{
			ddata |= 0x01;
		}
	}
	MSR_SD3178_SCK = 0;
	return ddata;
}

/******I2C写一个字节******/
static bool I2CWriteOneByte(unsigned char add, unsigned char date)
{		
	if(!I2CStart())return FALSE;
	I2CSendByte(RTC_Address);      
	if(!I2CWaitAck()){I2CStop(); return FALSE;} 
	I2CSendByte(add);		//设置写地址      
	I2CWaitAck();	
	I2CSendByte(date);		//写数据
	I2CWaitAck();	
	I2CStop(); 
	return	TRUE;
}

/******I2C读一个字节程序******/
static unsigned char I2CReadOneByte(unsigned char add)
{		
	unsigned char dat;
	if(!I2CStart())return FALSE;
	I2CSendByte(RTC_Address);      
	if(!I2CWaitAck()){I2CStop(); return FALSE;}
    I2CSendByte(add);		//设置要读的地址
	I2CWaitAck();
	I2CStart();	
 	I2CSendByte(RTC_Address+1);	
	I2CWaitAck();	
	dat = I2CReceiveByte();		//读数据
	I2CNoAck();
	I2CStop(); 
	return  dat;
}

/******RTC写允许程序******/
static bool WriteRTC_Enable(void)
{		
	if(!I2CWriteOneByte(CTR2,0x80))return FALSE;
	I2CWriteOneByte(CTR1,0x84);
	return	TRUE;
}

/******RTC写禁止程序******/
static bool WriteRTC_Disable(void)
{		
	if(!I2CWriteOneByte(CTR1,0))return FALSE;
	I2CWriteOneByte(CTR2,0);
	return	TRUE;
}

/******读RTC实时数据寄存器******/
bool I2CReadDate(S_Time	*psRTC)
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
	I2CNoAck();		//读时间完成，发送NoAck
	I2CStop();
	return	TRUE;
}

/******写RTC实时数据寄存器******/
bool I2CWriteDate(S_Time *psRTC)	//写时间操作要求一次对实时时间寄存器(00H~06H)依次写入，
{                               //不可以单独对7个时间数据中的某一位进行写操作,否则可能会引起时间数据的错误进位. 
                                //要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据.

	WriteRTC_Enable();				//使能，开锁
	
	I2CStart();
	I2CSendByte(RTC_Address); 
	if(!I2CWaitAck()){I2CStop(); return FALSE;}
	I2CSendByte(0x00);			//设置写起始地址      
	I2CWaitAck();	
	I2CSendByte(psRTC->second);		//second     
	I2CWaitAck();	
	I2CSendByte(psRTC->minute);		//minute      
	I2CWaitAck();	
	I2CSendByte(psRTC->hour|0x80);		//hour ,同时设置小时寄存器最高位（0：为12小时制，1：为24小时制）
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
	
	WriteRTC_Disable();				//使能，关锁
	return	TRUE;
}


/******设置RTC报警中断演示程序演示******/
void WriteALARM(void)				//设置报警时间：2016年2月14日 8：00
{						
	WriteRTC_Enable();
	I2CWriteOneByte(Alarm_HR,0x08);	//8时
	I2CWriteOneByte(Alarm_DY,0x14);	//14日
	I2CWriteOneByte(Alarm_MO,0x02);	//02月
	I2CWriteOneByte(Alarm_YR,0x16);	//16年
	I2CWriteOneByte(Alarm_EN,0x74);	//设置报警允许（使能年、月、日、小时报警）
	I2CWriteOneByte(CTR2,0x92);	//设置INT中断选通（INTS1，INTS0），及报警中断总允许位（INTAE）
	WriteRTC_Disable();
}

/******设置RTC倒计时中断演示******/
void SetDjs(void)							//设置倒计时中断
{
	WriteRTC_Enable();
	I2CWriteOneByte(CTR2,0xF0);//先清倒计时中断总允许位（INTDE）
	I2CWriteOneByte(CTR2,0xF4);//设置周期性中断（IM=1）INT中断选通（INTS1，INTS0），配置倒计时中断总允许位（INTDE）
	I2CWriteOneByte(CTR3,0x20);//选择定时器频率源（TDS1、TDS0）为1HZ
	I2CWriteOneByte(Timer_Counter1,5);//倒计时初值寄存器，设置8位倒计时计数初值（5s）
	I2CWriteOneByte(Timer_Counter2,0);
	I2CWriteOneByte(Timer_Counter3,0);
	WriteRTC_Disable();
}

/******设置RTC频率中断演示******/
void SetFrq_2Hz(void)					
{
	WriteRTC_Enable();
	I2CWriteOneByte(CTR2,0xA1);	//选通频率中断（INTS1，INTS0），设置频率中断总允许位（INTFE）
	I2CWriteOneByte(CTR3,0x09);	//设置2Hz频率中断
	WriteRTC_Disable();
}
/******禁止RTC中断******/
void ClrINT(void)         
{
	WriteRTC_Enable();
	I2CWriteOneByte(CTR2,0x80);
	WriteRTC_Disable();
}
/************RTC连续读多个字节************/
unsigned char I2CReadSerial(unsigned char Address, unsigned char length,unsigned char *ps)
{
	unsigned char	i;
	I2CStart();
	I2CSendByte(RTC_Address);      
	if(!I2CWaitAck()){I2CStop(); return FALSE;}
	I2CSendByte(Address);			//设置要读的地址
	I2CWaitAck();
	I2CStart();	
 	I2CSendByte(RTC_Address+1);
	I2CWaitAck();
	for(i = 0;i<length-1;i++,ps++)
	{
		*ps = I2CReceiveByte();		//读数据
		I2CAck();
	}
	*ps = I2CReceiveByte();	
	I2CNoAck();
	I2CStop(); 
	return	TRUE;
}

//******************RTC连续写多个字节******************|
unsigned char RTCWriteSerial(unsigned char Address, unsigned char length,unsigned char *ps)
{
	unsigned char	i;
	WriteRTC_Enable();
	
	I2CStart();
	I2CSendByte(RTC_Address);      	//器件地址
	if(!I2CWaitAck()){I2CStop(); return FALSE;}
	I2CSendByte(Address);			//设置起始地址
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
#if TEST_MODE
//////*****主程序演示*****//////
void rtcDisplay_Test(void)
{
	unsigned int		Bat;
	unsigned char  	Batbit8, Batbit0_7;
	
	I2CWriteDate(&RTC);	//设置时间演示，16年1月26日......
//	WriteALARM();    	//设置报警中断时间演示
//	SetDjs();		 	//设置倒计时中断演示
	SetFrq_2Hz();			//设置频率中断（从INT脚输出方波）演示
	
	I2CReadSerial(IDcode,8,&Sram);		//读内部8字节ID号演示
	RTCWriteSerial(UserRam_30H,8,&Sram);		//把内部8字节的ID号写入用户通用存储器的0x30-0x37地址演示
	
	WriteRTC_Enable();	
#if EN_CHARGE		
	I2CWriteOneByte(CHARGE_MG,0x82);//外接充电电池的使能充电功能
#else
	I2CWriteOneByte(CHARGE_MG,0);//外接一次性电池的禁止充电功能，避免损坏电池
#endif	
	WriteRTC_Disable();
	
	//while(1)
	{
		I2CReadDate(&RTC);		//读时间演示

		Batbit8 = I2CReadOneByte(Bat_bit8);		//读SD25XX的电池电量最高位
		Batbit0_7 = I2CReadOneByte(Bat_Low8);		//读SD25XX的电池电量低八位
		Bat = (Batbit8>>7)*255+Batbit0_7;				//计算电池电量值演示。如Bat=285则表示2.85V

		//把时间数据从串口打出来显示
        LOGD("[Time] %01bd%01bd/%01bd%01bd/%01bd%01bd [Week]%01bd %01bd%01bd:%01bd%01bd:%01bd%01bd [BatVolt] %bdmV\r\n",
            (RTC.year >> 4),(RTC.year & 0x0f),
            (RTC.month >> 4),(RTC.month & 0x0f),
            (RTC.day >> 4),(RTC.day & 0x0f),
            (RTC.week),
            (RTC.hour >> 4),(RTC.hour & 0x0f),
            (RTC.minute >> 4),(RTC.minute & 0x0f),
            (RTC.second >> 4),(RTC.second & 0x0f),
            Bat);
	}

}
#endif
//特别提醒：当写实时时间数据时 (00H~06H), 不可以单独 对 7 个时间数据中的某一位进行写操作 ,
//否则可能会引起时间数据的错误进位 , 所以要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据 .
/*********************************************END OF FILE**********************/