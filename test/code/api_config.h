#ifndef		__CONFIG_H
#define		__CONFIG_H

#include "STC15Fxxxx.H"
#include "USART.h"
#include "EEPROM.h"
#include "stdio.h"

/*********************************************************/

#define DEBUG                      (1)	//开启调试模式
#define TEST_MODE                  (1)
#define PHASE_OUTPUT_MODE          (1) // 仅用于相位输出调试
//#define LOGD                       uart_printf
#define LOGD                       printf

#define bool                       bit
#define ON                         (0)
#define OFF                        (1)
#define LED_ON                     (0)
#define LED_OFF                    (1)

#define HX710A1                    (1)
#define HX710A2                    (2)

#define STATUSMACHINE_BOOTINIT     (0)
#define STATUSMACHINE_SYNCADDR     (1)
#define STATUSMACHINE_CTRLMODE     (2)
#define STATUSMACHINE_STOPMODE     (3)

sbit FZH181_PIN_CLK             = P0^6;
sbit FZH181_PIN_DIO             = P0^7;
sbit FZH181_PIN_STB             = P0^5;

sbit MAX485_EN                  = P3^4;
//sbit MAX485_TX1               = P4^7; //TXD2
//sbit MAX485_RX1               = P4^6; //RXD2

sbit PWR_PHASE_PIN_A            = P3^5; //
sbit PWR_PHASE_PIN_B            = P3^6; //INT2
sbit PWR_PHASE_PIN_C            = P3^7; //INT3

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
sbit SLV_HX710A1_SDA            = P0^0;
sbit SLV_HX710A2_SCK            = P0^3;
sbit SLV_HX710A2_SDA            = P0^2;

sbit SLV_RELAY_CTRL1_XJ         = P4^1;
sbit SLV_RELAY_CTRL1_SS         = P4^0;
sbit SLV_RELAY_CTRL2_XJ         = P4^3;
sbit SLV_RELAY_CTRL2_SS         = P4^2;

sbit SLV_ADXL345_CS             = P1^2;
sbit SLV_ADXL345_SDA            = P1^3;
sbit SLV_ADXL345_SDO            = P1^4;
sbit SLV_ADXL345_CLK            = P1^5;

/*********************************************************/
typedef enum {
    MSR_KEY_NONE   = 0x0000, /* 无键 */
    MSR_KEY_SYNC   = 0x0001, /* 同步 */
    MSR_KEY_SET    = 0x0002, /* 设置 */
    MSR_KEY_UP     = 0x0004, /* 上升 */
    MSR_KEY_DOWN   = 0x0008, /* 下降 */
    MSR_KEY_PAUSE  = 0x0010, /* 暂停 */
    MSR_KEY_CMUT   = 0x0020, /* 通讯 */
    MSR_KEY_STOP   = 0x0040, /* 停止 */
    MSR_KEY_BOOT   = 0x0080, /* 启动 */
    MSR_KEY_PRE    = 0x0100, /* 预紧 */
    MSR_KEY_UNLOAD = 0x0200, /* 卸载 */
    MSR_KEY_ALL    = 0x03FF, /* 所有 */

    SLV_KEY_NONE   = 0x0000, /* 无键 */
    SLV_KEY_RSET   = 0x0001, /* 休眠 */
    SLV_KEY_SET    = 0x0002, /* 设置 */
    SLV_KEY_MUP    = 0x0004, /* 上升 */
    SLV_KEY_MDOWN  = 0x0008, /* 下降 */
    SLV_KEY_PAUSE  = 0x0010, /* 暂停 */
    SLV_KEY_CMUT   = 0x0020, /* 通信 */
    SLV_KEY_POFF   = 0x0040, /* 急停 */
    SLV_KEY_ALL    = 0x007F /* 所有 */
} KEYCODE_DEF_E;

typedef enum {
    MSR_LED_INCLINE_INDICAT    = 0,  //倾斜指示灯
    MSR_LED_OVER_LOADING       = 1,  //超载
    MSR_LED_UNDER_LOADING      = 2,  //欠载
    MSR_LED_LOSS_PHASE         = 3,  //缺相
    MSR_LED_SYNCHRO_START      = 4,  //同步启动
    MSR_LED_UP_INDICAT         = 5,  //上升
    MSR_LED_DOWN_INDICAT       = 6,  //下降
    MSR_LED_COMMUNICAT_INDICAT = 7,  //通讯
    MSR_LED_POWER_START        = 8,  //电源启动指示
    MSR_LED_ALL                = 9,  /* Off All Led */

    SLV_LED_OVER_LOADING       = 10, //超载
    SLV_LED_UNDER_LOADING      = 11, //欠载
    SLV_LED_LOSS_PHASE         = 12, //缺相
    SLV_LED_SK_INDICAT         = 13, //受控指示
    SLV_LED_UP_INDICAT         = 14, //上升
    SLV_LED_DOWN_INDICAT       = 15, //下降
    SLV_LED_COMMUNICAT_INDICAT = 16, //通讯
    SLV_LED_YCG_INDICAT        = 17, //右超高
    SLV_LED_WARN_INDICAT       = 18, //报警指示
    SLV_LED_ZCG_INDICAT        = 19, //左超高
    SLV_LED_ALL                = 20  /* Off All Led */
} LEDCODE_DEF_E;

typedef struct {
    KEYCODE_DEF_E secondKeyCode;
    KEYCODE_DEF_E firstKeyCode;
} KEYCODE_REC_S;


#define DEV_NUM (32)

typedef struct {
    u8 devAddr;
    u8 cpuid[8];
} DEV_INFO_S;

typedef struct  {
    u8 devnum; 
    DEV_INFO_S devinfo[DEV_NUM];
} ADDR_CONFIGURE_S;

typedef struct 
{
    /* System */
    u32 reboot_counter; /* reboot times  */

    /* Master Device Infomation and Configure */

    /* Slave Device Infomation and Configure */
    u8 slaveDeviceIsAlreadyConfigAddr;
    u8 slave_devnum; /* Slave device number*/
    DEV_INFO_S devinfo[DEV_NUM];

} SYS_INFO_S;

/*********************************************************/
/* Common */
void delay_us(u32 t);
void delay_ms(u32 t);
void delay5us(void);
void delay500ms();
u32 getSysTick(void);
bool isMasterDevice(void);
void init_Watch_Dog(void);
void Reboot_System(void);
void PrintSystemInfoToSerial(bool msterFlag);
bool checkAddrSyncStatus(void);
void readInitSystemInfo(void);
void updateRebootCounterInfo(void);
SYS_INFO_S *getSystemInfo(void);

/* Led Control */
void ledStatusManageService(u32 iSec);
void MSR_LedStatusCtrl(LEDCODE_DEF_E LedNo, bool StatusFlag);
void MSR_LedFlashCtrl(LEDCODE_DEF_E LedNo);
void SLV_LedStatusCtrl(LEDCODE_DEF_E LedNo, bool StatusFlag);
void SLV_LedFlashCtrl(LEDCODE_DEF_E LedNo);

/* HX710A */
u32 getHX710Count(u8 iNo);

/*max485*/
bool SendATCommand(u8 *cmd);

/* FZH181 */
void ledDisplayClose(u8 ledNo);
void ledDisplayCtrl(u8 ledNo, i8 dispVal);
void ledDisplayFlashEnable(u8 ledNo, bool bFlag);
void clrKeyStatus(KEYCODE_DEF_E sKey);
KEYCODE_REC_S* getKeyCode(void);


/* SD3178 */
/* PHASE CHECK */
void checkPhaseSeqALost(void);
void checkPhaseSeqBLost(void);
void checkPhaseSeqCLost(void);
u16 checkACPowerPhaseSequence(void);

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
void LedDisplay_Test(i8 Val);
bool LedKeyScan_Test(void);
void rtcDisplay_Test(void);
void hx710_Test(void);
void adxl345_Test(void);
#endif

/*********************************************************/
#endif
