//#define PIN_DC 8 //data/command switch -> PCD8544_SPI_FB.h
//#define PIN_RESET 9 //reset switch -> PCD8544_SPI_FB.h

#include <SPI.h>
#include <PCD8544_SPI.h>
#include </root/sketchbook/wtemp/nokia/font.h>

#define PIN_SCE 10// SS -> PCD8544_SPI_FB.h
#define PIN_SDIN 11// MOSI Hardwired SPI(4Mhz)
#define PIN_SCLK 13// SCK Hardwired SPI(4Mhz)

#define USE_FRAME_BUFFER// pre-buffered LCD output
#define LCD_POWER 2

//LCD number character horizotal position
const byte pos[6] = {0,15,30,43,56};

//create frame buffered instance
PCD8544_SPI_FB lcd;

void setup() {
  pinMode(LCD_POWER, OUTPUT);
  digitalWrite(LCD_POWER, HIGH);
  lcd.begin(0, 0xB1, 0x04, 0x13); //Invert, Contrast, TempCoef, Bias
  lcd.print("3.2V      3.1V");
  lcd_font(10, pos[0]);
  lcd_font(7 , pos[1]);
  lcd_font(8 , pos[2]);
  lcd_font(11, pos[3]);
  lcd_font(9 , pos[4]);
  lcd_thermometr();
  lcd.renderAll();
}

void loop () {
}

//Numbers bitmap 3x14 PROGMEM variable position
void lcd_font(byte i, byte x){
  uint8_t buff[42];
  memcpy_P(buff, font[i], 42);
  lcd.writeBitmap(buff, x, 2, 14, 3);
}
//Thermometr bitmap 2x10 PROGMEM fixed position
void lcd_thermometr(){
  uint8_t buff[20];
  memcpy_P(buff, thermo, 20);
  lcd.writeBitmap(buff, 74, 2, 10, 2);
}
