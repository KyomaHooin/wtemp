
/////////////////////////////////////////////////
// Wireless thermometr server by Richard Bruna //
/////////////////////////////////////////////////

#include <SPI.h>
#include <RF24.h>
#include <PCD8544_SPI.h>
#include </root/sketchbook/wtemp/nokia/font.h>
#include <LowPower.h>

#define RF_CE 8 //RX/TX mode
#define RF_CSN 9 //SS SPI

#define PIN_SCE 10// SS -> PCD8544_SPI_FB.h
#define PIN_SDIN 11// MOSI Hardwired SPI(4Mhz)
#define PIN_SCLK 13// SCK Hardwired SPI(4Mhz)
#define USE_FRAME_BUFFER// pre-buffered LCD output
#define LCD_POWER 2

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
//PCD8544_SPI_FB lcd;

unsigned long rf_timeout;
float vref = 3.1;
float temperature = 25.56;

//SETUP

void setup() {
  //DEBUG
  Serial.begin(9600);
  //Wakeup level(1)
  rf.begin();
  //Set PA level
  rf.setPALevel(RF24_PA_LOW);
  //Open RX pipe
  rf.openReadingPipe(1, rf_rx_pipe);
}

//MAIN

void loop() {
  //RF start
  rf.powerUp();
  //Listen
  rf.startListening();
  //RF timeout counter
  rf_timeout = millis();
  //RF sync(max. = sleep time) => time to sync 0-10ms
  while(!rf.available()) {
    if (millis() - rf_timeout > 1000 ) { break; }
  };
  Serial.print("Delay: ");
  Serial.println(millis() - rf_timeout);
  //RF get payload
  if (rf.available()){
    rf.read(&payload, sizeof(payload));
    Serial.println(payload.voltage);
    Serial.println(payload.temperature);
  } else { Serial.println("E"); }
  //RF stop
  rf.stopListening();
  rf.powerDown();
 //sleep goes here..
 delay(5095);//4000 + 750 DS conversion delay(!) + 340 PowerUp delay(!)
}

long readVcc() {
  // Read 1.1V reference against AVcc 
  // set the reference to Vcc and the measurement to the internal 1.1V reference
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
//void lcd_font(byte i, byte x){
//  uint8_t buff[42];
//  memcpy_P(buff, font[i], 42);
//  lcd.writeBitmap(buff, x, 2, 14, 3);
//}

//Get 2x10 thermometr logo PROGMEM bitmap.
//void lcd_thermometr(){
//  uint8_t buff[20];
//  memcpy_P(buff, thermo, 20);
//  lcd.writeBitmap(buff, 74, 2, 10, 2);
//}

//Display server/client voltage reference.
//void lcd_display_voltage(float server_vcc, float client_vcc) {
//  lcd.print(server_vcc);
//  lcd.gotoXY(18, 0);
//  lcd.print("V");
//  lcd.gotoXY(60, 0);
//  lcd.print(client_vcc);
//  lcd.gotoXY(78, 0);
//  lcd.print("V");
//}

//Display temperature digits.
//void lcd_display_temperature(float f) {
//  if (f < 0) { lcd_font(10,0); } // "Negative One" ;).. 
//  if (abs(int(f) / 10) > 0) { lcd_font(abs(int(f) / 10), 15); }
//  lcd_font(abs(int(f) % 10), 30);
//  lcd_font(11, 43); //Precision point.
//  lcd_font(abs(int(f * 10.0) % 10), 56);
//  lcd_thermometr();
//}

//Display error.
//void lcd_display_error() {
//  lcd_font(12, 56);
//  lcd_thermometr();
//}

void sleep(){
  LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);  
}

