/**
  Blink SOS signals
*/

#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))

#ifndef MORSE_H
#define MORSE_H

// dit: duration of short morse
// dah: duration of long morse
// Each dit or dah within an encoded character is followed by a period of signal absence, called a space, equal to the dit duration.
// The letters of a word are separated by a space of duration equal to three dits, 
// Words are separated by a space equal to seven dits.
int DIT_LENGTH = 300;
int DAH_LENGTH = (3 * DIT_LENGTH);
int SPACE_LENGTH = (DIT_LENGTH);
int LETTER_SEP_LENGTH = (3 * DIT_LENGTH);
int WORD_SEP_LENGTH = (7 * DIT_LENGTH);

const char DIT = '.';
const char DAH = '_';

// A-Z 0-9
struct MORSE_CODE {
  String code;
  char letter;
};

/**
 * Class that can decode morse signals on the Arduino analog input, and write
 * Morse code on a digital pin.
 */
class MorseCoder {
  public:
    MorseCoder(uint8_t in, uint8_t out, int dit_length = 300) {

    }

  private:
    uint8_t in_pin = A0;    // input analog pin 
    uint8_t out_pin = 7;    // output digital pin 
    int dit_length = 300;
}

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




#endif MORSE_H


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

void printLetterSeparator() {
    Serial.print("   ");    
}

void printWordSeparator() {
    Serial.print("       ");    
}

void helloWorld() {
  char hello[] = "HELLO WORLD";
  Serial.println(hello);

  bool startWord = true;
  
  for(int i = 0; i < NUMELEMENTS(hello); i++) {
    
      if(hello[i] == ' ') {
        printWordSeparator();
        startWord = true;
      }
      else {
        if(startWord) {
          startWord = false;  
        } else{
          printLetterSeparator();
        }  
        Serial.print(hello[i]);
        Serial.print((*lookupMorseCode(hello[i])).code);

      }

  }
    Serial.println();
}

void printDebug(String text) {
  //return;
  Serial.println(text);
}



/////////////////////////////////////////
/////// SIGNALING CODE //////////////////

// Light up the diode for dit time
void signalDit() {
  Serial.print('.');

  digitalWrite(LED_BUILTIN, HIGH);
  delay(DIT_LENGTH);
  digitalWrite(LED_BUILTIN, LOW);
}

// Light up the diode for dah time
void signalDah() {
  Serial.print('_');

  digitalWrite(LED_BUILTIN, HIGH);
  delay(DAH_LENGTH);
  digitalWrite(LED_BUILTIN, LOW);
}

// pause signal for DIT_LENGHT, between dits and dahs
void signalSpace() {
  delay(DIT_LENGTH);
}

// pause 
void signalLetterSep() {
  Serial.print(' ');
  delay(LETTER_SEP_LENGTH);
}

void signalWordSep() {
  Serial.print("   ");
  delay(WORD_SEP_LENGTH);
}

// signal a single code.
// @param mCode for example ".---"
void signalCode(MORSE_CODE* mCode) {

  for(int i = 0; i < (*mCode).code.length(); i++) {
      
      if(i>0) signalSpace();
  
      if((*mCode).code[i] == '.') 
        signalDit();
      else 
        signalDah();

  }
}

// signal a message as morse code
void signalText(String text) {
  Serial.println();
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

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);


  Serial.begin(9600);
  while(!Serial.availableForWrite()){}

  Serial.print("C++ version: " );
  Serial.println(__cplusplus);
  //helloWorld();

}

void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(2000);
  //digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
  ///signalText("SOS");
  signalText("Janice Williams"); // .--- .- -. .. -.-. .

}
