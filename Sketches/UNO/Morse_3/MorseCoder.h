#ifndef MORSE_H
#define MORSE_H

bool DEBUG_MORSE = true;

const char DIT = '.';
const char DAH = '-';

// A-Z 0-9
struct MORSE_CODE {
  String code;
  char letter;
};
// MORSE CODES, dit/dah array plus length
MORSE_CODE mc__0 = {"-----", '0'};
MORSE_CODE mc__1 = {".----", '1'};
MORSE_CODE mc__2 = {"..---", '2'};
MORSE_CODE mc__3 = {"...--", '3'};
MORSE_CODE mc__4 = {"....-", '4'};
MORSE_CODE mc__5 = {".....", '5'};
MORSE_CODE mc__6 = {"-....", '6'};
MORSE_CODE mc__7 = {"--...", '7'};
MORSE_CODE mc__8 = {"---..", '8'};
MORSE_CODE mc__9 = {"----.", '9'};
MORSE_CODE mc__A = {".-", 'A'};
MORSE_CODE mc__B = {"-...", 'B'};
MORSE_CODE mc__C = {"-.-.", 'C'};
MORSE_CODE mc__D = {"-..", 'D'};
MORSE_CODE mc__E = {".", 'E'};
MORSE_CODE mc__F = {"..-.", 'F'};
MORSE_CODE mc__G = {"--.", 'G'};
MORSE_CODE mc__H = {"....", 'H'};
MORSE_CODE mc__I = {"..", 'I'};
MORSE_CODE mc__J = {".---", 'J'};
MORSE_CODE mc__K = {"-.-", 'K'};
MORSE_CODE mc__L = {".-..", 'L'};
MORSE_CODE mc__M = {"--", 'M'};
MORSE_CODE mc__N = {"-.", 'N'};
MORSE_CODE mc__O = {"---", 'O'};
MORSE_CODE mc__P = {".--.", 'P'};
MORSE_CODE mc__Q = {"--.-", 'Q'};
MORSE_CODE mc__R = {".-.", 'R'};
MORSE_CODE mc__S = {"...", 'S'};
MORSE_CODE mc__T = {"-", 'T'};
MORSE_CODE mc__U = {"..-", 'U'};
MORSE_CODE mc__V = {"...-", 'V'};
MORSE_CODE mc__W = {".--", 'W'};
MORSE_CODE mc__X = {"-..-", 'X'};
MORSE_CODE mc__Y = {"-.--", 'Y'};
MORSE_CODE mc__Z = {"--..", 'Z'};

// "hashtable" of morse codes
MORSE_CODE MORSE_CODES[36] = {mc__0, mc__1, mc__2, mc__3, mc__4, mc__5, mc__6, mc__7, mc__8, mc__9, 
                              mc__A, mc__B, mc__C, mc__D, mc__E, mc__F, mc__G, mc__H, mc__I, mc__J, 
                              mc__K, mc__L, mc__M, mc__N, mc__O, mc__P, mc__Q, mc__R, mc__S, mc__T, 
                              mc__U, mc__V, mc__W, mc__X, mc__Y, mc__Z};






/**
 * Class that can decode morse signals on the Arduino analog input, and write
 * Morse code on a digital pin.
 */
class MorseCoder {
  public:

    MorseCoder(uint8_t in, uint8_t out, int dit_length = 300) : 
        in_pin(in), out_pin(out) {
      setDitLength(dit_length);
    }

    // set the time duration of a dit, and all related timings
    void setDitLength(int dit) {
      dit_length = dit;
      dah_length = (3 * dit_length);
      space_length = (dit_length);
      letter_sep_length = (3 * dit_length);
      word_sep_length = (7 * dit_length);
    }

    /////////////////////////////////////////
    /////// SIGNALING CODE //////////////////

    // Light up the diode for dit time
    void signalDit() {

      digitalWrite(out_pin, HIGH);
      delay(dit_length);
      digitalWrite(out_pin, LOW);
    }

    // Light up the diode for dah time
    void signalDah() {
      digitalWrite(out_pin, HIGH);
      delay(dah_length);
      digitalWrite(out_pin, LOW);
    }

    // pause signal for DIT_LENGHT, between dits and dahs
    void signalSpace() {
      delay(dit_length);
    }

    // pause 
    void signalLetterSep() {
      delay(letter_sep_length);
    }

    void signalWordSep() {
      delay(word_sep_length);
    }

    // signal a single code.
    // @param mCode for example ".---"
    void signalCode(MORSE_CODE* mCode) {
      if(DEBUG_MORSE) Serial.println(String(mCode->letter) + " " + mCode->code);

      for(int i = 0; i < (*mCode).code.length(); i++) {
          
          if(i>0) signalSpace();
      
          if((*mCode).code[i] == '.') 
            signalDit();
          else 
            signalDah();

      }
    }

    /**
    * Hash function to get the morse code from a letter. Return the code.
    * @param letter only upper case letters accepted.
    * @return The MORSE_CODE of the letter.
    */
    MORSE_CODE * lookupMorseCode(char letter) {

      // convert lower case letters to upper case
      if(letter >= 97 && letter <= 122) 
        letter -= 32;

      if(letter >=48 && letter <=48)  // A == 65
        return & MORSE_CODES[letter - 48]; // numbers
      else if (letter >= 65 && letter <= 90)
        return & MORSE_CODES[letter - 55]; // letters
    }

    // signal a message as morse code
    void signalText(String text) {
      bool startWord = true;
      
      for(int i = 0; i < text.length(); i++) {

          if(text[i] == ' ') {
            signalWordSep();
            startWord = true;
          }
          else {
            if(startWord) {
              startWord = false;  
            } else{
              signalLetterSep();
            }  
            MORSE_CODE * code = lookupMorseCode(text[i]);
            signalCode(code);
            
          }

      }

    }

    /////// END SIGNALING CODE //////////////
    /////////////////////////////////////////

  private:
    uint8_t in_pin;    // input analog pin 
    uint8_t out_pin;    // output digital pin 

    // dit: duration of short morse
    int dit_length;

    // dah: duration of long morse
    int dah_length;
    
    // Each dit or dah within an encoded character is followed by a period of
    // signal absence, called a space, equal to the dit duration.
    int space_length;
    
    // The letters of a word are separated by a duration equal to three dits.
    int letter_sep_length;
    
    // Words are separated by a space equal to seven dits.
    int word_sep_length;

    
};



#endif MORSE_H