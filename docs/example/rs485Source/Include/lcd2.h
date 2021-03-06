/***********************************************/
/* LCD.H   Header for LCD routines             */
/***********************************************/

   void WaitBusyFlag(void);
   void LCDSetup(void);
   void FunctionMode(void);
   void DataMode(void);
   void Write_8_Bit( char dh );
   void LcdWrite(char dl );
   void Delay(void);
   void Clear(void);
   void SetPos(char Pos);
   void Line1(void);
   void Line2(void);
   void ClearLine1(void);
   void ClearLine2(void);
   void WriteString( char *lcdptr );

