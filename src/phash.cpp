#include "phash.hpp"
using namespace std;

int main(int argc, char *argv[]) {
  string orig = "This could be your password text.";
  if(argc > 1) {
    orig = string(argv[1]);
  }
  string str(orig);
  int amount = str.size();
  if(argc > 2) {
    amount = atoi(argv[2]);
  }

  string phash;  
  unsigned char tmp;
  for(int i = 0; i < amount; ++i) {
    tmp = str[i % str.size()];

    for(int j = 1; j <= 7; ++j) {
      tmp ^= bitrotr(str[(i+j) % str.size()], j);
    }

    str[i % str.size()] = tmp;
    phash.push_back(tmp);
  }

  for(int i = 0; i < amount; ++i) {
    cout << int((unsigned char) orig[i % orig.size()]) << " => " << int((unsigned char) phash[i % phash.size()]) << endl;
  }
}
