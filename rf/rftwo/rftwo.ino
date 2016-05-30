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
//const uint64_t rf_rx_pipe = 0xE8E8F0F0E1LL;
byte addresses[][6] = {"1Node","2Node"};
//RF debug
static FILE uartout = {0};
static int uart_putchar (char c, FILE *stream) {
  Serial.write(c);
  return 0;
}

//SETUP

void setup() {
  //DEBUG setup
  Serial.begin(9600);
  // fill in the UART file descriptor with pointer to writer.
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  // The uart is the standard output device STDOUT.
  stdout = &uartout ;
  //RF SETUP
  //Wakeup LEVEL 1
  rf.begin();
  //Set power level
  rf.setPALevel(RF24_PA_LOW);
  //  Serial.println("Geting status...");
  rf.printDetails();
  //Open RX pipe
//  rf.openReadingPipe(1, rf_rx_pipe);
  rf.openWritingPipe(addresses[0]);
  rf.openReadingPipe(1,addresses[1]);
  //Open RX queue
  rf.startListening();
}

//MAIN

void loop() {
    unsigned long got_time;
    if( rf.available()){
      while (rf.available()) {
        rf.read( &got_time, sizeof(unsigned long) );
      }
      Serial.print(F("Got response: "));
      Serial.println(got_time);  
   }
}
