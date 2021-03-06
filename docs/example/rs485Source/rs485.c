//**************************************************************************
// Rs495.c
//                      RS485 network protocol
//
// Version 1.0 beta
//
// Processor: PIC16F87x
//
// Network packet protocol:
//
// STX ( 0x02 )
// NET ADDRESS HIGH  
// NET ADDRESS LOW
// PAYLOAD LENGTH
// COMMAND
// DATA ( Optional )
// CRC HIGH
// CRC LOW
//
//
//
//
//
// By: J.Winpenny
// j.winpenny@ukonline.co.uk
//
//**************************************************************************
#include <Rs485.h>
#include <lcd2.h>
// #include <lcd873.h>



//struct
//{
static char cOurAddrHigh;
static char cOurAddrLow;
static char cRs485RxChar;
static char cRS485State;

static char cStart;
static char cNetAddrHigh, cNetAddrLow;
static char cLenExpected;
static char cCommand;
static char c485Buf[64];
static char cRxCrcHigh, cRxCrcLow;
static char cCalcCrcHigh, cCalcCrcLow;
static char cBufPtr;
static char cError;
static char cCrcTmp, cCrcTmp5, cCrcTmp4, cCrcTmp3, cCrcTmp2;
//} RS485_Protocol;


//****************************************************************************
// void Rs485Initialise(void)
//
// Initialise RS485 network driver
//****************************************************************************
void Rs485Initialise(char cAddrHigh, char cAddrLow)
{
  cOurAddrHigh = cAddrHigh;
  cOurAddrLow = cAddrLow;
  cRS485State = PKT_WAIT_START;
  BIT_CLEAR( RS485_CONTROL, OUTPUTS_ON );           // Disable driver
  BIT_SET( PIE1, RCIE );                            // Enable Receive Interrupt
}


//****************************************************************************
// char PacketForUs(void)
//
// Decide if packet valid and destined for this node.
// Ignore invalid packets and packets for other nodes
//
//****************************************************************************
#separate
char Rs485Process(void)
{
char cOurPkt, cPktReady;

    cOurPkt = FALSE;
    cPktReady = FALSE;

    disable_interrupts(GLOBAL);

    if ( cRS485State == PKT_COMPLETE )
    {
       	if ( ( cNetAddrHigh == cOurAddrHigh )&&   // Invalid and destined for this node
					( cNetAddrLow == cOurAddrLow ) )
			{
			   cOurPkt = TRUE;
			}
			else
			{
			   ClearLine2();
			   LcdWrite("traffic");                  // Network traffic for other nodes
            delay_ms(200);
      	}

      cRS485State = PostValidatePacket();         // Validate packet CRC

		if ( (cRS485State == PKT_INVALID)||(cRS485State == PKT_VALID) )
		{
			// Reject invalid packets
			if ( cRS485State == PKT_INVALID )          // NAK our invalid packets
			{
			   ClearLine2();
			   if ( cError == BAD_CRC ) LcdWrite("Bad CRC");
            else if ( cError == BAD_LENGTH ) LcdWrite("Bad length");
            delay_ms(200);
			   //if ( cOurPkt ) Rs485SendPacket( SENSOR_NAK, 0, NULL );
			   cRS485State = PKT_WAIT_START;
			}
			else if ( cRS485State == PKT_VALID )       // If packet valid
			{                                          // and destined for this node
			   if ( cOurPkt ) cPktReady = TRUE;
			   else  cRS485State = PKT_WAIT_START;
			}
		}
   }

   enable_interrupts(GLOBAL);

 return cPktReady;

}


//****************************************************************************
// void Rs485Decode(void)
//
// Decode an incomming packet on the RS485 network
//
// Expecting:
// START,
// NETWORK ADDRESS_HIGH,
// NETWORK ADDRESS_LOW,
// PAYLOAD LENGTH,
// COMMAND,
// optional DATA,
// CRC HIGH,
// CRC LOW
//
//****************************************************************************
#separate
char Rs485Decode( void )
{

   switch ( cRS485State )
   {
        case    PKT_WAIT_START:  cStart = cRs485RxChar;
                                 if ( cRs485RxChar == PKT_START ) // Check for the start of packet byte
                                 {
                                    cRS485State++;
                                 }
                                 break;

        case PKT_WAIT_ADDR_HIGH: cNetAddrHigh = cRs485RxChar;
                                 cRS485State++;
                                 break;

        case  PKT_WAIT_ADDR_LOW: cNetAddrLow = cRs485RxChar;
                                 cRS485State++;
                                 break;

        case       PKT_WAIT_LEN: cLenExpected = cRs485RxChar;
                                 if ( cLenExpected > sizeof(c485Buf) )
                                 {
                                    cRS485State = PKT_INVALID;
                                    cError = BAD_LENGTH;
                                 }
                                 else
                                 {
                                   cBufPtr = 0;
                                   cRS485State++;
                                 }
                                 break;

        case           PKT_CMD:  cCommand = cRs485RxChar;
                                 if ( PacketHasPayload() ) cRS485State = PKT_WAIT_DATA;
                                 else cRS485State = PKT_WAIT_CRC_HIGH;
                                 break;

        case     PKT_WAIT_DATA:  c485Buf[cBufPtr] = cRs485RxChar;
                                 cBufPtr++;
                                 if ( cBufPtr == cLenExpected ) // If last byte of data received
                                 {
                                   cRS485State++;               // next byet is the CRC high byte
                                 }
                                 break;

        case PKT_WAIT_CRC_HIGH:  cRxCrcHigh = cRs485RxChar;
                                 cRS485State++;
                                 break;

        case  PKT_WAIT_CRC_LOW:  cRxCrcLow = cRs485RxChar;
                                 cRS485State = PKT_COMPLETE;
                                 break;

        case      PKT_COMPLETE:  break;       // Idle state
        case         PKT_VALID:  break;       // Idle state
        case       PKT_INVALID:  break;       // Idle state

        default:                 cRS485State = PKT_WAIT_START;
                                 break;
     }

 return cRS485State;
}


//****************************************************************************
// void Rs485SendPacket( char cAddr, char cCmd, char cLen, char *cData )
//
// Send a packet over the RS485 link
//
// Input: NETWORK_ADDRESS, COMMAND, PAYLOAD_LENGTH, optional DATA
//
//****************************************************************************
void Rs485SendPacket( char cCmd, char cLen, char *cData )
{
char c, d;

   BIT_CLEAR( PIE1, RCIE );                          // Disable Receive Interrupt
   BIT_SET( RS485_CONTROL, OUTPUTS_ON );             // Enable driver

   delay_ms(1);                                      // Line turnarround time

   cCalcCrcHigh = 0xff;                              // Clear CRC
   cCalcCrcLow = 0xff;
                                                     // Send some NULL preamblesfopr receiving UART
   for ( c=0; c < NUM_TX_PREAMBLE; c++ ) Rs485SendChar( 0x00 );

   Rs485UpdateCrc( PKT_START );
   Rs485SendChar( PKT_START );       // Send packet start character
   Rs485UpdateCrc( cOurAddrHigh );
   Rs485SendChar( cOurAddrHigh );    // Send address high
   Rs485UpdateCrc( cOurAddrLow );
   Rs485SendChar( cOurAddrLow );     // Send address low
   Rs485UpdateCrc( cLen );
   Rs485SendChar( cLen );            // Send length
   Rs485UpdateCrc( cCmd );
   Rs485SendChar( cCmd );            // Send command


   if ( cLen != 0 )                  // If payload not empty send data
   {
     for ( c = 0; c < cLen; c++ )
     {
        d = cData[c];
        Rs485UpdateCrc( d );
     }
     for ( c = 0; c < cLen; c++ )
     {
        d = cData[c];
        Rs485SendChar( d );          // Send data
     }
   }

   Rs485SendChar(cCalcCrcHigh);
   Rs485SendChar(cCalcCrcLow);

   delay_ms(1);
   BIT_CLEAR( RS485_CONTROL, OUTPUTS_ON );           // Disable driver
   BIT_SET( PIE1, RCIE );                            // Enable Receive Interrupt
}


//****************************************************************************
// void Rs485GetPacket( char *cCommand, char cLen, char *cData )
//
// Pass packet to main application
//
//****************************************************************************
void Rs485GetPacket( char *cCom, char *cLen, char *cData )
{
char c;

  *cCom = cCommand;
  *cLen = cLenExpected;
  for ( c=0; c < cLenExpected;c++ )  cData[c] = c485Buf[c];

  cData[cLenExpected] = 0x00; // Termninate
}


/*************************************************************************
 * Example Table Driven CRC16 Routine using 4-bit message chunks
 *
 * By Ashley Roll
 * Digital Nemesis Pty Ltd
 * www.digitalnemesis.com
 * ash@digitalnemesis.com
 *
 * The following is an example of implementing a restricted size CRC16
 * table lookup. No optimisation as been done so the code is clear and
 * easy to understand.
 *
 * Test Vector: "123456789" (character string, no quotes)
 * Generated CRC: 0x29B1
 *
 * Modified for CCS compiler by J.Winpenny
 *************************************************************************/

/*
 * CRC16 Lookup tables (High and Low Byte) for 4 bits per iteration.
 */

const char CRC16_LookupHigh[16] = {
        0x00, 0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70,
        0x81, 0x91, 0xA1, 0xB1, 0xC1, 0xD1, 0xE1, 0xF1
};
const char CRC16_LookupLow[16] = {
        0x00, 0x21, 0x42, 0x63, 0x84, 0xA5, 0xC6, 0xE7,
        0x08, 0x29, 0x4A, 0x6B, 0x8C, 0xAD, 0xCE, 0xEF
};


/*
 * Before each message CRC is generated, the CRC register must be
 * initialised by calling this function
 */
void CRC16_Init( void )
{
	// Initialise the CRC to 0xFFFF for the CCITT specification
	cCalcCrcHigh = 0xFF;
	cCalcCrcLow = 0xFF;
}

/*
 * Process 4 bits of the message to update the CRC Value.
 *
 * Note that the data must be in the low nibble of val.
 */
void CRC16_Update4Bits( char val )
{
	char	t;

	// Step one, extract the Most significant 4 bits of the CRC register
	t = cCalcCrcHigh >> 4;

	// XOR in the Message Data into the extracted bits
	t = t ^ val;

	// Shift the CRC Register left 4 bits
	cCalcCrcHigh = (cCalcCrcHigh << 4) | (cCalcCrcLow >> 4);
	cCalcCrcLow = cCalcCrcLow << 4;

	// Do the table lookups and XOR the result into the CRC Tables
	cCalcCrcHigh = cCalcCrcHigh ^ CRC16_LookupHigh[t];
	cCalcCrcLow = cCalcCrcLow ^ CRC16_LookupLow[t];
}

/*
 * Process one Message Byte to update the current CRC Value
 */
void Rs485UpdateCrc( char cVal )
{
	CRC16_Update4Bits( cVal >> 4 );		// High nibble first
	CRC16_Update4Bits( cVal & 0x0F );	// Low nibble
}

//****************************************************************************
// void Rs485SendChar( char c )
//
// Driver level of RS485 protocol
// Output character on RS485 driver
// // Include line turn around time
//****************************************************************************
void Rs485SendChar( char c )
{
   TXREG = c;                            // Load data to send
   while ( !( TXSTA & TRMT_MASK ));       // Wait for TX Empty
}


//****************************************************************************
// char PostValidatePacket(void)
//
// Verify the CRC on the last packet received
//
// Check if the CRC is correct
// and return the updated state as the result
//
//****************************************************************************
char PostValidatePacket(void)
{
char c, d;

  CRC16_Init();
  Rs485UpdateCrc(PKT_START);
  Rs485UpdateCrc(cNetAddrHigh);
  Rs485UpdateCrc(cNetAddrLow);
  Rs485UpdateCrc(cLenExpected);
  Rs485UpdateCrc(cCommand);

  if ( PacketHasPayload() )  // If the packet has a payload,
  {                          // then include the data in the CRC.
     for ( c = 0; c < cLenExpected; c++ )
     {
        d = c485Buf[c];
        Rs485UpdateCrc( d );
     }
  }
                             // Check if the CRC is correct
                             // and return the updated state as the result
  if ( (cRxCrcHigh == cCalcCrcHigh)&&(cRxCrcLow == cCalcCrcLow) )
  {
     cRS485State = PKT_VALID;
  }
  else
  {
     cError = BAD_CRC;
     cRS485State = PKT_INVALID;
     ClearLine2();
     BinToHexAscii(cRxCrcHigh );
     BinToHexAscii(cRxCrcLow );
     LcdWrite(' ');
     BinToHexAscii(cCalcCrcHigh);
     BinToHexAscii(cCalcCrcHigh);
     delay_ms(255);
     delay_ms(255);
     delay_ms(255);
     delay_ms(255);
  }

  return cRS485State;
}

//****************************************************************************
// char GetPacketCmdType(void)
//
// Check packet command type
// Return TRUE if packet has a data payload.
//
//****************************************************************************
char PacketHasPayload(void)
{
  if ( cCommand == SENSOR_GET_DATA ) return TRUE;
  else return FALSE;
}

//****************************************************************************
// void BinToHexAscii( char c )
//
// Contributed by: Nick De Smith
//
//****************************************************************************
void BinToHexAscii( char c )
{
 const char hexMap[17] = "0123456789ABCDEF";

    LcdWrite( hexMap[(c >> 4) & 0xF] );
    LcdWrite( hexMap[(c & 0xF)] );
}











