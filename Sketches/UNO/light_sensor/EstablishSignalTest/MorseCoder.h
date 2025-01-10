#ifndef MORSE_H
#define MORSE_H

#define DEBUG_MORSE true

#define MORSE_COUNT 36


#include <Arduino.h>  // Required for the Arduino String class


// A-Z 0-9
struct MORSE_CODE {
  String code;
  char letter;
};

// MORSE CODES, dit/dah array plus length
extern MORSE_CODE mc__0;
extern MORSE_CODE mc__1;
extern MORSE_CODE mc__2;
extern MORSE_CODE mc__3;
extern MORSE_CODE mc__4;
extern MORSE_CODE mc__5;
extern MORSE_CODE mc__6;
extern MORSE_CODE mc__7;
extern MORSE_CODE mc__8;
extern MORSE_CODE mc__9;
extern MORSE_CODE mc__A;
extern MORSE_CODE mc__B;
extern MORSE_CODE mc__C;
extern MORSE_CODE mc__D;
extern MORSE_CODE mc__E;
extern MORSE_CODE mc__F;
extern MORSE_CODE mc__G;
extern MORSE_CODE mc__H;
extern MORSE_CODE mc__I;
extern MORSE_CODE mc__J;
extern MORSE_CODE mc__K;
extern MORSE_CODE mc__L;
extern MORSE_CODE mc__M;
extern MORSE_CODE mc__N;
extern MORSE_CODE mc__O;
extern MORSE_CODE mc__P;
extern MORSE_CODE mc__Q;
extern MORSE_CODE mc__R;
extern MORSE_CODE mc__S;
extern MORSE_CODE mc__T;
extern MORSE_CODE mc__U;
extern MORSE_CODE mc__V;
extern MORSE_CODE mc__W;
extern MORSE_CODE mc__X;
extern MORSE_CODE mc__Y;
extern MORSE_CODE mc__Z;


// "hashtable" of morse codes
extern MORSE_CODE MORSE_CODES[MORSE_COUNT];

/**
 * Class that can decode morse signals on the Arduino analog input, and write
 * Morse code on a digital pin.
 */
class MorseCoder {
public:

    static const char DIT;
    static const char DAH;

    MorseCoder(uint8_t in, uint8_t out, uint16_t dit_length = 300);

    // set the time duration of a dit, and all related timings
    void setDitLength(const uint16_t dit);

    inline uint16_t getDitLength() const;

    /////////////////////////////////////////
    /////// SIGNALING CODE //////////////////

    // Light up the diode for dit time
    void signalDit();

    // Light up the diode for dah time
    void signalDah();

    // pause signal for DIT_LENGHT, between dits and dahs
    void signalSpace();

    // pause 
    void signalLetterSep();

    void signalWordSep();

    // signal a single code.
    // @param mCode for example ".---"
    void signalCode(MORSE_CODE* mCode);

    /**
    * Hash function to get the morse code from a letter. Return the code.
    * @param letter only upper case letters accepted.
    * @return The MORSE_CODE of the letter.
    */
    MORSE_CODE * lookupMorseCode(char letter);

    // signal a message as morse code
    void signalText(String text);

    /////// END SIGNALING CODE //////////////
    /////////////////////////////////////////

  private:
    uint8_t in_pin;    // input analog pin 
    uint8_t out_pin;    // output digital pin 

    // dit: duration of short morse in milliseconds
    uint16_t dit_length;

    // dah: duration of long morse
    uint16_t dah_length;
    
    // Each dit or dah within an encoded character is followed by a period of
    // signal absence, called a space, equal to the dit duration.
    uint16_t space_length;
    
    // The letters of a word are separated by a duration equal to three dits.
    uint16_t letter_sep_length;
    
    // Words are separated by a space equal to seven dits.
    uint16_t word_sep_length;

    unsigned long time_start;
    unsigned long time_end;

    
};



#endif MORSE_H