#include <xc.h>
//#pragma config FOSC = XT
//#pragma config WDTE = OFF
//#pragma config PWRTE = OFF
//#pragma config CP = OFF
//#pragma config BOREN = ON
//#pragma config LVP = ON
//#pragma config CPD = OFF 
//#pragma config WRT = ON
#define _XTAL_FREQ 4000000

#define rs          RB0
#define rw          RB1
#define e           RB2

#define port_lcd    PORTB


#define Sil 1
#define BasaDon 2
#define SolaYaz 4
#define SagaYaz 6
#define ImlecGizle 12
#define ImlecAltta 14
#define ImlecYanSon 15
#define ImlecGeri 16
#define KaydirSaga 24
#define KaydirSola 28
#define EkraniKapat 8
#define BirinciSatir 0x80 // 128
#define IkinciSatir  0xC0 // 192
#define KarakUretAdres 64
#define CiftSatir 40
#define TekSatir 32
/* write a byte to the LCD in 4 bit mode */

extern void lcd_komut(unsigned char);

/* Clear and home the LCD */

extern void lcd_sil(void);

/* write a string of characters to the LCD */

extern void lcd_yaz(const char * s);

/* Go to the specified position */

void lcd_gotoxy(unsigned char x, unsigned char y);
	
/* intialize the LCD - call before anything else */

extern void lcd_hazirla(void);

extern void veri_yolla(char);

/*	Set the cursor position */

#define	lcd_cursor(x)	lcd_komut(((x)&0x7F)|0x80)
