/*  File: cmatrix_tests.cpp
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
  cout << "testing constructor" << endl;
  test_constructor();
  cout << "testing outputs" << endl;
  test_constructor();
  cout << "testing inputs" << endl;
  test_constructor();
  cout << "testing operations" << endl;
  test_constructor();
  cout << "testing access operator" << endl;
  test_constructor();
  cout << "testing math operator" << endl;
  test_constructor();
  cout << "testing equality operator" << endl;
  test_constructor();
  clock_t end = clock();
  double time = difftime(end, start) / CLOCKS_PER_SEC;
  cout << "tests took " << time << " seconds" << endl;
}

void test_constructor() {
  {
    cmc::CMatrix mat;         // default constructor, 2*2 matrix, all elems are 0.0
    assert(mat.height() == 2);
    assert(mat.width() == 2);
    assert(mat[0] == 0.0);
  }
  {
    cmc::CMatrix mat(3,3);
    assert(mat.height() == 3);
    assert(mat.width() == 3);
    assert(mat[0] == 0.0);
  }
  {
    cmc::CMatrix mat(2,2,2.0);
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
    cmc::CMatrix cmat(2, 2, &mat);
    assert(cmat[0] == 1.0);
  }*/
  {
    cmc::CMatrix moo;
    moo.set(0,0,1.0);
    cmc::CMatrix mat(moo);
    assert(mat[0] = 1.0);
  }
}

void test_ouputs() {
  cmc::CMatrix mat(2,2,1.0);
  assert(mat(0,0) == 1.0);
  assert(mat(0,1) == 1.0);
  assert(mat(1,0) == 1.0);
  assert(mat(1,1) == 1.0);
  for(size_t i = 0; i < mat.size(); ++i) {
    assert(mat[i] == 1.0);
  }
  assert(mat.get(0,0) == 1.0);
  assert(mat.get(0,1) == 1.0);
  assert(mat.get(1,0) == 1.0);
  assert(mat.get(1,1) == 1.0);
}

void test_inputs() {
  cmc::CMatrix mat(2,2,1.0);
  mat.set(0,0, 0.0);
  mat(0,1) = 0.0;
  mat[2] = 0.0;
  mat.set(1,1, 0.0);
  for(size_t i = 0; i < mat.size(); ++i) {
    assert(mat[i] == 0.0);
  }
}

// FIXME: not enuff tests here
void test_operations() {
  cmc::CMatrix mat(3,3);
  mat.resize(2,2);
  assert(mat.width() == 2);
  assert(mat.height() == 2);
  for(size_t i = 0; i < mat.size(); ++i) {
    assert(mat[i] == 0.0);
  }

  assert(mat.determinant() == 0);
  mat.escape();
  assert(mat.determinant() != 0);
  assert(mat.square() == true);
}

void test_math_operator() {
  cmc::CMatrix mat;
  mat += 1;
  mat *= 4;
  mat /= 2;
  mat -= -1;
  for(size_t i = 0; i < mat.size(); ++i) {
    assert(mat[i] == 3.0);
  }
}

void test_equality_operator() {
  cmc::CMatrix mat(2,2,3.0), ma1(2,2,2.0), ma2(2,3,2.0);
  mat -= 1.0;
  assert(mat == ma1);
  assert(mat != ma2);
}
