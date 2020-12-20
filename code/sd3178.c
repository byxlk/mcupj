#pragma code
#include <intrins.h>
#include "api_config.h"

#if 0
//************************************************************
// 实时时钟SD30XX读写C51演示程序
// AT89S52   11.0592MHz
// E-mail:   FAE@whwave.com.cn
// TEL：		 0755-83114387
// Last update:                   2014/11/14
//************************************************************

#define		RTC_Address			0x64		//RTC器件地址
#define		IDcode				0x72		//8字节ID号起始地址
#define		Bat_bit8			0x1A		//电量最高位寄存器地址
#define		Bat_Low8			0x1B		//电量低八位寄存器地址

#define		true  			1
#define 	false 			0

//*********变量及IO口定义*********
typedef unsigned char uchar;
typedef unsigned int uint;

sbit	SDA=P3^4; 		
sbit	SCL=P3^5;		

uchar	Batbit8,Batlow7;	//电池电量寄存器值
uint  	Bat;			//电池电量值
uchar	Sram[8];		//通用数据缓存器
uchar	data1[8];		//通用数据缓存器
typedef	struct
{
	uchar 	second;
	uchar	minute;
	uchar	hour;
	uchar	week;
	uchar	day;
	uchar	month;
	uchar	year;
}S_Time;	

S_Time  RTC={0x55,0x59,0x14,0x01,0x12,0x11,0x14};	//初始化时间结构体变量（设置时间：2014年11月12日 14:59:55  星期一）
//           55秒 59分 14时 周一 10日 11月 14年
/********SD30XX函数名********/
uchar I2CReadOneByte(uchar DeviceAddress,uchar add);//读一字节
bit I2CWriteOneByte(uchar DeviceAddress,uchar add, uchar date);//写一字节
uchar I2CReadSerial(uchar DeviceAddress, uchar Address, uchar length,uchar *ps);//连续读
uchar I2CWriteSerial(uchar DeviceAddress, uchar Address, uchar length,uchar *ps);//连续写
bit I2CWriteDate(S_Time	SetRTC);//写时间	
bit I2CReadDate(S_Time	*psRTC);//读时间
bit	WriteTimeOn(void);//写允许
bit	WriteTimeOff(void);//写禁止
void Delay(uint nn);

/*********I2C延时4us***********/
void I2CWait(void)//4us
{	
	_nop_();_nop_();_nop_();_nop_();//AT89S52一个nop是1us
}

/********开启SD30XX的I2C总线********/
bit I2CStart(void)
{
	SDA=1;
	SCL=1;
	I2CWait();
	if(!SDA)return false;	//SDA线为低电平则总线忙,退出
	SDA=0;
	I2CWait();
	while(SDA)return false;	//SDA线为高电平则总线出错,退出
	SCL=0;
	I2CWait();
	return true;
}

/********关闭SD30XX的I2C总线*******/
void I2CStop(void)
{
	SDA=0;
	SCL=0;
	I2CWait();
	SCL=1;
	I2CWait();
	SDA=1;
}

/*********发送 ACK*********/
void I2CAck(void)
{	
	SDA=0;
	SCL=0;
	I2CWait();
	SCL=1;
	I2CWait();
	SCL=0;
}

/*********发送NO ACK*********/
void I2CNoAck(void)
{	
	SDA=1;
	SCL=0;
	I2CWait();
	SCL=1;
	I2CWait();
	SCL=0;
}

/*********读取ACK信号*********/
bit I2CWaitAck(void) 	 //返回为:1=有ACK,0=无ACK
{
	SCL=0;
	SDA=1;		//设置SDA为输入（其它类型的单片机需要配置IO输入输出寄存器）
	I2CWait();
	SCL=1;
	I2CWait();
	while(SDA)
	{
		SCL=0;
		return false;
	}
	SCL=0;
	return true;
}

/************MCU向SD30XX发送一个字节*************/
void I2CSendByte(uchar demand) //数据从高位到低位//
{
	uchar i=8;                       
	                                 
	
	while(i--)
	{
		SCL=0;
		_nop_();
		SDA=(bit)(demand&0x80);
		demand<<=1;
		I2CWait();
		SCL=1;
		I2CWait();
	}
	SCL=0;
}

/*********MCU从SD30XX读入一字节*********/
uchar I2CReceiveByte(void)      //数据从高位到低位//
{
	uchar i=8;
	uchar ddata=0;
	SDA=1;			//设置SDA为输入（其它类型的单片机需要配置IO输入输出寄存器）
	while(i--)
	{
		ddata<<=1;      //数据从高位开始读取
		SCL=0;
		I2CWait();
		SCL=1;
		I2CWait();	//从高位开始 ddata|=SDA;ddata<<=1
		if(SDA)
		{
			ddata|=0x01;
		}
	}
	SCL=0;
	return ddata;
}

/******I2C写一个字节******/
bit I2CWriteOneByte(uchar DeviceAddress,uchar add, uchar date)
{		
	if(!I2CStart())return false;
	I2CSendByte(DeviceAddress);      
	I2CWaitAck();   
	I2CSendByte(add);		//设置写地址      
	I2CWaitAck();	
	I2CSendByte(date);		//写数据
	I2CWaitAck();	
	I2CStop(); 
	return	true;
}

/******I2C读一个字节程序******/
uchar I2CReadOneByte(uchar DeviceAddress,uchar add)
{		
	uchar dat;
	if(!I2CStart())return false;
	I2CSendByte(DeviceAddress);      
	if(!I2CWaitAck()){I2CStop(); return false;}
  	I2CSendByte(add);		//设置要读的地址
	I2CWaitAck();
	I2CStart();	
 	I2CSendByte(DeviceAddress+1);	
	I2CWaitAck();	
	dat=I2CReceiveByte();		//读数据
	I2CNoAck();
	I2CStop(); 
	return  dat;
}

/******写SD30XX允许程序******/
bit WriteTimeOn(void)
{		
	if(!I2CWriteOneByte(RTC_Address,0x10,0x80))return false;
	I2CWriteOneByte(RTC_Address,0x0f,0xff);
	return	true;
}

/******写SD30XX禁止程序******/
bit WriteTimeOff(void)
{		
	if(!I2CWriteOneByte(RTC_Address,0x0f,0x7b))return false;
	I2CWriteOneByte(RTC_Address,0x10,0);
	return	true;
}

/******读SD30XX实时数据寄存器******/
bit I2CReadDate(S_Time	*psRTC)
{
	
	if(!I2CStart())return false;
	I2CSendByte(RTC_Address+1); 
    	if(!I2CWaitAck()){I2CStop(); return false;}
	psRTC->second=I2CReceiveByte();
	I2CAck();
	psRTC->minute=I2CReceiveByte();
	I2CAck();
	psRTC->hour=I2CReceiveByte();
	I2CAck();
	psRTC->week=I2CReceiveByte();
	I2CAck();
	psRTC->day=I2CReceiveByte();
	I2CAck();
	psRTC->month=I2CReceiveByte();
	I2CAck();
	psRTC->year=I2CReceiveByte();
	I2CNoAck();		//读时间完成，发送NoAck
	I2CStop();
	return	true;
}

/******写SD30XX实时数据寄存器******/
bit I2CWriteDate(S_Time	SetRTC)	//写时间操作要求一次对实时时间寄存器(00H~06H)依次写入，
{                               //不可以单独对7个时间数据中的某一位进行写操作,否则可能会引起时间数据的错误进位. 
                                //要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据.
	S_Time 	*psRTC;
    	psRTC=&SetRTC;
	WriteTimeOn();				//使能，开锁
	if(!I2CStart())return false;
	I2CSendByte(RTC_Address); 
	if(!I2CWaitAck()){I2CStop(); return false;}
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
	
	WriteTimeOff();				//使能，关锁
	return	true;
}


/******设置SD30XX报警中断演示程序演示******/
void WriteALARM(void)				//设置报警时间：2015年2月14日 8：00
{						//只有设置未来的时间才有效
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x09,0x08);	//8时
	I2CWriteOneByte(RTC_Address,0x0b,0x14);	//14日
	I2CWriteOneByte(RTC_Address,0x0c,0x02);	//02月
	I2CWriteOneByte(RTC_Address,0x0d,0x15);	//15年
	I2CWriteOneByte(RTC_Address,0x0e,0x74);	//设置报警允许（使能年、月、日、小时报警）
	I2CWriteOneByte(RTC_Address,0x10,0x92);	//设置INT中断选通（INTS1，INTS0），及报警中断总允许位（INTAE）
	WriteTimeOff();
}

/******关闭SD30XX报警中断程序******/
void ClrALARM(void)				//关闭报警中断
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0x90);
	WriteTimeOff();
}
/******设置SD30XX倒计时中断演示******/
void SetDjs(void)				//设置倒计时中断
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0x0f);//先清倒计时中断总允许位（INTDE）
	I2CWriteOneByte(RTC_Address,0x10,0xf4);//设置周期性中断（IM=1）INT中断选通（INTS1，INTS0），配置倒计时中断总允许位（INTDE）
	I2CWriteOneByte(RTC_Address,0x11,0x30);//选择定时器频率源（TDS1、TDS0）为1/60HZ
	I2CWriteOneByte(RTC_Address,0x13,0x05);//倒计时初值寄存器，设置8位倒计时计数初值（5min）
	WriteTimeOff();
}

/******关闭SD30XX倒计时中断程序******/
void ClrDjs(void)          
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0xf0);
	WriteTimeOff();
}
/******设置SD30XX频率中断演示******/
void SetFrq(void)					
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0xa1);	//选通频率中断（INTS1，INTS0），设置频率中断总允许位（INTFE）
	I2CWriteOneByte(RTC_Address,0x11,0x09);	//设置2Hz频率中断
	WriteTimeOff();
}
/******关闭SD30XX频率中断******/
void ClrFrq(void)         
{
	WriteTimeOn();
	I2CWriteOneByte(RTC_Address,0x10,0xa0);
	WriteTimeOff();
}
//|************I2C连续读多个字节************|
uchar I2CReadSerial(uchar DeviceAddress, uchar Address, uchar length,uchar *ps)
{
	uchar	i;
	if(!I2CStart())return false;
	I2CSendByte(DeviceAddress);      
	if(!I2CWaitAck()){I2CStop(); return false;}
  	I2CSendByte(Address);			//设置要读的地址
	I2CWaitAck();
	I2CStart();	
 	I2CSendByte(DeviceAddress+1);
	I2CWaitAck();
	for(i=0;i<length-1;i++,ps++)
	{
		*ps=I2CReceiveByte();		//读数据
		I2CAck();
	}
	*ps=I2CReceiveByte();	
	I2CNoAck();
	I2CStop(); 
	return	true;
}

//|******************I2C连续写多个字节******************|
uchar I2CWriteSerial(uchar DeviceAddress, uchar Address, uchar length,uchar *ps)
{
	uchar	i;
	if(!WriteTimeOn())return false;
	if(!I2CStart())return false;
	I2CSendByte(DeviceAddress);      //器件地址
	if(!I2CWaitAck()){I2CStop(); return false;}
  	I2CSendByte(Address);		//设置起始地址
	I2CWaitAck();
	for(i=0;i<length;i++)
	{
		I2CSendByte(*(ps++));		
		I2CAck();					
	}
	I2CStop(); 
	WriteTimeOff();
	return	true;
}

/*********延时子程序*********/
void Delay2_5ms()      //延时2.5ms
{
	uchar	i=255;
	while(i--);
}

void Delayms(uchar n)
{
	while(n--)
		Delay2_5ms();
}
//////*****主程序演示*****//////
void main()
{

	I2CWriteDate(RTC);	//设置时间演示
//	WriteALARM();    	//设置报警中断时间演示
//	SetDjs();		 	//设置倒计时中断演示
	SetFrq();			//设置频率中断演示
	
	I2CReadSerial(RTC_Address,IDcode,8,Sram);		//读内部8字节ID号
	I2CWriteSerial(RTC_Address,0x30,8,Sram);		//把内部8字节的ID号写入用户通用存储器的0x30-0x37地址
	
	I2CReadSerial(RTC_Address,0x30,8,data1);		//读从用户RAM寄存器把先前存入的ID号读出来（0x30-0x37地址）
	Batbit8=I2CReadOneByte(RTC_Address,Bat_bit8);		//读SD30XX的电池电量最高位
	Batlow7=I2CReadOneByte(RTC_Address,Bat_Low8);		//读SD30XX的电池电量低八位
	Bat=(Batbit8>>7)*255+Batlow7;				//计算电池电量值演示。如Bat=285则表示2.85V

    while(1)
	{
 		I2CReadDate(&RTC);		//读时间演示
		Delayms(100);			//延时250ms，1s读4次
    	}

}
#endif

//特别提醒：当写实时时间数据时 (00H~06H), 不可以单独 对 7 个时间数据中的某一位进行写操作 ,
//否则可能会引起时间数据的错误进位 , 所以要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据 .
/*********************************************END OF FILE**********************/