
void Setup(void);
void ConfigureComms(void);


// EEPROM addresses

#define NET_ADDRESS_HIGH 0
#define NET_ADDRESS_LOW  1

#ifndef MASTER
// PORT Configuration
#define PortAConfig 0x3F
#define PortBConfig 0x00
#define PortCConfig 0x00
#define PortDConfig 0x00
#define PortEConfig 0x00
#else
// PORT Configuration
#define PortAConfig 0x00
#define PortBConfig 0x00
#define PortCConfig 0x0f
#define PortDConfig 0x00
#define PortEConfig 0x00
#endif


//*********************************************************************


#define TMR0_RELOAD 255         // TMR0 reload value 
//#define TMR0_RELOAD 248       // TMR0 reload value 


#BYTE PORTA = 0x05
#BYTE PORTB = 0x06
#BYTE PORTC = 0x07
#BYTE PORTD = 0x08
#BYTE PORTE = 0x09
#BYTE RCREG = 0x1a
#BYTE TXREG = 0x19
#BYTE STATUS = 0x03
#BYTE SSPBUF = 0x13
#BYTE SSPCON = 0x14
#BYTE SSPCON2 = 0x91
#BYTE PR2 = 0x92
#BYTE SSPADD = 0x93
#BYTE SSPSTAT = 0x94
#BYTE SPBRG = 0x99
#BYTE ADCON1 = 0x9f
#BYTE OPTIONREG = 0x81

#BYTE PIE1 = 0x8c
#BYTE PIR1 = 0x0c
#BYTE INTCON = 0x0b
#BYTE TMR0 = 0x101
#BYTE RCSTA = 0x18
#BYTE TXSTA = 0x98

#BYTE TRISA = 0x85
#BYTE TRISB = 0x86
#BYTE TRISC = 0x87
#BYTE TRISD = 0x88
#BYTE TRISE = 0x89

// INTCON BITS
#define GIE 7
#define PEIE 6
#define T0IE 5
#define INTE 4
#define RBIE 3
#define T0IF 2
#define INTF 1
#define RBIF 0

// USART Register bits
#define CSCR 7
#define TX9  6
#define TXEN 5
#define SYNC 4
#define BRGH 2
#define TRMT 1
#define TX9D 0
#define SPEN 7
#define RX9  6
#define SREN 5
#define CREN 4
#define ADDEN 3
#define FERR 2
#define OERR 1
#define RX9D 0

#define TRMT_MASK 2

// Bits of STATUS register
#define IRP   7
#define RP1   6
#define RP0   5
#define T0    4
#define PD    3
#define ZERO  2
#define DC    1
#define CARRY 0

// Bits of PIE1
#define PSPIE 7
#define ADIE 6
#define RCIE 5
#define TXIE 4
#define SSPIE 3
#define CCP1IE 2
#define TMR2IE 1
#define TMR1IE 0

// Bits of SSPSTAT
#define SMP 7
#define CKE 6
#define D_A 5
//#define P   4 // Cause compiler error because of identifier s
//#define S   3
#define R_W 2
#define R_W_MASK 0x04
#define UA  1
#define BF  0

// Bits of PIR1
#define PSPIF  7
#define ADIF   6
#define RCIF   5
#define TXIF   4
#define SSPIF  3
#define SSPIF_MASK 0x08
#define CCP1IF 2
#define TMR2IF 1
#define TMR1IF 0

// Bits of SSPCON2

#define GCEN    7
#define ACKSTAT 6
#define ACKDT   5
#define ACKEN   4
#define RCEN    3
#define PEN     2
#define RSEN    1
#define SEN     0
