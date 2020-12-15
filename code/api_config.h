#ifndef		__CONFIG_H
#define		__CONFIG_H

#include "stclib/STC15Fxxxx.H"

/*********************************************************/

#define DEBUG_MODE 0				//开启调试模式
#define bool bit

/*********************************************************/
sbit MASTER_INDICATOR_FLAG = P5^5; // 1: 主机   0：从机
sbit HARDWARE_DEBUG_FLAG = P5^4;  // 1:开启debug 0：禁用debug

sbit MSR_FZH181_CLK = P0^6;
sbit MSR_FZH181_DIO = P0^7;
//sbit MSR_FZH181_STB = P3^3; INT1

sbit MSR_LED_PHASE_INDICAT = P2^7; //相序指示灯
sbit MSR_LED_OVER_LOADING = P2^6; //超载
sbit MSR_LED_UNDER_LOADING = P2^5; //欠载
sbit MSR_LED_LOSS_PHASE = P2^4; //缺相
sbit MSR_LED_SYNCHRO_START = P2^3; //同步启动
sbit MSR_LED_UP_INDICAT = P2^2; //上升
sbit MSR_LED_DOWN_INDICAT = P2^1; //下降
sbit MSR_LED_COMMUNICAT_INDICAT = P2^0; //通讯
sbit MSR_LED_POWER_START = P4^5; //电源启动指示

sbit MSR_MAX485_EN2 = P1^1;
//sbit MSR_MAX485_TX2 = P4^7; //TXD2
//sbit MSR_MAX485_RX2 = P4^6; //RXD2

sbit MSR_MAX485_EN0 = P3^4;
//sbit MSR_MAX485_TX0 = P3^1; //TXD0
//sbit MSR_MAX485_RX0 = P3^0; //RXD0

sbit MSR_SD3178_SCK = P0^3;
sbit MSR_SD3178_SDA = P0^4;
//sbit MSR_SD3178_INT = P3^2; // INT0

sbit MSR_PHASE_A = P3^5; //
sbit MSR_PHASE_B = P3^6; //INT2
sbit MSR_PHASE_C = P3^7; //INT3

sbit MSR_WARN_OUTPUT = P4^4; //报警输出

/*********************************************************/
// 从机pin脚定义
sbit SLV_FZH181_CLK = P0^6;
sbit SLV_FZH181_DIO = P0^7;
//sbit SLV_FZH181_STB = P3^3; INT1

sbit SLV_LED_ZCG_INDICAT = P4^5; //左超高
sbit SLV_LED_OVER_LOADING = P2^6; //超载
sbit SLV_LED_UNDER_LOADING = P2^5; //欠载
sbit SLV_LED_LOSS_PHASE = P2^4; //缺相
sbit SLV_LED_SK_INDICAT = P2^3; //受控指示
sbit SLV_LED_UP_INDICAT = P2^2; //上升
sbit SLV_LED_DOWN_INDICAT = P2^1; //下降
sbit SLV_LED_COMMUNICAT_INDICAT = P2^0; //通讯
sbit SLV_LED_YCG_INDICAT = P2^7; //右超高
sbit SLV_LED_WARN_INDICAT = P4^4; //报警指示

sbit SLV_MAX485_EN1 = P3^4;
//sbit SLV_MAX485_TX1 = P3^1; //TXD0
//sbit SLV_MAX485_RX1 = P3^0; //RXD0

sbit SLV_HX710A1_SCK = P0^1;
sbit SLV_HE710A1_SDA = P0^0;
sbit SLV_HX710A2_SCK = P0^3;
sbit SLV_HE710A2_SDA = P0^2;

sbit SLV_PHASE_A = P3^5;
sbit SLV_PHASE_B = P3^6;
sbit SLV_PHASE_C = P3^7;

sbit SLV_RELAY_CTRL1_XJ = P4^1;
sbit SLV_RELAY_CTRL1_SS = P4^0;
sbit SLV_RELAY_CTRL2_XJ = P4^3;
sbit SLV_RELAY_CTRL2_SS = P4^2;

sbit SLV_ADXL345_CS = P1^2;
sbit SLV_ADXL345_SDA = P1^3;
sbit SLV_ADXL345_SDO = P1^4;
sbit SLV_ADXL345_CLK = P1^5;

/*********************************************************/

/*********************************************************/
#endif