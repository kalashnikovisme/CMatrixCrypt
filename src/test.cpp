#include <iostream>
#include <string>
#include <vector>
#include "ascii86.hpp"
#include "cmatrix.hpp"
#include "gzip.hpp"
#include "util.hpp"
#include "mops.hpp"
using namespace std;

int main(int argc, char *argv[]) {
  cout << matrixEncrypt("hello world", "pass") << endl;
}
