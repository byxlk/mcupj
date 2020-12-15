#include "api_config.h"
#include "stclib/STC15Fxxxx.H"

/*************	本地常量声明	**************/

/*************	本地变量声明	**************/
//bit SwitchOnStatus_preStatus = 0;
//bit SwitchOffStatus_preStatus = 0;
//bit ExAutoCtrl_preStatus = 0;
//bit MotorRunningCtrl_Running = 0;//电机控制 -- 不转
//bit isSystemStatusChanged = 0;
//unsigned char TimeoutCount = STOP_TIMERCOUNT;
//unsigned char PinStatus = 0x00;  //high byte: last status; low byte:current status
//unsigned char idata BackUpStatus = 0x00;

/*************	本地函数声明	**************/

/*************  外部函数和变量声明 *****************/

#if 0
/*************  串口1初始化函数 *****************/
void	UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 38400ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityLow;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2
}

/******************** IO配置函数 **************************/
void GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //结构定义
#if DEBUG
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_5;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化

	GPIO_InitStructure.Pin  = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化

	GPIO_InitStructure.Pin  = GPIO_Pin_5;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
#else	
	GPIO_InitStructure.Pin  = GPIO_Pin_1 | GPIO_Pin_5;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化
	
	GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化
#endif
	// init output port status
	MotorRunningCtrl = MONTOR_STOP;  //shut down motor power
}

/************************ 定时器配置 ****************************/
void	Timer_Config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;			//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 20));	//初值, 节拍为20HZ(50ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
}
#endif

void doRunning_MasterMain(void)
{
	//MSR_GPIO_Config();//GPIO init
	//MSR_Timer_Config();//Timer init
	//MSR_UART_config(); //UART init	
	EA = 1;

	//PrintSystemInfoToSerial();
	//LOGD("=====================> Hardware Init Ok <=====================\r\n");

	//watch dog init
	WDT_CONTR = 0x3A;  //0011 1010
	//LOGD(" Watch Dog start ... \r\n");
	
	//while(1) {
		
	//}	
}

void doRunning_SlaveMain(void)
{
	//SLV_GPIO_Config();//GPIO init
	//SLV_Timer_Config();//Timer init
	//SLV_UART_config(); //UART init	
	EA = 1;

	//PrintSystemInfoToSerial();
	//LOGD("=====================> Hardware Init Ok <=====================\r\n");

	//watch dog init
	WDT_CONTR = 0x3A;  //0011 1010
	//LOGD(" Watch Dog start ... \r\n");
	
	//while(1) {
		
	//}	
}

static bool isMasterDevice(void)
{
    return MASTER_INDICATOR_FLAG;
}

void main(void)
{
	//
	if( isMasterDevice()) {
		doRunning_MasterMain();
	} else {
		doRunning_SlaveMain();
	}
	
	//if run to here, system must be reboot now from user space
	IAP_CONTR &= 0x3F; // 0011 1111
	IAP_CONTR |= 0x60; // 0010 0000
}