/*  File: sha512_tests.cpp
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
#include <cassert>
#include <stdexcept>
#include "sha512.hpp"

using namespace cmc;
using std::cout;
using std::cin;
using std::endl;
using std::string;

void test_basic_hashing();
void test_hashing_inputs();
void test_hashing_outputs();
void test_hashing_comparisons();
void test_hashing_errors();

string reference_hash(string d) {
	char h[64];
	polarssl::sha4( (unsigned char *) d.c_str(), d.size(), (unsigned char *) h, 0);
	return(string(h, 64));
}

int main() {
	// start timer
  clock_t start = clock();
	
	// testing code
	cout << "testing basic hashing" << endl;
	test_basic_hashing();
	cout << "testing hashing inputs" << endl;
	test_hashing_inputs();
	cout << "testing hashing outputs" << endl;
	test_hashing_outputs();
	cout << "testing hashing comparisons" << endl;
	test_hashing_comparisons();
	cout << "testing hashing exceptions" << endl;
	test_hashing_errors();

	// stop timer
  clock_t end = clock();
	// get difference
  double time = difftime(end, start) / CLOCKS_PER_SEC;
	// set cout to show all digits of floating point numbers
  cout.setf(std::ios_base::fixed);
	// reveal the time taken
  cout << "tests took " << time << " seconds" << endl;
}

void test_basic_hashing() {
	SHA512 hasher;
	hasher.write("vossibaer");
	hasher.close();
	assert(hasher.gethash() == reference_hash("vossibaer"));

	SHA512 tigger;
	tigger.write("honey");
	tigger.close();
	assert(tigger.gethash() == reference_hash("honey"));

	SHA512 leo;
	leo.write("lifestyle");
	leo.close();
	const unsigned char should[64] = 
	{	0xc0, 0x25, 0xb1, 0x2c, 0x03, 0xa6, 0xea, 0x2d, 
		0xe6, 0x76, 0x00, 0xf5, 0x79, 0x25, 0xb0, 0x60, 
		0xdd, 0x70, 0xa5, 0xbd, 0x02, 0xb1, 0x4f, 0x39, 
		0x7f, 0x4e, 0x69, 0x95, 0xec, 0xaa, 0xd4, 0x4c, 
		0xfc, 0x66, 0xaf, 0xc8, 0xea, 0xcd, 0x61, 0x55, 
		0x1f, 0x00, 0x93, 0x7c, 0x03, 0x31, 0x83, 0xff, 
		0x30, 0xa8, 0xb3, 0xe5, 0xbd, 0x45, 0xaf, 0x2c, 
		0x1b, 0x56, 0xbd, 0xf0, 0x44, 0xf1, 0x58, 0x95,	};
	unsigned char current[64];
	leo.gethash(current);

	for(int i = 0; i < 64; ++i) {
		assert(should[i] == current[i]);
	}
}

void test_hashing_inputs() {
	// standard way of hash input
	SHA512 h1;
	h1.write("gucci");
	h1.close();
	assert(h1.gethash() == reference_hash("gucci"));

	// input per constructor
	SHA512 h2("gucci");
	h2.close();
	assert(h2.gethash() == reference_hash("gucci"));

	// input per operator <<
	SHA512 h3;
	h3 << "gucci";
	h3.close();
	assert(h3.gethash() == reference_hash("gucci"));
}

void test_hashing_outputs() {
	SHA512 hash("gucci");
	hash.close();
	assert(hash.gethash() == reference_hash("gucci"));
	assert(hash.hexdigest() == "f04b44d9f3928f27a44d6d51d40dc8fe4069e2e2d8e9350475c0eaa93dc872f8d8da535d6e34a100831105a0aeb9eb5299d9f04d11e81169da2b7e2ad5dee766");
	unsigned char hsum[64];
	hash.gethash(hsum);
	const unsigned char* ssum = (const unsigned char*)reference_hash("gucci").c_str();

	for(int i = 0; i < 64; ++i) {
		assert(hsum[i] == ssum[i]);
	}
}

void test_hashing_comparisons() {
	SHA512 valid;
	valid.write("monk");
	valid.close();

	assert(valid == "83d558db35b474e2e930f6866583bb166fd497dc310d044edcffd8e2e656fce8f2f2564fafc202635e08a9504348fc4932e2e0d65de2c536922096eb82302032");
	assert(valid == reference_hash("monk"));
	SHA512 helper;
	helper.write("monk");
	helper.close();
	assert(valid == helper);

	SHA512 invalid;
	invalid.write("m0nk");
	invalid.close();
	assert(invalid != "83d558db35b474e2e930f6866583bb166fd497dc310d044edcffd8e2e656fce8f2f2564fafc202635e08a9504348fc4932e2e0d65de2c536922096eb82302032");
	assert(invalid != reference_hash("monk"));
	assert(invalid != helper);
}

void test_hashing_errors() {
	bool caught_exception = false;
	SHA512 test;
	test << "hello";
	string hash;

	try {
		hash = test.gethash();
	} catch(std::runtime_error e) {
		caught_exception = true;
	}

	assert(caught_exception);

	test.close();
	caught_exception = false;

	try {
		test << "more";
	} catch(std::runtime_error e) {
		caught_exception = true;
	}

	assert(caught_exception);
}
