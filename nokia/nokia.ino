//#define PIN_DC 8 //data/command switch -> PCD8544_SPI_FB.h
//#define PIN_RESET 9 //reset switch -> PCD8544_SPI_FB.h

//INCLUDE

#include <SPI.h>
#include <PCD8544_SPI.h>
#include </root/sketchbook/wtemp/nokia/font.h>

#define PIN_SCE 10// SS -> PCD8544_SPI_FB.h
#define PIN_SDIN 11// MOSI Hardwired SPI(4Mhz)
#define PIN_SCLK 13// SCK Hardwired SPI(4Mhz)

#define USE_FRAME_BUFFER// pre-buffered LCD output
#define LCD_POWER 2

//VAR & CONST

//create frame buffered instance
PCD8544_SPI_FB lcd;

float vref = 3.1;
float temperature = 25.56;

//MAIN

void setup() {
  pinMode(LCD_POWER, OUTPUT);
  digitalWrite(LCD_POWER, HIGH);
  lcd.begin(0, 0xB1, 0x04, 0x13); //invert, contrast, tempcoef, bias
  lcd_display_voltage(vref, vref);
  lcd_display_temperature(temperature);
  lcd.renderAll();
}

void loop () {
}

//FUNC

//Get single 3x14 digit PROGMEM bitmap.
void lcd_font(byte i, byte x){
  uint8_t buff[42];
  memcpy_P(buff, font[i], 42);
  lcd.writeBitmap(buff, x, 2, 14, 3);
}
//Get 2x10 thermometr logo PROGMEM bitmap.
void lcd_thermometr(){
  uint8_t buff[20];
  memcpy_P(buff, thermo, 20);
  lcd.writeBitmap(buff, 74, 2, 10, 2);
}

//Display server/client voltage reference.
void lcd_display_voltage(float server_vcc, float client_vcc) {
  lcd.print(server_vcc);
  lcd.gotoXY(18, 0);
  lcd.print("V");
  lcd.gotoXY(60, 0);
  lcd.print(client_vcc);
  lcd.gotoXY(78, 0);
  lcd.print("V");
}

//Display temperature digits.
void lcd_display_temperature(float f) {
  if (f < 0) {lcd_font(10,0); } // "Negative One" ;).. 
  if (abs(int(f) / 10) > 0) {lcd_font(abs(int(f) / 10), 15); }
  lcd_font(abs(int(f) % 10), 30);
  lcd_font(11, 43); //Precision point.
  lcd_font(abs(int(f * 10.0) % 10), 56);
  lcd_thermometr();
}

