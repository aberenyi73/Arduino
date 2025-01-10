/**
 * Read background light level for 2 second, sampling every 100 ms.
 */

#include <cppQueue.h>

const uint8_t IN_LIGHT = A0;  // Configure light sensor analog input (values 0-1023)
const uint8_t OUT_LED = 7;    // configure LED output to digital pin 

const int SAMPLING_INTERVAL = 50; // milliseconds. @TODO: make it adjustable by variable resistor.
const int TIME_RECORD = 2000; // Length of time the ambient light data is kept.

// The number of records light records to keep for a TIME_RECORD length of time, given sampling interval.
// The device "warmup time" also depends on this value before it can be used.
// The sampling interval times this value determines the actual length of time the ambient light data was collected.
int QUEUE_SIZE = (TIME_RECORD / SAMPLING_INTERVAL); 

int SENSITIVITY = 1.2; // signal detection sensitivity. It is a multiplier for the ambient light to arrive at the 
// signal threshold level. In low ambient light set this value higher, > 2, to prevent accidental triggering of
// a signal by low level fluctuations. 
// @TODO: make it adjustable by variable resistor.
// It could be also automatically set as the percentage of the ambient light level. 
// THe lower the ambient light level, the higher the 

int sensorValue = 200; // Variable to read light sensor data into

// Hold rolling values of light reading in FIFO queue.
cppQueue	backgroundLevel(sizeof(int), QUEUE_SIZE, FIFO);	// Instantiate queue


//const int LIGHT_THRESHOLD = 200;  // LED turn on/off level
int rec, i, sum, avg; // temporary record
String s = "";

void setup() {
  // put your setup code here, to run once:
  pinMode(OUT_LED, OUTPUT); // initialize pinout
  Serial.begin(9600);
  for(int i = 0; i < QUEUE_SIZE; i++){ // initialize ambient ligth data
    backgroundLevel.push(&sensorValue);  
  }
}


void loop() {
  // put your main code here, to run repeatedly:
  sensorValue = analogRead(IN_LIGHT);

  // replace oldest value with new light sensor data
	backgroundLevel.pop(&rec);
  backgroundLevel.push(&sensorValue);
	
  // calculate new average in queue
  s = "";
  sum = 0;
  for(i = 0; i < QUEUE_SIZE; i++) {
    backgroundLevel.pop(&rec);
    //s = s + String(rec) + ", ";
    sum += rec;
    backgroundLevel.push(&rec);
  }	
  avg = sum/QUEUE_SIZE;
  s += " Ambient light level = " + String(avg) + " ;  sensorValue = " + String(sensorValue);
  Serial.println(s);
	if(sensorValue > avg * SENSITIVITY) 
    digitalWrite(OUT_LED, HIGH);
  else
    digitalWrite(OUT_LED, LOW);

  delay(SAMPLING_INTERVAL); // sample every 100 millisecond

  // @TODO Find a light source to begin reading signal
  // If detecting a signal X percent above background, begin decoding MORSE codes.
  // Determine what is the value of X.

}
