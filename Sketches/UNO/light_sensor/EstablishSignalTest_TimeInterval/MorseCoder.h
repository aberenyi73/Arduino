#ifndef MORSE_H
#define MORSE_H

#define DEBUG_MORSE true

#define MORSE_COUNT 36


#include <Arduino.h>  // Required for the Arduino String class


// The signal being read is in either one of there states.
enum MState {
  LIGHT_LOW = 0,
  LIGHT_HIGH = 1
};

// One of the possible signals in Morse signaling:
// INVALID, DIT, DAH, SPACE, LETTER_SEP, WORD_SEP
enum MSignal {
  INVALID = '0',
  DIT = '.',
  DAH = '-',
  SPACE = ' ',
  LETTER_SEP = 4,
  WORD_SEP = 5
};


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

    // for printing purposes, '.';
    //static const char DIT; 
    // for printing purposes, '-'
    //static const char DAH;
    // for printing purposes, ' '
    //static const char SPACE;

    MorseCoder(uint8_t in, uint8_t out, uint16_t dit_length = 300);

    // set the time duration of a dit, and all related timings
    void setDitLength(const uint16_t dit);
    

    inline uint16_t MorseCoder::getDitLength() const {
      return dit_length;
    }

    inline uint16_t MorseCoder::getDahLength() const {
      return dah_length;
    }

    inline uint16_t MorseCoder::getSpaceLength() const {
      return space_length;
    }

    inline uint16_t MorseCoder::getMaxSpaceLength() const {
      return max_space_length;
    }

    inline uint16_t MorseCoder::getLetterSepLength() const {
        return letter_sep_length;
    }

    inline uint16_t MorseCoder::getWorSepLength() const {
        return word_sep_length;
    }

    inline uint16_t MorseCoder::getMaxWorSepLength() const {
        return max_word_sep_length;
    }

    inline uint16_t MorseCoder::getMinWorSepLength() const {
        return min_word_sep_length;
    }

    inline float MorseCoder::getTimeTolerance() const {
      return time_tolerance;
    }

    void setTimeTolerance(const float delta);

    String toString();

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
    MORSE_CODE * lookupMorseCode(const char letter);

    /**
    * Hash function to get the morse code from a letter. Return the code.
    * @param code A string of DITs and DAHs, for example ".-"
    * @return The character or number encoed by the code.
    */
    char lookupChar(const String & code);
   



    // signal a message as morse code
    void signalText(String text);

    /////// END SIGNALING CODE //////////////
    /////////////////////////////////////////



    /////// SIGNAL DETECTION CODE ///////////

    /**
     * Given a state (HIGH | LOW), and a signal duration time, determine if it was DIT, DAH, SPACE, etc.
     * The rules are: 
     * The default, undecodable signal is INVALID.
     * If it was a LOW signal and the signal_duration_time > max_word_sep_length, it will be a WORD_SEP.
     * If it was a LOW signal and the signal_duration_time is between the respective min and max values 
     * of letter_sep_length or space_length, it will be LETTER_SEP or SPACE.
     * If it was a HIGH signal, and the signal time duration falls between dit_length or dah_length, it will be
     * DIT or DAH.
     */
    MSignal decodeSignal(const MState hi_low, const unsigned long signal_duration_time) const;

    ///// END  SIGNAL DETECTION CODE ////////



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

    // the minimum time duration that a signal must stay HIGH in order to qualify as DIT
    uint16_t min_dit_length;
    uint16_t max_dit_length;

    uint16_t min_dah_length;
    uint16_t max_dah_length;
    
    uint16_t min_space_length;
    uint16_t max_space_length;
    
    uint16_t min_letter_sep_length;
    uint16_t max_letter_sep_length;
    
    uint16_t min_word_sep_length;
    uint16_t max_word_sep_length;

    // the time the current signal level (HIGH | LOW) started
    unsigned long time_start;
    // the time the current signal level (HIGH | LOW) ended
    unsigned long time_end;

    /* The percentage of the dit length that a signal can deviate from the nominal value.
     * It is a fraction, such as 0.1.
     */
    float time_tolerance;

    
};



#endif MORSE_H