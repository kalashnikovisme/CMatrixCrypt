#pragma once

#include <string.h>
#include <bitset>
using namespace std;

// PROTOTYPES
string intToBinString(unsigned num);            // turns 'num' into a binary string
string intToBinString(unsigned num, int size);  // turns 'num' into a binary string of size 'size'. 
unsigned binStringToInt(string bin);            // turns binary string 'bin' into an unsigned integer.

void padLeft (string &str, const size_t num, const char paddingChar); // pads string str to length num with char paddingChar
void padRight(string &str, const size_t num, const char paddingChar); // does the same but adds the paddingChar to the right size (end of string)
