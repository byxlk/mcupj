#include "api_config.h"
#include "USART.h"

/* BOF | CMD | TokenID | ADDR | DataLen | DATA | CS | EOF | */
/*  1  |  1  |    2    |   1  |    1    |   4  |  2 |  1  | */
/* 0x68  0x00    0x00    0x00      0x01  */
/* Command Format: AT+CMD=addr#data */
/**/
// DLT645 Frame EOF/BOF Flag
#define DLT645_FRAME_BOF   0x68
#define DLT645_FRAME_EOF   0x16
#define MSR_ADDR 0x00

static void rs485_enable_send(void)
{
    MAX485_EN = 1;
}

static void rs485_enable_recv(void)
{
    MAX485_EN = 0;
}


//发送一帧数据包
//SendCmdDataTo485("AT+XXX");
bool SendATCommand(u8 *cmd)
{
	//u8 i;

    if(cmd == NULL) return FALSE;

    rs485_enable_send();//485发送数据使能
    delay_ms(2);

    for (; ((*cmd != 0) && (*cmd != '\0'));	cmd++) {
        TX2_write2buff(*cmd);
        //delay_ms(20);
    }

    delay_ms(2);
	rs485_enable_recv();//数据发送完毕485等待接受数据

    return TRUE;
}


void slaveDeviceAddrConfigure(void)
{
    
}

