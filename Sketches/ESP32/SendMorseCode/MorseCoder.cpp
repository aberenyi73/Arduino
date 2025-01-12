
#include "MorseCoder.h"


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
MORSE_CODE MORSE_CODES[MORSE_COUNT] = {mc__0, mc__1, mc__2, mc__3, mc__4, mc__5, mc__6, mc__7, mc__8, mc__9, 
                              mc__A, mc__B, mc__C, mc__D, mc__E, mc__F, mc__G, mc__H, mc__I, mc__J, 
                              mc__K, mc__L, mc__M, mc__N, mc__O, mc__P, mc__Q, mc__R, mc__S, mc__T, 
                              mc__U, mc__V, mc__W, mc__X, mc__Y, mc__Z};


MorseCoder::MorseCoder(uint8_t in, uint8_t out, uint16_t dit_length) : 
  in_pin(in), out_pin(out) {
  setDitLength(dit_length);
  setTimeTolerance(0.05f);
}

// set the time duration of a dit, and all related timings
void MorseCoder::setDitLength(const uint16_t ditLength) {
  dit_length = ditLength;
  dah_length = (3 * dit_length);
  space_length = (dit_length);
  letter_sep_length = (3 * dit_length);
  word_sep_length = (7 * dit_length);
  setTimeTolerance(getTimeTolerance());
}



void MorseCoder::setTimeTolerance(const float _tolerance) {
  time_tolerance = _tolerance;

  uint16_t delta = static_cast<float>(dit_length) * _tolerance;
  
  min_dit_length = dit_length - delta;
  max_dit_length = dit_length + delta;

  min_dah_length = dah_length - delta;
  max_dah_length = dah_length + delta;
  
  min_space_length = space_length - delta;
  max_space_length = space_length + delta;
  
  min_letter_sep_length = letter_sep_length - delta;
  max_letter_sep_length = letter_sep_length + delta;

  min_word_sep_length = word_sep_length - delta;
  max_word_sep_length = word_sep_length + delta;


}

String MorseCoder::toString() {
  String s = "dit_length: " + String(dit_length) + "; min_dit_length: " + String(min_dit_length) +
              "; max_dit_length: " + String(max_dit_length);
  return s;
}


// Light up the diode for dit time
void MorseCoder::signalDit() {
  digitalWrite(out_pin, HIGH);
  time_end = millis() + dit_length;
  while(millis() < time_end) {}
  digitalWrite(out_pin, LOW);
}

// Light up the diode for dah time
void MorseCoder::signalDah() {
  digitalWrite(out_pin, HIGH);
  time_end = millis() + dah_length;
  while(millis() < time_end) {}
  digitalWrite(out_pin, LOW);
}

  // pause signal for DIT_LENGHT, between dits and dahs
  void MorseCoder::signalSpace() {
    digitalWrite(out_pin, LOW);
    time_end = millis() + space_length;
    while(millis() < time_end) {}
  }

  // pause 
  void MorseCoder::signalLetterSep() {
    digitalWrite(out_pin, LOW);
    time_end = millis() + letter_sep_length;
    while(millis() < time_end) {}
  }

  void MorseCoder::signalWordSep() {
    delay(word_sep_length);
    digitalWrite(out_pin, LOW);
    time_end = millis() + word_sep_length;
    while(millis() < time_end) {}

  }

  // signal a single code.
  // @param mCode for example ".---"
  void MorseCoder::signalCode(MORSE_CODE* mCode) {
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
MORSE_CODE * MorseCoder::lookupMorseCode(const char letter) {

  MORSE_CODE * ret = NULL;

  if(letter >=48 && letter <=57)  
    ret = & MORSE_CODES[letter - 48]; // numbers

  else if (letter >= 65 && letter <= 90)
    ret = & MORSE_CODES[letter - 55]; // letters
  
  // convert lower case letters to upper case by subtracting 32
  else if(letter >= 97 && letter <= 122) 
    ret = & MORSE_CODES[letter - 87 /* 32 - 55 */ ]; 

  return ret;

}

char MorseCoder::lookupChar(const String & code) {
  for(int i {0}; i < MORSE_COUNT; i++) {
    if (code == MORSE_CODES[i].code)
      return MORSE_CODES[i].letter;
  }
  return '?';
}


bool MorseCoder::isCleanText(String inputText) {
  
  for(int i {0}; i< inputText.length(); i++) {
    MORSE_CODE * m ;
    m = lookupMorseCode(inputText[i]);    
    if(m == NULL) {
      if(inputText[i] != ' ') 
        return false;   
    }
      
  }
  return true;
}

// remove the characters that are not morse code or a space.
String MorseCoder::cleanText(String inputText) {
  String ret;
  for(int i {0}; i< inputText.length(); i++) {
    MORSE_CODE * m ;
    m = lookupMorseCode(inputText[i]);    
    if(m != NULL)
      ret.concat(m->letter);    
    else if(inputText[i] == ' ') 
      ret.concat(' ');    
    
  }
  return ret;
}

// signal a message as morse code
void MorseCoder::signalText(const String & text) {
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

MSignal MorseCoder::decodeSignal(const MState hi_low, const unsigned long signal_duration_time) const {
    
  if(hi_low == LIGHT_LOW) {
    // WORD_SEP, LETTER_SEP or SPACE

    // most of the time there will be no signal, so return quickly.
    if(signal_duration_time >= min_word_sep_length &&
       signal_duration_time <= max_word_sep_length )
      return MSignal::WORD_SEP; 

    // if we got a space, don't check letter
    if(signal_duration_time >= min_space_length && 
       signal_duration_time <= max_space_length)
      return MSignal::SPACE; 
      
    if(signal_duration_time >= min_letter_sep_length && 
       signal_duration_time <= max_letter_sep_length)
      return MSignal::LETTER_SEP; 

  } else {
  // DIT, DAH

    // return after dit
    if(signal_duration_time >= min_dit_length && 
       signal_duration_time <= max_dit_length)
      return MSignal::DIT; 

    if(signal_duration_time >= min_dah_length && 
       signal_duration_time <= max_dah_length)
      return MSignal::DAH; 

  }
  // default value
  return MSignal::INVALID;

}




