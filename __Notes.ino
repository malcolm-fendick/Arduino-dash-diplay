/*
  BlueTooth
  tx pin = 16
  rx pin = 17

  Speedo
  speedoPin = 12

  Speedo transducer used is the Hunter 65H from Sailes Marketing.
  It has dual output with 8 pulses per rev.
  165/65 R15 tyres on 4.5 rims have a circumference of 1.876 m,
  giving  4.264392324 pulses per meter.

  Red cable is +5v, Blue cable is GND, Signal 1 is Yellow, Signal 2 is Green

  The speedo transducer signal is connected to pin D26 via 5v-3.3v shifter
  and triggers on RISING edge.

  FTOLED
  pin_reset = 2
  pin_dc = 3
  pin_sd_cs = 4
  pin_cs = 5

  GPS
  ledPin = 13
  txPin = 18
  rxPin = 19

  GPS set @ 1 Hz sampling, $GPRMC only and 115200 serial speed in FLASH memory.
  Venus GPS connected to Due Serial1, TX1 pin 18, RX1 pin 19.

  $GPRMC
  Recommended minimum specific GPS/Transit data

  eg. $GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70
              1    2    3    4    5     6    7    8      9     10  11 12

      1   220516     Time Stamp
      2   A          validity - A-ok, V-invalid
      3   5133.82    current Latitude
      4   N          North/South
      5   00042.24   current Longitude
      6   W          East/West
      7   173.8      Speed in knots
      8   231.8      True course
      9   130694     Date Stamp
      10  004.2      Variation
      11  W          East/West
      12  *70        checksum

  Based on example code for connecting a Parallax GPS module to the Arduino by
  Igor Gonzalez Martin. 05-04-2007 - igor.gonzalez.martin@gmail.com
  English translation by djmatic 19-05-2007

  Temperature
  Data wire is plugged into port 2 on the Arduino
  ONE_WIRE_BUS pin = 24

  Signal (yellow) wire from sensor connected to pin 24 which is pulled high by
  4k7 resistor.

  SD Card
  SD card attached to Arduino Due SPI bus as follows:

  MOSI - SPI pin 4
  MISO - SPI pin 1
  SCK  - SPI pin 3
  CS   - DUE pin 4

  SD card needs 5v. Using a 3.3 Vcc will not work.







*/
