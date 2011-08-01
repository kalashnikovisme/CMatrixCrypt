#include <string>
#include <iostream>
#include <fstream>
#include "deflate.hpp"
#include "ascii86.hpp"
using namespace std;

void read_file(cme::Deflate&, cme::Encode86&, string filename);
void read_input(cme::Deflate&, cme::Encode86&);

int main(int argc, char *argv[]) {
  if(argc < 2) {
    cout << "please specify a file to compress as argument" << endl;
    return(-1);
  }

  cme::Deflate deflate;
  cme::Encode86 encode;
  string file(argv[1]);

  if(file == "-") {
    read_input(deflate, encode);
  } else {
    read_file(deflate, encode, file);
  }

  deflate.close();
  encode << deflate.read();
  encode.close();
  cout << encode.read();
  cout << endl;
}

void read_file(cme::Deflate& deflate, cme::Encode86& encode, string filename) {
  ifstream input(filename.data());
  string line;

  if(!input.good()) {
    cout << "error opening file " << filename << endl;
    exit(-1);
  }

  while(!input.eof()) {
    getline(input, line);
    deflate << line;
    encode << deflate.read();
    cout << encode.read();
  }
}

void read_input(cme::Deflate& deflate, cme::Encode86& encode) {
  string line;

  if(!cin.good()) {
    cout << "error opening stdin" << endl;
    exit(-1);
  }

  while(!cin.eof()) {
    getline(cin, line);
    deflate << line;
    encode << deflate.read();
    cout << encode.read();
  }
}
