#include "SensorBuffer.h"

SensorBuffer::SensorBuffer(uint16_t _size, int initValue = 200) : size(_size) {
  myBuffer = new int[size];
  for(int i {0}; i < BUFFER_SIZE; i++)
    push(initValue);
  Serial.print("SensorBuffer(" + String(initValue) + ")");
}

SensorBuffer::~SensorBuffer() {
  delete[] myBuffer;
}

uint16_t SensorBuffer::getSize() const {
    return size;
  }

// Add an new value to the buffer, overwriting the oldest value.
void SensorBuffer::push(int value) {
    myBuffer[current_index++] = value;
    if(current_index == BUFFER_SIZE) {current_index = 0;}
}

// fill the buffer with new data for less than 5 ms
void SensorBuffer::fill(int (*readFunc)(int), const uint8_t IN_LIGHT) {
  unsigned long start_time = millis();
  unsigned long end_time = start_time + 5;
  
  for(int i {0}; i < BUFFER_SIZE; i++) {
    if(millis() > end_time) {
      Serial.println("Breaking out of SensorBuffer.fill() " + String(i));
      break;
    }
    push(readFunc(IN_LIGHT));
  }
  //Serial.println("SensorBuffer.fill() in " + String(millis() - start_time));

}


int SensorBuffer::avg(){
  int sum = myBuffer[0];
  for(int i = 1; i < BUFFER_SIZE; i++) {
    sum += myBuffer[i];
  }
  return sum/BUFFER_SIZE;
}

// fill the buffer with new data for less than 5 ms
int SensorBuffer::fillAvg(int (*readFunc)(int), const uint8_t IN_LIGHT) {
  fill(readFunc, IN_LIGHT);
  return avg();
}

