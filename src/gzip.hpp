#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <zlib.h>

std::string compress_string(const std::string& str, int compressionlevel /*= Z_BEST_COMPRESSION*/);
std::string decompress_string(const std::string& str);

namespace cme {
  class Deflate {
    private:
      char **output_buffer;   // buffer to read zlib's output from
      int output_buffer_size; // keep track of how big the buffer is
      std::string output;     // where to store the output

      z_stream compress_stream; // zlib compression stream
      bool closed;              // whether or not this is closed
      mutable int outpos;       // where we are in the outputting
    public:
      Deflate();                // default
      Deflate(int);             // specific bufsize
      Deflate(std::string);     // string to start deflating
      Deflate(int, std::string);// specific bufsize and string to start deflating
      ~Deflate();               // destructor

      // OUTPUT FUNCTIONS
      char get() const;         // read a single character
      std::string read() const; // read new data
      std::string data() const; // read all data

      // INPUT FUNCTIONS
      void put(char);           // write a single character
      void write(std::string);  // write a string

      // OPERATORS
      Deflate& operator<<(const std::string&);
      Deflate& operator>>(std::string&) const;

      // MISC
      bool closed();
      void close();
      void setup();
      void reset();
      bool eol();
  }
}
