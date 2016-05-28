
/////////////////////////////////////////////////
// Wireless thermometr client by Richard Bruna //
/////////////////////////////////////////////////
//
//NRF24L01+
//#define p_variant true
//
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
//

#include <SPI.h>
#include <RF24.h>
#include <OneWire.h>
#include <LowPower.h>
//#include <stdio.h>

//VAR

#define RF_POWER 3
#define DS_POWER 4
#define DS_DQ 5 //OneWire data
#define RF_CE 8 //RX/TX mode
#define RF_CSN 9 //SS SPI
//#define RF_MOSI 11 -> Hardwared MOSI SPI(10 Mhz)
//#define RF_MISO 12 -> Hardwired MISO SPI(10 Mhz)
//#define RF_SCK 13 -> Hardwired SCK SPI(10 Mhz)
//#define LED_POWER 13 -> SCK SPI
//OneWire class instance on DS_DQ data pin
OneWire ds(DS_DQ);
//RF24 class instance on CE/CSN pins
RF24 rf(RF_CE, RF_CSN);
//RF24 data TX pipe address:
const uint64_t rf_pipe_address = 0xF0F0F0F0E1LL;

//RF debug
static FILE uartout = {0};

static int uart_putchar (char c, FILE *stream)
   {
       Serial.write(c);
       return 0;
   }
   
//SETUP

void setup() {
//DEBUG setup
  Serial.begin(9600);
//RF debug
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;
//DS setup  
  pinMode(DS_POWER, OUTPUT);
  digitalWrite(DS_POWER, LOW);
//RF setup
  pinMode(RF_POWER, OUTPUT);
  digitalWrite(RF_POWER, LOW);
  pinMode(RF_CSN, OUTPUT);
  digitalWrite(RF_CSN, HIGH); //SS disable
//LED setup
//  pinMode(LED_POWER, OUTPUT);
//  digitalWrite(LED_POWER, LOW);
}

//LOOP

void loop() {
  Serial.println("---------------");
  ds_up();
  ds_temp_request();
  rf_up();
  rf.printDetails();
  float rf_data = ds_get_temperature();
  Serial.println(rf_data);
  rf_send(rf_data);
  ds_down();
  rf_down();
  delay(20000);
//  arduino_down();
}

//FUNCTION

void ds_up() {
  Serial.println("DS powering up..");
  digitalWrite(DS_POWER, HIGH);
}

void ds_temp_request() {
  Serial.println("DS asking for conversion..");
  ds.reset();
  ds.skip();
  ds.write(0x44);//"Convert T" command
  delay(750);//conversion delay
}

float ds_get_temperature() {
  Serial.println("DS reading scratchpad..");
  byte ds_data[12];
  ds.reset();
  ds.skip();
  ds.write(0xBE); //"Read Scratchpad" command
  //read 8 bytes/bits (?) of data
  for (byte i = 0; i < 9; i++) {
    ds_data[i] = ds.read();
  }
  //calculate temperature
  int16_t raw = (ds_data[1] << 8) | ds_data[0];
  //raw = raw & ~7;
  return (float)raw / 16.0;
}

void ds_down() {
  Serial.println("DS powering down..");
  digitalWrite(DS_POWER, LOW);
}

void rf_up() {
  Serial.println("RF powering up..");
  digitalWrite(RF_POWER, HIGH);
  rf.begin();
  rf.setPALevel(RF24_PA_LOW);
  rf.openWritingPipe(rf_pipe_address);// TX pipe
}

void rf_send(float buff) {
  Serial.println("RF sending data..");
  rf.write(&buff, sizeof(float));
}

void rf_down() {
  Serial.println("RF powering down..");
  rf.powerDown();
  digitalWrite(RF_POWER, LOW);
}

void arduino_down() {
  //sleep 10 min.
  for (int i = 0; i < 75; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}
