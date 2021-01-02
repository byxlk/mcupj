#include "api_config.h"
#include "STC15Fxxxx.H"
#include "USART.h"
#include "GPIO.h"
#include "timer.h"
#include "ADC.h"
#include "Exti.h"
#include "spi.h"



/*************	本地常量声明	**************/

/*************	本地变量声明	**************/
static unsigned short phaseSeq = 0x0;


/*************	本地函数声明	**************/

/*************  外部函数和变量声明 *****************/

/*************  串口1初始化函数 *****************/
static void UART_config(void)
{
	COMx_InitDefine		COMx_InitStructure;					//结构定义

    /* Debug Uart Config */
	COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
	COMx_InitStructure.UART_BaudRate  = 38400ul;			//波特率, 一般 110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.BaudRateDouble = DISABLE;			//波特率加倍, ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityHigh;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART1_SW_P30_P31;	//切换端口,   UART1_SW_P30_P31,UART1_SW_P36_P37,UART1_SW_P16_P17(必须使用内部时钟)
	COMx_InitStructure.UART_RXD_TXD_Short = DISABLE;		//内部短路RXD与TXD, 做中继, ENABLE,DISABLE
	USART_Configuration(USART1, &COMx_InitStructure);		//初始化串口1 USART1,USART2

    /* MAX485 Uart Config */
    COMx_InitStructure.UART_Mode      = UART_8bit_BRTx;		//模式,       UART_ShiftRight,UART_8bit_BRTx,UART_9bit,UART_9bit_BRTx
	COMx_InitStructure.UART_BRT_Use   = BRT_Timer2;			//使用波特率,   BRT_Timer1, BRT_Timer2 (注意: 串口2固定使用BRT_Timer2)
    COMx_InitStructure.UART_BaudRate  = 38400ul;			//波特率,     110 ~ 115200
	COMx_InitStructure.UART_RxEnable  = ENABLE;				//接收允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Interrupt = ENABLE;				//中断允许,   ENABLE或DISABLE
	COMx_InitStructure.UART_Polity    = PolityHigh;			//中断优先级, PolityLow,PolityHigh
	COMx_InitStructure.UART_P_SW      = UART2_SW_P46_P47;	//切换端口,   UART2_SW_P10_P11,UART2_SW_P46_P47
	USART_Configuration(USART2, &COMx_InitStructure);		//初始化串口2 USART1,USART2
}

/******************** IO配置函数 **************************/
static void MSR_SLV_CheckGpioConfig(void)
{
    GPIO_InitTypeDef	GPIO_InitStructure;		            //结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;     //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P5,&GPIO_InitStructure);
}
static void MSR_GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	           //初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_7;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	           //初始化
    
	GPIO_InitStructure.Pin  = GPIO_Pin_1;	               //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_All;	               //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	           //初始化
    
    GPIO_InitStructure.Pin  = GPIO_Pin_4;	               //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	           //初始化
    
	GPIO_InitStructure.Pin  = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	           //初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_4 | GPIO_Pin_5;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	           //初始化

    // init output port status

}

static void SLV_GPIO_Config(void)
{
	GPIO_InitTypeDef	GPIO_InitStructure;		            //结构定义

    GPIO_InitStructure.Pin  = GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_5 | GPIO_Pin_6;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	           //初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_2 | GPIO_Pin_7;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_PullUp;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P0,&GPIO_InitStructure);	           //初始化
    
	GPIO_InitStructure.Pin  = GPIO_Pin_1;	               //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P1,&GPIO_InitStructure);	           //初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_All;	               //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P2,&GPIO_InitStructure);	           //初始化
    
    GPIO_InitStructure.Pin  = GPIO_Pin_4;	               //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	           //初始化
    
	GPIO_InitStructure.Pin  = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;	//指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_HighZ;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P3,&GPIO_InitStructure);	           //初始化

    GPIO_InitStructure.Pin  = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;	   //指定要初始化的IO, GPIO_Pin_0 ~ GPIO_Pin_7, 或操作
	GPIO_InitStructure.Mode = GPIO_OUT_PP;		           //指定IO的输入或输出方式,GPIO_PullUp,GPIO_HighZ,GPIO_OUT_OD,GPIO_OUT_PP
	GPIO_Inilize(GPIO_P4,&GPIO_InitStructure);	           //初始化

	// init output port status

}

static void SLV_SPI_Config(void)
{
	SPI_InitTypeDef		SPI_InitStructure;
	SPI_InitStructure.SPI_Module    = ENABLE;              //SPI启动    ENABLE, DISABLE
	SPI_InitStructure.SPI_SSIG      = DISABLE;			  //片选位     ENABLE, DISABLE
	SPI_InitStructure.SPI_FirstBit  = SPI_MSB;			  //移位方向   SPI_MSB, SPI_LSB
	SPI_InitStructure.SPI_Mode      = SPI_Mode_Slave;	  //主从选择   SPI_Mode_Master, SPI_Mode_Slave
	SPI_InitStructure.SPI_CPOL      = SPI_CPOL_High;      //时钟相位   SPI_CPOL_High,   SPI_CPOL_Low
	SPI_InitStructure.SPI_CPHA      = SPI_CPHA_1Edge;	  //数据边沿   SPI_CPHA_1Edge,  SPI_CPHA_2Edge
	SPI_InitStructure.SPI_Interrupt = ENABLE;			  //中断允许   ENABLE,DISABLE
	SPI_InitStructure.SPI_Speed     = SPI_Speed_16;		  //SPI速度    SPI_Speed_4, SPI_Speed_16, SPI_Speed_64, SPI_Speed_128
	SPI_InitStructure.SPI_IoUse     = SPI_P12_P13_P14_P15; //IO口切换   SPI_P12_P13_P14_P15, SPI_P24_P23_P22_P21, SPI_P54_P40_P41_P43
	SPI_Init(&SPI_InitStructure);
	
	SPI_TxRxMode = SPI_Mode_Slave;
}

/************************ 定时器配置 ****************************/
static void Timer_Config(void)
{
	TIM_InitTypeDef		TIM_InitStructure;					//结构定义
	TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityLow;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_12T;		    //指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (12 * 20));	//初值, 节拍为20HZ(50ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer0,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
    
    TIM_InitStructure.TIM_Mode      = TIM_16BitAutoReload;	//指定工作模式,   TIM_16BitAutoReload,TIM_16Bit,TIM_8BitAutoReload,TIM_16BitAutoReloadNoMask
	TIM_InitStructure.TIM_Polity    = PolityHigh;			//指定中断优先级, PolityHigh,PolityLow
	TIM_InitStructure.TIM_Interrupt = ENABLE;				//中断是否允许,   ENABLE或DISABLE
	TIM_InitStructure.TIM_ClkSource = TIM_CLOCK_1T;		//指定时钟源,     TIM_CLOCK_1T,TIM_CLOCK_12T,TIM_CLOCK_Ext
	TIM_InitStructure.TIM_ClkOut    = DISABLE;				//是否输出高速脉冲, ENABLE或DISABLE
	TIM_InitStructure.TIM_Value     = 65536 - (MAIN_Fosc / (1 * 1000));	//初值, 节拍为1000HZ(1ms)
	TIM_InitStructure.TIM_Run       = ENABLE;				//是否初始化后启动定时器, ENABLE或DISABLE
	Timer_Inilize(Timer1,&TIM_InitStructure);				//初始化Timer0	  Timer0,Timer1,Timer2
}

static void MSR_External_Interrupt_Config(void)
{
    //INT0
    EXTI_InitTypeDef	EXTI_InitStructure;					//结构定义

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
    
    EXTI_InitStructure.EXTI_Mode      = EXT_MODE_Fall;	    //中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT3,&EXTI_InitStructure);				//初始化INT1	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}

static void SLV_External_Interrupt_Config(void)
{
    //INT0 and INT1
    EXTI_InitTypeDef	EXTI_InitStructure;					//结构定义

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT0,&EXTI_InitStructure);				//初始化INT0	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4

	EXTI_InitStructure.EXTI_Mode      = EXT_MODE_RiseFall;	//中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityLow;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT1,&EXTI_InitStructure);				//初始化INT1	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
    
    EXTI_InitStructure.EXTI_Mode      = EXT_MODE_Fall;	    //中断模式,  	EXT_MODE_RiseFall, EXT_MODE_Fall
	EXTI_InitStructure.EXTI_Polity    = PolityHigh;			//中断优先级,   PolityLow,PolityHigh
	EXTI_InitStructure.EXTI_Interrupt = ENABLE;				//中断允许,     ENABLE或DISABLE
	Ext_Inilize(EXT_INT3,&EXTI_InitStructure);				//初始化INT1	EXT_INT0,EXT_INT1,EXT_INT2,EXT_INT3,EXT_INT4
}

static void doRunning_MasterMain(void)
{
    /* Step1: */
    EA = 0; // disbale all interrupt

	MSR_GPIO_Config();//GPIO init
    MSR_External_Interrupt_Config();
    
    /* Debug Uart(P3.0/P3.1) and MAX485 Uart(P4.6/P4.7) init */
    UART_config();

	Timer_Config();//Timer init

	EA = 1; // enable all interrupt

	PrintSystemInfoToSerial(TRUE);
	LOGD("====> Master Device Hardware Init Ok \r\n");

	//watch dog init
    init_Watch_Dog();
    
    
	while(1) {
        /* Step1: Check AC Power PhaseSequence */
        phaseSeq = checkACPowerPhaseSequence();
        if((phaseSeq & 0xF000) == 0) {/* 反序 */
            /* Step1: Show alart led light */ 
            /* Step1: Flasher alart show phase info on LED display  */
            continue;
        }

	}	
}

static void doRunning_SlaveMain(void)
{
    EA = 0; // disbale all interrupt

    SLV_GPIO_Config();//GPIO init
    SLV_SPI_Config();
    SLV_External_Interrupt_Config();

    /* Debug Uart(P3.0/P3.1) and MAX485 Uart(P4.6/P4.7) init */
    UART_config();
	Timer_Config();//Timer init

	EA = 1; // enable all interrupt

	PrintSystemInfoToSerial(FALSE);
	LOGD("====> Slave Device Hardware Init Ok \r\n");

	//watch dog init
    init_Watch_Dog();
	
	while(1) {
		NOP(5);
	}
}

static bool isMasterDevice(void)
{
    return MASTER_INDICATOR_FLAG;
}

void main(void)
{
	// Check device is master or slave
    MSR_SLV_CheckGpioConfig();
	if( isMasterDevice()) {
		doRunning_MasterMain();
	} else {
		doRunning_SlaveMain();
	}
	
	//if run to here, system must be reboot now from user space
    Reboot_System();
}

/********************* Timer0中断函数************************/
#define TIMER_VALUE (65536 - (MAIN_Fosc / (12 * 50)))
void timer0_int (void) interrupt TIMER0_VECTOR //20ms@24.000MHz
{
	// process watch dog signal
	WDT_CONTR &= 0x7F;
    WDT_CONTR |= 0x10;


    /* 减小定时器误差 */
	//TL0 = TIMER_VALUE % 256 - TL0;
	//TH0 = TIMER_VALUE / 256 - TH0;
}