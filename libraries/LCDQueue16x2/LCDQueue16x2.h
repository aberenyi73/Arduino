#ifndef LCDQUEUE16x2_H
#define LCDQUEUE16x2_H

// include the library code:

#include <LiquidCrystal.h>

/**
 * Class to manage LCD display rolling on a 16x2 Arduino LCD.
 * The LCD display will be scrolling the oldest character off the screen when a new one is added.
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
  
  // Add an item to the queue and advance the index by one. If index reaches
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

#endif