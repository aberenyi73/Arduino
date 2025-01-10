/**
 * Read background light level for 2 second, sampling every 100 ms.
 */

#include <cppQueue.h>
#include "SensorBuffer.h"
#include "MorseCoder.h"

const uint8_t IN_LIGHT = A0;  // Configure light sensor analog input (values 0-1023)
const uint8_t OUT_LED = 7;    // configure LED output to digital pin 

// Logical high and low states for the light signal. 
// Determined from comparing the light level to the ambient light level.

enum STATE {
  LIGHT_LOW = 0,
  LIGHT_HIGH = 1
};

int SENSITIVITY = 2; // signal detection sensitivity. 
// It is a multiplier for the ambient light to arrive at the 
// signal threshold level. In low ambient light set this value higher, > 2, to prevent accidental triggering of
// a signal by low level fluctuations. 
// @TODO: make it adjustable by variable resistor.
// It could be also automatically set as the percentage of the ambient light level. 
// THe lower the ambient light level, the higher the 


int sensorValue;  // Variable to read light sensor data into
int ambientLight; // The background light level to which we compare the high value. 


STATE currentState = LIGHT_LOW; // The STATE the machine is in
STATE readState = LIGHT_LOW; // the STATE calculated from the currentRead
STATE lastReadState = LIGHT_LOW; // the STATE the machine was is at the lst read time


inline STATE stateFromValue(int sensorValue) {
  return (sensorValue > ambientLight * SENSITIVITY) ? LIGHT_HIGH : LIGHT_LOW;
}

SensorBuffer buf;

unsigned long signal_start_time;
unsigned long signal_end_time;

unsigned long signal_duration_time; // the duration between two transitions.

unsigned long MIN_DIT_TIME = 50; // milliseconds. If shorter than this, it is not a DIT signal, but noise.
unsigned long MAX_DIT_TIME = 330; // milliseconds. If longerm then not a DIT, but letter or word sep.


unsigned long BG_SAMPLE_INTERVAL = 10000; // sample ambient light every 10 seconds
unsigned long last_sample_time = 0; // sample ambient light every 10 seconds

unsigned long now;

void setup() {
  // put your setup code here, to run once:
  pinMode(OUT_LED, OUTPUT); // initialize pinout

  Serial.begin(9600);
  delay(200);

  // (IS) Establish initial state, light level
  ambientLight = buf.fillAvg(analogRead, IN_LIGHT);
  currentState = LIGHT_LOW; // The STATE the machine is in
  readState = LIGHT_LOW; // the STATE calculated from the currentRead
  lastReadState = LIGHT_LOW; // the STATE the machine was is at the lst read time
  
}



void loop() {
  
  sensorValue = analogRead(IN_LIGHT);
  now = millis();

  readState = stateFromValue(sensorValue);
  //Serial.println("bg: " + String(ambientLight) + "; sv: " +  String(sensorValue) + "; rs:" + String(readState));


  // (LH) detect LOW to HIGH transition
  if(currentState == LIGHT_LOW && 
     lastReadState == LIGHT_HIGH && 
     readState == LIGHT_HIGH) {

      
    // start timing the signal
    // signal_end_time = now;
    signal_duration_time = now - signal_start_time;
    signal_start_time = now;
    
    // get DIT or DAH from signal duration

    // switch to LIGHT_HIGH state
    currentState = LIGHT_HIGH;

    Serial.println("LH, low signal duration: " + String(signal_duration_time));

  // (HL) HIGH to LOW transition
  } else if (currentState == LIGHT_HIGH && 
     lastReadState == LIGHT_LOW && 
     readState == LIGHT_LOW) {

    // start timing the signal
    // signal_end_time = now;
    signal_duration_time = now - signal_start_time;
    signal_start_time = now;

    // switch to LIGHT_HIGH state
    currentState = LIGHT_LOW;

    Serial.println("HL, High signal duration: " + String(signal_duration_time));

  } 
  
  if (currentState == LIGHT_LOW && 
     lastReadState == LIGHT_LOW && 
     readState == LIGHT_LOW) {
        if(now - last_sample_time > BG_SAMPLE_INTERVAL ) {
          ambientLight = buf.fillAvg(analogRead, IN_LIGHT);
          last_sample_time = now;
          Serial.println("BG sample, bg: " + String(ambientLight));

        }

  }

  // if it has been in LIGHT_HIGH for more than MAX_TIME, reset start_time

  lastReadState = readState;
 
}
