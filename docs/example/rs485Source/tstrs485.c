//*********************************************************************************
// TsRs485.c
//
//                      Test of RS485 network protocol
//
//
// Version 1.0 beta 
//
// Processor: PIC16F87x
// Compiler: CCS PCM 3.023
//
//
// By: J.Winpenny
// j.winpenny@ukonline.co.uk
//
//
//
//*********************************************************************************
#DEVICE PIC16F873 *=16
#include <16f873.h>
#define MASTER

#use delay(clock=4000000)
#fuses HS,NOWDT,PUT,BROWNOUT,NOPROTECT,NOLVP,NOCPD,NOWRT

#include <Tst485.h>
#include <stdlib.h>
#include <Rs485.c>    // RS485 packet protocol driver
//#include <lcd2.c>   // LCD routine
#include <lcd873b.c>  // Keypad example LCD routine


#use fast_io(A)
#use fast_io(B)
#use fast_io(C)
#use fast_io(D)
#use fast_io(E)

char cTest[10];

//**************************************************
// Timer Interrupt service routine
//**************************************************
#INT_RTCC
clock_isr()
{
     TMR0 = TMR0_RELOAD;        // Reload PICs timer
     BIT_CLEAR( INTCON, T0IF ); // Clear interrupt flag
}

//**************************************************
// USART interrupt service routine
//**************************************************
#INT_RDA
void UsartInterrupt(void)
{

    while ( BIT_TEST( PIR1, RCIF )  )              // If USART RX Interrupt
    {

         TMR0 = TMR0_RELOAD;

         cRs485RxChar = RCREG;

         if ( !( RCSTA & 6 ) )                 // Then if no errors
         {                                     // Process received character
             Rs485Decode();                    // Decode RS485 network trafic
             bit_set( RCSTA, CREN );           // Enable receiver.
         }
         else
         {
             bit_clear( RCSTA, CREN );         // Clear any errors
             cRs485RxChar = RCREG;
             cRs485RxChar = RCREG;
             bit_set( RCSTA, CREN );           // Enable receiver.
         }

        bit_clear( PIR1, RCIF );

    }

}

void main(void)
{
char cPacketReady;
char cLenOfData;
char cCmd, c;

   Setup();
   BIT_CLEAR( INTCON, T0IE ); // Timer interrupts off

#ifndef MASTER
   LcdWrite("Waiting........");
#else
   LcdWrite("Sending message");
#endif

     // Just setup the network address
     //write_eeprom( NET_ADDRESS_HIGH, 0x10 );
     //write_eeprom( NET_ADDRESS_LOW, 0x10 );
     //cOurAddrHigh = read_eeprom( NET_ADDRESS_HIGH  );
     //cOurAddressLow = read_eeprom( NET_ADDRESS_LOW );

      while ( 1 )
      {

        cOurAddrHigh = 0x10;
        cOurAddrLow = 0x10;
        Rs485Initialise( cOurAddrHigh,cOurAddrLow );

        #ifdef MASTER
        cTest[0] = 'H';
        cTest[1] = 'e';
        cTest[2] = 'l';
        cTest[3] = 'l';
        cTest[4] = 'o';
        cTest[5] = 0;

        ClearLine2();
        LcdWrite("GET DATA PKT");
        Rs485SendPacket( SENSOR_GET_DATA, 5, &cTest[0] );
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        ClearLine2();
        LcdWrite("POLL PKT");
        Rs485SendPacket( SENSOR_POLL_CMD, 0, &cTest[0] );
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);

        cOurAddrHigh = 0x10;
        cOurAddrLow = 0x18;
        // Send a packet to another node
        Rs485Initialise( cOurAddrHigh,cOurAddrLow );

        Rs485SendPacket( SENSOR_POLL_CMD, 0, &cTest[0] );
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);
        delay_ms(250);

        #endif

        cPacketReady = Rs485Process();

        if ( cPacketReady )
        {
           Rs485GetPacket( &cCmd, &cLenOfData, &cTest[0] );

           ClearLine2();
           LcdWrite( "CMD: " );
           BinToHexAscii( cCmd );
           LcdWrite( ' ' );

           if ( cLenOfData != 0 )
           {
             for ( c=0; c<cLenOfData; c++ ) LcdWrite( cTest[c] );
           }

           //Rs485SendPacket( SENSOR_ACK, 0, NULL );
           delay_ms(255);
           Rs485Initialise(cOurAddrHigh,cOurAddrLow);
           ClearLine1();
           LcdWrite("Waiting........");
        }

   }

}


//**************************
// Setup  Hardware
//**************************
void Setup(void)
{
    OPTIONREG = 0x84;             //  pullups,prescailer = TMR0 , rate = 1:32
    INTCON = 0;                   // disable global interupts
    ADCON1 = 0x07;                // Disable ADC
    TRISA = PortAConfig;
    TRISB = PortBConfig;
    TRISC = PortCConfig;
    TRISD = PortDConfig;
    TRISE = PortEConfig;
    PORTC = 0xFF;
    LCDSetup();                         // Setup the LCD Display
    ConfigureComms();                  /* Configure USART for Asyncronous Comms */
    PORTC = 0xF7;
}


//*******************************************************/
//* Configure PIC16F87x USART for communications        */
//*                                                     */
//* Asynchronous mode                                   */
//* 8 data bits  ( For other rates see PIC16F8XX Data ) */
//* 1 stop bits                                         */
//* No Parity                                           */
//*                                                     */
//*******************************************************/
void ConfigureComms(void)
{
    BIT_SET( RCSTA, SPEN );    // Enable Serial port
    BIT_CLEAR( RCSTA, RX9 );   // 8 bit receive mode
    BIT_CLEAR( TXSTA, TX9 );   // 8 bit transmit mode

    // SPBRG = 0;              // SPBRG = 1  ( Set Baud rate 115,200 )
    // SPBRG = 5;              // SPBRG = 5  ( Set Baud rate 38,400 )
    // SPBRG = 22;             // SPBRG = 22 ( Set Baud rate 9,600 )
    // SPBRG = 11;             // SPBRG = 11 ( Set Baud rate 19,200 )
    // SPBRG = 22;             // SPBRG = 22 ( Set Baud rate 9,600 )
    // SPBRG = 47;             // SPBRG = 47 ( 9600 Baud at 7.3728 Mhx clock )
    // SPBRG = 22;             // SPBRG = 22 ( 9600 Baud at 3.579545Mhz clock )

    SPBRG = 25;                // SPBRG = 25 ( 9600 Baud at 4 Mhx clock )

    BIT_SET( TXSTA, BRGH );    // BRGH = 1   ( High speed mode )
    BIT_CLEAR( TXSTA, SYNC );  // Asyncronous mode;
    BIT_SET( TXSTA, TXEN );    // Enable Transmitter
    BIT_SET( RCSTA, CREN );    // Enable continuous receive
    BIT_SET( INTCON, PEIE );   // Enable all Peripheral Interrupts
}





