/*  File: util_tests.cpp
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
#include <cassert>
#include <ctime>
#include "util.hpp"

using namespace std;

void test_int_to_bin_fixed_size();
void test_int_to_bin();
void test_pad_right();
void test_pad_left();

int main() {
  clock_t start = clock();
  cout.setf(ios_base::fixed);
  cout << "testing int to bin fixed size" << endl;
  test_int_to_bin_fixed_size();
  cout << "testing int to bin" << endl;
  test_int_to_bin();
  cout << "testing pad right" << endl;
  test_pad_right();
  cout << "testing pad left" << endl;
  test_pad_left();
  clock_t end = clock();
  double time = difftime(end, start) / CLOCKS_PER_SEC;
  cout << "tests took " << time << " seconds" << endl;
}

void test_int_to_bin_fixed_size() {
  int num = 555;
  assert(cmc::intToBin(num, 12) == "001000101011");
  num = 255;
  assert(cmc::intToBin(num, 10) == "0011111111");
  assert(cmc::intToBin(num, 7) == "1111111");
}

void test_int_to_bin() {
  int num = 128;
  assert(cmc::intToBin(num) == "10000000");
  num = 1020;
  assert(cmc::intToBin(num) == "1111111100");
  num = 4;
  assert(cmc::intToBin(num) == "100");
}

void test_pad_right() {
  string str = "Too short.";
  assert(cmc::padRight(str, 11, ' ') == "Too short. ");
  str = "Fun times";
  assert(cmc::padRight(str, 12, '.') == "Fun times...");
}

void test_pad_left() {
  string str = "server";
  assert(cmc::padLeft(str, 10, ' ') == "    server");
  str = "here";
  assert(cmc::padLeft(str, 5, '-') == "-here");
}
