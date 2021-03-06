void LCDSetup(void);
void FunctionMode(void);
void DataMode(void);
void Write_8_Bit(char dh );
void LcdWrite(char dl );
void Delaylcd(void);
void Clear(void);
void SetPos(char Pos);
void Line_1(void);
void Line_2(void);
void ClearLine1(void);
void ClearLine2(void);




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

