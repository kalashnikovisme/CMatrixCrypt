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
	Encrypter enc1("pwandstuf");
	enc1.write("fun stuff");
	enc1.close();
	enc1.debug();

	Encrypter enc2("pwandstuff");
	enc2.write("fun stuff");
	enc2.close();
	enc2.debug();

	Encrypter enc3("pwandstufff");
	enc3.write("fun stuff");
	enc3.close();
	enc3.debug();

	Encrypter enc4("pwandstuffff");
	enc4.write("fun stuff");
	enc4.close();
	enc4.debug();

	Encrypter enc5("");
	enc5.debug();
}
