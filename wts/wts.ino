
/////////////////////////////////////////////////
// Wireless thermometr server by Richard Bruna //
/////////////////////////////////////////////////

#include <SPI.h>
#include <RF24.h>
#include <PCD8544_SPI.h>
#include <LowPower.h>
#include <stdio.h>

//VAR

#define LCD_POWER 2
#define RF_POWER 3
#define RF_CE 6 //RX/TX mode
#define RF_CSN 7 //SPI SS
//#define RF_MOSI 11 -> Hardwared MOSI SPI(10 Mhz)
//#define RF_MISO 12 -> Hardwired MISO SPI(10 Mhz)
//#define RF_SCK 13 -> Hardwired SCK SPI(10 Mhz)
//#define PIN_DC 8 data/command switch -> PCD8544_SPI_FB.h
//#define PIN_RESET 9 reset switch -> PCD8544_SPI_FB.h
#define PIN_SCE 10 //SPI SS -> PCD8544_SPI_FB.h
//#define PIN_SDIN 11 -> MOSI Hardwired SPI(4Mhz)
//#define PIN_SCLK 13 -> SCK Hardwired SPI(4Mhz)
//#define LED_POWER 13 -> SCK SPI
//5110 buffered mode
#define USE_FRAME_BUFFER
//Create RF24 class instance
RF24 rf(RF_CE, RF_CSN);
//5110 LCD class instance
PCD8544_SPI_FB lcd;
//RF24 data RX pipe address
const uint64_t rf_pipe_address = 0xF0F0F0F0D2LL;
//5110 LCD size
//const byte LCD_WIDTH = 84;
//const byte LCD_HEIGHT = 48;

// A bitmap graphic (10x2) of a thermometer...
static const byte THERMO_WIDTH = 10;
static const byte THERMO_HEIGHT = 2;
//Temperature symbol
static const byte thermometer[] = {0x00, 0x00, 0x48, 0xfe, 0x01, 0xfe, 0x00, 0x02, 0x05, 0x02,
                                   0x00, 0x00, 0x62, 0xff, 0xfe, 0xff, 0x60, 0x00, 0x00, 0x00};                                    
//DEBUG

// create a FILE structure to reference our UART output function
static FILE uartout = {0};
// create a output function
   // This works because Serial.write, although of
   // type virtual, already exists.
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
  
//RF setup
  pinMode(RF_POWER, OUTPUT);
  digitalWrite(RF_POWER, LOW);
  pinMode(RF_CSN, OUTPUT);
  digitalWrite(RF_CSN, HIGH); //SS default disable
//5110 setup
  pinMode(LCD_POWER, OUTPUT);
  digitalWrite(LCD_POWER, LOW);
  pinMode(PIN_CE, OUTPUT);
  digitalWrite(PIN_CE, HIGH); //SS default disable
//LED setup
//  pinMode(LED_POWER, OUTPUT);
//  digitalWrite(LED_POWER, LOW);
}

//LOOP

void loop() {
  //rf_up();
  //rf_status();
  lcd_up();
  //float temp = rf_receive();
  //Serial.println(temp);
  lcd_display();
  //rf_down();
  //lcd_down();
  //arduino_down();
  delay(10000);
}

//FUNCTION

void rf_up() {
  digitalWrite(RF_POWER, HIGH);
  rf.begin();
  rf.setPALevel(RF24_PA_LOW);
  rf.openReadingPipe(1,rf_pipe_address);//RX pipe
}

void rf_status() {
  if(digitalRead(PIN_SCE) == HIGH) {
    rf.printDetails();
  }
}

float rf_receive() {
  float buff;
  if(digitalRead(PIN_SCE) == HIGH) {
    rf.startListening();
    while (rf.available()) {
      rf.read(&buff, sizeof(float));
    }
    rf.stopListening();
 } 
 return buff;
}

void rf_down() {
  rf.powerDown();
  digitalWrite(RF_POWER, LOW);
}

void lcd_up() {
  digitalWrite(LCD_POWER, HIGH);
  //setup invert,tempCoef,bias, 
  //lcd.begin(false,0xB1,0x04,0xB1);
  lcd.begin();
}

void lcd_display() {
  if(digitalRead(RF_CSN) == HIGH) {
  lcd.clear();
  lcd.writeBitmap(thermometer, 0, 2, THERMO_WIDTH, THERMO_HEIGHT);
  lcd.renderAll();
  }
}

void lcd_down() {
  //lcd.stop();
  digitalWrite(LCD_POWER, LOW);
}

void arduino_down() {
  //sleep 10 min.
  for (int i = 0; i < 75; i++) {
    LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);
  }
}

