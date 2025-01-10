/**
  Blink SOS signals
*/

#include "MorseCoder.h"

//DEBUG_MORSE = true;

#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))



void printLetterSeparator() {
    Serial.print("   ");    
}

void printWordSeparator() {
    Serial.print("       ");    
}

MorseCoder mc(A0, 8, 200);

void helloWorld() {
  char hello[] = "Janice Williams";
  Serial.println(hello);

  bool startWord = true;
  
  for(int i = 0; i < NUMELEMENTS(hello); i++) {
    
      if(hello[i] == ' ') {
        printWordSeparator();
        startWord = true;
      }
      else {
        if(startWord) {
          startWord = false;  
        } else{
          printLetterSeparator();
        }  
        Serial.print(hello[i]);
        Serial.print((* (mc.lookupMorseCode(hello[i]))).code);

      }

  }
    Serial.println();
}



void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);


  Serial.begin(9600);
  while(!Serial.availableForWrite()){}

  Serial.print("C++ version: " );
  Serial.println(__cplusplus);
  helloWorld();

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(2000);
  //digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
  ///signalText("SOS");
  mc.signalText("Janice Williams"); // .--- .- -. .. -.-. .

}
