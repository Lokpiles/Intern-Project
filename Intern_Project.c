/*
 * File:   Intern_Project.c
 * Author: Emircan Furkan Bayendur
 *
 * Created on 28 Temmuz 2021 Çarþamba, 10:29
 */



#include <xc.h>
#include <eeprom_routines.h>
#include "lcd.h"

// PIC16F819 Configuration Bit Settings

// 'C' source line config statements

// CONFIG
#pragma config FOSC = INTOSCIO 
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // RA5/MCLR/VPP Pin Function Select bit (RA5/MCLR/VPP pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF         // Low-Voltage Programming Enable bit (RB3/PGM pin has PGM function, Low-Voltage Programming enabled)
#pragma config CPD = ON         // Data EE Memory Code Protection bit (Data EE memory locations code-protected)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CCPMX = RB2      // CCP1 Pin Selection bit (CCP1 function on RB2)
#pragma config CP = ON          // Flash Program Memory Code Protection bit (All memory locations code-protected)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
#include <pic16f819.h>

__eeprom unsigned char my_eeprom_values[] =  
        {   
            250,0,
            10
                    
        };



#define _XTAL_FREQ 4000000
  int sayac;
  unsigned char hysteresis;
  

  
#define BUTON1      RA1
#define BUTONARTI   RA2
#define BUTONEKSI   RA3
#define ROLE RA6

#define BASILI  0

  enum setstatus{
      SETNONE,
      SETTARGETTEMP,
      SETRELAYCONTROL,
      SETHIST
  };
  
  
  const float oran=5/1024.0;
int c;

    unsigned int ham = 0,
                Dizi[20],
                DiziInd=0,
                Sicaklik=0,
                SicaklikSet=250,
                Kaydet=0,
                AyarMenu=0;
            

    unsigned char alt,ust;
    int onoffAyar=0;
    
   
/*****************************************************/
void lcdintbas(unsigned int veri,unsigned char hane, unsigned char nokta)
{
    switch(hane)
    {
        case 5: veri_yolla(veri/10000%10+48);
        case 4: veri_yolla(veri/1000%10+48); if(nokta == 3){veri_yolla('.');}
        case 3: veri_yolla(veri/100%10+48); if(nokta == 2){veri_yolla('.');}
        case 2: veri_yolla(veri/10%10+48); if(nokta == 1){veri_yolla('.');}
        case 1: veri_yolla(veri%10+48);
    }
}
/*****************************************************/
void lcdbytebas(unsigned char veri,unsigned char hane, unsigned char nokta)
{
    switch(hane)
    {
        case 3: veri_yolla(veri/100%10+48); if(nokta == 2){veri_yolla('.');}
        case 2: veri_yolla(veri/10%10+48); if(nokta == 1){veri_yolla('.');}
        case 1: veri_yolla(veri%10+48);
    }
}
/*****************************************************/
void AdcHazirla(){
    ADCON1 = 0b10001110;    // adc0 haric hepsi dijital
    ADCON0 = 0b11000001;   
}

/*****************************************************/
unsigned int AdcOku()
{
    GO_DONE=1;
    while(GO_DONE){};
    return (ADRESH*256+ADRESL);
}

/*****************************************************/



/***********************************************************/
void PrintScreen()
{
    lcd_gotoxy(0,0);
    //lcdintbas(ham,5,0);
    lcd_yaz("Temp:");
    lcdintbas(Sicaklik,3,1);
    lcd_gotoxy(10,0);
    //lcd_yaz(223);
    lcd_yaz("C");
   // sembolyaz();
    lcd_gotoxy(0,1);
    lcd_yaz("Target:");
    lcdintbas(SicaklikSet,3,1);
    

    
    lcd_gotoxy(12,1);

//    if(Sicaklik >= SicaklikSet){
//         lcd_yaz("-");
//     }
//    else if(SicaklikSet <= (Sicaklik-hysteresis)){
//         lcd_yaz("+");
//     }

    if(onoffAyar==0){
         lcd_yaz("OFF");
     }
     else{
         lcd_yaz(" ON");
     }
}

/*****************************************************/
void SicaklikOku()
{
  Dizi[DiziInd] = AdcOku();
  if(++DiziInd > 19)
  {
      DiziInd = 0;
  }    
    ham = 0;
  unsigned char i;
    for (i = 0; i < 20; i++) {
        ham += Dizi[i];
    }

  Sicaklik = (unsigned long)ham * 1000 / 4100;

}

/*****************************************************/
/*100ms*/
#define KAYITSURESI 30  /*3 saniye sonra kaydet*/

void ButonKontrol(){
    
    if(BUTONARTI == BASILI)
    {
       __delay_ms(50);
        if(++SicaklikSet > 999)
        {    
            
            SicaklikSet = 999;
        }
         __delay_ms(60);
        Kaydet = KAYITSURESI;
    }    
    
    if (BUTONEKSI == BASILI)
    {
        __delay_ms(50);
        Kaydet = KAYITSURESI;
        if(--SicaklikSet < 10)
        {
            SicaklikSet = 10;
        }
        __delay_ms(60);
    }
    
    if (Kaydet)
    {
        if (--Kaydet == 0) 
        {
            //eeprom_write(0,SicaklikSet);
            alt = (unsigned char)SicaklikSet;
            ust = (unsigned char)(SicaklikSet >> 8); 
            
            eeprom_write(0,alt);
            eeprom_write(1,ust);
        }
    }
    
}

/*****************************************************/
/*100ms*/
#define KAYITSURESIHYS 30  /*3 saniye sonra kaydet*/

void ButonKontrolHys(){
    
    if(BUTONARTI == BASILI)
    {
       __delay_ms(50);
        if(++hysteresis > 99)
        {    
            
            hysteresis = 99;
        }
         __delay_ms(60);
        Kaydet = KAYITSURESIHYS;
    }    
    
    if (BUTONEKSI == BASILI)
    {
        __delay_ms(50);
        Kaydet = KAYITSURESIHYS;
        if(--hysteresis < 1)
        {
            hysteresis = 1;
        }
        __delay_ms(60);
    }
    
    if (Kaydet)
    {
        if (--Kaydet == 0) 
        {
            
            
            eeprom_write(2,hysteresis);
        }
    }
    
}


/*****************************************************/
void RelayControl(){

    static unsigned char Acilis=30;
    if (!Acilis)
    {
        if(onoffAyar)
        {
            if (Sicaklik >= SicaklikSet)
            {
                ROLE = 1;
            }
            else if(Sicaklik <= (SicaklikSet - hysteresis))
            {
                ROLE = 0;
            }
        }
        else
        {
            ROLE = 0;
        }
    }
    else
    {
        Acilis--;
    }
}

/***********************************************************/
void ayarKontrol(){
    
    
    
    if(BUTON1==BASILI){  
        while(BUTON1==BASILI){}
        AyarMenu++;
//        if(AyarMenu==3){
//            AyarMenu=1;
//        } 
    }
    
    switch(AyarMenu){
        case SETTARGETTEMP:
            lcd_sil();
            lcd_gotoxy(0,0);
            lcd_yaz("Target Temp: ");
            while(AyarMenu==SETTARGETTEMP){
                if(BUTON1==BASILI){
                    while(BUTON1==BASILI){}
                    AyarMenu=SETRELAYCONTROL;
                }
                else{
                    lcd_gotoxy(0,1);
                    lcd_yaz("SET: ");
                    lcdintbas(SicaklikSet,3,1);
    
                    ButonKontrol();
                }
            }
            break;
        case SETRELAYCONTROL:
            lcd_sil();
            lcd_gotoxy(0,0);
            lcd_yaz("Alarm Status:");
            lcd_gotoxy(0,1);
            if(onoffAyar==1){
                lcd_yaz("ON ");
            }
            else{
                lcd_yaz("OFF");
            }
            while(AyarMenu==SETRELAYCONTROL){
                if(BUTON1==BASILI){
                    while(BUTON1==BASILI){}
                    AyarMenu=SETHIST;
                }
                else{
                    lcd_gotoxy(0,12);
                    
                    if(BUTONARTI==BASILI){
                        onoffAyar=1;
                        lcd_yaz(" ON");
                    }
                    if(BUTONEKSI==BASILI){
                        onoffAyar=0;
                        lcd_yaz("OFF");
                    }
                    
                }
            }
            break;
            
        case SETHIST:
            lcd_sil();
            lcd_gotoxy(0,0);
            lcd_yaz("Set Hysteresis:");
            lcd_gotoxy(0,1);
          
            
            while(AyarMenu==SETHIST){
                  lcdbytebas(hysteresis,2,1);
                if(BUTON1==BASILI){
                    while(BUTON1==BASILI){}
                    lcd_sil();
                    AyarMenu=SETNONE;
                }
                else{
                    lcd_gotoxy(0,1);
                    ButonKontrolHys();
                    
                    
                    
                }
                  
            }
            
            
      
            
            
            
            
    }
    
     
}
/*****************************************************/

void main(void) {
    
    sayac=0;
    
    TRISA=0b10111111;
    TRISB=0;
    PORTB = 0;
    OSCCON = 0b01100000;    //4mhz
    ROLE = 0;
    AdcHazirla();
    __delay_ms(300);
    lcd_hazirla();
   
   
    unsigned char butonsay = 0;
    unsigned char yuzms=0;
    
    lcd_gotoxy(0,0);
    lcd_yaz("Emircan Furkan");
    lcd_gotoxy(0,1);
    lcd_yaz("Bayendur");
    __delay_ms(1000);
    lcd_sil();
    
    //eeprom_write(0,SicaklikSet);
    alt = eeprom_read(0);
    ust = eeprom_read(1);
    
    SicaklikSet = ust*256+alt;
    hysteresis=eeprom_read(2);
    
//    unsigned char i;
//    for (i = 0; i < 20; i++) {
//        Dizi[i] = SicaklikSet;
//    }
    
    while(1)
    {
        if(++yuzms > 9)
        {
            yuzms = 0;
            SicaklikOku();
           
           PrintScreen();
            ayarKontrol();
            RelayControl(); 
            
           
        }
        __delay_ms(10);
    }
}
    
//    c=(int)((((ADRESH*256+ADRESL)*oran*100)-50)*10);

//        if (++butonsay > 9) 
//        {
//            butonsay = 0;
            
            //if(BUTON1 == BASILI)
            //{
               // while(BUTON1 == BASILI){__delay_ms(10);}
               // sayac++;

                //eeprom_write(0,sayac);
               // __delay_ms(10);

                //PORTB=EEPROM_READ(0);        
                
            //}
     
  //  }

//}

