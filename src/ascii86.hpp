#pragma once
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>

// PROTOTYPES
//std::string encode86(std::string data); //encodes a string of any length
//std::string decode86(std::string code); //decodes a string of any length
//std::string encode86_tuple(std::string tuple); //encodes a tuple (of 4 characters)
//std::string decode86_tuple(std::string tuple); //decodes a tuple (of 5 characters)

namespace cme {
  class Encode86 {
    private:
      // buffers for input/output
      std::string inbuf, outbuf;
      // position for outputting
      mutable int outpos;
      // whether the input is closed
      bool closed_input;

      void encode_tuple(std::string);
      void encode();
    public:
      // constructors
      Encode86();           // empty constructur
      Encode86(std::string);// fill up the input buffer with std::string

      // read encoded data
      char get() const;             // reads just one character
      std::string read() const;     // reads as much data as there is in the output buffer
      std::string data() const;     // reads the whole output buffer

      // write data to be encoded
      void put(char);           // write a single character
      void write(std::string);  // write some data

      // operator overloads
      Encode86& operator>>(std::string&);         // write output to string
      Encode86& operator<<(const std::string&);   // read from string

      // informative
      bool closed() const;// check if the input is closed (this needs to be done explicitly)
      void close();       // close the input
      void reset();       // reset everything
      void debug();
  };
}
