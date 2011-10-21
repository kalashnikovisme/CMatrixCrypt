/*  File: mops_tests.cpp
 *  
 *  Copyright (C) 2011, Patrick M. Elsen
 *
 *  This file is part of CMatrixCrypt (http://github.com/xfbs/CMatrixCrypt)
 *  Author: Patrick M. Elsen <pelsen.vn (a) gmail.com>
 *
 *  All rights reserved.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License along
 *  with this program; if not, write to the Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <iostream>
#include <string>
#include <ctime>
#include <cassert>
#include "mops.hpp"
#include "deflate.hpp"
#include "ascii86.hpp"

using namespace cmc;
using std::string;
using std::cout;
using std::cin;
using std::endl;

// testing encryption
void test_encryption_short_input();
void test_encryption_short_pass();
void test_encryption_medium_input();
void test_encryption_long_input();
void test_encryption();
void test_inputs();
void test_outputs();

int main(void) {
	// start timer
  clock_t start = clock();
	
	// testing code
	test_encryption_short_pass();

	// stop timer
  clock_t end = clock();
	// get difference
  double time = difftime(end, start) / CLOCKS_PER_SEC;
	// set cout to show all digits of floating point numbers
  cout.setf(std::ios_base::fixed);
	// reveal the time taken
  cout << "tests took " << time << " seconds" << endl;
}

// decode the encoded output
string inflate86(string data) {
	cout << data << endl;
	// decoder/inflater objects
	Decode86 decoder;
	Inflate inflater;
	// feed the decoder object with data
	decoder.write(data);
	// feed the data to the inflater
	inflater.write(decoder.dread());
	// close the inflating
	inflater.close();
	// return the result
	return(inflater.dread());
}

// compare the encrypted output with reference data
bool compare_encrypted(string encrypted, const unsigned char* data, size_t size) {
	string d( (const char*) data, size);
	cout << encrypted << endl;
  return(d == inflate86(encrypted));
}

void test_encryption_short_pass() {
	const unsigned char data1[17] = {0x0c, 0xd8, 0x02, 0xa3, 0x03, 0x36, 0x00, 0xa8, 0xc0, 0x54, 0xd0, 0x0b, 0x10, 0x00, 0x00, 0x00, 0x00};
	const unsigned char data2[17] = {0x0c, 0xd8, 0x06, 0xc1, 0x83, 0x36, 0x01, 0xb0, 0x60, 0x54, 0xd0, 0x2d, 0x2c, 0x00, 0x00, 0x00, 0x00};
	bool correct = true;
	Encrypter enc1(".");
	enc1.write("roro");
	enc1.close();
	cout << enc1.data() << endl;
	correct = compare_encrypted(enc1.data(), data1, 17);
	assert(correct);

	Encrypter enc2(".1");
	enc2.write("roro");
	enc2.close();
	correct = compare_encrypted(enc2.data(), data2, 17);
	assert(correct);
}
