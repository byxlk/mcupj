
#include "api_config.h"

#define		IDATA_ID_START_ADDR	0xf1		//??idata?ID??????
#define		CODE_ID_START_ADDR	(8192-7)	//??code ?ID??????
u8	idata ID_idata[7];	//idata ??ID?
u8	idata ID_code[7];	//code  ??ID?

static void Delay_1us(void)		//@24.000MHz
{
	unsigned char i;

	_nop_();
	_nop_();
	i = 3;
	while (--i);
}

static void Delay_1ms(void)		//@24.000MHz
{
	unsigned char i, j;

	i = 24;
	j = 85;
	do
	{
		while (--j);
	} while (--i);
}

void delay_us(unsigned int t)
{
    while(t--) Delay_1ms();
}

void delay_ms(unsigned int t)
{
    while(t--) Delay_1us();
}

void readCpuId()
{
	u8	idata *ip;
	u8	code  *cp;
	u8	i;
	
	for(i=0, ip=IDATA_ID_START_ADDR; ip<(IDATA_ID_START_ADDR+7); ip++)	ID_idata[i++] = *ip;
	for(i=0, cp=CODE_ID_START_ADDR;  cp<(CODE_ID_START_ADDR+7);  cp++)	ID_code[i++]  = *cp;
}