#include "polarssl/sha4.h"
#include <string>
#include <iostream>
using namespace std;

int main() {
	// sample data
	string uh("this is raw");
	string oh(" data");

	// new sha4 context
  polarssl::sha4_context con;
	// initialize to SHA512
  polarssl::sha4_starts(&con, 0);
  
	// start hashing
	polarssl::sha4_update(&con, (const unsigned char *) uh.c_str(), uh.size());
	// and do some more hashing
	polarssl::sha4_update(&con, (const unsigned char *) oh.c_str(), oh.size());

	// char array to store the output
  char output[64];
  polarssl::sha4_finish(&con, (unsigned char *) output);

	string hash(output, 64);
	cout << hash << endl;
}
