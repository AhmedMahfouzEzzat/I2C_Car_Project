/*
 * LCD.h
 *
 * Created: 12/5/2018 2:34:04 PM
 *  Author: Moaty
 */ 


void LCD_Command(unsigned char cmnd);
void LCD_Char(unsigned char data); /* Send character to LCD */
void LCD_Init (void); /* LCD Initialize function */
void LCD_String (char *str); /* Send string to LCD function */
void LCD_String_xy (char row, char pos, char *str); /* Send string to LCD with xy position */
void LCD_Clear();

