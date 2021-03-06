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
#include "lcd2.h"



/* Definitions for the LCD interface */
#define PORT_D_CONFIG 0x00

#define LCD_SEL 3 /* Port A bit 3 ( Enables LCD ) */
#define LCD_WR  2 /* Port A bit 2 ( Logic 0 = Write ) */
#define LCD_RS  1 /* Port A bit 1 ( Register select ) */
#define LCD_DATA PORTD     /* The port the lcd data bus is connected to */
#define LCD_CONTROL PORTD  /* The port the lcd control bus is connected to */

#define BUSY_BIT 7
#define BUSY_MASK 0x80

#define LCD_DATA_4 4   /* LCD BIT 0 */
#define LCD_DATA_5 5   /* LCD BIT 1 */
#define LCD_DATA_6 6   /* LCD BIT 2 */
#define LCD_DATA_7 7   /* LCD BIT 3 */

#define LCD_LINE_LEN 16

/************************************************************/
/* LCD Commands ( Refer to LCD Data Sheet )                 */
/* Standard command should work with most common devices    */
/************************************************************/

#define clear_lcd         0x01 /* Clear Display                       */
#define return_home       0x02 /* Cursor to Home position             */
#define entry_mode        0x06 /* Normal entry mode                   */
#define entry_mode_shift  0x07 /* - with shift                        */
#define system_set_8_bit  0x38
/* 8 bit data mode 2 line ( 5x7 font ) */
#define system_set_4_bit  0x28
/* 4 bit data mode 2 line ( 5x7 font ) */
#define display_on        0x0c /* Switch ON Display                   */
#define display_off       0x08 /* Cursor plus blink                   */
#define set_dd_line1      0x80 /* Line 1 position 1                   */
#define set_dd_line2      0xC0 /* Line 2 position 1                   */
#define set_dd_ram        0x80 /* Line 1 position 1                   */
#define write_data        0x00 /* With rs = 1                         */
#define cursor_on         0x0E /* Switch Cursor ON                    */
#define cursor_off        0x0C /* Switch Cursor OFF                   */




char Modeflags, line;

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
   Delay();
   Write_8_Bit( system_set_4_bit );
   Delay();
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
   BIT_CLEAR( LCD_CONTROL, LCD_RS );

}

/***********************************/
/* Put LCD in Data Mode            */
/***********************************/

void DataMode(void)
{
   BIT_SET( ModeFlags, 0 ); /* Save Mode */
   BIT_SET( LCD_CONTROL, LCD_RS );
}

/***********************************/
/* Write a single byte to the LCD  */
/* 8 Bit Mode                      */
/***********************************/
void Write_8_Bit( char dh )
{

   BIT_CLEAR( dh, LCD_WR );      /* Write mode    */
   BIT_CLEAR( dh, LCD_RS );      /* Function mode */
   BIT_CLEAR( dh, LCD_SEL);

   //LCD_CONTROL = 0;
   BIT_CLEAR( dh, LCD_SEL );     /* de-select LCD */
   delay_ms(1);

   LCD_DATA = dh;                          /* Setup data    */
   #asm
   nop
   nop
   nop
   nop
   nop
   nop
   nop
   nop
   nop
   nop
   nop
   nop
   #endasm
   BIT_SET( LCD_CONTROL, LCD_SEL );        /* Select LCD    */
   Delay();
   BIT_CLEAR( LCD_CONTROL, LCD_SEL );     /* de-select LCD */
   Delay();

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

   BIT_CLEAR( dl, LCD_WR );            /* Write mode */

   if ( ( ModeFlags & 0x01 ) == 0 )
      BIT_CLEAR( dl, LCD_RS );         /* Function mode */
   else
      BIT_SET( dl, LCD_RS );           /* Data mode */

   BIT_CLEAR( dl, LCD_SEL);

   LCD_DATA = dl;                           /* Setup data    */
   Delay();
   BIT_SET( LCD_CONTROL, LCD_SEL);         /* Select LCD    */
   Delay();
   BIT_CLEAR( LCD_CONTROL, LCD_SEL );      /* de-select LCD */


   dl = e;                                  /* Restore lower 4 bits */
   dl <<= 4;

/*********************************** Output Lower 8 bits ******************************/

   BIT_CLEAR( dl, LCD_WR );       /* Write mode    */

   if ( ( ModeFlags & 0x01 ) == 0 )
      BIT_CLEAR( dl, LCD_RS );    /* Function mode */
   else
      BIT_SET( dl, LCD_RS );      /* Data mode */

   BIT_CLEAR( dl, LCD_SEL);
   LCD_DATA = dl;                           /* Setup data    */
   #asm
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
      nop
   #endasm
   BIT_SET( LCD_CONTROL, LCD_SEL);         /* Select LCD    */
   Delay();
   BIT_CLEAR( LCD_CONTROL, LCD_SEL );      /* de-select LCD */
   Delay();

}

/***********************************/
/* LCD timing delay                */
/* Adjust for your LCD Display     */
/***********************************/
void Delay(void)
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
   line = 1;
   FunctionMode();
   LcdWrite( set_dd_line1 );
   DataMode();
}

/***********************************/
/* Set Position to line 2          */
/***********************************/
void Line2(void)
{
   line = 2;
   FunctionMode();
   LcdWrite( set_dd_line2 );
   DataMode();
}

/*******************************************/
/* Clear Line 1                            */
/*******************************************/
void ClearLine1(void)
{
char c;

   Line1();

   for( c = 0; c < LCD_LINE_LEN; c++ )
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
char c;

  Line2();

  for( c = 0; c < 16; c++ )
     {
         LcdWrite(' ');
     }

  Line2();
}

/*******************************************/
/* Write a const string to the LCD         */
/*******************************************/
void WriteString( char *lcdptr )
{
char c;

  c = 0;
                // Check for end of string
    while( lcdptr[c] !=  0 )
         {
            // Don't display CR's
            if ( lcdptr[c] == 13 )  break;

            LcdWrite( lcdptr[c++] );// Display on LCD
         }

}




