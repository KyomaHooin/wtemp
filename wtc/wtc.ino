
/////////////////////////////////////////////////
// Wireless thermometr client by Richard Bruna //
/////////////////////////////////////////////////

//#define RF_MOSI 11 -> Hardwared MOSI SPI(10 Mhz)
//#define RF_MISO 12 -> Hardwired MISO SPI(10 Mhz)
//#define RF_SCK 13  -> Hardwired SCK SPI (10 Mhz)

#include <SPI.h>
#include <RF24.h>
#include <OneWire.h>
#include<LowPower.h>

#define RF_CE 8 //RX/TX mode
#define RF_CSN 9 //SS SPI
#define DS_POWER 4 //OneWire power
#define DS_DQ 6 //OneWire data

//RF class instance
RF24 rf(RF_CE, RF_CSN);
//RF data TX/RX pipe address:
const uint64_t rf_tx_pipe = 0xE8E8F0F0E1LL;
//RF payload struct
struct payloadStruct {
  float voltage;
  float temperature;
};
//OneWire class instance on DS_DQ data pin
OneWire ds(DS_DQ);

//SETUP

void setup() {
  //RF Wakeup LEVEL 1
  rf.begin();
  //RF Set PA level
  rf.setPALevel(RF24_PA_LOW);
  //RF Open TX pipe
  rf.openWritingPipe(rf_tx_pipe);
  //DS setup  
  pinMode(DS_POWER, OUTPUT);
  digitalWrite(DS_POWER, LOW);
}

//MAIN

void loop() {
  //DS up
  ds_up();
  //DS request conversion
  ds_temp_request();
  //Payload
  payloadStruct payload = {float(readVcc())/1000, ds_get_temperature()};
  //DS down
  ds_down();
  //RF start
  rf.powerUp();
  //RF send payload 
  rf.write(&payload, sizeof(payload));
  //RF stop
  rf.powerDown();
  //sleep goes here..
  sleep();
}

//FUNC

//Power up DS.
void ds_up() {
  digitalWrite(DS_POWER, HIGH);
}

//Request temperature.
void ds_temp_request() {
  ds.reset();
  ds.skip();
  ds.write(0x44);//"Convert T".
  delay(750); //12-Bit resolution delay.
}

//Calculate temperature.
float ds_get_temperature() {
  byte ds_data[2];
  ds.reset();
  ds.skip();
  ds.write(0xBE); //"Read Scratchpad".
  ds.read_bytes(ds_data,2); //LSB & MSB
  short raw = ds_data[1] << 8 | ds_data[0]; //2-Byte to 16 bit int
  return (float)raw / 16.0; //-> int to double precission float
}

//Power down DS.
void ds_down() {
  digitalWrite(DS_POWER, LOW);
}

//Calculate VCC.
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

//Sleep for 4s.
void sleep(){
    LowPower.powerDown(SLEEP_4S, ADC_OFF, BOD_OFF);  
}

