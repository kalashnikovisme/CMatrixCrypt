#include <string>
#include <iostream>
#include <fstream>
#include "deflate.hpp"
#include "ascii86.hpp"
using namespace std;

int main(int argc, char *argv[]) {
  if(argc < 2) {
    cout << "please specify a file to compress as argument" << endl;
    return(-1);
  }

  cme::Deflate deflate;
  cme::Encode86 encode;
  ifstream input(argv[1]);
  string line;

  if(!input.good()) {
    cout << "error opening file" << endl;
    return(-1);
  }

  while(!input.eof()) {
    getline(input, line);
    deflate << line;
    encode << deflate.read();
    cout << encode.read();
  }

  deflate.close();
  encode << deflate.read();
  encode.close();
  cout << encode.read();
  cout << endl;
}
