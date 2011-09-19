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

using namespace cme;
using std::cout;
using std::cin;
using std::endl;

// testing encryption
void test_encryption_short_input();
void test_encryption_short_pass();
void test_encryption_empty_pass();
void test_encryption();

void test_inputs();
void test_outputs();

int main(void) {
	// start timer
  clock_t start = clock();
	
	// testing code

	// stop timer
  clock_t end = clock();
	// get difference
  double time = difftime(end, start) / CLOCKS_PER_SEC;
	// set cout to show all digits of floating point numbers
  cout.setf(std::ios_base::fixed);
	// reveal the time taken
  cout << "tests took " << time << " seconds" << endl;
}

// useful function: check if the encrypted data matches
void test_match(string data, size_t len, unsigned char *expected) {
	assert(data.size() == len);

	// the data is currently compressed and encoded
	Decode86 dec;
	Inflate inf;
	// write it to the decoder
	dec.write(data);
	// finish decoding
	dec.close();
	// feed that into the inflater
	inf.write(dec.dread());
	// finish inflating
	inf.close();
	// and take the result and put it back into data
	data = inf.dread();

	for(size_t pos = 0; pos < len, ++pos) {
		assert(((unsigned char) data[pos]) == expected[pos]);
	}
}
