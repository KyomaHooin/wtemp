
/////////////////////////////////////////////////
// Wireless thermometr server by Richard Bruna //
/////////////////////////////////////////////////
//
//vccRead() by Scott "dky" 
//
//#define PIN_DC 8 //data/command switch -> PCD8544_SPI_FB.h
//#define PIN_RESET 9 //reset switch -> PCD8544_SPI_FB.h
//#define RF_MOSI 11 -> Hardwared MOSI SPI(10 Mhz)
//#define RF_MISO 12 -> Hardwired MISO SPI(10 Mhz)
//#define RF_SCK 13  -> Hardwired SCK SPI (10 Mhz)

#include <SPI.h>
#include <RF24.h>
#include <PCD8544_SPI.h>
#include </root/sketchbook/wtemp/nokia/font.h>
#include <LowPower.h>

#define RF_CE 5 //RF RX/TX mode
#define RF_CSN 6 //RF SS SPI
#define PIN_SCE 10// LCD SS -> PCD8544_SPI_FB.h
#define PIN_SDIN 11// LCD MOSI Hardwired SPI(4Mhz)
#define PIN_SCLK 13// LCD SCK Hardwired SPI(4Mhz)
#define USE_FRAME_BUFFER//LCD pre-buffered LCD output

//RF24  class instance na pinech CE/CSN.
RF24 rf(RF_CE, RF_CSN);
//RF24 data RX/TX pipe address:
const uint64_t rf_rx_pipe = 0xE8E8F0F0E1LL;
//RF payload struct
struct payloadStruct {
  float voltage;
  float temperature;
} payload;
//create frame buffered instance
PCD8544_SPI_FB lcd;

//RF sync counter
unsigned long rf_timeout;

//SETUP

void setup() {
  //DEBUG
  Serial.begin(9600);
  //RF Wakeup level(1)
  rf.begin();
  //RF PA level
  rf.setPALevel(RF24_PA_LOW);
  //RF open RX pipe
  rf.openReadingPipe(1, rf_rx_pipe);
  //LCD setup
  lcd.begin(0, 0xBD, 0x04, 0x13); //invert, contrast, tempcoef, bias
}

//MAIN

void loop() {
  //RF start
  rf.powerUp();
  //Listen
  rf.startListening();
  //RF timeout counter
  rf_timeout = millis();
  //RF sync max. sleep time
  while(!rf.available()) {
    if (millis() - rf_timeout > 1000 ) { break; }
  };
  //RF get payload.
  if (rf.available()){
    rf.read(&payload, sizeof(payload));
    rf.stopListening();
    rf.powerDown();
    //Display payload.
    lcd_display_voltage(payload.voltage, float(readVcc())/1000);
    lcd_display_temperature(payload.temperature);
    lcd.renderAll();
    Serial.println(payload.voltage);
    Serial.println(payload.temperature);
  } else {
    //Display error.
    rf.stopListening();
    rf.powerDown();
    lcd.clear();
    lcd_display_error();
    lcd.renderAll();
  }
  //sleep goes here..
  sleep();
  delay(500); //Sleep 4000 + 500 WakeUp delay
}

long readVcc() {
  // Read 1.1V reference against AVcc, set the reference to Vcc and the measurement to the internal 1.1V reference
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA,ADSC)); // measuring
  uint8_t low  = ADCL; // must read ADCL first - it then locks ADCH
  uint8_t high = ADCH; // unlocks both
  long result = (high<<8) | low;
  result = 1125300L / result; // Calculate Vcc (in mV); 1125300 = 1.1*1023*1000
  return result; // Vcc in millivolts
}

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
  lcd.gotoXY(24, 0);
  lcd.print("V");
  lcd.gotoXY(54, 0);
  lcd.print(client_vcc);
  lcd.gotoXY(78, 0);
  lcd.print("V");
}

//Display temperature digits.
void lcd_display_temperature(float f) {
  if (f < 0) { lcd_font(10,0); } // "Negative One" ;).. 
  if (abs(int(f) / 10) > 0) { lcd_font(abs(int(f) / 10), 15); }
  lcd_font(abs(int(f) % 10), 30);
  lcd_font(11, 43); //Precision point.
  lcd_font(abs(int(f * 10.0) % 10), 56);
  lcd_thermometr();
}

//Display error.
void lcd_display_error() {
  lcd.gotoXY(18, 2);
  lcd.print("NO DATA!");
  //lcd_thermometr();
}

//Sleep for 4s.
void sleep(){
  LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);  
}

