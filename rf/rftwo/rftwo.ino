

#include <SPI.h>
#include <RF24.h>
#include <stdio.h>

//#define RF_POWER 3
#define RF_CE 8 //RX/TX mode
#define RF_CSN 9 //SS SPI

//#define RF_MOSI 11 -> Hardwared MOSI SPI(10 Mhz)
//#define RF_MISO 12 -> Hardwired MISO SPI(10 Mhz)
//#define RF_SCK 13  -> Hardwired SCK SPI (10 Mhz)

uint8_t payload;

//RF24  class instance na pinech CE/CSN.
RF24 rf(RF_CE, RF_CSN);

//RF24 data TX pipe address:
const uint64_t pipe = 0xE8E8F0F0E1LL;

//RF debug
static FILE uartout = {0};

static int uart_putchar (char c, FILE *stream) {
  Serial.write(c);
  return 0;
}

void setup() {
//DEBUG setup
//  Serial.begin(9600);
  Serial.begin(115200);
// fill in the UART file descriptor with pointer to writer.
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
// The uart is the standard output device STDOUT.
  stdout = &uartout ;

//RF SETUP

  //Wakeup LEVEL 1
  rf.begin();
  //Set power level
  rf.setPALevel(RF24_PA_LOW);
  
  //Open TX pipe
  rf.openWritingPipe(pipe);
 
}

void loop() {
//  Serial.println("Geting status...");
//  rf.printDetails();
//  delay(10000);
    // if there is data ready
    if ( rf.available() )
    {
      // Dump the payloads until we've gotten everything
      while (rf.available())
      {
          rf.read(payload, sizeof(payload));
      }
    }
}
