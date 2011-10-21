/*  File: deflate.cpp
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

#include "deflate.hpp"

namespace cmc {
  /* DEFLATE */
  // standard constructor
  Deflate::Deflate() {
    compressionlevel = Z_BEST_COMPRESSION; // set it up with the best compression level
    setup();
  }

  // constructor specifying the compression level
  Deflate::Deflate(int complvl) {
    compressionlevel = complvl; // set the specified compression level
    setup();
  }

  // destructor
  Deflate::~Deflate() {
    if(!closed_input) {
      close();  // if not closed already, do so (to deallocate some stuff)
    }
  }

  // get a single character
  char Deflate::get() const {
    if(outpos < output.size()) {
      ++outpos; // increase the position in the output stream
      return(output[outpos-1]); // return a character
    } else {
      return(0);  // if at end of stream, return NULL (this is why you should check for eol() before getting a character)
    }
  }

  // read as much data as there is in the output buffer
  std::string Deflate::read() const {
    // check if there's new data in the output buffer
    if((output.size() - outpos) > 0) {
      // if so, construct a string containing the new data
      std::string ret = output.substr(outpos, (output.size() - outpos));
      outpos = output.size(); // move the output pointer
      return(ret);            // and return the contructed string
    } else {
      return("");             // else, simply return an empty string
    }
  }

  // read as much data as there is in the output buffer
  std::string Deflate::dread() {
    std::string ret;
    // check if there's new data in the output buffer
    if((output.size() - outpos) > 0) {
      // if so, construct a string containing the new data
      ret = output.substr(outpos, (output.size() - outpos));
      outpos = output.size(); // move the output pointer
    } else {
      ret = "";
    }
    clearbuf();
    return(ret);
  }

  // return any and all data in the output buffer
  std::string Deflate::data() const {
    return(output);
  }

  // compress a string
  void Deflate::write(const std::string& str) {
    // make sure that the input isn't closed
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }

    // to capture the return value
    int ret;
    char output_buffer[output_buffer_size];

    // prepare the stream's input
    compress_stream.next_in = (unsigned char*)str.data();
    // tell zlib how much data to expect
    compress_stream.avail_in = str.size();

    // compress
    do {
      // tell it the address of the output buffer
      compress_stream.next_out = reinterpret_cast<unsigned char*>(output_buffer);
      // as well as it's size
      compress_stream.avail_out = output_buffer_size;

      // deflate some data
      ret = deflate(&compress_stream, Z_NO_FLUSH);

      // if there is new, inflated data in the output buffer,
      if(total_out < compress_stream.total_out) {
        // add it to the output string
        output.append(output_buffer, compress_stream.total_out - total_out);
        total_out = compress_stream.total_out;  // keep track of how many bytes were output so far
      }
    } while(ret == Z_OK); // repeat while the return value indicates that this should be done

    if (ret != Z_STREAM_END && ret != Z_BUF_ERROR) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression [write]: (" << ret << ") " << compress_stream.msg;
        throw(std::runtime_error(oss.str()));
    }
  }

  // add data (basically the same as write())
  Deflate& Deflate::operator<<(const std::string& str) {
    write(str);
    return(*this);
  }

  // fetch data
  Deflate& Deflate::operator>>(std::string& str) {
    str += read();
    return(*this);
  }

  void Deflate::clearbuf() {
    output.clear();
    outpos = 0;
  }

  // check if the input is closed
  bool Deflate::closed() {
    return(closed_input);
  }

  // close the input
  void Deflate::close() {
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }
    closed_input = true;  // indicate this via a variable
    // to store the return value
    int ret;
    char output_buffer[output_buffer_size];

    // finish up the compression
    compress_stream.next_in = NULL; // no more stuff to compress
    compress_stream.avail_in = 0;

    do {
      compress_stream.next_out = reinterpret_cast<unsigned char*>(output_buffer);
      compress_stream.avail_out = output_buffer_size;

      ret = deflate(&compress_stream, Z_FINISH);  // Z_FINISH indicates that this is the last data to be compressed

      // add any remaining stuff from the flush to the output string
      if(total_out < compress_stream.total_out) {
        output.append(output_buffer, compress_stream.total_out - total_out);
        total_out = compress_stream.total_out;  // keep track of byte output
      }
    } while(ret == Z_OK);

    if (ret != Z_STREAM_END && ret != Z_BUF_ERROR) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib compression [close]: (" << ret << ") " << compress_stream.msg;
        throw(std::runtime_error(oss.str()));
    }

    // this deallocates some stuff
    deflateEnd(&compress_stream);
  }

  // setup the compression
  void Deflate::setup() {
    closed_input = false;
    output.clear();

    // set all to zero
    memset(&compress_stream, 0, sizeof(compress_stream));
    // initialize data
    int ret = deflateInit(&compress_stream, compressionlevel);
    if(ret != Z_OK) {
      std::ostringstream oss;
      oss << "deflateInit failed while decompressing, error code: " << ret;
      if(compress_stream.msg != NULL) {
        oss << ", mesg: " << compress_stream.msg;
      }
      oss << ", cpmlvl: " << compressionlevel;
      throw std::runtime_error(oss.str());
    }

    outpos = 0;
    total_out = 0;
  }

  void Deflate::reset() {
    if(!closed_input) {
      close();
    }
    setup();
  }

  // check for end of output (useful for use with get())
  bool Deflate::eol() {
    return(outpos >= output.size());
  }

  /* INFLATE */
  // standard constructor
  Inflate::Inflate() {
    setup();
  }

  // destructor
  Inflate::~Inflate() {
    if(!closed_input) {
      close();  // if not closed already, do so (to deallocate some stuff)
    }
  }

  // get a single character
  char Inflate::get() const {
    if(outpos < output.size()) {
      ++outpos; // increase the position in the output stream
      return(output[outpos-1]); // return a character
    } else {
      return(0);  // if at end of stream, return NULL (this is why you should check for eol() before getting a character)
    }
  }

  // read as much data as there is in the output buffer
  std::string Inflate::read() const {
    // check if there's new data in the output buffer
    if((output.size() - outpos) > 0) {
      // if so, construct a string containing the new data
      std::string ret = output.substr(outpos, (output.size() - outpos));
      outpos = output.size(); // move the output pointer
      return(ret);            // and return the contructed string
    } else {
      return("");             // else, simply return an empty string
    }
  }

  // read as much data as there is in the output buffer and clear the latter
  std::string Inflate::dread() {
    std::string ret;
    // check if there's new data in the output buffer
    if((output.size() - outpos) > 0) {
      // if so, construct a string containing the new data
      ret = output.substr(outpos, (output.size() - outpos));
      outpos = output.size(); // move the output pointer
    } else {
      ret = "";
    }
    clearbuf();
    return(ret);
  }

  // return any and all data in the output buffer
  std::string Inflate::data() const {
    return(output);
  }

  // decompress a string
  void Inflate::write(const std::string& str) {
    // make sure that the input isn't closed
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }

    // to capture the return value
    int ret;
    char output_buffer[output_buffer_size];

    // prepare the stream's input
    decompress_stream.next_in = (unsigned char*)str.data();
    // tell zlib how much data to expect
    decompress_stream.avail_in = str.size();

    // compress
    do {
      // tell it the address of the output buffer
      decompress_stream.next_out = reinterpret_cast<unsigned char*>(output_buffer);
      // as well as it's size
      decompress_stream.avail_out = output_buffer_size;

      // inflate some data
      ret = inflate(&decompress_stream, Z_NO_FLUSH);

      // if there is new, inflated data in the output buffer,
      if(total_out < decompress_stream.total_out) {
        // add it to the output string
        output.append(output_buffer, decompress_stream.total_out - total_out);
        total_out = decompress_stream.total_out;  // keep track of how many bytes were output
      }
    } while(ret == Z_OK); // repeat while the return value indicates that this should be done


    if (ret != Z_STREAM_END && ret != Z_BUF_ERROR) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression [write]: (" << ret << ") " << decompress_stream.msg;
        throw(std::runtime_error(oss.str()));
    }
  }

  // add data (basically the same as write())
  Inflate& Inflate::operator<<(const std::string& str) {
    write(str);
    return(*this);
  }

  // fetch data
  Inflate& Inflate::operator>>(std::string& str) {
    str += read();
    return(*this);
  }

  void Inflate::clearbuf() {
    output.clear();
    outpos = 0;
  }

  // check if the input is closed
  bool Inflate::closed() {
    return(closed_input);
  }

  // close the input
  void Inflate::close() {
    if(closed_input) {
      throw std::runtime_error("already closed input");
    }
    closed_input = true;  // indicate this via a variable
    // to store the return value
    int ret;
    char output_buffer[output_buffer_size];

    // finish up the decompression
    decompress_stream.next_in = NULL; // no more stuff to compress
    decompress_stream.avail_in = 0;

    do {
      decompress_stream.next_out = reinterpret_cast<unsigned char*>(output_buffer);
      decompress_stream.avail_out = output_buffer_size;

      ret = inflate(&decompress_stream, Z_FINISH);  // Z_FINISH indicates that this is the last data to be compressed

      // add any remaining stuff from the flush to the output string
      if(total_out < decompress_stream.total_out) {
        output.append(output_buffer, decompress_stream.total_out - total_out);
        total_out = decompress_stream.total_out;  // keep track of how many bytes output yet
      }
    } while(ret == Z_OK);


    if (ret != Z_STREAM_END && ret != Z_BUF_ERROR) {          // an error occurred that was not EOF
        std::ostringstream oss;
        oss << "Exception during zlib decompression [close]: (" << ret << ") " << decompress_stream.msg;
        throw(std::runtime_error(oss.str()));
    }

    // this deallocates some stuff
    inflateEnd(&decompress_stream);
  }

  // setup the decompression
  void Inflate::setup() {
    closed_input = false;
    output.clear();

    // set all to zero
    memset(&decompress_stream, 0, sizeof(decompress_stream));
    // initialize data
    if(inflateInit(&decompress_stream) != Z_OK) {
      throw std::runtime_error("inflateInit failed while decompressing.");
    }

    outpos = 0;
    total_out = 0;
  }

  void Inflate::reset() {
    if(!closed_input) {
      close();
    }
    setup();
  }

  // check for end of output (useful for use with get())
  bool Inflate::eol() {
    return(outpos >= output.size());
  }

}

