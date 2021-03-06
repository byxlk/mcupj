/***************************************************************************/
/*  Lcd.c       LCD interface routines                                     */
/*  Version :   2.1. for PIC16F87x                                         */
/*                                                                         */
/*  J. Winpenny  1/8/2000                                                  */
/*                                                                         */
/*                                                                         */
/*  Mode : HD44780 type LCD displays                                       */
/*                                                                         */
/*                                                                         */
/* Notes : Updated version                                                 */
/*         to allow greater adaptability to different displays             */
/*                                                                         */
/***************************************************************************/

#include "lcd873b.h"

/* Definitions for the LCD interface */

#define LCD_SEL 3 /* Port A bit 3 ( Enables LCD ) */
#define LCD_WR  2 /* Port A bit 2 ( Logic 0 = Write ) */
#define LCD_RS  0 /* Port A bit 1 ( Register select ) */
#define LCD_DATA PORTB
#define LCD_CONTROL PORTA


#define BUSY_BIT 7
#define BUSY_MASK 0x80

#define LCD_DATA_4 4   /* LCD BIT 0 */
#define LCD_DATA_5 5   /* LCD BIT 1 */
#define LCD_DATA_6 6   /* LCD BIT 2 */
#define LCD_DATA_7 7   /* LCD BIT 3 */


char ModeFlags;
char LCD_gp;


/***********************************/
/* Setup the lcd device            */
/***********************************/
void LCDSetup(void)
{
 /* Reset the LCD */

   ModeFlags = 0;           /* Default to Function Mode */


   Write_8_Bit( system_set_4_bit ); /* This sequence resets the LCD */
   delay_ms(5);
   Write_8_Bit( system_set_4_bit );
   delay_ms(1);
   Write_8_Bit( system_set_4_bit );
   delay_ms(1);
   LcdWrite( system_set_4_bit );

   LcdWrite( display_on );
   LcdWrite( clear_lcd );
   LcdWrite( entry_mode );
   LcdWrite( set_dd_ram );
   ModeFlags = 1; /* Data Mode */

}



/***********************************/
/* Put LCD in Function Mode        */
/***********************************/
void FunctionMode(void)
{
   BIT_CLEAR( ModeFlags, 0 ); /* Save Mode */
   #asm BCF LCD_CONTROL, LCD_RS #endasm
   Delaylcd();
}

/***********************************/
/* Put LCD in Data Mode            */
/***********************************/

void DataMode(void)
{
   BIT_SET( ModeFlags, 0 ); /* Save Mode */
   #asm BSF LCD_CONTROL, LCD_RS #endasm
   Delaylcd();
}

/***********************************/
/* Write a single byte to the LCD  */
/* 8 Bit Mode                      */
/***********************************/
void Write_8_Bit( char dh )
{
   #asm BCF LCD_CONTROL, LCD_WR #endasm
   #asm BCF LCD_CONTROL, LCD_RS #endasm
   #asm BCF LCD_CONTROL, LCD_SEL #endasm

   //LCD_CONTROL = 0;

   delay_ms(1);
   LCD_DATA = dh;                          /* Setup data    */
   BIT_SET( LCD_CONTROL, LCD_SEL );        /* Select LCD    */
   Delaylcd();
   BIT_CLEAR( LCD_CONTROL, LCD_SEL );     /* de-select LCD */
   Delaylcd();
   
}


/***********************************/
/* Write a single byte to the LCD  */
/* 4 Bit Mode                      */
/***********************************/
void LcdWrite(char dl )
{

char e;

   e = dl;                           /* Save lower 4 bits */

/*********************************** Output Upper 8 bits ******************************/

   BIT_CLEAR( LCD_CONTROL, LCD_WR );       /* Write mode */

   if ( ( ModeFlags & 0x01 ) == 0 )
      BIT_CLEAR( LCD_CONTROL, LCD_RS );    /* Function mode */
   else
      BIT_SET( LCD_CONTROL, LCD_RS );      /* Data mode */

   BIT_CLEAR( LCD_CONTROL, LCD_SEL);
   LCD_DATA = dl;                           /* Setup data    */

   BIT_SET( LCD_CONTROL, LCD_SEL);         /* Select LCD    */
   Delaylcd();
   BIT_CLEAR( LCD_CONTROL, LCD_SEL );      /* de-select LCD */


   dl = e;                                  /* Restore lower 4 bits */
   dl <<= 4;

/*********************************** Output Lower 8 bits ******************************/

   BIT_CLEAR( LCD_CONTROL, LCD_WR );       /* Write mode    */

   if ( ( ModeFlags & 0x01 ) == 0 )
      BIT_CLEAR( LCD_CONTROL, LCD_RS );    /* Function mode */
   else
      BIT_SET( LCD_CONTROL, LCD_RS );      /* Data mode */

   BIT_CLEAR( LCD_CONTROL, LCD_SEL);
   LCD_DATA = dl;                           /* Setup data    */

   BIT_SET( LCD_CONTROL, LCD_SEL);         /* Select LCD    */
   Delaylcd();
   BIT_CLEAR( LCD_CONTROL, LCD_SEL );      /* de-select LCD */
   Delaylcd();
}

/***********************************/
/* LCD timing delay                */
/* Adjust for your LCD Display     */
/***********************************/
void Delaylcd(void)
{
   delay_ms(2);
}

/***********************************/
/* Clear LCD Screen                */
/***********************************/
void Clear(void)
{

   FunctionMode();
   LcdWrite(clear_lcd);
   DataMode();

}

/***********************************/
/* Set the cursor position         */
/***********************************/
void SetPos(char Pos)
{

   FunctionMode();
   LcdWrite( Pos );
   DataMode();

}

/***********************************/
/* Set Position to line 1          */
/***********************************/
void Line1(void)
{
   FunctionMode();
   LcdWrite( set_dd_line1 );
   DataMode();
}

/***********************************/
/* Set Position to line 2          */
/***********************************/
void Line2(void)
{
   FunctionMode();
   LcdWrite( set_dd_line2 );
   DataMode();
}

/*******************************************/
/* Clear Line 1                            */
/*******************************************/
void ClearLine1(void)
{

   Line1();

   for( LCD_gp = 0; LCD_gp < 16; LCD_gp++ )
      {
          LcdWrite(' ');
      }

   Line1();
}

/*******************************************/
/* Clear Line 2                            */
/*******************************************/
void ClearLine2(void)
{

  Line2();

  for( LCD_gp = 0; LCD_gp < 16; LCD_gp++ )
     {
         LcdWrite(' ');
     }

  Line2();
}

