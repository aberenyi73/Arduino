/**
  Blink SOS signals


*/

#define NUMELEMENTS(x) (sizeof(x) / sizeof(x[0]))

// define keys for morse codes "hashtable"
#define __0__ 0
#define __1__ 1
#define __2__ 2
#define __3__ 3
#define __4__ 4
#define __5__ 5
#define __6__ 6
#define __7__ 7
#define __8__ 8
#define __9__ 9
#define __A__ 10
#define __B__ 11
#define __C__ 12
#define __D__ 13
#define __E__ 14
#define __F__ 15
#define __G__ 16
#define __H__ 17
#define __I__ 18
#define __J__ 19
#define __K__ 20
#define __L__ 21
#define __M__ 22
#define __N__ 23
#define __O__ 24
#define __P__ 25
#define __Q__ 26
#define __R__ 27
#define __S__ 28
#define __T__ 29
#define __U__ 30
#define __V__ 31
#define __W__ 32
#define __X__ 33
#define __Y__ 34
#define __Z__ 35

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
  byte len;
  char *code;
  char *letter;
};


// MORSE CODES, dit/dah array plus length
MORSE_CODE mc__0 = {5, "-----", "0"};
MORSE_CODE mc__1 = {5, ".----", "1"};
MORSE_CODE mc__2 = {5, "..---", "2"};
MORSE_CODE mc__3 = {5, "...--", "3"};
MORSE_CODE mc__4 = {5, "....-", "4"};
MORSE_CODE mc__5 = {5, ".....", "5"};
MORSE_CODE mc__6 = {5, "-....", "6"};
MORSE_CODE mc__7 = {5, "--...", "7"};
MORSE_CODE mc__8 = {5, "---..", "8"};
MORSE_CODE mc__9 = {5, "----.", "9"};
MORSE_CODE mc__A = {2, ".-", "A"};
MORSE_CODE mc__B = {4, "-...", "B"};
MORSE_CODE mc__C = {4, "-.-.", "C"};
MORSE_CODE mc__D = {3, "-..", "D"};
MORSE_CODE mc__E = {1, ".", "E"};
MORSE_CODE mc__F = {4, "..-.", "F"};
MORSE_CODE mc__G = {3, "--.", "G"};
MORSE_CODE mc__H = {4, "....", "H"};
MORSE_CODE mc__I = {2, "..", "I"};
MORSE_CODE mc__J = {4, ".---", "J"};
MORSE_CODE mc__K = {3, "-.-", "K"};
MORSE_CODE mc__L = {4, ".-..", "L"};
MORSE_CODE mc__M = {2, "--", "M"};
MORSE_CODE mc__N = {2, "-.", "N"};
MORSE_CODE mc__O = {3, "---", "O"};
MORSE_CODE mc__P = {4, ".--.", "P"};
MORSE_CODE mc__Q = {4, "--.-", "Q"};
MORSE_CODE mc__R = {3, ".-.", "R"};
MORSE_CODE mc__S = {3, "...", "S"};
MORSE_CODE mc__T = {1, "-", "T"};
MORSE_CODE mc__U = {3, "..-", "U"};
MORSE_CODE mc__V = {4, "...-", "V"};
MORSE_CODE mc__W = {3, ".--", "W"};
MORSE_CODE mc__X = {4, "-..-", "X"};
MORSE_CODE mc__Y = {4, "-.--", "Y"};
MORSE_CODE mc__Z = {4, "--..", "Z"};

// "hashtable" of morse codes
MORSE_CODE MORSE_CODES[36] = {mc__0, mc__1, mc__2, mc__3, mc__4, mc__5, mc__6, mc__7, mc__8, mc__9, 
                              mc__A, mc__B, mc__C, mc__D, mc__E, mc__F, mc__G, mc__H, mc__I, mc__J, 
                              mc__K, mc__L, mc__M, mc__N, mc__O, mc__P, mc__Q, mc__R, mc__S, mc__T, 
                              mc__U, mc__V, mc__W, mc__X, mc__Y, mc__Z};


#endif MORSE_H

void printDebug(const char text[]) {
  //return;
  Serial.println(text);
}

MORSE_CODE sos[] = {mc__S, mc__O, mc__S};
size_t sosLen = NUMELEMENTS(sos);

/**
 * Hash function to get the morse code from a letter. Return the code.
 * @param letter only upper case letters accepted.
 * @return The MORSE_CODE of the letter.
*/

MORSE_CODE * lookupMorseCode(char letter) {

  if(letter >=48 && letter <=48)  // A == 65
    return & MORSE_CODES[letter - 48]; // numbers
  else if (letter >= 65 && letter <= 90)
    return & MORSE_CODES[letter - 55]; // letters
}

// print out the single code letter, with spaces between dots
void printMorseLetter(MORSE_CODE code) {
    
    String b = code.letter;
    //Serial.print(code.code);
    char val = 'A';
    Serial.print(val, DEC);

    bool first = true;
    for(int i = 0; i < code.len; i++) {
      if(first) {
        first = false;
      } else {
        // SPACE_LENGTH
        Serial.print(" ");
        b += " ";
      }
      b += String(code.code[i]);
    }
    Serial.print(b);
}

void printLetterSeparator() {
    Serial.print("   ");    
}

void printWordSeparator() {
    Serial.print("       ");    
}


void printABC() {
    // print to the Serial port 
    char letz[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

    Serial.print("{");
    for(int i = 0; i < 36; i++) {
      Serial.print(String(letz[i]));
      Serial.println(letz[i], DEC);

      //String d = "MORSE_CODE " + String(letz[i]) + " = {, \"\", \"" + String(letz[i]) + "\"};";  
      //if(i) Serial.print(", ");    
      //String d = "mc__" + String(letz[i]);

      //Serial.print(d);
    }

    Serial.println("}");
}

void helloWorld() {
  char hello[] = "HELLO WORLD";
  Serial.println(hello);

  bool startWord = true;
  
  for(int i = 0; i < NUMELEMENTS(hello); i++) {
  //for(int i = 0; i < 11; i++) {
    
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

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);

  Serial.begin(9600);
  while(!Serial.availableForWrite()){}

  Serial.print("C++ version: " );
  Serial.println(__cplusplus);
  //printABC();  
  helloWorld();

}

/////////////////////////////////////////
/////// SIGNALING CODE //////////////////

// Light up the diode for dit time
void signalDit() {
  printDebug("signalDit()");

  digitalWrite(LED_BUILTIN, HIGH);
  delay(DIT_LENGTH);
  digitalWrite(LED_BUILTIN, LOW);
}

// Light up the diode for dah time
void signalDah() {
  printDebug("signalDah()");

  digitalWrite(LED_BUILTIN, HIGH);
  delay(DAH_LENGTH);
  digitalWrite(LED_BUILTIN, LOW);
}

// pause signal for DIT_LENGHT, between dits and dahs
void signalSpace() {
  printDebug("signalSpace()");

  delay(DIT_LENGTH);
}

// pause 
void signalLetterSep() {
  printDebug("signalLetterSep()");

  delay(LETTER_SEP_LENGTH);
}

void signalWordSep() {
  printDebug("signalWordSep()");

  delay(WORD_SEP_LENGTH);
}

// signal a single code.
// @param mCode for example ".---"
void signalCode(MORSE_CODE* mCode) {
    printDebug("signalCode() ".ctrcat((*mCode).code));

  for(int i = 0; i < (*mCode).len; i++) {
      Serial.print("  signaling i: ");
      Serial.println(i);
      
      if(i>0) signalSpace();
  
      if((*mCode).code[i] == '.') 
        signalDit();
      else 
        signalDah();

  }
}

// signal a message as morse code
void signalText(String text) {

  Serial.print("Begin message: ");
  Serial.println(text);

  bool startWord = true;
  
  for(int i = 0; i < text.length(); i++) {
      Serial.println("signalLetter: " + String(text[i]) + " ");

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
        Serial.print("morse code: ");
        Serial.println((*code).letter);

        signalCode(code);
        
      }

  }
  Serial.println("End message");

}

/////// END SIGNALING CODE //////////////
/////////////////////////////////////////


void loop() {
  // put your main code here, to run repeatedly:
  //digitalWrite(LED_BUILTIN, HIGH);
  //delay(2000);
  //digitalWrite(LED_BUILTIN, LOW);
  delay(3000);
  signalText("SOS");

}
