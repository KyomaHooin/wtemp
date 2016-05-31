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

//RF24  class instance na pinech CE/CSN.
RF24 rf(RF_CE, RF_CSN);
//RF24 data TX/RX pipe address:
const uint64_t rf_tx_pipe = 0xE8E8F0F0E1LL;

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

void loop() {
  //payload
  float payload = 12.34;
  //write payload
  rf.write(&payload, sizeof(float));
  // Try again 1s later
  delay(1000);
}
