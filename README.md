![Wireless Thermometr](https://github.com/KyomaHooin/wtemp/raw/master/wt_screen.jpg "screenshot")

DESCRIPTION

Low power wireless thermometr with LCD display.

DEBUG

<pre>
//Print STDOUT to UART..

static FILE uartout = {0};
static int uart_putchar (char c, FILE *stream) {
  Serial.write(c);
  return 0;
}

void setup() {
  Serial.begin(9600);
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;
  rf.printDetails();
}
</pre>

FILE

<pre>
wt_client.ino - Wireless thermometr client.
wt_server.ino - Wireless thermometr server.
       font.h - Font file.          

PCD8544_SPI/ - PCD8544 driver library by Arthur Liberman.
       RF24/ - nRF24L01+ library by J. Coliz.
    OneWire/ - DS18B20 OneWire library by Jim Studt, Paul Stoffregen & Co.
   LowPower/ - Low power library by Lim Phang Moh.
</pre>

CONTACT

Author: richard_bruna@nm.cz<br>
Source: https://github.com/KyomaHooin/wtemp

