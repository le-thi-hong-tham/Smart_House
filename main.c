#include <main.h>

#define LCD_RS_PIN PIN_B6    // RS noi voi B6
#define LCD_RW_PIN PIN_B5
#define LCD_ENABLE_PIN PIN_B4 // chan E noi voi B4

#define LCD_DATA4 PIN_B3    //D4 noi voi B3
#define LCD_DATA5 PIN_B2
#define LCD_DATA6 PIN_B1
#define LCD_DATA7 PIN_B0

#define tang PIN_A2
#define giam PIN_A3
#define coi PIN_D1

#define echo PIN_D7
#define trig PIN_D6

#include <lcd.c>

signed char max;
unsigned int xung;
void doccambien()
{  
  output_high(trig);
  delay_us(20);  
  output_low(trig);
  xung=0;
  while(input(echo) == 0);
  while(input(echo) == 1){xung++;delay_us(10);}
  xung = xung / 4;
}
void main()
{ 
   //read eep
   max=read_eeprom(0);
   if(max>=0 && max <=99){;}
   else
   {
    max=0;
    write_eeprom(0,0);
   }
   
   set_tris_a(0xff); //in
   set_tris_c(0x00); //out
   set_tris_d(0x80); //out  echo in
   output_high(coi);
   output_low(trig);
   lcd_init();
   lcd_gotoxy(1,1); 
   lcd_putc("K.Cach:");  
   lcd_gotoxy(1,2); lcd_putc("Max:"); 
   lcd_putc(max/10+48);
   lcd_putc(max%10+48);
   lcd_putc(" cm");
   while(TRUE)
   {
      doccambien();
      lcd_gotoxy(8,1); 
      printf(lcd_putc,"%i cm  ",xung);
      
      if(!input(tang))
      {
         delay_ms(20);
         while(!input(tang));
         max++;if(max>99)max=0;
         write_eeprom(0,max);
      }
      
      if(!input(giam))
      {
         delay_ms(20);
         while(!input(giam));
         max--;if(max<0)max=99;
         write_eeprom(0,max);
      }
      
      lcd_gotoxy(5,2);
      lcd_putc(max/10+48);
      lcd_putc(max%10+48);
      
      if(xung < max) output_low(coi);
      else output_high(coi);
      
      delay_ms(1000);  
   }
}
