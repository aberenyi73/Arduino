#include "MorseCoder.h"

/**
 * Type morse code in the Serial port and blink it on the led.
 */

// The GPIO pin for the LED
const uint8_t OUT_LED = 26;    // configure LED output to digital pin 
const uint8_t IN_GPIO = 34;    // configure LED input to digital pin 


unsigned long interval = 300; // ms
unsigned long time_end; // ms

char in_char;

void _signalDit() {
  digitalWrite(OUT_LED, HIGH);
  time_end = millis() + interval;
  while(millis() < time_end) {}
  digitalWrite(OUT_LED, LOW);
}

MorseCoder mc(IN_GPIO, IN_GPIO, interval);


void setup() {
  // put your setup code here, to run once:
  pinMode(OUT_LED, OUTPUT);
  Serial.begin(9600);
  delay(200);
  Serial.println("Awaiting text input.");
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available()) {
    // wait a bit for the entire message to arrive
    String text_in = "";
    delay(100);
    Serial.println("Reading input");

    while (Serial.available() > 0) {
      // display each character to the LCD
      in_char = Serial.read();
      text_in += in_char;
      Serial.print(in_char);
      _signalDit();
      delay(10);
    }
    Serial.println();

    if(!mc.isCleanText(text_in)) {
      Serial.println("WARNING: The message contains unsendable characters.");
      text_in = mc.cleanText(text_in);
      Serial.println("   ***  Cleaned message text ***");    
      Serial.println(text_in);
    }
    
    Serial.println("Awaiting text input..");
  }

}
