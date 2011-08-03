#include <string>
#include <iostream>
#include <fstream>
#include "deflate.hpp"
#include "ascii86.hpp"
using namespace std;

// read text from file
void read_file(cme::Deflate&, cme::Encode86&, string filename);
// read text from stdin
void read_input(cme::Deflate&, cme::Encode86&);

int main(int argc, char *argv[]) {
  // class to handle the compression
  cme::Deflate deflate;
  // class to handle the encoding
  cme::Encode86 encode;

  // if there is no commandline argument
  if(argc < 2) {
    // read from stdin
    read_input(deflate, encode);
  } else { // if there is a command line argument...
    string file(argv[1]); // make a string with the command line argument
    // if said string is -
    if(file == "-") {
      // read from stdin
      read_input(deflate, encode);
    } else {
      // else, read from the specified file
      read_file(deflate, encode, file);
    }
  }

  // once done, we have to close the deflation (this means deflate flushes the output)
  deflate.close();
  // we encode the last data to be read
  encode << deflate.read();
  // close the encoding stream
  encode.close();
  // write the last encoded stuff to stdout
  cout << encode.read();
  // flush output & write newline
  cout << endl;
}

// read from file
void read_file(cme::Deflate& deflate, cme::Encode86& encode, string filename) {
  // construct an ifstream to read from the file
  ifstream input(filename.data());
  // temp string to hold a single line of the file
  string line;

  // check if the input stream is usable
  if(!input.good()) {
    // if not, say so
    cerr << "error opening file " << filename << endl;
    // and quit like a fool
    exit(-1);
  }

  // while there is more data in the file, and the encoders and deflaters aren't closed,
  while(!input.eof() && !encode.closed() && !deflate.closed()) {
    // read a line
    getline(input, line);
    // add \n to that line (because readline doesn't read the newline char
    line.push_back('\n');
    // deflate said line
    deflate << line;
    // encode it
    encode << deflate.read();
    // and output it
    cout << encode.read();
  }
}

// read from stdin
void read_input(cme::Deflate& deflate, cme::Encode86& encode) {
  // temp string to hold a single line from stdin
  string line;

  // if there is an error
  if(!cin.good()) {
    // say so
    cerr << "error opening stdin" << endl;
    // and quit like a fool
    exit(-1);
  }

  // while there's stuff in cin, and the streams aren't closed
  while(!cin.eof() && !deflate.closed() && !encode.closed()) {
    // get a line of input
    getline(cin, line);
    // add a newline char
    line.push_back('\n');
    // deflate the line
    deflate << line;
    // encode it 
    encode << deflate.read();
    // and output it
    cout << encode.read();
  }
}
