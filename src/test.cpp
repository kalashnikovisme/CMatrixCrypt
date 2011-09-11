/*  File: test.cpp
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
#include <vector>
#include "ascii86.hpp"
#include "cmatrix.hpp"
#include "gzip.hpp"
#include "util.hpp"
#include "mops.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  if(argc < 3) {
    cout << "Need 2 arguments" << endl;
    return(-1);
  }
  string mesg, pass;
  mesg = argv[1];
  pass = argv[2];
  cout << matrixEncrypt(mesg, pass) << endl;
}
