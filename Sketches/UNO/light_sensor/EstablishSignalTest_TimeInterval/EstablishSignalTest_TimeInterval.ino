#include <LiquidCrystal.h>
#include <LCDQueue16x2.h>
#include <cppQueue.h>
#include "SensorBuffer.h"
#include <MorseCoder.h>
#include "CodeBuffer.h"

#define DEBUG_TEST1 false // most detailed, signal included
#define DEBUG_TEST2 true // only code



/******************** LCD SETUP CODE ********************************/
  // Initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
LCDQueue16x2 lq;
/********************************************************************/


using MState::LIGHT_LOW;
using MState::LIGHT_HIGH;

const uint8_t IN_LIGHT = A0;  // Configure light sensor analog input (values 0-1023)
const uint8_t OUT_LED = 7;    // configure LED output to digital pin 

// Logical high and low states for the light signal. 
// Determined from comparing the light level to the ambient light level.


float SENSITIVITY = 2; // signal detection sensitivity. 
// It is a multiplier for the ambient light to arrive at the 
// signal threshold level. In low ambient light set this value higher, > 2, to prevent accidental triggering of
// a signal by low level fluctuations. 
// @TODO: make it adjustable by variable resistor.
// It could be also automatically set as the percentage of the ambient light level. 
// THe lower the ambient light level, the higher the 


int sensorValue;  // Variable to read light sensor data into
int ambientLight; // The background light level to which we compare the high value. 

MState currentState = LIGHT_LOW; // The STATE the machine is in
MState readState = LIGHT_LOW; // the STATE calculated from the currentRead


inline MState stateFromValue(int sensorValue) {
  return (sensorValue > ambientLight * SENSITIVITY) ? LIGHT_HIGH : LIGHT_LOW;
}

SensorBuffer<10> buf;

unsigned long signal_start_time;
unsigned long signal_end_time;
unsigned long signal_duration_time; // the duration between two transitions.


unsigned long BG_SAMPLE_INTERVAL = 10000; // sample ambient light every 10 seconds
unsigned long last_sample_time = 0; // sample ambient light every 10 seconds

MorseCoder mc(IN_LIGHT, OUT_LED, 300);
MSignal decoded = INVALID;

unsigned long now;

char code_char;

CodeBuffer<10> codeBuffer;

void writeCodeFromBuffer(CodeBuffer<10> & cb, LCDQueue16x2 & lq, LiquidCrystal * pLcd, bool addSpace = false) {

  if(!cb.isEmpty()) {
      String code = cb.getCode();
      code_char = mc.lookupChar(code);
      lq.push(code_char);
      lq.write(pLcd);
      if(DEBUG_TEST2)
        Serial.println(code + " " + code_char);
      cb.reset();
      
  }
  if(addSpace) {
    lq.push(' ');
    lq.write(pLcd);
      if(DEBUG_TEST2)
        Serial.println();
  }

}

void setup() {
  // put your setup code here, to run once:
  pinMode(OUT_LED, OUTPUT); // initialize pinout

  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();

  delay(200);

  // (IS) Establish initial state, light level
  ambientLight = buf.fillAvg(analogRead, IN_LIGHT);
  currentState = LIGHT_LOW; // The STATE the machine is in
  readState = LIGHT_LOW; // the STATE calculated from the currentRead
  
}



void loop() {

  
  sensorValue = analogRead(IN_LIGHT);
  now = millis();
  // figure out how long the signal lasted  
  signal_duration_time = now - signal_start_time;

  readState = stateFromValue(sensorValue);

  if(DEBUG_TEST1)
    Serial.println("bg:" + String(ambientLight) + " sv:" +  String(sensorValue) + " rs:" + String(readState*100));


  // (LH) detect LOW to HIGH transition
  if(currentState == LIGHT_LOW  &&
     readState == LIGHT_HIGH) {
    
    // get SPACE, LETTER_SEP or WORD_SEP from signal duration
    decoded = mc.decodeSignal(currentState, signal_duration_time);

    if(DEBUG_TEST1)
      Serial.println("(LH), low signal duration: " + String(signal_duration_time) + "; decoded: " + decoded);
    
    if(decoded == MSignal::INVALID)
      codeBuffer.reset();

    else if (decoded == MSignal::SPACE) {
      // we are still writing

    } else if(decoded == MSignal::LETTER_SEP) {
      // end of letter, print letter and reset buffer
      writeCodeFromBuffer(codeBuffer, lq, &lcd);

      /*if(!codeBuffer.isEmpty()) {
        String code = codeBuffer.getCode();
        code_char = mc.lookupChar(code);
        lq.push(code_char);
        lq.write(&lcd);
        Serial.println(code + " " + code_char);
        
        codeBuffer.reset();
      }*/
    } else if(decoded == MSignal::WORD_SEP) {
      writeCodeFromBuffer(codeBuffer, lq, &lcd, true);

      /*
      if(!codeBuffer.isEmpty()) {
        String code = codeBuffer.getCode();
        code_char = mc.lookupChar(code);
        lq.push(code_char);
        lq.write(&lcd);
        Serial.println(code + " " + code_char + " ");
        codeBuffer.reset();
      }
        Serial.println();
      */
    }

    // switch to LIGHT_HIGH state
    currentState = LIGHT_HIGH;
    signal_start_time = now;

  // (HL) HIGH to LOW transition
  } else if (currentState == LIGHT_HIGH && 
     readState == LIGHT_LOW) {

    // get DIT or DAH from signal duration
    decoded = mc.decodeSignal(currentState, signal_duration_time);
    if(DEBUG_TEST1)
      Serial.println("(HL), high signal duration: " + String(signal_duration_time) + "; decoded: " + decoded);
    
    if(decoded == MSignal::INVALID)
      codeBuffer.reset();
    else {
      codeBuffer.push(decoded); // DIT or DAH
    }


    // switch to LIGHT_HIGH state
    currentState = LIGHT_LOW;
    signal_start_time = now;

    //Serial.println("HL, High signal duration: " + String(signal_duration_time));

  } else if (currentState == LIGHT_LOW && 
     readState == LIGHT_LOW) {

      // After the last (HL) transion, the buffer may have data
      // read out the buffer if not empty and print out


      if(!codeBuffer.isEmpty() && signal_duration_time > mc.getLetterSepLength()) {
        /*
        String code = codeBuffer.getCode();
        code_char = mc.lookupChar(code);
        lq.push(code_char);
        lq.write(&lcd);

        Serial.println(code + " " + code_char);
        codeBuffer.reset();
        */
        writeCodeFromBuffer(codeBuffer, lq, &lcd, (signal_duration_time > mc.getMaxWorSepLength()));
        /*
        if(signal_duration_time > mc.getMaxWorSepLength()) {
          Serial.println();
          lq.push(' ');
          lq.write(&lcd);
        }
        */
      }


      // sample 
      if(now - last_sample_time > BG_SAMPLE_INTERVAL && 
          signal_duration_time > mc.getMaxSpaceLength()
          ) {
        buf.push(sensorValue);
        ambientLight = buf.avg();
        last_sample_time = now;

        if(DEBUG_TEST1)
          Serial.println("LOW-LOW BG sample, bg: " + String(ambientLight));

      }

  } else if (currentState == LIGHT_HIGH && 
     readState == LIGHT_HIGH) {
  
        // stuck in HIGH
        /*if(signal_duration_time > mc.getMaxWorSepLength()) {
          ambientLight = buf.fillAvg(analogRead, IN_LIGHT);
          last_sample_time = now;
          Serial.println("HIGH-HIGH BG sample, bg: " + String(ambientLight));

        }*/
      
  }
  //delay(1);
 
}
