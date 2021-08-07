
#define _XTAL_FREQ 4000000

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <xc.h>
#include "lcd.h"

#define	LCD_RS RB0
#define LCD_EN RB2
#define LCD_DATA4 RB4
#define LCD_DATA5 RB5
#define LCD_DATA6 RB6
#define LCD_DATA7 RB7


#define	LCD_STROBE()	((LCD_EN = 1),(LCD_EN=0))

/* write a byte to the LCD in 4 bit mode */

void
lcd_komut(unsigned char c)
{
	__delay_us(40);
        unsigned char cv = (c>>4);
        LCD_DATA4 = (cv >> 0) & 0x01;
        LCD_DATA5 = (cv >> 1) & 0x01;
        LCD_DATA6 = (cv >> 2) & 0x01;
        LCD_DATA7 = (cv >> 3) & 0x01;
	LCD_STROBE();
        LCD_DATA4 = (c >> 0) & 0x01;
        LCD_DATA5 = (c >> 1) & 0x01;
        LCD_DATA6 = (c >> 2) & 0x01;
        LCD_DATA7 = (c >> 3) & 0x01;
	LCD_STROBE();
}

/*
 * 	Clear and home the LCD
 */

void
lcd_sil(void)
{
	LCD_RS = 0;
	lcd_komut(0x1);
	__delay_ms(2);
}

/* write a string of chars to the LCD */

void
lcd_yaz(const char * s)
{
	LCD_RS = 1;	// write characters
	while(*s)
		lcd_komut(*s++);
}

/* write one character to the LCD */

void
veri_yolla(char c)
{
	LCD_RS = 1;	// write characters
	lcd_komut( c );
}


/*
 * Go to the specified position
 */

void lcd_gotoxy(unsigned char x, unsigned char y)
{
	LCD_RS = 0;
    if(y==0){
        lcd_komut(BirinciSatir+x);
    }
    else{
        lcd_komut(IkinciSatir+x);
    }    
	//lcd_komut(0x80+pos);
}
	
/* initialise the LCD - put into 4 bit mode */
void
lcd_hazirla()
{
	char init_value;

	init_value = 0x3;
	LCD_RS = 0;
	LCD_EN = 0;
	
	__delay_ms(15);	// wait 15mSec after power applied,
        LCD_DATA4 = (init_value >> 0) & 0x01;
        LCD_DATA5 = (init_value >> 1) & 0x01;
        LCD_DATA6 = (init_value >> 2) & 0x01;
        LCD_DATA7 = (init_value >> 3) & 0x01;
	LCD_STROBE();
	__delay_ms(5);
	LCD_STROBE();
	__delay_us(200);
	LCD_STROBE();
	__delay_us(200);
        LCD_DATA4 = 0;
        LCD_DATA5 = 1;
        LCD_DATA6 = 0;
        LCD_DATA7 = 0;
	LCD_STROBE();

	lcd_komut(0x28); // Set interface length
	lcd_komut(0x0C); // Display On, Cursor Off, Cursor Blink
	lcd_sil();	// Clear screen
	lcd_komut(0x6); // Set entry Mode 
}
