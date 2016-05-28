
//!REMOVE THIS CODE AFTER SET!
//set resolution
//ds.reset();
//ds.skip();
//ds.write(0x44); //"Write Scratchpad" command
//ds.write(0x00); //unused 2nd byte
//ds.write(0x00); //unused 3rd byte
//ds.write(0x1F); //1F/ -> 9-bit conversion
//delay(?);
//write resolution to EEPROM 
//ds.reset();
//ds.skip();
//ds.write(0x48); //"copy Scratchpad to EEPROM" command
//!REMOVE CODE AFTER SET!

#include <OneWire.h>

//VAR

#define DS_POWER 4 //OneWire power
#define DS_DQ 9 //OneWire data

//OneWire class instance on DS_DQ data pin
OneWire ds(DS_DQ);
   
//SETUP

void setup() {
//DEBUG setup
  Serial.begin(9600);
//DS setup  
  pinMode(DS_POWER, OUTPUT);
  digitalWrite(DS_POWER, LOW);
}

//LOOP

void loop() {
  ds_up();
  ds_temp_request();
  Serial.println(ds_get_temperature());
  ds_down();
  delay(10000);
}

//FUNCTION

void ds_up() {
  digitalWrite(DS_POWER, HIGH);
}

void ds_temp_request() {
  ds.reset();
  ds.skip();
  ds.write(0x44);//"Convert T".
  delay(750); //12-Bit resolution delay.
}

float ds_get_temperature() {
  byte ds_data[2];
  ds.reset();
  ds.skip();
  ds.write(0xBE); //"Read Scratchpad".
  ds.read_bytes(ds_data,2); //LSB & MSB
  short raw = ds_data[1] << 8 | ds_data[0]; //2-Byte to 16 bit int
  return (float)raw / 16.0; //-> int to double precission float
}

void ds_down() {
  digitalWrite(DS_POWER, LOW);
}

