//RF client
//#define RF_MOSI 11 -> Hardwared MOSI SPI(10 Mhz)
//#define RF_MISO 12 -> Hardwired MISO SPI(10 Mhz)
//#define RF_SCK 13  -> Hardwired SCK SPI (10 Mhz)
//#define RF_POWER 3

#include <SPI.h>
#include <RF24.h>
#include <stdio.h>

#define RF_CE 8 //RX/TX mode
#define RF_CSN 9 //SS SPI

//RF class instance
RF24 rf(RF_CE, RF_CSN);
//RF data TX/RX pipe address:
const uint64_t rf_tx_pipe = 0xE8E8F0F0E1LL;
//RF payload struct
struct payloadStruct {
  float voltage;
  float temperature;
};

//SETUP

void setup() {
  //DEBUG
  Serial.begin(9600);
  //Wakeup LEVEL 1
  rf.begin();
  //Set PA level
  rf.setPALevel(RF24_PA_LOW);
  //Open TX pipe
  rf.openWritingPipe(rf_tx_pipe);
}

//MAIN

payloadStruct payload = {3.1, 25.15};

void loop() {
  //RF start
  rf.powerUp();
  //RF send payload 
  rf.write(&payload, sizeof(payload));
  //RF stop
  rf.powerDown();
  //sleep goes here..
  delay(5000);
}

