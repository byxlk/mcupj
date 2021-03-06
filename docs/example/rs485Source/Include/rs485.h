#separate
char Rs485Process(void);
void Rs485Initialise(char cAddrHigh, char cAddrLow);
#separate
char Rs485Decode( void );
void Rs485UpdateCrc(char cVal );
void CRC16_Init( void );
void CRC16_Update4Bits( char val );
void Rs485SendPacket( char cCmd, char cLen, char *cData );
void Rs485GetPacket( char *cCom, char *cLen, char *cData );
void Rs485SendChar( char c );
char PostValidatePacket(void);
char PacketHasPayload(void);
void BinToHexAscii( char c );


#define NULL 0

// Configurables
#define RS485_CONTROL PORTC
#define OUTPUTS_ON 5
#define NUM_TX_PREAMBLE 3


// Protocol defines
#define PKT_START 0x02

// States of packet decoder state machine
#define PKT_WAIT_START      0
#define PKT_WAIT_ADDR_HIGH  1
#define PKT_WAIT_ADDR_LOW   2
#define PKT_WAIT_LEN        3
#define PKT_CMD             4
#define PKT_WAIT_DATA       5
#define PKT_WAIT_CRC_HIGH   6
#define PKT_WAIT_CRC_LOW    7
#define PKT_COMPLETE        8
#define PKT_VALID           9
#define PKT_INVALID       255

// Error codes
#define BAD_LENGTH 1
#define BAD_CRC    2


// Packet types
#define SENSOR_POLL_CMD        0xA1
#define SENSOR_PING_SYNC       0xA2
#define SENSOR_CONFIG          0xA3
#define SENSOR_GET_DATA        0xA4



#define SENSOR_ACK             0xB0
#define SENSOR_NAK             0xC0







