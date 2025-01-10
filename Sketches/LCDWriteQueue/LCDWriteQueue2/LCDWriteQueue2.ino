/**
 * Type in a character in the Serial Monitor, and it will print 
 * it to the LCD monitor, typewriter style
 */ 

// include the library code:
#include <LiquidCrystal.h>


char in_char = 32;  // space

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/**
 * Class to manage LCD display rolling
 */
class LCDQueue16x2 {

public:
  
  static const int QUEUE_SIZE = 32;
  static const int COLUMNS = 16;

  LCDQueue16x2() {
    for(int i = 0; i < QUEUE_SIZE; i++){
      queue[i] = 32; // space
    }
  }
  
  // add an item to the queue and advance the index by one. If index reaches
  // the end, start from 0.
  void push(char item) {
    queue[queue_index] = item;
    queue_index++;
    if(queue_index == QUEUE_SIZE)
      queue_index = 0;
  }

  // Write to the 16x2 LCD all the buffer data. Start from queue_index, filling
  // up the first row, then the second row.
  void write(LiquidCrystal *lcd) {
    lcd->clear();
    lcd->setCursor(0, 0);
    for(int i = 0; i < QUEUE_SIZE; i++) { 
      if(i == COLUMNS) {
        lcd->setCursor(0, 1);
      }
      lcd->write(queue[(i + queue_index) % QUEUE_SIZE]);
    }
  }

private:
  char queue[QUEUE_SIZE] = {};
  // always points to the first element (head) of the queue. 
  // Start printing from this index of insert a new char to this index.
  // After insert increment by 1.
  uint8_t queue_index = 0;
};

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
