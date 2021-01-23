
#include <intrins.h>			   //包含nop指令头文件
#include "api_config.h"			   //MCU头文件

/********************定义数据*************************/
#define MSR_B1_K1S1_SYNC           (0x01) /* 同步 */
#define MSR_B1_K1S2_SET            (0x08) /* 设置 */
#define MSR_B2_K1S3_UP             (0x01) /* 上升 */
#define MSR_B2_K1S4_DOWN           (0x08) /* 下降 */
#define MSR_B3_K1S5_PAUSE          (0x01) /* 暂停 */
#define MSR_B3_K1S6_CMUT           (0x08) /* 通讯 */
#define MSR_B1_K2S1_STOP           (0x02) /* 停止 */
#define MSR_B1_K2S2_BOOT           (0x10) /* 启动 */
#define MSR_B2_K2S3_PRE            (0x02) /* 预紧 */
#define MSR_B2_K2S4_UNLOAD         (0x10) /* 卸载 */

#define SLV_B1_K1S1_RSET           (0x01) /* 休眠 */
#define SLV_B1_K1S2_SET            (0x08) /* 设置 */
#define SLV_B2_K1S3_MUP            (0x01) /* 上升 */
#define SLV_B2_K1S4_MDOWN          (0x08) /* 下降 */
#define SLV_B3_K1S5_PAUSE          (0x01) /* 暂停 */
#define SLV_B3_K1S6_CMUT           (0x08) /* 通信 */
#define SLV_B1_K2S1_POFF           (0x02) /* 急停 */

static bool isUpdateDisplayContentNow = 0;
static unsigned char dispBuf[14] = {0x0};
static unsigned char lastDispBuf[10] = {0x0};
static unsigned char KeyCode[5] = {0x0};							 //为存储按键值定义的数组
static unsigned short KeyBitNo = 0x0; /* 0: no key press 1: key press */
static unsigned short FlashBitNo = 0x0;

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
static void getKeyCodeFromChipReg(void)			  //读取5字节按键值并存入数组KEY[],从低字节、低位开始
{
    unsigned char i;
    unsigned char j;
    
    //KeyCode[0] = KeyCode[1] = KeyCode[2] = 0;
    //KeyCode[3] = KeyCode[4] = 0;
    
    //if(isUpdateDisplayContentNow) return;
    
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
                KeyCode[j] = KeyCode[j] | 0x80;
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
unsigned short getKeyCode(void)
{
    return KeyBitNo;
}

void clrKeyStatus(unsigned short sKey)
{
    if(isMasterDevice()) {
        if(sKey == MSR_KEY_SYNC)   { KeyBitNo &= ~(MSR_KEY_SYNC); }
        if(sKey == MSR_KEY_SET)    { KeyBitNo &= ~(MSR_KEY_SET); }
        if(sKey == MSR_KEY_STOP)   { KeyBitNo &= ~(MSR_KEY_STOP); }
        if(sKey == MSR_KEY_BOOT)   { KeyBitNo &= ~(MSR_KEY_BOOT); }
        
        if(sKey == MSR_KEY_UP)     { KeyBitNo &= ~(MSR_KEY_UP); }
        if(sKey == MSR_KEY_DOWN)   { KeyBitNo &= ~(MSR_KEY_DOWN); }
        if(sKey == MSR_KEY_PRE)    { KeyBitNo &= ~(MSR_KEY_PRE); }
        if(sKey == MSR_KEY_UNLOAD) { KeyBitNo &= ~(MSR_KEY_UNLOAD); }
        
        if(sKey == MSR_KEY_PAUSE)  { KeyBitNo &= ~(MSR_KEY_PAUSE); }
        if(sKey == MSR_KEY_CMUT)   { KeyBitNo &= ~(MSR_KEY_CMUT); }
    } else {
        if(sKey == SLV_KEY_RSET)   { KeyBitNo &= ~(SLV_KEY_RSET); } //K1与KS1按键按下，数码管显示数字0~6
        if(sKey == SLV_KEY_SET)    { KeyBitNo &= ~(SLV_KEY_SET); }
        if(sKey == SLV_KEY_POFF)   { KeyBitNo &= ~(SLV_KEY_POFF); }
    
        if(sKey == SLV_KEY_MUP)    { KeyBitNo &= ~(SLV_KEY_MUP); } //K1与KS2按键按下，数码管显示关闭
        if(sKey == SLV_KEY_MDOWN)  { KeyBitNo &= ~(SLV_KEY_MDOWN); }

        if(sKey == SLV_KEY_PAUSE)  { KeyBitNo &= ~(SLV_KEY_PAUSE); } //K1与KS2按键按下，数码管显示关闭
        if(sKey == SLV_KEY_CMUT)   { KeyBitNo &= ~(SLV_KEY_CMUT); }
    }
}

void ledDisplayFlashEnable(unsigned char ledNo, bool bFlag)
{
    if(bFlag) {
        FlashBitNo |= (0x1 << ledNo);
    } else {
        FlashBitNo &= ~(0x1 << ledNo);
    }
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
    
    if(lastDispBuf[ledNo] == tabCode) return;
    else lastDispBuf[ledNo] = tabCode;
    
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

    isUpdateDisplayContentNow = 1;
    //updateDisplayContent(); /* update display content */
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
        case 'B': tabCode = 0x7F; break;
        case 'b': tabCode = 0x7C; break;
        case 'c': tabCode = 0x58; break;
        case 'C': tabCode = 0x39; break;
        case 'D':
        case 'd': tabCode = 0x5E; break;
        case 'E': tabCode = 0x79; break;
        case 'F': tabCode = 0x71; break;
        case 'H': tabCode = 0x76; break;
        case 'L': tabCode = 0x38; break;
        case 'l': tabCode = 0x30; break;
        case 'n': tabCode = 0x37; break;
        case 'O':
        case 'o': tabCode = 0x5C; break;
        case 'P':
        case 'p': tabCode = 0x73; break;
        case 'Q':
        case 'q': tabCode = 0x67; break;
        case 'S': tabCode = 0x6D; break;
        case 'U':
        case 'u': tabCode = 0x3E; break;
        case 'Y':
        case 'y': tabCode = 0x6E; break;
        case '-': tabCode = 0x40; break;
        default: return; 
    }
    
    if(lastDispBuf[ledNo] == tabCode) return;
    else lastDispBuf[ledNo] = tabCode;

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

    //while(isUpdateDisplayContentNow) ;
    isUpdateDisplayContentNow = 1;
    //updateDisplayContent(); /* update display content */

    return ;
}

/*****************按键处理函数*********************/


#if TEST_MODE
static unsigned int tmp = 0;
void LedDisplay_Test(char Val)
{
    ledDisplayCtrl(0,Val);	    //上电显示0~6
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
}

bool LedKeyScan_Test(void)
{
    if(isMasterDevice()) {
        if(KeyCode[0] == MSR_B1_K1S1_SYNC)   {ledDisplayCtrl(0,'p');}
        if(KeyCode[0] == MSR_B1_K1S2_SET)    {ledDisplayCtrl(0,'E');}
        if(KeyCode[0] == MSR_B1_K2S1_STOP)   {ledDisplayCtrl(0,'o');}
        if(KeyCode[0] == MSR_B1_K2S2_BOOT)   {ledDisplayCtrl(0,'b');}
        
        if(KeyCode[1] == MSR_B2_K1S3_UP)     {ledDisplayCtrl(1,'U');}
        if(KeyCode[1] == MSR_B2_K1S4_DOWN)   {ledDisplayCtrl(1,'D');}
        if(KeyCode[1] == MSR_B2_K2S3_PRE)    {ledDisplayCtrl(1,'P');}
        if(KeyCode[1] == MSR_B2_K2S4_UNLOAD) {ledDisplayCtrl(1,'L');}
        
        if(KeyCode[2] == MSR_B3_K1S5_PAUSE)  {ledDisplayCtrl(2,'P');}
        if(KeyCode[2] == MSR_B3_K1S6_CMUT)   {ledDisplayCtrl(2,'C');}
    } else {
        if(KeyCode[0] == SLV_B1_K1S1_RSET)   {ledDisplayCtrl(0,'-');} //K1与KS1按键按下，数码管显示数字0~6
        if(KeyCode[0] == SLV_B1_K1S2_SET)    {ledDisplayCtrl(0,'S');}
        if(KeyCode[0] == SLV_B1_K2S1_POFF)   {ledDisplayCtrl(0,'P');}
    
        if(KeyCode[1] == SLV_B2_K1S3_MUP)    {ledDisplayCtrl(1,'P');}  //K1与KS2按键按下，数码管显示关闭
        if(KeyCode[1] == SLV_B2_K1S4_MDOWN)  {ledDisplayCtrl(1,'-');} 

        if(KeyCode[2] == SLV_B3_K1S5_PAUSE)  {ledDisplayCtrl(2,'P');}  //K1与KS2按键按下，数码管显示关闭
        if(KeyCode[2] == SLV_B3_K1S6_CMUT)   {ledDisplayCtrl(2,'C');}
    }

    if(KeyCode[0] || KeyCode[1] || KeyCode[2] || KeyCode[3] || KeyCode[4]) {
        //LOGD("KeyCode[%04bu]: %02bx %02bx %02bx %02bx %02bx\r\n",tmp++,
        //        KeyCode[0],KeyCode[1],KeyCode[2],KeyCode[3],KeyCode[4]);
        return TRUE;
    } else {
        return FALSE;
    }
}
#endif

/********************* Timer2ÖÐ¶Ïº¯Êý************************/
static unsigned char iSecCounter = 0;
void timer1_int (void) interrupt TIMER1_VECTOR
{
    /* Check phase sequene lost */
    checkPhaseSeqALost();
    checkPhaseSeqBLost();
    checkPhaseSeqCLost();

    /* Check Keyboard Scan per 50ms */
    if((iSecCounter++) % 50 != 0) return;

    /* Update display content after buffer update */
    if(FlashBitNo != 0) {
        if(FlashBitNo & 0x001) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(0,lastDispBuf[0]);
            else                     ledDisplayClose(0);
        }
        if(FlashBitNo & 0x002) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(1,lastDispBuf[1]);
            else                     ledDisplayClose(1);
        }
        if(FlashBitNo & 0x004) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(2,lastDispBuf[2]);
            else                     ledDisplayClose(2);
        }
        if(FlashBitNo & 0x008) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(3,lastDispBuf[3]);
            else                     ledDisplayClose(3);
        }
        if(FlashBitNo & 0x010) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(4,lastDispBuf[4]);
            else                     ledDisplayClose(4);
        }
        if(FlashBitNo & 0x020) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(5,lastDispBuf[5]);
            else                     ledDisplayClose(5);
        }
        if(FlashBitNo & 0x040) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(6,lastDispBuf[6]);
            else                     ledDisplayClose(6);
        }
        if(FlashBitNo & 0x080) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(7,lastDispBuf[7]);
            else                     ledDisplayClose(7);
        }
        if(FlashBitNo & 0x100) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(8,lastDispBuf[8]);
            else                     ledDisplayClose(8);
        }
        if(FlashBitNo & 0x200) {
            if(iSecCounter % 2 == 0) ledDisplayCtrl(9,lastDispBuf[9]);
            else                     ledDisplayClose(9);
        }
    }
    if(isUpdateDisplayContentNow) {
        updateDisplayContent();
    }

    /* Process Key Code */
    getKeyCodeFromChipReg();
    if(isMasterDevice()) {
        if(KeyCode[0] == MSR_B1_K1S1_SYNC)   { KeyBitNo |= MSR_KEY_SYNC; }
        if(KeyCode[0] == MSR_B1_K1S2_SET)    { KeyBitNo |= MSR_KEY_SET; }
        if(KeyCode[0] == MSR_B1_K2S1_STOP)   { KeyBitNo |= MSR_KEY_STOP; }
        if(KeyCode[0] == MSR_B1_K2S2_BOOT)   { KeyBitNo |= MSR_KEY_BOOT; }
        
        if(KeyCode[1] == MSR_B2_K1S3_UP)     { KeyBitNo |= MSR_KEY_UP; }
        if(KeyCode[1] == MSR_B2_K1S4_DOWN)   { KeyBitNo |= MSR_KEY_DOWN; }
        if(KeyCode[1] == MSR_B2_K2S3_PRE)    { KeyBitNo |= MSR_KEY_PRE; }
        if(KeyCode[1] == MSR_B2_K2S4_UNLOAD) { KeyBitNo |= MSR_KEY_UNLOAD; }
        
        if(KeyCode[2] == MSR_B3_K1S5_PAUSE)  { KeyBitNo |= MSR_KEY_PAUSE; }
        if(KeyCode[2] == MSR_B3_K1S6_CMUT)   { KeyBitNo |= MSR_KEY_CMUT; }
    } else {
        if(KeyCode[0] == SLV_B1_K1S1_RSET)   { KeyBitNo |= SLV_KEY_RSET; } //K1与KS1按键按下，数码管显示数字0~6
        if(KeyCode[0] == SLV_B1_K1S2_SET)    { KeyBitNo |= SLV_KEY_SET; }
        if(KeyCode[0] == SLV_B1_K2S1_POFF)   { KeyBitNo |= SLV_KEY_POFF; }
    
        if(KeyCode[1] == SLV_B2_K1S3_MUP)    { KeyBitNo |= SLV_KEY_MUP; } //K1与KS2按键按下，数码管显示关闭
        if(KeyCode[1] == SLV_B2_K1S4_MDOWN)  { KeyBitNo |= SLV_KEY_MDOWN; }

        if(KeyCode[2] == SLV_B3_K1S5_PAUSE)  { KeyBitNo |= SLV_KEY_PAUSE; } //K1与KS2按键按下，数码管显示关闭
        if(KeyCode[2] == SLV_B3_K1S6_CMUT)   { KeyBitNo |= SLV_KEY_CMUT; }
    };

    return ;
}