/*  File: deflate.hpp
 *  
 *  Copyright (C) 2011, Patrick M. Elsen
 *
 *  This file is part of CMatrixCrypt (http://github.com/xfbs/CMatrixCrypt)
 *  Author: Patrick M. Elsen <pelsen.vn (a) gmail.com>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#pragma once
#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <zlib.h>

namespace cmc {
  class Deflate {
    private:
      static const size_t output_buffer_size = 10000; // keep track of how big the buffer is
      //char output_buffer[output_buffer_size];       // buffer to read zlib's output from
      std::string output;                           	// where to store the output

      z_stream compress_stream; // zlib compression stream
      bool closed_input;        // whether or not this is closed
      int compressionlevel;     // determines how good the compression should be
      mutable size_t outpos;    // where we are in the outputting
      unsigned long total_out;  // keep track of how many bytes of output there were

      void setup();   // set the compression up
    public:
      Deflate();                // default
      explicit Deflate(int);    // specific compression level
      ~Deflate();               // destructor

      // OUTPUT FUNCTIONS
      char get() const;         // read a single character
      std::string read() const; // read new data
      std::string data() const; // read all data
      std::string dread();      // read new data & clear output buffer

      // INPUT FUNCTIONS
      void write(const std::string&);  // write a string

      // OPERATORS
      Deflate& operator<<(const std::string&);  // add data
      Deflate& operator>>(std::string&);        // get data

      // MISC
      void clearbuf();// clear the output buffer & outpos
      bool closed();  // check if the input is closed
      void close();   // close the input
      void reset();   // close input and then set it up again
      bool eol();     // check if the output is closed
  };

  class Inflate {
    private:
      static const size_t output_buffer_size = 10000; // keep track of how big the buffer is
      //char output_buffer[output_buffer_size];       // buffer to read zlib's output from
      std::string output;                           	// where to store the output

      z_stream decompress_stream; // zlib decompression stream
      bool closed_input;          // whether or not this is closed
      mutable size_t outpos;      // where we are in the outputting
      unsigned long total_out;    // keep track of how many bytes of output there were

      void setup();   // set the decompression up
    public:
      Inflate();                // default
      ~Inflate();               // destructor

      // OUTPUT FUNCTIONS
      char get() const;         // read a single character
      std::string read() const; // read new data
      std::string data() const; // read all data
      std::string dread();      // read all new data & clear the output buffer

      // INPUT FUNCTIONS
      void write(const std::string&);  // write a string

      // OPERATORS
      Inflate& operator<<(const std::string&);  // add data
      Inflate& operator>>(std::string&);        // get data

      // MISC
      void clearbuf();// clear the output buffer
      bool closed();  // check if the input is closed
      void close();   // close the input
      void reset();   // close input and then set it up again
      bool eol();     // check if the output is closed
  };
}
