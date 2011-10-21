/*  File: ascii86.cpp
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

#include "ascii86.hpp"

namespace cmc {
  /* ENCODE86 CODE */
  /* CONSTRUCTORS */
  Encode86::Encode86() {
    reset();
  }

  Encode86::Encode86(std::string data) {
    reset();
    write(data);
  }

  /* ENCODING FACILITIES */
  void Encode86::encode_tuple(const std::string& tuple) {
    std::string enc = "";      // encoded string
    long int int_rep = 0; // integer representation of the string, this is a LONG int so that it's min. 32bit

    long int multiplier = 256*256*256;                // turn the tuple into an int, this is, too, a LONG int
    for(int i = 0; i < tuple.length(); i++) {         // loop thru the characters
      int_rep += multiplier * (unsigned char)tuple[i];// add the byte to the int_representation. coerce this to unsigned char because 
      multiplier /= 256;                              // increase the multiplier
    }

    unsigned char chr;           // store current character
    for(int i = 0; i < 5; i++) { // loop 5 times
      chr = (int_rep%85)+33;     // extract a character
      enc.insert(0, 1, chr);     // insert the character at the beginning (the extraction process is in reverse)
      int_rep /= 85;             // prime the int for the next extraction
    }

    outbuf += enc;  // add the encoded stuff to the outbuffer
  }

  void Encode86::encode() {
    if(!closed_input) {
      int amount = inbuf.size()/4;
      int pos;
      for(pos = 0; pos < amount; ++pos) {
        encode_tuple(inbuf.substr(pos*4, 4));
      }
      inbuf.erase(0, amount*4);
    }
  }

  /* OUTPUT FACILITIES */
  // get a single character
  char Encode86::get() const {
    if(outpos < outbuf.size()) {
      ++outpos;
      return(outbuf[outpos-1]);
    } else {
      return(0);
    }
  }

  // read as much data as there is
  std::string Encode86::read() const {
    if((outbuf.size() - outpos) > 0) {
      std::string ret = outbuf.substr(outpos, (outbuf.size() - outpos));
      outpos = outbuf.size();
      return(ret);
    } else {
      return("");
    }
  }

  // read as much data as there is
  std::string Encode86::dread() {
    std::string ret;
    if((outbuf.size() - outpos) > 0) {
      ret = outbuf.substr(outpos, (outbuf.size() - outpos));
      //outpos = outbuf.size(); // no need since we reset this anyway
    } else {
      //ret = ""; // no need cus that's what string start out with anyways
    }
    clearbuf(); // reset output buffer & position
    return(ret);
  }

  std::string Encode86::data() const {
    return(outbuf);
  }

  /* INPUT FACILITIES */
  // push a single character
  void Encode86::put(char chr) {
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }
    inbuf.push_back(chr);
    encode(); // tell the class to try to encode the data we got
  }

  void Encode86::write(std::string data) {
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }
    inbuf += data;
    encode();
  }

  /* OPERATOR OVERLOADS */
  // write to a string
  Encode86& Encode86::operator>>(std::string& out) {
    out += read();
    return(*this);
  }

  Encode86& Encode86::operator<<(const std::string& in) {
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }
    write(in);
    encode();
    return(*this);
  }

  /* OTHER STUFF */
  // check if the input is closed
  bool Encode86::closed() const {
    return(closed_input);
  }

  // close the input
  void Encode86::close() {
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }

    encode();             // encode the rest of the stuff in the input buffer
    closed_input = true;  // prevent new input from coming in
    
    int appendixes = (inbuf.size() % 4);  // find out how many characters are missing
    if(appendixes != 0) {
      for(int i = 0; i < (4-appendixes); ++i) { 
        inbuf.push_back('\0');              // add the null characters
      }

      // encode the last four characters
      encode_tuple(inbuf.substr(inbuf.size()-4, 4));

      for(int i = 0; i < (4-appendixes); ++i) { // remove the unneeded characters
        outbuf.erase(outbuf.size()-1, 1);
      }
    }

    // reset inbuf
    inbuf = "";

    // finish outbuf
    outbuf += "~>";
  }

  void Encode86::reset() {
    closed_input = false;
    inbuf = "";
    outbuf = "<~";
    outpos = 0;
  }

  bool Encode86::eol() {
    return(outpos >= outbuf.size());
  }

  void Encode86::clearbuf() {
    outpos = 0;
    outbuf.clear();
  }

  void Encode86::debug() {
    std::cout << "inbuf: " << inbuf << std::endl;
    std::cout << "outbuf: " << outbuf << std::endl;
    std::cout << "outpos: " << outpos << std::endl;
    std::cout << "closed: " << closed_input << std::endl;
  }

  /* DECODE86 CODE */
  /* CONSTRUCTORS */
  Decode86::Decode86() {
    reset();
  }

  Decode86::Decode86(std::string data) {
    reset();
    write(data);
  }

  /* DECODING FACILITIES */
  void Decode86::decode_tuple(const std::string& tuple) {
    std::string dec = "";      // decoded string
    long int int_rep = 0; // integer representation of the tuple, long int so that it's at least 32bit

    long int multiplier = 85*85*85*85;        // turn the tuple into an int, long so that it's at least 32bit
    for(int i = 0; i < tuple.length(); i++) { // loop thru the characters
      int_rep += (((unsigned char)tuple[i])-33) * multiplier; // add the byte to the int_representation, subtracting 33
                                                              // coerced to unsigned char to correctly multiply chars >128
      multiplier /= 85;                       // increase the multiplier
    }

    unsigned char chr;           // store current unsigned character
    for(int i = 0; i < 4; i++) { // loop 4 times
      chr = (int_rep%256);       // extract a character
      dec.insert(0, 1, chr);     // insert the character at the beginning (the extraction process is in reverse)
      int_rep /= 256;            // prime the int for the next extraction
    }

    outbuf += dec;
  }

  void Decode86::decode() {
    // start_decoding indicates wheather <~ has been met yet (start of encoded stream)
    if(!start_decoding) {
      int pos = inbuf.find("<~", 0);    // try to find <~
      if(pos != std::string::npos) {    // if it's found
        inbuf.erase(0, pos+2);          // erase everything before it and itself
        start_decoding = true;          // and start decoding
      } else {
        inbuf.erase(0, inbuf.size()-1); // else, ignore the stuff and move on
      }
    }

    // let's try to find the end of the input
    int end = inbuf.find("~>", 0);
    if(end != std::string::npos) {        // in case we do find it,
      inbuf.erase(end, inbuf.size()-end); // erase it and all the crap after it
    }

    if(inbuf.size() > 5 or end!=std::string::npos) {
      int amount = inbuf.size()/5;                // how many tuples need to be decoded?
      outbuf.reserve(outbuf.size() + (amount*4)); // make space in outbuf
      int pos;
      for(pos = 0; pos < amount; ++pos) {
        decode_tuple(inbuf.substr(pos*5, 5));
      }
      inbuf.erase(0, amount*5);
    }

    if(end != std::string::npos) {  // if we found the end, 
      close();                      // close the input
    }
  }

  /* OUTPUT FACILITIES */
  // get a single character
  char Decode86::get() const {
    if(outpos < outbuf.size()) {
      ++outpos;
      return(outbuf[outpos-1]);
    } else {
      return(0);
    }
  }

  // read as much data as there is
  std::string Decode86::read() const {
    if((outbuf.size() - outpos) > 0) {
      std::string ret = outbuf.substr(outpos, (outbuf.size() - outpos));
      outpos = outbuf.size();
      return(ret);
    } else {
      return("");
    }
  }

  // read as much data as there is
  std::string Decode86::dread() {
    std::string ret;
    if((outbuf.size() - outpos) > 0) {
      ret = outbuf.substr(outpos, (outbuf.size() - outpos));
      //outpos = outbuf.size(); // no need sice it's reset anyway
    } else {
      //ret = ""; // no need since that's what string start out with anyway
    }
    clearbuf(); // clear the output buffer & output position in the buffer
    return(ret);
  }

  std::string Decode86::data() const {
    return(outbuf);
  }

  /* INPUT FACILITIES */
  // push a single character
  void Decode86::put(char chr) {
    if(closed_input) {
      throw std::runtime_error("input already closed");
    }
    inbuf.push_back(chr);
    decode(); // tell the class to try to encode the data we got
  }

  void Decode86::write(std::string data) {
    if(closed_input) {
      throw std::runtime_error("input already closed");
    }
    inbuf.reserve(inbuf.size() + data.size());
    for(int i = 0; i < data.size(); ++i) {
      if((32 < data[i] && data[i] < 118) || (data[i] == '~')) {
        inbuf.push_back(data[i]);
      }
    }
    //inbuf += data;
    decode();
  }

  /* OPERATOR OVERLOADS */
  // write to a string
  Decode86& Decode86::operator>>(std::string& out) {
    out += read();
    return(*this);
  }

  Decode86& Decode86::operator<<(const std::string& in) {
    if(closed_input) {
      throw std::runtime_error("input already closed");
    }
    write(in);
    decode();
    return(*this);
  }

  /* OTHER STUFF */
  // check if the input is closed
  bool Decode86::closed() const {
    return(closed_input);
  }

  // close the input
  void Decode86::close() {
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }

    decode();             // encode the rest of the stuff in the input buffer
    closed_input = true;  // prevent new input from coming in
    
    int appendixes = (inbuf.size() % 5);  // find out how many characters are missing
    if(appendixes != 0) {
      for(int i = 0; i < (5-appendixes); ++i) { 
        inbuf.push_back('u');              // add the characters
      }

      // encode the last four characters
      decode_tuple(inbuf.substr(inbuf.size()-5, 5));

      for(int i = 0; i < (5-appendixes); ++i) { // remove the unneeded characters
        outbuf.erase(outbuf.size()-1, 1);
      }
    }

    // reset inbuf
    inbuf = "";
  }

  bool Decode86::eol() {
    return(outpos >= outbuf.size());
  }

  void Decode86::reset() {
    closed_input = false;
    start_decoding = false;
    inbuf = "";
    outbuf = "";
    outpos = 0;
  }

  void Decode86::clearbuf() {
    outpos = 0;
    outbuf.clear();
  }

  void Decode86::debug() {
    std::cout << "inbuf: " << inbuf << std::endl;
    std::cout << "outbuf: " << outbuf << std::endl;
    std::cout << "outpos: " << outpos << std::endl;
    std::cout << "closed: " << closed_input << std::endl;
  }
}
