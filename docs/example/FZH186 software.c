 /*****************************************************************************

 *MCU  ：STC12C5608AD
 * ：Keil uVision4
 * ：11.0592MHZ
 *

********************************************************************************/
#include <reg52.h>			   //MCU头文件
#include "intrins.h"			   //包含nop指令头文件

#define uchar unsigned char		   //数据类型宏定义
#define uint unsigned int		   //数据类型宏定义

#define nop  _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();//宏定义


/********************定义控制端口**********************/
sbit DIO=P1^0;					   //定义FZH186 DIO为P1^0;
sbit CLK=P1^1;					   //定义FZH186 CLK为P1^1;
sbit STB=P1^2;					   //定义FZH186 STB为P1^2;


/********************定义数据*************************/
uchar code CODE[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};  //共阴数码管0~9字型码
uchar code TAB[10]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  //共阴数码管无显示字型码

uchar KEY[5]={0};							 //为存储按键值定义的数组

/***************延时函数nms**************/
void delay_nms(uint n)
{
 uint i;
 while(n--)
 {
 for(i=0;i<550;i++);
 }
}

/***************发送8bit数据，从低位开始**************/
void send_8bit(uchar dat)	 //发送8位数据,从低位开始
{
  uchar i;
  for(i=0;i<8;i++)
  {
  	CLK=0;
    if(dat&0x01)
	   DIO=1;
	else
	   DIO=0;
	_nop_();
	_nop_();
	_nop_();
	CLK=1;
	dat>>=1;
  }
  CLK=0;
  DIO=0;
}

/******************发送控制命令***********************/
void send_command(uchar com)  //发送命令字节
{
 STB=1;						  //STB置高
 nop;
 STB=0;						  //STB置低
 send_8bit(com);			          //发送8bit数据
}

/****************读取按键值并存储*******************/
void read_key(void)			   //读取5字节按键值并存入数组KEY[],从低字节、低位开始
{
 unsigned char i,j;
 send_command(0x42);		          //发送读按键命令
 DIO=1;					  //释放DIO数据总线
 for(j=0;j<5;j++)			  //连续读取5个字节
 {
  for(i=0;i<8;i++)
 {
  CLK=0;
  KEY[j]=KEY[j]>>1;
  CLK=1;
  if(DIO==1)
  {
    KEY[j]=KEY[j]|0x80;
  }
  nop;
}
  nop;
  nop;
}
  CLK=0;
  DIO=0;
  STB=1;
}

/************显示函数，数码管1~7位显示0~6************/
void display(uchar *p)   //显示函数，1~7位数码管显示0~6
{
 uchar i;
 send_command(0x03); 	//设置显示模式，7位10段模式
 send_command(0x40);	//设置数据命令，采用地址自动加1模式
 send_command(0xc0);	//设置显示地址，从00H开始

 for(i=0;i<7;i++)
 {
   send_8bit(*p);	    //从00H开始，偶数地址送显示数据
   send_8bit(0x00);	    //因为SEG9-14未用到，所以奇数地址送全0
   p++;
  }
  send_command(0x8F);	    //设置显示命令，打开显示并设置占空比14/16.
  STB=1;
}

/*****************按键处理函数*********************/
void key_process()		// 按键处理函数
{
    if(KEY[0]!=0x00)
   {
    if((KEY[0]==0x01)) {display(CODE);} //K1与KS1按键按下，数码管显示数字0~6
    if((KEY[0]==0x08)) {display(TAB);}  //K1与KS2按键按下，数码管显示关闭
   }
}

void main()
{
display(CODE);	    //上电显示0~6
delay_nms(1);
while(1)
{
 read_key();	   //读按键值
 key_process();	   //按键处理
 delay_nms(200);   //延时200ms再读按键
 }
}


