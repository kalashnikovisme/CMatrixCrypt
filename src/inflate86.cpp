#include <string>
#include <iostream>
#include <fstream>
#include "deflate.hpp"
#include "ascii86.hpp"
using namespace std;

// decompress a file
void read_file(cme::Inflate&, cme::Decode86&, string filename);
// decompress stdin
void read_input(cme::Inflate&, cme::Decode86&);

int main(int argc, char *argv[]) {
  // object to inflate (decompress)
  cme::Inflate inflate;
  // decode object
  cme::Decode86 decode;

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
void read_file(cme::Inflate& inflate, cme::Decode86& decode, string filename) {
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
void read_input(cme::Inflate& inflate, cme::Decode86& decode) {
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
