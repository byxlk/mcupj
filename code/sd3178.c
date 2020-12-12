extern I2C_HandleTypeDef hi2c2;

/****************** 本地变量 **********************/
u8    data_Buf[8];
u8    VBAT_Buf[2];
u16    VBAT_VAL;
Time_Def time_init={0x01,0x19,0x12,0x05,0x23,0x10,0x20};    //初始化实时时间2020-10-23 12:19:01
Time_Def Alarm_init={0x30,0x19,0x17,0x00,0x15,0x08,0x18};    //初始化报警时间
Time_Def sysTime;



/*********************************************
 * 函数名：WriteRTC_Enable
 * 描  述：RTC写允许程序
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
 ********************************************/
HAL_StatusTypeDef WriteRTC_Enable(void)
{
    
        uint8_t ret=0x80;
    static HAL_StatusTypeDef i;
        i = HAL_I2C_Mem_Write(&hi2c2,RTC_Address,CTR2,I2C_MEMADD_SIZE_8BIT,&ret,1,100);

        ret=0x84;
        i = HAL_I2C_Mem_Write(&hi2c2,RTC_Address,CTR1,I2C_MEMADD_SIZE_8BIT,&ret,1,100);
    
    return i;
}

/*********************************************
 * 函数名：WriteRTC_Disable
 * 描  述：RTC写禁止程序
 * 输  入：无
 * 输  出：TRUE:操作成功，FALSE:操作失败
 ********************************************/
HAL_StatusTypeDef WriteRTC_Disable(void)
{
        uint8_t ret[]={0x0,0x0};
    static HAL_StatusTypeDef i;
        i = HAL_I2C_Mem_Write(&hi2c2,RTC_Address,CTR1,I2C_MEMADD_SIZE_8BIT,ret,2,100);
    return i; 
}

/*********************************************
 * 函数名：RTC_WriteDate
 * 描  述：写RTC实时数据寄存器
 * 输  入：时间结构体指针
 * 输  出：TRUE:操作成功，FALSE:操作失败
 ********************************************/
HAL_StatusTypeDef RTC_WriteDate(Time_Def    *psRTC)    //写时间操作要求一次对实时时间寄存器(00H~06H)依次写入，
{                               //不可以单独对7个时间数据中的某一位进行写操作,否则可能会引起时间数据的错误进位. 
                                //要修改其中某一个数据 , 应一次性写入全部 7 个实时时钟数据.
    static HAL_StatusTypeDef i;
    psRTC->hour=psRTC->hour|0x80;
      i =I2CWriteSerial(RTC_Address, 0, 7, (uint8_t *)psRTC);
        return    i;
}

/*********************************************
 * 函数名：RTC_ReadDate
 * 描  述：读RTC实时数据寄存器
 * 输  入：时间结构体指针
 * 输  出：TRUE:操作成功，FALSE:操作失败
 ********************************************/
HAL_StatusTypeDef RTC_ReadDate(Time_Def    *psRTC)
{
    static HAL_StatusTypeDef i;
    i=I2CReadSerial(RTC_Address, 0, 7, (uint8_t *)psRTC);
      psRTC->hour=psRTC->hour & 0x7F;
        return    i;
}

/*********************************************
 * 函数名     ：I2CWriteSerial
 * 描  述     ：I2C在指定地址写N字节数据
 * Device_Addr：I2C设备地址
 * Address    ：内部地址
 * length     ：字节长度
 * ps         ：缓存区指针
 * 输出       ：TRUE 成功，FALSE 失败
 ********************************************/    
HAL_StatusTypeDef I2CWriteSerial(u8 DeviceAddress, u8 Address, u8 length, u8 *ps)
{

    static HAL_StatusTypeDef i;
        if(DeviceAddress == RTC_Address)  WriteRTC_Enable();
    
    i = HAL_I2C_Mem_Write(&hi2c2,DeviceAddress,Address,I2C_MEMADD_SIZE_8BIT,ps,length,100);
    HAL_Delay(5);    

        if(DeviceAddress == RTC_Address)  WriteRTC_Disable();
        return    i;
}

/*********************************************
 * 函数名     ：I2CReadSerial
 * 描  述     ：I2C在指定地址写N字节数据
 * Device_Addr：I2C设备地址
 * Address    ：内部地址
 * length     ：字节长度
 * ps         ：缓存区指针
 * 输出       ：TRUE 成功，FALSE 失败
 ********************************************/    
HAL_StatusTypeDef I2CReadSerial(u8 DeviceAddress, u8 Address, u8 length, u8 *ps)
{
      static HAL_StatusTypeDef i;
        if(DeviceAddress == RTC_Address)  WriteRTC_Enable();
    
    i=HAL_I2C_Mem_Read(&hi2c2,DeviceAddress,Address,I2C_MEMADD_SIZE_8BIT,ps,length,100);
    HAL_Delay(5);    
    
        if(DeviceAddress == RTC_Address)  WriteRTC_Disable();
        return    i;
    
}

/*********************************************
 * 函数名：Set_CountDown
 * 描  述：设置倒计时中断
 * 输  入：CountDown_Init 倒计时中断结构体指针 
 * 输  出：无
 ********************************************/
void Set_CountDown(CountDown_Def *CountDown_Init)                    
{
        u8 buf[6];
        u8 tem=0xF0;
        buf[0] = (CountDown_Init->IM<<6)|0xB4;                            //10H
        buf[1] = CountDown_Init->d_clk<<4;                                    //11H
        buf[2] = 0;                                                                                    //12H
        buf[3] = CountDown_Init->init_val & 0x0000FF;                //13H
        buf[4] = (CountDown_Init->init_val & 0x00FF00) >> 8;//14H
        buf[5] = (CountDown_Init->init_val & 0xFF0000) >> 16;//15H
        I2CWriteSerial(RTC_Address,CTR2,1,&tem);
        I2CWriteSerial(RTC_Address,CTR2,6,buf);
}

/*********************************************
 * 函数名：Set_Alarm
 * 描  述：设置报警中断（闹钟功能）
 * Enable_config：使能设置  
 * psRTC：报警时间的时间结构体指针
 * 输  出：无
 ********************************************/
void Set_Alarm(u8 Enable_config, Time_Def *psRTC)                    
{
        u8 buf[10];
        buf[0] = psRTC->second;
        buf[1] = psRTC->minute;
        buf[2] = psRTC->hour;
        buf[3] = 0;
        buf[4] = psRTC->day;
        buf[5] = psRTC->month;
        buf[6] = psRTC->year;
        buf[7] = Enable_config;
        buf[8] = 0xFF;
        buf[9] = 0x92;    
        I2CWriteSerial(RTC_Address,Alarm_SC,10,buf);
}

/*********************************************
 * 函数名：SetFrq
 * 描  述：设置RTC频率中断，从INT脚输出频率方波
 * 输  入：频率值
 * 输  出：无
 ********************************************/
void SetFrq(enum Freq F_Out)                    
{
        u8 buf[2];
        buf[0] = 0xA1;
        buf[1] = F_Out;
        I2CWriteSerial(RTC_Address,CTR2,2,buf);
}

/*********************************************
 * 函数名：ClrINT
 * 描  述：禁止中断
 * int_EN：中断类型 INTDE、INTDE、INTDE
 * 输  出：无
 ********************************************/
void ClrINT(u8 int_EN)         
{
        u8 buf;
        buf = 0x80 & (~int_EN);
        I2CWriteSerial(RTC_Address,CTR2,1,&buf);
}
/*********************************************END OF FILE**********************/