#pragma once
#include <string>

// PROTOTYPES
namespace cme {
  std::string intToBin(unsigned);         // turns 'num' into a binary string
  std::string intToBin(unsigned, int);    // turns 'num' into a binary string of size 'size'. 
  unsigned binToInt(const std::string&);  // turns binary string 'bin' into an unsigned integer.

  std::string padLeft (const std::string&, const size_t, const char); // pads string str to length num with char paddingChar
  std::string padRight(const std::string&, const size_t, const char); // does the same but adds the paddingChar to the right size (end of string)
}
