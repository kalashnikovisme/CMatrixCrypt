#include <iostream>
#include <string>
#include <vector>
#include "ascii86.hpp"
#include "cmatrix.hpp"
#include "gzip.hpp"
#include "util.hpp"
#include "mops.hpp"
using namespace std;

int main(int argc, char *argv[]) {
  if(argc < 3) {
    cout << "Need 2 arguments" << endl;
    return(-1);
  }
  string mesg, pass;
  mesg = argv[1];
  pass = argv[2];
  cout << matrixEncrypt(mesg, pass) << endl;
}
