//RF server
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
//RF24 data RX/TX pipe address:
const uint64_t rf_rx_pipe = 0xE8E8F0F0E1LL;

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
  //Open RX queue
  rf.startListening();
}

//MAIN

void loop() {
  //payload
    float payload;
    //read payload
    if( rf.available()){
      while (rf.available()) {
        rf.read(&payload, sizeof(float));
      }
      Serial.println(payload);
   }
   //Queue delay
   delay(1000);
}
