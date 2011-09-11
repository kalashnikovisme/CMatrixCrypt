/*  File: phash.cpp
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

#include "phash.hpp"

using namespace std;

int main(int argc, char *argv[]) {
  string orig = "This could be your password text.";
  if(argc > 1) {
    orig = string(argv[1]);
  }
  string str(orig);
  int amount = str.size();
  if(argc > 2) {
    amount = atoi(argv[2]);
  }

  string phash;  
  unsigned char tmp;
  for(int i = 0; i < amount; ++i) {
    tmp = str[i % str.size()];

    for(int j = 1; j <= 7; ++j) {
      tmp ^= bitrotr(str[(i+j) % str.size()], j);
    }

    str[i % str.size()] = tmp;
    phash.push_back(tmp);
  }

  for(int i = 0; i < amount; ++i) {
    cout << int((unsigned char) orig[i % orig.size()]) << " => " << int((unsigned char) phash[i % phash.size()]) << endl;
  }
}
