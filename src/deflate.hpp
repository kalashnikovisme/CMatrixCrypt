#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <zlib.h>

//std::string compress_string(const std::string& str, int compressionlevel /*= Z_BEST_COMPRESSION*/);
//std::string decompress_string(const std::string& str);

namespace cme {
  class Deflate {
    private:
      static const int output_buffer_size = 10000;  // keep track of how big the buffer is
      //char output_buffer[output_buffer_size];       // buffer to read zlib's output from
      std::string output;                           // where to store the output

      z_stream compress_stream; // zlib compression stream
      bool closed_input;        // whether or not this is closed
      int compressionlevel;     // determines how good the compression should be
      mutable int outpos;       // where we are in the outputting

      void setup();   // set the compression up
    public:
      Deflate();                // default
      Deflate(int);             // specific compression level
      ~Deflate();               // destructor

      // OUTPUT FUNCTIONS
      char get() const;         // read a single character
      std::string read() const; // read new data
      std::string data() const; // read all data

      // INPUT FUNCTIONS
      void write(const std::string&);  // write a string

      // OPERATORS
      Deflate& operator<<(const std::string&);  // add data
      Deflate& operator>>(std::string&);        // get data

      // MISC
      bool closed();  // check if the input is closed
      void close();   // close the input
      void reset();   // close input and then set it up again
      bool eol();     // check if the output is closed
  };

  class Inflate {
    private:
      static const int output_buffer_size = 10000;  // keep track of how big the buffer is
      //char output_buffer[output_buffer_size];       // buffer to read zlib's output from
      std::string output;                           // where to store the output

      z_stream decompress_stream; // zlib decompression stream
      bool closed_input;          // whether or not this is closed
      mutable int outpos;         // where we are in the outputting

      void setup();   // set the decompression up
    public:
      Inflate();                // default
      ~Inflate();               // destructor

      // OUTPUT FUNCTIONS
      char get() const;         // read a single character
      std::string read() const; // read new data
      std::string data() const; // read all data

      // INPUT FUNCTIONS
      void write(const std::string&);  // write a string

      // OPERATORS
      Inflate& operator<<(const std::string&);  // add data
      Inflate& operator>>(std::string&);        // get data

      // MISC
      bool closed();  // check if the input is closed
      void close();   // close the input
      void reset();   // close input and then set it up again
      bool eol();     // check if the output is closed
  };
}
