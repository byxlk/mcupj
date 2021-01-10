
#include <intrins.h>			   //包含nop指令头文件
#include "api_config.h"			   //MCU头文件

#define MSR_KEY_BOOT (0x10)
//#define MSR_KEY_UP ()
//#define MSR_KEY_DOWN ()

/********************定义数据*************************/
static bool isUpdateDisplayContentNow = 0;
static unsigned char dispBuf[14] = {0x0};
static unsigned char KeyCode[5] = {0x0};							 //为存储按键值定义的数组

/***************发送8bit数据，从低位开始**************/
static void send_8bit( unsigned char dat )	 //发送8位数据,从低位开始
{
    unsigned char i;

    for(i = 0; i < 8; i++)
    {
        FZH181_PIN_CLK = 0;
        delay_us(5);

        if( dat&0x01 )
            FZH181_PIN_DIO = 1;
        else
            FZH181_PIN_DIO = 0;
        delay_us(5);

        FZH181_PIN_CLK = 1;
        delay_us(10);
        dat >>= 1;
    }
 
    //FZH181_PIN_CLK = 0;
    FZH181_PIN_DIO = 0;
}

/******************发送控制命令***********************/
static void send_command(unsigned char com)  //发送命令字节
{
    FZH181_PIN_STB = 1;				  //STB置高

    delay_us(10);

    FZH181_PIN_STB = 0;				  //STB置低
    delay_us(5);

    send_8bit(com);			          //发送8bit数据
    delay_us(10);
}

/****************读取按键值并存储*******************/
void ledGetKeyCode(void)			  //读取5字节按键值并存入数组KEY[],从低字节、低位开始
{
    unsigned char i;
    unsigned char j;
    
    //KeyCode[0] = KeyCode[1] = KeyCode[2] = 0;
    //KeyCode[3] = KeyCode[4] = 0;
    
    if(isUpdateDisplayContentNow) return;
    
    send_command(0x42);		          //发送读按键命令
    delay_us(30);
    FZH181_PIN_DIO = 1;				  //释放DIO数据总线


    for(j = 0; j < 5; j++)			  //连续读取5个字节
    {
        for(i = 0; i < 8; i++)
        {
            FZH181_PIN_CLK = 0;
            delay_us(10);

            KeyCode[j] = KeyCode[j]>>1;

            FZH181_PIN_CLK = 1;

            if(FZH181_PIN_DIO == 1)
            {
                KeyCode[j] = KeyCode[j]|0x80;
            }
            delay_us(10);
        }
        delay_us(20);
    }

    FZH181_PIN_CLK = 1;
    FZH181_PIN_DIO = 0;
    FZH181_PIN_STB = 1;
}

/************显示函数，数码管1~7位显示0~6************/
static void updateDisplayContent(void)   //显示函数，1~10位数码管显示0~9
{
    unsigned char i;

    isUpdateDisplayContentNow = 1;
    
    send_command(0x03);     //设置显示模式，7位10段模式
    send_command(0x40);	    //设置数据命令，采用地址自动加1模式
    send_command(0xc0);	    //设置显示地址，从00H开始

    for(i = 0; i < 14; i++)
    {
        send_8bit(dispBuf[i]);	    //从00H开始，偶数地址送显示数据
        //send_8bit(0x00);	//因为SEG9-14未用到，所以奇数地址送全0
    }

    send_command(0x8F);	    //设置显示命令，打开显示并设置占空比14/16.
    FZH181_PIN_STB=1;

    isUpdateDisplayContentNow = 0;
}

void ledDisplayClose(unsigned char ledNo)
{
    unsigned char tabCode = 0x00;
    unsigned short bufVal = (0x1 << ledNo);

    /* Check ledNo */
    if(isMasterDevice()) {
        if(ledNo > 5 || ledNo < 0) return ;
    } else {
        if(ledNo > 9 || ledNo < 0) return ;
    }
    
    if(ledNo > 7) {
        dispBuf[1] &= ( ~(0x1 << (ledNo - 8)));
        dispBuf[3] &= ( ~(0x1 << (ledNo - 8)));
        dispBuf[5] &= ( ~(0x1 << (ledNo - 8)));
        dispBuf[7] &= ( ~(0x1 << (ledNo - 8)));
        dispBuf[9] &= ( ~(0x1 << (ledNo - 8)));
        dispBuf[11] &= ( ~(0x1 << (ledNo - 8)));
        dispBuf[13] &= ( ~(0x1 << (ledNo - 8)));
    } else {
        dispBuf[0] &= ( ~(0x1 << ledNo));
        dispBuf[2] &= ( ~(0x1 << ledNo));
        dispBuf[4] &= ( ~(0x1 << ledNo));
        dispBuf[6] &= ( ~(0x1 << ledNo));
        dispBuf[8] &= ( ~(0x1 << ledNo));
        dispBuf[10] &= ( ~(0x1 << ledNo));
        dispBuf[12] &= ( ~(0x1 << ledNo));
    }

    updateDisplayContent(); /* update display content */
}

/* ledNo: Master = 0 -5  Slave = 0 -9 */
void ledDisplayCtrl(unsigned char ledNo, char dispVal)
{
    unsigned char tabCode = 0x00;
    unsigned short bufVal = (0x1 << ledNo);

    /* Check ledNo */
    if(isMasterDevice()) {
        if(ledNo > 5 || ledNo < 0) return ;
    } else {
        if(ledNo > 9 || ledNo < 0) return ;
    }

    switch(dispVal) {
        case '0':
        case   0: tabCode = 0x3F; break;
        case '1':
        case   1: tabCode = 0x06; break;
        case '2':
        case   2: tabCode = 0x5B; break;
        case '3':
        case   3: tabCode = 0x4F; break;
        case '4':
        case   4: tabCode = 0x66; break;
        case '5':
        case   5: tabCode = 0x6D; break;
        case '6':
        case   6: tabCode = 0x7D; break;
        case '7':
        case   7: tabCode = 0x07; break;
        case '8':
        case   8: tabCode = 0x7F; break;
        case '9':
        case   9: tabCode = 0x6F; break;

        case 'A': tabCode = 0x77; break;
        case 'c': tabCode = 0x58; break;
        case 'C': tabCode = 0x39; break;
        case 'd': tabCode = 0x5E; break;
        case 'E': tabCode = 0x79; break;
        case 'F': tabCode = 0x71; break;
        case 'H': tabCode = 0x76; break;
        case 'L': tabCode = 0x38; break;
        case 'n': tabCode = 0x37; break;
        case 'O':
        case 'o': tabCode = 0x5C; break;
        case 'P':
        case 'p': tabCode = 0x73; break;
        case 'Q':
        case 'q': tabCode = 0x67; break;
        case 'U':
        case 'u': tabCode = 0x3E; break;
        case 'Y':
        case 'y': tabCode = 0x6E; break;
        case '-': tabCode = 0x40; break;
        default: return; 
    }

    if(ledNo > 7) {
        if(tabCode & 0x1)   dispBuf[1] |= (0x1 << (ledNo - 8));
        else                dispBuf[1] &= ( ~(0x1 << (ledNo - 8)));
        if(tabCode & 0x2)   dispBuf[3] |= (0x1 << (ledNo - 8));
        else                dispBuf[3] &= ( ~(0x1 << (ledNo - 8)));
        if(tabCode & 0x4)   dispBuf[5] |= (0x1 << (ledNo - 8));
        else                dispBuf[5] &= ( ~(0x1 << (ledNo - 8)));
        if(tabCode & 0x8)   dispBuf[7] |= (0x1 << (ledNo - 8));
        else                dispBuf[7] &= ( ~(0x1 << (ledNo - 8)));
        if(tabCode & 0x10)  dispBuf[9] |= (0x1 << (ledNo - 8));
        else                dispBuf[9] &= ( ~(0x1 << (ledNo - 8)));
        if(tabCode & 0x20)  dispBuf[11] |= (0x1 << (ledNo - 8));
        else                dispBuf[11] &= ( ~(0x1 << (ledNo - 8)));
        if(tabCode & 0x40)  dispBuf[13] |= (0x1 << (ledNo - 8));
        else                dispBuf[13] &= ( ~(0x1 << (ledNo - 8)));
    } else {
        if(tabCode & 0x1)   dispBuf[0] |= (0x1 << ledNo);
        else                dispBuf[0] &= ( ~(0x1 << ledNo));
        if(tabCode & 0x2)   dispBuf[2] |= (0x1 << ledNo);
        else                dispBuf[2] &= ( ~(0x1 << ledNo));
        if(tabCode & 0x4)   dispBuf[4] |= (0x1 << ledNo);
        else                dispBuf[4] &= ( ~(0x1 << ledNo));
        if(tabCode & 0x8)   dispBuf[6] |= (0x1 << ledNo);
        else                dispBuf[6] &= ( ~(0x1 << ledNo));
        if(tabCode & 0x10)  dispBuf[8] |= (0x1 << ledNo);
        else                dispBuf[8] &= ( ~(0x1 << ledNo));
        if(tabCode & 0x20)  dispBuf[10] |= (0x1 << ledNo);
        else                dispBuf[10] &= ( ~(0x1 << ledNo));
        if(tabCode & 0x40)  dispBuf[12] |= (0x1 << ledNo);
        else                dispBuf[12] &= ( ~(0x1 << ledNo));
    }

    updateDisplayContent(); /* update display content */

    return ;
}

/*****************按键处理函数*********************/


#if TEST_MODE
static unsigned char tmp = 0;
void LedDisplay_Test(char Val)
{
    MSR_WARN_OUTPUT = ~MSR_WARN_OUTPUT;
    ledDisplayCtrl(0,Val);	    //上电显示0~6
    MSR_WARN_OUTPUT = 0;
    ledDisplayCtrl(1,Val);

    ledDisplayCtrl(2,Val);
    ledDisplayCtrl(3,Val);
    ledDisplayCtrl(4,Val);
    ledDisplayCtrl(5,Val);
    
    if(! isMasterDevice()) {
        ledDisplayCtrl(6,Val);
        ledDisplayCtrl(7,Val);
        ledDisplayCtrl(8,Val);
        ledDisplayCtrl(9,Val);
    }
    //delay_ms(1);

    tmp++;
    if(isMasterDevice()) {
        if(KeyCode[0] == 0x10) ledDisplayCtrl(0,'p');
    } else {
        if((KeyCode[0] != 0x0)) {ledDisplayCtrl(0,(tmp) % 10);} //K1与KS1按键按下，数码管显示数字0~6
    //     if((KeyCode[0] == 0x10)) {ledDisplayCtrl(1,(tmp) % 10);}  //K1与KS2按键按下，数码管显示关闭
    //     if((KeyCode[0] == 0x01)) {ledDisplayCtrl(2,(tmp) % 10);}  //K1与KS2按键按下，数码管显示关闭
    //    else if((KeyCode[0] == 0x00)) {ledDisplayCtrl(2,(tmp) % 2);}
    }

    LOGD("KeyCode[%04bu]: %02bx %02bx %02bx %02bx %02bx\r\n",tmp,
            KeyCode[0],KeyCode[1],KeyCode[2],KeyCode[3],KeyCode[4]);
    
}
#endif