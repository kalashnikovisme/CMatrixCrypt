/*  File: cryptest.cpp
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
#include "mops.hpp"
using namespace std;
using namespace cmc;

int main(int argc, char *argv[]) {
	// test if a password was given as commandline argument
	if(argc < 2) {
		cout << "need to specify password as commandline argument" << endl;
		return -1;
	}

	// encode all input
	string pass(argv[1]);
  Encrypter enc(pass);
	while(!cin.eof()) {
		string data;
		getline(cin, data);
		if(data=="") {
			break;
		}
		enc.write(data);
		enc.write("\n");
	}
	enc.close();
	cout << enc.data() << endl;
}
