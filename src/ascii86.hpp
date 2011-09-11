/*  File: ascii86.hpp
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
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <stdexcept>

namespace cme {
  class Encode86 {
    private:
      // buffers for input/output
      std::string inbuf, outbuf;
      // position for outputting
      mutable int outpos;
      // whether the input is closed
      bool closed_input;

      void encode_tuple(const std::string&);
      void encode();
    public:
      // constructors
      Encode86();           // empty constructur
      Encode86(std::string);// fill up the input buffer with std::string

      // read encoded data
      char get() const;             // reads just one character
      std::string read() const;     // reads as much data as there is in the output buffer
      std::string dread();          // reads as much data as there is in the output buffer and then clears the output buffer
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
      void clearbuf();
      bool eol();         // is the output buffer exhausted?
  };

  class Decode86 {
    private:
      // buffers for i/o
      std::string inbuf, outbuf;
      // position in the outbuf
      mutable int outpos;
      // closed input
      bool closed_input;
      bool start_decoding;

      // decoding functions
      void decode_tuple(const std::string&);
      void decode();
      void close();
    public:
      // constructors
      Decode86();
      Decode86(std::string);

      // reading facilities
      char get() const;
      std::string read() const;
      std::string dread();      // reads from output buffer and then clears it
      std::string data() const;

      // input facilities
      void put(char);
      void write(std::string);

      // operators
      Decode86& operator>>(std::string&);
      Decode86& operator<<(const std::string&);

      // others
      bool closed() const;
      void reset();
      void debug();
      bool eol();   // has the output reached end of line yet?
      void clearbuf();
  };
}
