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
  assert(cme::intToBin(num, 12) == "001000101011");
  num = 255;
  assert(cme::intToBin(num, 10) == "0011111111");
  assert(cme::intToBin(num, 7) == "1111111");
}

void test_int_to_bin() {
  int num = 128;
  assert(cme::intToBin(num) == "10000000");
  num = 1020;
  assert(cme::intToBin(num) == "1111111100");
  num = 4;
  assert(cme::intToBin(num) == "100");
}

void test_pad_right() {
  string str = "Too short.";
  assert(cme::padRight(str, 11, ' ') == "Too short. ");
  str = "Fun times";
  assert(cme::padRight(str, 12, '.') == "Fun times...");
}

void test_pad_left() {
  string str = "server";
  assert(cme::padLeft(str, 10, ' ') == "    server");
  str = "here";
  assert(cme::padLeft(str, 5, '-') == "-here");
}
