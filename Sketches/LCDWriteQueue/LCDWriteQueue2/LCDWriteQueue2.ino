/**
 * Type in a character in the Serial Monitor, and it will print 
 * it to the LCD monitor, typewriter style.
 */ 

// include the library code:
#include <LiquidCrystal.h>
#include "LCDQueue16x2.h"

char in_char = 32;  // space

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LCDQueue16x2 lq;


void setup() {
  Serial.begin(9600);
  while(!Serial.available()) {

  }
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
}

void loop() {
  // when characters arrive over the serial port...
  //Serial.println(millis());

  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    delay(100);
    lcd.clear();

    while (Serial.available() > 0) {
      // display each character to the LCD
      in_char = Serial.read();
      Serial.println(in_char);
      lq.push(in_char);
      lq.write(&lcd);

      //lcd.write(in_char);
      

    }
    
  }
}
