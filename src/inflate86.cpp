#include <string>
#include <iostream>
#include <fstream>
#include "deflate.hpp"
#include "ascii86.hpp"
using namespace std;

void read_file(cme::Inflate&, cme::Decode86&, string filename);
void read_input(cme::Inflate&, cme::Decode86&);

int main(int argc, char *argv[]) {
  if(argc < 2) {
    cout << "please specify a file to decompress as argument" << endl;
    return(-1);
  }

  cme::Inflate inflate;
  cme::Decode86 decode;
  string file(argv[1]);

  if(file == "-") {
    read_input(inflate, decode);
  } else {
    read_file(inflate, decode, file);
  }

  inflate.close();
  cout << inflate.read();
  cout << endl;
}

void read_file(cme::Inflate& inflate, cme::Decode86& decode, string filename) {
  ifstream input(filename.data());
  string line;

  if(!input.good()) {
    cout << "error opening file " << filename << endl;
    exit(-1);
  }

  while(!input.eof() && !inflate.closed() && !decode.closed()) {
    getline(input, line);
    decode << line;
    inflate << decode.read();
    cout << inflate.read();
  }
}

void read_input(cme::Inflate& inflate, cme::Decode86& decode) {
  string line;

  if(!cin.good()) {
    cout << "error opening stdin" << endl;
    exit(-1);
  }

  while(!cin.eof() && !decode.closed() && !inflate.closed()) {
    cout << "GETLINE!!!" << endl;
    getline(cin, line);
    cout << "DECODE!!!" << endl;
    decode << line;
    cout << "INFLATE!!!" << endl;
    inflate << decode.read();
    cout << "COUTING!!!" << endl;
    cout << inflate.read();
  }
}
