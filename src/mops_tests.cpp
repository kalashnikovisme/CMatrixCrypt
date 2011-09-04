#include <iostream>
#include <string>
#include <ctime>
#include <cassert>
#include "mops.hpp"
using namespace cme;
using std::cout;
using std::cin;
using std::endl;

// all tests
void test_encryption();

int main(void) {
	// start timer
  clock_t start = clock();
	
	// testing code
	test_encryption();

	// stop timer
  clock_t end = clock();
	// get difference
  double time = difftime(end, start) / CLOCKS_PER_SEC;
	// set cout to show all digits of floating point numbers
  cout.setf(std::ios_base::fixed);
	// reveal the time taken
  cout << "tests took " << time << " seconds" << endl;
}

// test if encryption works
void test_encryption() {
	Encrypter enc("pw");
	enc.write("fun stuff");
	enc.close();
	enc.debug();
}
