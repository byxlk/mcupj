#include "api_config.h"
#include "USART.h"

// DLT645 Frame EOF/BOF Flag
#define DLT645_FRAME_BOF   0x68
#define DLT645_FRAME_EOF   0x16

static void rs485_enable_send(void)
{
    MAX485_EN = 1;
}

static void rs485_enable_recv(void)
{
    MAX485_EN = 0;
}

//发送一帧数据包
void SendDataFrame(unsigned char Length,unsigned char *str)
{
	unsigned char i;
    rs485_enable_send();//485发送数据使能

	for (i=0;i<Length;i++)
	{
		TX2_write2buff(*(str++));
        delay_ms(20);
	}
    
	rs485_enable_recv();//数据发送完毕485等待接受数据
}

//ÇóÒ»¸öÊý×éµÄÐ£ÑéºÍ
unsigned char Get_Chksum(unsigned char *ps,unsigned char length)
{
	unsigned char i = 0;
    unsigned char checkSum = 0;
 
	for(i = 0; i < length; i++) 
        checkSum += ps[i];
 
	return checkSum;		
}

