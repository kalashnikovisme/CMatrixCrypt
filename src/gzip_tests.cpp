#include <iostream>
#include <string>
#include <cassert>
#include "gzip.hpp"
using namespace std;

void deflate_tests();
void deflate_test_words();
void deflate_test_sentences();
void deflate_test_input();
void deflate_test_output();
void deflate_test_constructor();
void deflate_test_misc();

// shorter alias to decompress
string inflate(string in) {
  return(decompress_string(in));
}

int main(void) {
  deflate_tests();
}

void deflate_tests() {
  cout << "test words" << endl;
  deflate_test_words();
  cout << "test sentences" << endl;
  deflate_test_sentences();
  cout << "test input" << endl;
  deflate_test_input();
  cout << "test output" << endl;
  deflate_test_output();
  cout << "test constructor" << endl;
  deflate_test_constructor();
  cout << "test misc" << endl;
  deflate_test_misc();
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
  def.reset();
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
