#ifndef SENSOR_BUFFER_H
#define SENSOR_BUFFER_H

#define BUFFER_SIZE 10


#include <Arduino.h>



// Circular buffer to hold sensor data.
class SensorBuffer {  
// The number of records light records to keep
// This should be small enough to collect data in less than 10ms, which is the shortest DIT time interval.

public:
  SensorBuffer(uint16_t size = BUFFER_SIZE, int initValue = 200);

  ~SensorBuffer();

  uint16_t getSize() const;
  
  // Add an new value to the buffer, overwriting the oldest value.
  void push(int value) ;

  // fill the buffer with new data for less than 5 ms
  void fill(int (*readFunc)(int), const uint8_t IN_LIGHT);


  int avg();

 
  /**
   * @brief Fill the buffer with new data for less than 5 ms.
   * @param readFunc The function pointer to analogRead().
   * @param IN_LIGHT The pin of the light sensor.
   * @return the newly calculated average value of the buffer.
   */
  int fillAvg(int (*readFunc)(int) , const uint8_t IN_LIGHT);

private:
  //int myBuffer[BUFFER_SIZE];
  int * myBuffer;
  uint16_t size;
  size_t current_index = 0;
};

#endif