/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

************************************************************
**            TOPWAY LMB162ABC LCD Module                 **
************************************************************

  Terminal Functions
----------------------------------------------------------
|Pin No.|  Pin   |   I/O  |  Description                 |
|       |  Name  |        |                              |
----------------------------------------------------------
|   1   |  VSS   | Power  | Power Supply, Ground (0V)    |
----------------------------------------------------------
|   2   |  VDD   | Power  | Power Supply, Positive (5V)  |
----------------------------------------------------------
|   3   |   V0   | Power  | LCD Contrast Reference supply|
----------------------------------------------------------
|   4   |   RS   | Input  | Register Select              |
|       |        |        |  HIGH = tx display data      |
|       |        |        |  LOW  = tx instruction data  |
----------------------------------------------------------
|   5   |  R/W   | Input  | Read / Write Control Bus:    |
|       |        |        |  HIGH = Read mode selected   |
|       |        |        |  LOW  = Write mode selected  |
----------------------------------------------------------
|   6   |   E    | Input  | Data Enable                  |
----------------------------------------------------------
|   7   |  DB0   |        | Bi-directional tri-state Data|
------------------        - bus.                         |
|   :   |   :    |  I/O   |  In 8 bit mode, D0 ~ D7 used |
------------------        -  In 4 bit mode, D4 ~ D7 used,|
|  14   |  DB7   |        |    DB0 ~ DB3 leave open      |
----------------------------------------------------------
|  15   |  BLA   | Power  | Backlight positive supply    |
----------------------------------------------------------
|  16   |  BLK   | Power  | Backlight negative supply    |
----------------------------------------------------------




-------------------------------


 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The LCD circuit:        ARDUINO
  ---------------        -------
 * LCD  (1) VSS pin to   ground
 * LCD  (2) VDD pin to   5V
 * LCD  (3) V0  pin to   10K Potentiometer "wiper", potentiometer ends to +5V and ground
 * LCD  (4) RS  pin to   digital pin 12 (white)
 * LCD  (5) R/W pin to   ground
 * LCD  (6) E   pin to   digital pin 11 (orange)
 * LCD (11) D4  pin to   digital pin  5 (blue)
 * LCD (12) D5  pin to   digital pin  4 (green)
 * LCD (13) D6  pin to   digital pin  3 (yellow)
 * LCD (14) D7  pin to   digital pin  2 (white)
 * LCD (15) BLA pin to   5V through a 220 ohm resistor
 * LCD (16) BLK pin to   ground
 
 

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 http://www.arduino.cc/en/Tutorial/LiquidCrystalHelloWorld

*/

// include the library code:
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);




void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Hello, Kitty!");
}

void loop() {
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis()/1000);
  delay(1000);
}

