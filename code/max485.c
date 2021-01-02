#include "api_config.h"
#include "USART.h"

static void rs485_enable_send(void)
{
    MAX485_EN = 1;
}

static void rs485_enable_recv(void)
{
    MAX485_EN = 0;
}


//串口发送一个字节数据
void SendByteData(unsigned char dat)
{
	//ES=0;  //关串口中断
    //TI=0;  //清零串口发送完成中断请求标志
    TX2_write2buff(dat);
    //while(TI ==0); //等待发送完成
    //TI=0;  //清零串口发送完成中断请求标志
    //ES=1;  //允许串口中断
}

//发送一帧数据包
void SendDataFrame(unsigned char Length,unsigned char *str)
{
	unsigned char i;
    rs485_enable_send();//485发送数据使能

	for (i=0;i<Length;i++)
	{
		SendByteData(*(str++));
        delay_ms(20);
	}
    
	rs485_enable_recv();//数据发送完毕485等待接受数据
}
