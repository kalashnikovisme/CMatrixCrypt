/*  File: deflate_tests.cpp
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
#include <ctime>
#include "deflate.hpp"
using namespace std;

void deflate_tests();
void deflate_test_words();
void deflate_test_sentences();
void deflate_test_input();
void deflate_test_output();
void deflate_test_constructor();
void deflate_test_misc();
void inflate_tests();
void inflate_test_words();
void inflate_test_sentences();
void inflate_test_input();
void inflate_test_output();
void inflate_test_misc();

// shorter alias to decompress
string inflate(string in) {
  cme::Inflate inf;
  inf.write(in);
  inf.close();
  return(inf.data());
}

string deflate(string in) {
  cme::Deflate def;
  def << in;
  def.close();
  return(def.read());
}

int main(void) {
  clock_t start = clock();
  cout.setf(ios_base::fixed);
  deflate_tests();
  inflate_tests();
  clock_t end = clock();
  double time = difftime(end, start) / CLOCKS_PER_SEC;
  cout << "tests took " << time << " seconds" << endl;
}

void deflate_tests() {
  cout << "deflate test words" << endl;
  deflate_test_words();
  cout << "deflate test sentences" << endl;
  deflate_test_sentences();
  cout << "deflate test input" << endl;
  deflate_test_input();
  cout << "deflate test output" << endl;
  deflate_test_output();
  cout << "deflate test constructor" << endl;
  deflate_test_constructor();
  cout << "deflate test misc" << endl;
  deflate_test_misc();
}

void inflate_tests() {
  cout << "inflate test words" << endl;
  inflate_test_words();
  cout << "inflate test sentences" << endl;
  inflate_test_sentences();
  cout << "inflate test input" << endl;
  inflate_test_input();
  cout << "inflate test output" << endl;
  inflate_test_output();
  cout << "inflate test misc" << endl;
  inflate_test_misc();
}

void deflate_test_words() {
  cme::Deflate def;
  def << "Harrow";
  def.close();
  assert(inflate(def.data()) == "Harrow");
  def.reset();

  def << "leapfrog";
  def.close();
  assert(inflate(def.data()) == "leapfrog");
  def.reset();
}

void deflate_test_sentences() {
  cme::Deflate def;
  def << "Harrow, harrow, sais the Korean.";
  def.close();
  assert(inflate(def.data()) == "Harrow, harrow, sais the Korean.");
  def.reset();

  def << "That's what she said.";
  def.close();
  assert(inflate(def.data()) == "That's what she said.");
  def.reset();
}

void deflate_test_input() {
  cme::Deflate def;
  def << "Hello ";
  def.write("world");
  def.close();
  assert(inflate(def.data()) == "Hello world");
  def.reset();
}

void deflate_test_output() {
  cme::Deflate def;
  string out;
  def << "Long output test";
  out += def.read();
  def.close();
  out += def.read();
  assert(inflate(out) == "Long output test");
  assert(inflate(def.data()) == "Long output test");
  def.reset(); out.clear();

  def << "longer output";
  def.close();
  while(!def.eol()) {
    out.push_back(def.get());
  }

  assert(inflate(out) == "longer output");
  def.reset(); out.clear();

  def << "Output test";
  out += def.dread();
  def.close();
  out += def.dread();
  assert(inflate(out) == "Output test");
}

void deflate_test_constructor() {
}

void deflate_test_misc() {
  cme::Deflate def;
  assert(def.closed() == false);
  def.write("asafd");
  def.close();
  assert(def.closed() == true);
  def.read();
  assert(def.eol() == true);
}

void inflate_test_words() {
  cme::Inflate inf;
  inf << deflate("Phillip");
  inf.close();
  assert(inf.data() == "Phillip");
  inf.reset();

  inf << deflate("Monkey");
  inf.close();
  assert(inf.data() == "Monkey");
}

void inflate_test_sentences() {
  cme::Inflate inf;
  inf << deflate("Harrow, harrow");
  inf.close();
  assert(inf.data() == "Harrow, harrow");
  inf.reset();

  inf << deflate("Infiniminer preceded Minecraft");
  inf.close();
  assert(inf.data() == "Infiniminer preceded Minecraft");
}

void inflate_test_input() {
  cme::Inflate inf;
  string comp;
  comp = deflate("watch out, dog poop");
  inf.write(comp.substr(0, 10));
  inf << comp.substr(10, 100);
  inf.close();
  assert(inf.data() == "watch out, dog poop");
}

void inflate_test_output() {
  cme::Inflate inf;
  string str;
  inf << deflate("testing");
  inf.close();
  assert(inf.read() == "testing");
  inf.reset();

  inf << deflate("testing");
  inf.close();
  assert(inf.data() == "testing");
  inf.reset();

  inf << deflate("testing");
  inf.close();
  while(!inf.eol()) {
    str.push_back(inf.get());
  }
  assert(str == "testing");
  inf.reset(); str.clear();

  inf << deflate("fun stuff");
  str += inf.dread();
  inf.close();
  str += inf.dread();

  assert(str == "fun stuff");
}

void inflate_test_misc() {
  cme::Inflate inf;
  assert(inf.closed() == false);
  inf << deflate("text");
  inf.close();
  assert(inf.closed() == true);
  assert(inf.read() == "text");
  assert(inf.eol() == true);
}
