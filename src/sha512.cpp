/*  File: sha512.cpp
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

#include "sha512.hpp"

namespace cmc {
	void SHA512::sha4_starts(int is384) {
		if(closed_input) {
			throw std::runtime_error("sha4 input already closed");
		} else {
			polarssl::sha4_starts(&sha4_ctx, (is384) ? 1 : 0);
		}
	}

	void SHA512::sha4_update(const unsigned char* input, size_t ilen) {
		if(closed_input) {
			throw std::runtime_error("sha4 input already closed");
		} else {
			polarssl::sha4_update(&sha4_ctx, input, ilen);
		}
	}

	void SHA512::sha4_finish() {
		if(closed_input) {
			throw std::runtime_error("sha4 input already closed");
		} else {
			polarssl::sha4_finish(&sha4_ctx, hash);
		}
	}

	void SHA512::close() {
		sha4_finish();
		closed_input = true;
	}

	bool SHA512::closed() const {
		return(closed_input);
	}

	SHA512::SHA512() {
		closed_input = false;
		sha4_starts(0);
	}

	SHA512::SHA512(std::string data) {
		closed_input = false;
		sha4_starts(0);
    write(data);
	}

	SHA512::~SHA512() {
		if(!closed_input) {
			close();
		}
	}

	void SHA512::write(std::string input) {
		sha4_update( (const unsigned char *) input.c_str(), input.size() );
	}

	void SHA512::gethash(unsigned char data[64]) const {
		// check if the input is closed (and thus the hash is available)
		if(!closed_input) {
			throw std::runtime_error("sha4 input not closed yet");
		} else {
			// copy the hash into the array
			for(int i = 0; i < 64; ++i) {
				data[i] = hash[i];
			}
		}
	}

	std::string SHA512::gethash() const {
		// check if the input is closed (and thus the hash is available)
		if(!closed_input) {
			throw std::runtime_error("sha4 input not closed yet");
		} else {
			// package the hash as a string and return
			return(std::string( (char *) hash, 64));
		}
	}

	std::string SHA512::hexdigest() const {
		// check if the input is closed (and thus the hash is available)
		if(!closed_input) {
			throw std::runtime_error("sha4 input not closed yet");
		} else {
			// we'll store the hexdigest in this string
			std::string hex;
			// sha512 hexdigests always have 128 characters (64 bytes, 2 chars per byte)
			hex.reserve(128);

			// loop thru all bytes
			for(int i = 0; i < 64; ++i) {
				// there are two chars per byte
				char low, high;

				// extract the last four bits (00001111)
				low = hash[i] & 0x0F;
				// extract the first four bits (11110000)
				high = hash[i] >> 4;

				// we now have numbers between 0 and 16 that need to be turned into characters
				// +48 turns them into 0-9
				low+=48;
				if(low > '9') {
					// if they are bigger than 9, add another 39 for anything that is between a and f
					low+=39;
				}

				// same here: turn them into digits (0..9)
				high+=48;
				if(high > '9') {
					// and if not a digit but rather a letter (a..f), add another 39
					high+=39;
				}

				// add them to the string, high char first
				hex.push_back(high);
				hex.push_back(low);
			}

			// return the resulting string
			return(hex);
		}
	}

	bool SHA512::compare(const std::string ohash) const {
		if(!closed_input) {
			throw std::runtime_error("sha4 input not closed yet");
		}

		switch(ohash.size()) {
			case 64:
				return(this->gethash() == ohash);
				break;
			case 128:
				return(this->hexdigest() == ohash);
				break;
			default:
				throw std::runtime_error("invalid hash provided for comparison");
		}
	}

	bool SHA512::compare(const unsigned char data[64]) const {
		if(!closed_input) {
			throw std::runtime_error("sha4 input not closed yet");
		}

		for(int i = 0; i < 64; ++i) {
			if(hash[i] != data[i]) {
				return false;
			}
		}

		return true;
	}

	std::ostream& operator<<(std::ostream& out, const SHA512& h) {
		out << h.hexdigest();
		return(out);
	}

	std::istream& operator>>(std::istream& in, SHA512& h) {
		std::string line;
		std::getline(in, line);
		h.write(line);
		return(in);
	}

	SHA512& SHA512::operator>>(std::string& str) {
		str = this->hexdigest();
		return(*this);
	}

	SHA512& SHA512::operator<<(const std::string& str) {
		this->write(str);
		return(*this);
	}

	bool SHA512::operator==(const SHA512& other) {
		unsigned char ohash[64];
		other.gethash(ohash);
		return(this->compare(ohash));
	}
	
	bool SHA512::operator==(const std::string& ohash) {
		bool same;
		try {
			same = this->compare(ohash);
		} catch (std::runtime_error& e) {
			same = 0;
		}
		return(same);
	}

	bool SHA512::operator==(const unsigned char data[64]) {
		return(this->compare(data));
	}

	bool SHA512::operator!=(const SHA512& other) {
		return( !(*this == other) );
	}

	bool SHA512::operator!=(const std::string& ohash) {
		return( !(*this == ohash) );
	}

	bool SHA512::operator!=(const unsigned char data[64]) {
		return( !(*this == data) );
	}
}
