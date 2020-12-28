 /*****************************************************************************

 *MCU  ��STC12C5608AD
 * ��Keil uVision4
 * ��11.0592MHZ
 *

********************************************************************************/
#include <reg52.h>			   //MCUͷ�ļ�
#include "intrins.h"			   //����nopָ��ͷ�ļ�

#define uchar unsigned char		   //�������ͺ궨��
#define uint unsigned int		   //�������ͺ궨��

#define nop  _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();//�궨��


/********************������ƶ˿�**********************/
sbit DIO=P1^0;					   //����FZH186 DIOΪP1^0;
sbit CLK=P1^1;					   //����FZH186 CLKΪP1^1;
sbit STB=P1^2;					   //����FZH186 STBΪP1^2;


/********************��������*************************/
uchar code CODE[10]={0x3F,0x06,0x5B,0x4F,0x66,0x6D,0x7D,0x07,0x7F,0x6F};  //���������0~9������
uchar code TAB[10]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};  //�������������ʾ������

uchar KEY[5]={0};							 //Ϊ�洢����ֵ���������

/***************��ʱ����nms**************/
void delay_nms(uint n)
{
 uint i;
 while(n--)
 {
 for(i=0;i<550;i++);
 }
}

/***************����8bit���ݣ��ӵ�λ��ʼ**************/
void send_8bit(uchar dat)	 //����8λ����,�ӵ�λ��ʼ
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

/******************���Ϳ�������***********************/
void send_command(uchar com)  //���������ֽ�
{
 STB=1;						  //STB�ø�
 nop;
 STB=0;						  //STB�õ�
 send_8bit(com);			          //����8bit����
}

/****************��ȡ����ֵ���洢*******************/
void read_key(void)			   //��ȡ5�ֽڰ���ֵ����������KEY[],�ӵ��ֽڡ���λ��ʼ
{
 unsigned char i,j;
 send_command(0x42);		          //���Ͷ���������
 DIO=1;					  //�ͷ�DIO��������
 for(j=0;j<5;j++)			  //������ȡ5���ֽ�
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

/************��ʾ�����������1~7λ��ʾ0~6************/
void display(uchar *p)   //��ʾ������1~7λ�������ʾ0~6
{
 uchar i;
 send_command(0x03); 	//������ʾģʽ��7λ10��ģʽ
 send_command(0x40);	//��������������õ�ַ�Զ���1ģʽ
 send_command(0xc0);	//������ʾ��ַ����00H��ʼ

 for(i=0;i<7;i++)
 {
   send_8bit(*p);	    //��00H��ʼ��ż����ַ����ʾ����
   send_8bit(0x00);	    //��ΪSEG9-14δ�õ�������������ַ��ȫ0
   p++;
  }
  send_command(0x8F);	    //������ʾ�������ʾ������ռ�ձ�14/16.
  STB=1;
}

/*****************����������*********************/
void key_process()		// ����������
{
    if(KEY[0]!=0x00)
   {
    if((KEY[0]==0x01)) {display(CODE);} //K1��KS1�������£��������ʾ����0~6
    if((KEY[0]==0x08)) {display(TAB);}  //K1��KS2�������£��������ʾ�ر�
   }
}

void main()
{
display(CODE);	    //�ϵ���ʾ0~6
delay_nms(1);
while(1)
{
 read_key();	   //������ֵ
 key_process();	   //��������
 delay_nms(200);   //��ʱ200ms�ٶ�����
 }
}


