#include <iostream>
#include <string>
#include <cassert>
#include <ctime>
#include "cmatrix.hpp"
using namespace std;

void test_constructor();
void test_outputs();
void test_inputs();
void test_operations();
void test_math_operator();
void test_equality_operator();

int main(void) {
  clock_t start = clock();
  cout.setf(ios_base::fixed);
  cout << "test constructor" << endl;
  test_constructor();
  cout << "test outputs" << endl;
  test_constructor();
  cout << "test inputs" << endl;
  test_constructor();
  cout << "test operations" << endl;
  test_constructor();
  cout << "test access operator" << endl;
  test_constructor();
  cout << "test math operator" << endl;
  test_constructor();
  cout << "test equality operator" << endl;
  test_constructor();
  clock_t end = clock();
  double time = difftime(end, start) / CLOCKS_PER_SEC;
  cout << "tests took " << time << " seconds" << endl;
}

void test_constructor() {
  {
    cme::CMatrix mat;         // default constructor, 2*2 matrix, all elems are 0.0
    assert(mat.height() == 2);
    assert(mat.width() == 2);
    assert(mat[0] == 0.0);
  }
  {
    cme::CMatrix mat(3,3);
    assert(mat.height() == 3);
    assert(mat.width() == 3);
    assert(mat[0] == 0.0);
  }
  {
    cme::CMatrix mat(2,2,2.0);
    assert(mat.height() == 2);
    assert(mat.width() == 2);
    assert(mat[3] == 2.0);
  }
  /*{ FIXME: i won't compile
    float mat[2][2];
    mat[0][0] = 1.0;
    mat[0][1] = 1.0;
    mat[1][0] = 1.0;
    mat[1][1] = 1.0;
    cme::CMatrix cmat(2, 2, &mat);
    assert(cmat[0] == 1.0);
  }*/
  {
    cme::CMatrix moo;
    moo.set(0,0,1.0);
    cme::CMatrix mat(moo);
    assert(mat[0] = 1.0);
  }
}

void test_ouputs() {
  cme::CMatrix mat(2,2,1.0);
  assert(mat(0,0) == 1.0);
  assert(mat(0,1) == 1.0);
  assert(mat(1,0) == 1.0);
  assert(mat(1,1) == 1.0);
  for(int i = 0; i < mat.size(); ++i) {
    assert(mat[i] == 1.0);
  }
  assert(mat.get(0,0) == 1.0);
  assert(mat.get(0,1) == 1.0);
  assert(mat.get(1,0) == 1.0);
  assert(mat.get(1,1) == 1.0);
}

void test_inputs() {
  cme::CMatrix mat(2,2,1.0);
  mat.set(0,0, 0.0);
  mat(0,1) = 0.0;
  mat[2] = 0.0;
  mat.set(1,1, 0.0);
  for(int i = 0; i < mat.size(); ++i) {
    assert(mat[i] == 0.0);
  }
}

// FIXME: not enuff tests here
void test_operations() {
  cme::CMatrix mat(3,3);
  mat.resize(2,2);
  assert(mat.width() == 2);
  assert(mat.height() == 2);
  for(int i = 0; i < mat.size(); ++i) {
    assert(mat[i] == 0.0);
  }

  assert(mat.determinant() == 0);
  mat.escape();
  assert(mat.determinant() != 0);
  assert(mat.square() == true);
}

void test_math_operator() {
  cme::CMatrix mat;
  mat += 1;
  mat *= 4;
  mat /= 2;
  mat -= -1;
  for(int i = 0; i < mat.size(); ++i) {
    assert(mat[i] == 3.0);
  }
}

void test_equality_operator() {
  cme::CMatrix mat(2,2,3.0), ma1(2,2,2.0), ma2(2,3,2.0);
  mat -= 1.0;
  assert(mat == ma1);
  assert(mat != ma2);
}
