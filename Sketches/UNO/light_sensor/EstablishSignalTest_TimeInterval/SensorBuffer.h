
#ifndef SENSOR_BUFFER_H
#define SENSOR_BUFFER_H

#include <Arduino.h>



// Circular buffer to hold integer sensor data and quickly calculate the average.
template <size_t BUFFER_SIZE>
class SensorBuffer {  
// The number of records light records to keep
// This should be small enough to collect data in less than 10ms, which is the shortest DIT time interval.

public:
  SensorBuffer(int initValue = 200) {
    for(int i {0}; i < BUFFER_SIZE; i++)
      push(initValue);
  }

  constexpr size_t getSize() const {
    return BUFFER_SIZE;
  }
  
  // Add an new value to the buffer, overwriting the oldest value.
  void push(int value) {
    myBuffer[current_index++] = value;
    if(current_index == BUFFER_SIZE) {current_index = 0;}
  }

  // fill the buffer with new data for less than 5 ms
  void fill(int (*readFunc)(int), const uint8_t IN_LIGHT);

  int avg();

  /**
   * @brief Fill the buffer with new data for less than 5 ms.
   * @param readFunc The function pointer to analogRead().
   * @param IN_LIGHT The pin of the light sensor.
   * @return the newly calculated average value of the buffer.
   */
  fillAvg(int (*readFunc)(int), const uint8_t IN_LIGHT) {
    fill(readFunc, IN_LIGHT);
    return avg();
  }

private:
  int myBuffer[BUFFER_SIZE];
  size_t size;
  size_t current_index = 0;
};


// ************************************************* //
// **************** Implementation ***************** //


// fill the buffer with new data for less than 5 ms
template <size_t BUFFER_SIZE>
void SensorBuffer<BUFFER_SIZE>::fill(int (*readFunc)(int), const uint8_t IN_LIGHT) {
  unsigned long start_time = millis();
  unsigned long end_time = start_time + 5;
  
  for(int i {0}; i < BUFFER_SIZE; i++) {
    if(millis() > end_time) {
      // Serial.println("Breaking out of SensorBuffer.fill() " + String(i));
      break;
    }
    push(readFunc(IN_LIGHT));
  }

}

template <size_t BUFFER_SIZE>
int SensorBuffer<BUFFER_SIZE>::avg(){
  int sum = myBuffer[0];
  for(int i = 1; i < BUFFER_SIZE; i++) {
    sum += myBuffer[i];
  }
  return sum/BUFFER_SIZE;
}



#endif