/*  File: util.hpp
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

// PROTOTYPES
namespace cme {
  std::string intToBin(unsigned);         // turns 'num' into a binary string
  std::string intToBin(unsigned, int);    // turns 'num' into a binary string of size 'size'. 
  unsigned binToInt(const std::string&);  // turns binary string 'bin' into an unsigned integer.

  std::string padLeft (const std::string&, const size_t, const char); // pads string str to length num with char paddingChar
  std::string padRight(const std::string&, const size_t, const char); // does the same but adds the paddingChar to the right size (end of string)
}
