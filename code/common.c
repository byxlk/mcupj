
#include "api_config.h"

#define		IDATA_ID_START_ADDR	0xf1		//??idata?ID??????
#define		CODE_ID_START_ADDR	(8192-7)	//??code ?ID??????
u8	idata ID_idata[7];	//idata ??ID?
u8	idata ID_code[7];	//code  ??ID?

static void Delay_1us(void)		//@22.1184MHz
{
	unsigned char i;

	i = 3;
	while (--i);
}

static void Delay_1ms(void)		//@22.1184MHz
{
	unsigned char i, j;

	_nop_();
	_nop_();
	i = 22;
	j = 128;
	do
	{
		while (--j);
	} while (--i);
}

void delay_us(unsigned int t)
{
    while(t--) Delay_1us();
}

void delay_ms(unsigned int t)
{
    while(t--) Delay_1ms();
}

void delay500ms()		//@22.1184MHz
{
	unsigned char i, j, k;

	_nop_();
	_nop_();
	i = 43;
	j = 6;
	k = 203;
	do
	{
		do
		{
			while (--k);
		} while (--j);
	} while (--i);
}

void readCpuId()
{
	u8	idata *ip;
	u8	code  *cp;
	u8	i;
	
	for(i=0, ip=IDATA_ID_START_ADDR; ip<(IDATA_ID_START_ADDR+7); ip++)	ID_idata[i++] = *ip;
	for(i=0, cp=CODE_ID_START_ADDR;  cp<(CODE_ID_START_ADDR+7);  cp++)	ID_code[i++]  = *cp;
}

bool isMasterDevice(void)
{
    return MASTER_INDICATOR_FLAG;
}

void Reboot_System(void)
{
    //LOGD("Reboot System ...\n");
	//if run to here, system must be reboot now from user space
	if(IAP_CONTR & 0x40)
		IAP_CONTR &= 0x3F; // 0011 1111
	if(!(IAP_CONTR & 0x20))
		IAP_CONTR |= 0x60; // 0010 0000
}


void init_Watch_Dog(void)
{
    WDT_CONTR = 0x3A;  //0011 1010
	//LOGD(" Watch Dog start ... \r\n");
}

void System_PowerDown(void)
{
    //PCON |= 2	//MCU ?? ?? ??
    LOGD("System entery Power Down mode ...\r\n\r\n");

	_nop_();
    _nop_();
    _nop_();
    MCU_POWER_DOWN();
    _nop_();    // 
    _nop_();
    _nop_();
    _nop_();
}

bit POF_Boot_Delay(void)
{
    if(PCON & 0x10) {
		LOGD("Cold reset, delay 4s entery system ...\r\n");
        delay_ms(50)	; //boot delay 4s
        PCON &= 0xEF; // Clear POF register 

		return TRUE;
    } else {
		return FALSE;
    }
}


static void PrintSameString(u8 *puts, unsigned char nSize)
{
#if DEBUG
	unsigned char i = 0;
	if(nSize > 0)
	{ 
		for(i = 0; i < nSize; i++) 
			LOGD(puts);
		LOGD("\r\n");
	}
#endif
}

void PrintSystemInfoToSerial(bool msterFlag)
{	
#if DEBUG
	PrintSameString("*", 62);	   
	PrintSameString(" ", 1);
	LOGD("  Project Name  : Smart Climb Control System\r\n");
	LOGD("  Version No.   : Ver 1.0\r\n");
	LOGD("  Designed Time : 2021-01-01\r\n");
	LOGD("  Auth Name     : Li Shuliang\r\n");
	LOGD("  Auth Email    : 313424410@qq.com\r\n");
	LOGD("  Project Owner : RuiLong Tech.\r\n");
    if(msterFlag) LOGD("  Device Type   : Master\r\n");
    else LOGD("  Device Type   : Slave\r\n");
	PrintSameString(" ", 1);
	LOGD("  CopyRight     : All rights reserved @2021 by RuiLong Tech. \r\n");
	PrintSameString(" ", 1);
    PrintSameString("*", 62);
#endif
}
