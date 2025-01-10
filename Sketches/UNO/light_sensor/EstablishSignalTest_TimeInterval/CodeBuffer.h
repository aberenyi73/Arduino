#ifndef CODE_BUFFER_H
#define CODE_BUFFER_H



template <size_t BUFFER_SIZE>
class CodeBuffer {  
// The number of records light records to keep
// This should be small enough to collect data in less than 10ms, which is the shortest DIT time interval.

public:
  CodeBuffer() {
  }

  // Get the buffer contents as a String.
  String getCode() {
    return buffer.substring(0, current_index);
  }

  constexpr size_t getSize() const {
    return BUFFER_SIZE;
  }
  
  constexpr bool isFull() const {
    return current_index == BUFFER_SIZE;
  }

  constexpr bool isEmpty() const {
    return current_index == 0;
  }
  
  void reset() {
    current_index = 0;
  }
  
  // Add an new value to the buffer, overwriting the oldest value.
  void push(char value) {
    if(current_index < BUFFER_SIZE) {
      //myBuffer[current_index] = value;  
      buffer[current_index] = value;
      current_index++;
    }
  }


private:
  //char myBuffer[BUFFER_SIZE];
  String buffer = "          ";
  size_t current_index = 0; // the position we are writing. It can go beyond the last position, in which case it full.
};


#endif