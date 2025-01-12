#ifndef CODE_BUFFER_H
#define CODE_BUFFER_H

/**
 * Buffer class to collect DITs and DAHs during code detection.
 */
template <size_t BUFFER_SIZE>
class CodeBuffer {  

public:

  // Initialize the buffer with BUFFER_SIZE empty spaces. 
  // Slow to start, so create an instance before usage and reuse the instance.
  CodeBuffer() {
    for(int i {0}; i < BUFFER_SIZE; i++)
      buffer += " ";
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
  // The String whose chars are the buffer buckets.
  String buffer = "";
  
  // the position we are writing. It can go beyond the last position, in which case it full.
  size_t current_index = 0; 
};


#endif