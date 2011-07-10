#include "util.hpp"

// IMPLEMENTATIONS
string intToBinString(unsigned num) {
  string binaryString;                    // a place to store the binary string
  char currentBit;                        // the current bit stored as char (either '0' or '1')

  while(num!=0) {                         // convert until 'num' becomes 0
    currentBit = '0' + (num%2);           // get the current bit (either '0'+0 == '0') or ('0'+1 = '1')
    binaryString.insert(0,1,currentBit);  // prepend the current bit to the string (insert at pos 0)
    num = num/2;                          // prime 'num' for the next iteration
  }

  return(binaryString);                   // return the binary string
}

string intToBinString(unsigned num, int size) { // same as the def above but different while loop
  string binaryString;                    // a place to store the binary string
  char currentBit;                        // the current bit stored as char (either '0' or '1')

  while(size>0) {                         // convert until 'size'  becomes 0
    currentBit = '0' + (num%2);           // get the current bit (either '0'+0 == '0') or ('0'+1 = '1')
    binaryString.insert(0,1,currentBit);  // prepend the current bit to the string (insert at pos 0)
    num = num/2;                          // prime 'num' for the next iteration
    size--;                               // decrement 'size' once per interation
  }

  return(binaryString);                   // return the binary string
}

unsigned binStringToInt(string bin) {   // converts a binary string to an unsigned int
  int mult = 1;                         // multiplier (significance of current pos into the binary string)
  unsigned total = 0;                   // result

  for(int i = bin.size()-1; i>=0; i--) {// loop thru string's characters
    if(bin[i] == '1')                   // do somethin if there's a 1 in the current pos (0s are irrelevant)
      total += mult;                    // add current pos significance to the result if needed
                                        // FIXME: complain if string contains bad characters!
    mult *= 2;                          // update mult
  }

  return(total);
}

void padLeft(std::string &str, const size_t num, const char paddingChar = ' ') {
  if(num > str.size())
    str.insert(0, num - str.size(), paddingChar);
}

void padRight(std::string &str, const size_t num, const char paddingChar = ' ') {
  if(num > str.size())
    str.insert(str.size(), num - str.size(), paddingChar);
}
