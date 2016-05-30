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
//const uint64_t rf_tx_pipe = 0xE8E8F0F0E1LL;
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
  //Set PA level
  rf.setPALevel(RF24_PA_LOW);
  //Serial.println("Geting status...");
  rf.printDetails();
  //Open TX pipe
//  rf.openWritingPipe(rf_tx_pipe);
  rf.openWritingPipe(addresses[1]);
  rf.openReadingPipe(1,addresses[0]);
  //Open RX queue
  rf.startListening();
}

//MAIN

void loop() {
    rf.stopListening(); 
    unsigned long start_time = micros();
    if (!rf.write( &start_time, sizeof(unsigned long) )){
       Serial.println(F("failed"));
     }        
    // Try again 1s later
    delay(1000);
}
