/*  File: mops.cpp
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
#include "mops.hpp"
using namespace std;

/* OLD CODE - TO BE REPLACED
// turn a password into a matrices array usable for encryption
marry makePassMatrix(const string& pass) {
	// matrices array
	marry mats((pass.size()+3)/4);

	// copy the pass string over into marry
	for(int i = 0; i < pass.size(); ++i) {
	  mats[i/4][i%4] = pass[i];
	}

	// escape every matrix
	for(int i = 0; i < mats.size(); ++i) {
	  mats[i].escape();
	}

	// return matrices
	return(mats);
}

// turn any message into an array of matrices suitable for encryption
marry makeMesgMatrix(const string& mesg) {
	// matrices array
	marry mats((mesg.size()+4)/4);

	// copy the mesg over into the matrices
	for(int i = 0; i < mesg.size(); ++i) {
	  mats[i/4][i%4] = mesg[i];
	}

	// make the last character be a semicolon
	mats[mesg.size()/4][mesg.size()%4] = ';';

	// return matrices
	return(mats);
}

// encrypt marry mesg with marry pass
marry encryptMatrices(marry mesg, const marry& pass) {
	// now the fun part: let's crypt :D
	for(int i = 0; i < mesg.size(); ++i) {  // loop thru mesg
	  mesg[i] *= pass[i%pass.size()];       // multiply and store result in the mesg matrix array
	}

	// whoa, we're done!
	return(mesg);
}

// decrypt marry mesg with marry pass
marry decryptMatrices(marry mesg, const marry& pass) {
	// now the fun part: let's decrypt :D
	for(int i = 0; i < mesg.size(); ++i) {  // loop thru mesg
	  mesg[i] *= pass[i%pass.size()].inverse();// multiply with inverse
	}

	// whoa, we're done!
	return(mesg);
}

// turn two matrices into a string representation
string twoMatricesToString(const CMatrix& m1, const CMatrix& m2) {
	// the conversion temporarily converts the numbers in the matrices into a base 2 representation
	// this will be the buffer and the result
	string binpool, result;

	// tell both strings how much space to allocate
	binpool.reserve(136);
	result.reserve(17);

	// copy the first matrix into the binary pool
	for(int i = 0; i < 4; ++i) {
	  binpool += intToBinString(m1.get(i), 17);
	}
	// same with the second matrix
	for(int i = 0; i < 4; ++i) {
	  binpool += intToBinString(m2.get(i), 17);
	}
	// convert it into a string
	for(int i = 0; i < 17; ++i) {
	  result.push_back((char)binStringToInt(binpool.substr(i*8, 8))); // extract 8 chars and convert
	}

	// done :D
	return(result);
}

// turn only one matrix into a string
string oneMatrixToString(const CMatrix& mat) {
	CMatrix m2;
	return(twoMatricesToString(mat, m2));
}

// turn encrypted matrices into a string
string cryptedMatrixToString(const marry& crypt) {
	// place to store the result
	string result;
	// make space to store all the stuff
	result.reserve(((crypt.size()+1)/2) * 17);
	
	// convert all matrices to strings
	for(int i = 0; (i+1) < crypt.size(); i+=2) {
	  result.append(twoMatricesToString(crypt[i], crypt[i+1]));
	}

	if((crypt.size()%2) == 1) { // if there is an uneven number of matrices
	  result += oneMatrixToString(crypt[crypt.size()-1]); // add the last one
	}

	// return the beautiful result
	return(result);
}

// matrix encrypt
string matrixEncrypt(const std::string& mesg, const std::string& pass) {
	marry msg, pw, crypt; // matrix representation of the mesg and the password
	msg = makeMesgMatrix(mesg); // prepare mesg
	pw  = makePassMatrix(pass); // and pass
	crypt = encryptMatrices(msg, pw); // encrypt

	string enc = cryptedMatrixToString(crypt); // turn into string representation
	enc = compress_string(enc, Z_BEST_COMPRESSION);               // compress with gzip
	return encode86(enc);                     // encode86
}
*/ 

namespace cme {
	Encrypter::Encrypter(std::string password) {
		initialize();
		initializePassword(password);
	}

	void Encrypter::initialize() {
		outbuf_pos = 0;
		pass_offset = 0;
		inbuf.clear();
		outbuf.clear();
		encoder.reset();
		deflater.reset();
		closed_input = false;
		passMatrices.clear();
	}

	void Encrypter::initializePassword(std::string password) {
		int missing = (4-(password.size()%4))%4;	// calculates how many characters are missing until the length is a multiple of 4

		for(int i = 0; i < missing; ++i) {
			// add NULLs to the pass until it's length is a multiple of 4
			password.push_back('\0');
		}

		// for every 4 characters in password
		for(int i = 0; i < password.size(); i+=4) {
			// make a new CMatrix
			CMatrix pw;

			// which is filled up with the 4 characters
			for(int j = 0; j < 4; ++j) {
				// cast to unsigned char is necessary because we don't like signs in our matrix ;)
				pw[j] = (float)(unsigned char)password[i+j];
			}
			
			// escape the matrix (we need to be able to get it's inverse, this makes sure that this is the case)
			pw.escape();

			// add this CMatrix to the list of password matrices
			passMatrices.push_back(pw);
		}

		// what if the password has a length of 0? this means that there shouldn't be any encryption
		if(password.size() == 0) {
			// new password matrix
			CMatrix pw;

			// set it to the identity matrix
			pw[0] = pw[3] = 1;
			pw[1] = pw[2] = 0;
			// no need to call pw.escape() here because we know that the idenitity matrix doesn't need to be escaped

			passMatrices.push_back(pw);
		}
	}

	Encrypter::~Encrypter() {
	}

	// return the next password matrix to be used to encryption
	CMatrix Encrypter::nextPassMatrix() const {
		// the matrix the we want
		int requested = pass_offset;
		// increase the pass offset for the next call to nextPassMatrix()
		++pass_offset;
		// we also have to make sure that the pass_offset exists
		pass_offset %= passMatrices.size();
		// and return the requested pass matrix
		return(passMatrices[requested]);
	}

	void Encrypter::write(std::string str) {
		checkIfInputClosed();

		// add the string to the input buffer
		inbuf.append(str);
		// call encrypt to encrypt as much as possible at this point in time
		encrypt();
	}

	// return as many message matrices as can be extracted at this point
	marry Encrypter::getMesgMatrices() {
		// number or available mesg matrices
		int available = inbuf.size()/4;
		// we want an even number of mesg matrices
		available -= available%2;

		// marry to store the mesgMatrices
		marry mesgMatrices;

		// extract the matrices and add them to mesgMatrices
		for(int i = 0; i < (available*4); i+=4) {
			// new matrix
			CMatrix mesg;
			// fill matrix up
			mesg[0] = inbuf[i+0]; // okay, i could've done this as a loop...
			mesg[1] = inbuf[i+1];
			mesg[2] = inbuf[i+2];
			mesg[3] = inbuf[i+3];
			// add matrix to mesgMatrices
			mesgMatrices.push_back(mesg);
		}

		// now, delete the part of inbuf that we've just used
		inbuf.erase(0, available*4);

		return(mesgMatrices);
	}

	void Encrypter::encrypt() {
		// get the mesgMatrices that we wanna encrypt
		marry mesgMatrices = getMesgMatrices();

		// encrypt
		for(int i = 0; i < mesgMatrices.size(); ++i) {
			// multiply the mesg matrix with the next password matrix
			mesgMatrices[i] *= nextPassMatrix();
		}

		// turn the enxrypted mesgMatrices to strings (append to outbuf)
		for(int i = 0; i < mesgMatrices.size(); i+=2) {
			// turn to string and add to the output buffer
			matricesToOutbuf(mesgMatrices[i+0], mesgMatrices[i+1]);
		}
	}

	void Encrypter::close() {
		// make sure that the input isn't closed yet
		checkIfInputClosed(true);

		// append ; to the inbuf to denote then end of input
		inbuf.push_back(';');

		// caluclate how many NULLs we need to pad the inbuf with
		int missing = 8-(inbuf.size()%8);

		// pad the input until it's a multiple of 8
		for(int i = 0; i < missing; ++i) {
			inbuf.push_back('\0');
		}

		// do the last encryption cycle
		encrypt();

		// now extract the last bit of output
		deflater.close();
		encoder.write(deflater.dread());
		encoder.close();
		outbuf.append(encoder.dread());

		// aaaand we're done. almost.
		closed_input = true;
	}

	void Encrypter::checkIfInputClosed() const {
		if(closed_input) {
			throw std::runtime_error("input already closed");
		}
	}

	void Encrypter::checkIfInputClosed(bool input) const {
		if(closed_input == input) {
			if(closed_input) {
				throw std::runtime_error("input already closed");
			} else {
				throw std::runtime_error("input not closed yet");
			}
		}
	}

	std::string Encrypter::data() {
		return(outbuf);
	}

	void Encrypter::debug() {
		std::cout << "passMatrices: " << std::endl;
		for(int i = 0; i < passMatrices.size(); ++i) {
			std::cout << passMatrices[i] << std::endl;
		}
	}

	// turn two matrices into a string representation
	void Encrypter::matricesToOutbuf(const CMatrix& m1, const CMatrix& m2) {
		// the conversion temporarily converts the numbers in the matrices into a base 2 representation
		// this will be the buffer and the result FIXME: inefficient!!!
		string binpool, result;

		// tell both strings how much space to allocate
		binpool.reserve(136);
		result.reserve(17);

		// copy the first matrix into the binary pool
		for(int i = 0; i < 4; ++i) {
			binpool += intToBin(m1.get(i), 17);
		}

		// same with the second matrix
		for(int i = 0; i < 4; ++i) {
			binpool += intToBin(m2.get(i), 17);
		}

		// convert it into a string
		for(int i = 0; i < 17; ++i) {
			result.push_back((char)binToInt(binpool.substr(i*8, 8))); // extract 8 chars and convert
		}

		stringToOutbuf(result);
	}

	void Encrypter::stringToOutbuf(const std::string& add) {
		// FIXME: allow for custom treatment of the encrypted string ;)
		// add the string to the deflater (compresser) object
		deflater.write(add);
		// then take the compressed string and put it into the encoder
		encoder.write(deflater.dread());
		// take the encoded output and add it to the output buffer
		outbuf.append(encoder.dread());
	}

	void Encrypter::clear() {
		initialize();
	}
}
