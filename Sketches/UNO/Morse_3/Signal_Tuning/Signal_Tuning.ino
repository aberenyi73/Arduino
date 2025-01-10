/**
 * Signal -.-.-. repeatedly to help establish signal length.
 * ESP32 board
 */

const uint8_t OUT_LED = 26;    // configure LED output to digital pin 

unsigned long interval = 300; // ms
unsigned long time_start;
unsigned long time_end;


void setup() {
  // put your setup code here, to run once:
    pinMode(OUT_LED, OUTPUT);

}

void signalDit() {
  digitalWrite(OUT_LED, HIGH);
  time_end = millis() + interval;
  while(millis() < time_end) {}

  digitalWrite(OUT_LED, LOW);
}

void signalDah() {
  digitalWrite(OUT_LED, HIGH);
  time_end = millis() + interval*3;
  while(millis() < time_end) {}

  digitalWrite(OUT_LED, LOW);
}


void signalSpace() {
  digitalWrite(OUT_LED, LOW);
  time_end = millis() + interval;
  while(millis() < time_end) {}
}

void signalLetter() {
  digitalWrite(OUT_LED, LOW);
  time_end = millis() + interval*3;
  while(millis() < time_end) {}
}

void signalWord() {
  digitalWrite(OUT_LED, LOW);
  time_end = millis() + interval*5;
  while(millis() < time_end) {}
}

void loop() {
  // put your main code here, to run repeatedly:
  
  signalDah();
  signalSpace();
  signalDit();
  signalSpace();

  signalDah();
  signalSpace();
  signalDit();
  signalSpace();

  signalDah();
  signalSpace();
  signalDit();
  signalWord(); 
}
