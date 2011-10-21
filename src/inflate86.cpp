/*  File: inflate86.cpp
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

#include <string>
#include <iostream>
#include <fstream>
#include "deflate.hpp"
#include "ascii86.hpp"

using namespace std;

// decompress a file
void read_file(cmc::Inflate&, cmc::Decode86&, string filename);
// decompress stdin
void read_input(cmc::Inflate&, cmc::Decode86&);

int main(int argc, char *argv[]) {
  // object to inflate (decompress)
  cmc::Inflate inflate;
  // decode object
  cmc::Decode86 decode;

  // if there is no command line argument
  if(argc < 2) {
    // read from stdin
    read_input(inflate, decode);
  } else {
    // if there is, store it in a string
    string file(argv[1]);
    // check if it's -
    if(file == "-") {
      // if so, read from stdin
      read_input(inflate, decode);
    } else {
      // else read from file
      read_file(inflate, decode, file);
    }
  }

  // decode closes automagically when it has detected end of input
  if(!decode.closed()) {
    // when it isn't closed yet, complain about missing data
    cerr << "not enough input" << endl;
  }

  // after we're done inflating, close the stream (and flush it)
  inflate.close();
  // output it to stdout
  cout << inflate.dread();
  // and DON'T write a newline char [cuz that ain't part of the original file]
  //cout << endl;
}

// read from file
void read_file(cmc::Inflate& inflate, cmc::Decode86& decode, string filename) {
  // use an ifstream to read (easier to use)
  ifstream input(filename.data());
  // temp var to hold a single line
  string line;

  // if there is an error with the ifstream
  if(!input.good()) {
    // tell the use
    cerr << "error opening file " << filename << endl;
    // and exit
    exit(-1);
  }

  // while there is more stuff in the file and the streams aren't closed
  while(!input.eof() && !inflate.closed() && !decode.closed()) {
    // read a line
    getline(input, line);
    // deocde the read line
    decode << line;
    // inflate the line
    inflate << decode.dread();
    // and print the inflated stuff
    cout << inflate.dread();
  }
}

// read from stdin
void read_input(cmc::Inflate& inflate, cmc::Decode86& decode) {
  // a single line of data
  string line;

  // if stdin has errors
  if(!cin.good()) {
    // tell the user
    cerr << "error opening stdin" << endl;
    // and exit
    exit(-1);
  }

  // whiel there is still more input and the streams aren't closed
  while(!cin.eof() && !decode.closed() && !inflate.closed()) {
    // get a line
    getline(cin, line);
    // decode the line
    decode << line;
    // inflat the line
    inflate << decode.dread();
    // and write to cout
    cout << inflate.dread();
  }
}
