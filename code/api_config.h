#ifndef		__CONFIG_H
#define		__CONFIG_H

#include "STC15Fxxxx.H"
#include "USART.h"
#include "stdio.h"

/*********************************************************/

#define DEBUG                      (1)	//开启调试模式
#define TEST_MODE                  (0)
//#define LOGD                       uart_printf
#define LOGD                       printf

#define bool                       bit
#define ON                         (0)
#define OFF                        (1)
#define LED_ON                     (0)
#define LED_OFF                    (1)

#define HX710A1                    (1)
#define HX710A2                    (2)

#define MSR_LED_INCLINE_INDICAT    (0)  //倾斜指示灯
#define MSR_LED_OVER_LOADING       (1)  //超载
#define MSR_LED_UNDER_LOADING      (2)  //欠载
#define MSR_LED_LOSS_PHASE         (3)  //缺相
#define MSR_LED_SYNCHRO_START      (4)  //同步启动
#define MSR_LED_UP_INDICAT         (5)  //上升
#define MSR_LED_DOWN_INDICAT       (6)  //下降
#define MSR_LED_COMMUNICAT_INDICAT (7)  //通讯
#define MSR_LED_POWER_START        (8)  //电源启动指示
#define MSR_LED_ALL                (9)  /* Off All Led */

#define SLV_LED_OVER_LOADING       (10) //超载
#define SLV_LED_UNDER_LOADING      (11) //欠载
#define SLV_LED_LOSS_PHASE         (12) //缺相
#define SLV_LED_SK_INDICAT         (13) //受控指示
#define SLV_LED_UP_INDICAT         (14) //上升
#define SLV_LED_DOWN_INDICAT       (15) //下降
#define SLV_LED_COMMUNICAT_INDICAT (16) //通讯
#define SLV_LED_YCG_INDICAT        (17) //右超高
#define SLV_LED_WARN_INDICAT       (18) //报警指示
#define SLV_LED_ZCG_INDICAT        (19) //左超高
#define SLV_LED_ALL                (20)  /* Off All Led */

#define MSR_B1_K1S1_SYNC           (0x101) /*  */
#define MSR_B1_K1S2_SET            (0x208) /*  */
#define MSR_B2_K1S3_UP             (0x301) /*  */
#define MSR_B2_K1S4_DOWN           (0x408) /*  */
#define MSR_B3_K1S5_PAUSE          (0x501) /*  */
#define MSR_B3_K1S6_CMUT           (0x608) /*  */
#define MSR_B1_K2S1_STOP           (0x702) /*  */
#define MSR_B1_K2S2_BOOT           (0x810) /*  */
#define MSR_B2_K2S3_PRE            (0x902) /*  */
#define MSR_B2_K2S4_UNLOAD         (0xa10) /*  */

#define SLV_B1_K1S1_RSET           (0X101) /*  */
#define SLV_B1_K1S2_SET            (0X208) /* 设置 */
#define SLV_B2_K1S3_MUP            (0X301) /*  */
#define SLV_B2_K1S4_MDOWN          (0X408) /*  */
#define SLV_B3_K1S5_PAUSE          (0X501) /*  */
#define SLV_B3_K1S6_CMUT           (0X608) /* 通信 */
#define SLV_B1_K2S1_POFF           (0X702) /* 急停 */

sbit FZH181_PIN_CLK             = P0^6;
sbit FZH181_PIN_DIO             = P0^7;
sbit FZH181_PIN_STB             = P0^5;

sbit MAX485_EN                  = P3^4;
//sbit MAX485_TX1               = P4^7; //TXD2
//sbit MAX485_RX1               = P4^6; //RXD2

sbit PWR_PHASE_PIN_A                = P3^5; //
sbit PWR_PHASE_PIN_B                = P3^6; //INT2
sbit PWR_PHASE_PIN_C                = P3^7; //INT3

/*********************************************************/
sbit MASTER_INDICATOR_FLAG      = P5^5; // 1: 主机   0：从机
sbit HARDWARE_DEBUG_FLAG        = P5^4; // 1:开启debug 0：禁用debug

sbit MSR_LED_Loss_Phase         = P2^7; //缺相指示灯
sbit MSR_LED_Over_Loading       = P2^6; //超载
sbit MSR_LED_Under_Loading      = P2^5; //欠载
sbit MSR_LED_Incline_Indicat    = P2^4; //倾斜
sbit MSR_LED_Synchro_Start      = P2^3; //同步启动
sbit MSR_LED_Up_Indicat         = P2^2; //上升
sbit MSR_LED_Down_Indicat       = P2^1; //下降
sbit MSR_LED_Communicat_Indicat = P2^0; //通讯
sbit MSR_LED_Power_Start        = P4^5; //电源启动指示

sbit MSR_SD3178_SCK             = P0^3;
sbit MSR_SD3178_SDA             = P0^4;
//sbit MSR_SD3178_INT           = P3^2; // INT0

sbit MSR_WARN_OUTPUT            = P4^4; //报警输出
sbit MSR_RELY_CTRL              = P4^0;
/*********************************************************/
// 从机pin脚定义

sbit SLV_LED_ZCG_Indicat        = P4^5; //左超高
sbit SLV_LED_Over_Loading       = P2^6; //超载
sbit SLV_LED_Under_Loading      = P2^5; //欠载
sbit SLV_LED_Loss_Phase         = P2^4; //缺相
sbit SLV_LED_SK_Indicat         = P2^3; //受控指示
sbit SLV_LED_Up_Indicat         = P2^2; //上升
sbit SLV_LED_Down_Indicat       = P2^1; //下降
sbit SLV_LED_Communicat_Indicat = P2^0; //通讯
sbit SLV_LED_YCG_Indicat        = P2^7; //右超高
sbit SLV_LED_Warn_Indicat       = P4^4; //报警指示

sbit SLV_HX710A1_SCK            = P0^1;
sbit SLV_HE710A1_SDA            = P0^0;
sbit SLV_HX710A2_SCK            = P0^3;
sbit SLV_HE710A2_SDA            = P0^2;

sbit SLV_RELAY_CTRL1_XJ         = P4^1;
sbit SLV_RELAY_CTRL1_SS         = P4^0;
sbit SLV_RELAY_CTRL2_XJ         = P4^3;
sbit SLV_RELAY_CTRL2_SS         = P4^2;

sbit SLV_ADXL345_CS             = P1^2;
sbit SLV_ADXL345_SDA            = P1^3;
sbit SLV_ADXL345_SDO            = P1^4;
sbit SLV_ADXL345_CLK            = P1^5;

/*********************************************************/
/* Common */
void delay_us(unsigned int t);
void delay_ms(unsigned int t);
void delay5us(void)	;
void delay500ms();
bool isMasterDevice(void);
void init_Watch_Dog(void);
void Reboot_System(void);
void PrintSystemInfoToSerial(bool msterFlag);

/* Led Control */
void ledStatusManageService(unsigned int iSec);
void MSR_LedStatusCtrl(unsigned char LedNo, bool StatusFlag);
void MSR_LedFlashCtrl(unsigned char LedNo);
void SLV_LedStatusCtrl(unsigned char LedNo, bool StatusFlag);
void SLV_LedFlashCtrl(unsigned char LedNo);

/* HX710A */
unsigned long getHX710Count(unsigned char iNo);

/*max485*/
void SendByteData(unsigned char dat);
void SendDataFrame(unsigned char Length,unsigned char *str);

/* FZH181 */
void displayContentUpdateAndKeyScanService(void);
void ledDisplayClose(unsigned char ledNo);
void ledDisplayCtrl(unsigned char ledNo, char dispVal);


/* SD3178 */
/* PHASE CHECK */
unsigned short checkACPowerPhaseSequence(void);

/* Relay Ctrl */
void relayCtrl_XJ1(bool flag);
void relayCtrl_SS1(bool flag);
void relayCtrl_XJ2(bool flag);
void relayCtrl_SS2(bool flag);
void MSR_relayCtrl_PWR(bool flag);
void MSR_relayCtrl_WAR(bool flag);

/* adxl345 */

/* Hardware Test API interface */
#if TEST_MODE
void acPowerPhaseSequenceCheck_Test(void);
void ledLight_Test(bool StatusFlag);
void LedDisplay_Test(char Val);
bool LedKeyScan_Test(void);
void rtcDisplay_Test(void);
void hx710_Test(void);
void adxl345_Test(void);
#endif

/*********************************************************/
#endif
