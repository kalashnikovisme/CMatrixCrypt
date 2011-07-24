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
  string mesg, pass;
  if(argc > 2) {
    mesg = argv[1];
    pass = argv[2];
  } else {
    cout << "need 2 arguments" << endl;
    return(-1);
  }

  marry msg, pw, crypt;

  msg = makeMesgMatrix(mesg);
  pw  = makePassMatrix(pass);

  cout << "MESG" << endl;
  for(int i = 0; i < msg.size(); ++i) {
    cout << msg[i] << endl;
  }

  cout << "PASS" << endl;
  for(int i = 0; i < pw.size(); ++i) {
    cout << pw[i] << endl;
  }
}
