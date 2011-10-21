/*  File: mops.hpp
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
#include "cmatrix.hpp"
#include "ascii86.hpp"
#include "deflate.hpp"
#include "util.hpp"

namespace cmc {
	// bloody mary
	typedef std::vector<CMatrix> marry;

	/* OLD CODE - TO BE REPLACED WITH OBJECT ORIENTED CODE
	// turn string into list of pass matrices
	marry makePassMatrix(const std::string&);
	// turn string into list of mesg matrices
	marry makeMesgMatrix(const std::string&);

	// encrypt the mesg and pass matrices
	marry encryptMatrices(marry mesg, const marry& pass);
	// decrypt the mesg and pass matrices
	marry decryptMatrices(marry mesg, const marry& pass);

	// turn two encrpyted matrices into a string with 17 chars
	std::string twoMatricesToString(const CMatrix& m1, const CMatrix& m2);

	// turn one encrypted matrix into a string
	std::string oneMatrixToString(const CMatrix& mat);

	// turn that encrypted shiba into a string
	std::string crypedMatrixToString(const marry& crypt);

	std::string matrixEncrypt(const std::string& mesg, const std::string& pass);
	*/

	// this will be a bitch to code...
	class Encrypter {
	  private:
			/* input buffer
			 * writing to this class will fill up this buffer, and the encrypt
			 * function will take as much data from this buffer as it can encrypt
			 * and leaves the rest for the next encryption call. this buffer is
			 * cleared if the input is closed (with close())
			 */
	    std::string inbuf;

			/* output buffer
			 * this will get filled up by encrypt(). it can be accessed with data(),
			 * which returns the whole content of this buffer, or with read(),
			 * which remembers how much it read and, on further calls, only returns
			 * new data. it is recommended however to use dread(), which works in 
			 * the same way as read(), but it clears the output buffer
			 */
	    std::string outbuf;

	    // current position in the output buffer - needed for read() and dread()
	    mutable int outbuf_pos;

			/* password array
			 * this password array is used for encryption, it is looped thru to 
			 * encrypt each message matrix with a different password array (until
			 * all password matrices are used, then it wraps back to the beginning)
			 */
			marry passMatrices;

			// password matrix offset
	    mutable int pass_offset;

			/* delfater class
			 * this class is responsible for compressing the encrypted data
			 * to improve efficiency. it uses the DEFLATE algorithm as 
			 * implemented by zlib
			 */
	    Deflate deflater;

			/* encoding class
			 * this class encodes the encrypted data with ascii86 to make it
			 * safe for use in non-binary environments (like mail and chatting
			 * protocols)
			 */
	    Encode86 encoder;

			// keeps track of whether or not this is closed
	    bool closed_input;

			// check if the input is closed (or opened)
			void checkIfInputClosed() const;
			void checkIfInputClosed(bool input) const;

			/* initialize
			 * set everything up to be ready for encryption. this sets variables to
			 * their default, clears the boffers and all that stuff. should be called
			 * by the constructor and maybe the reset function.
			 */
			void initialize();

			/* initializePassword
			 * creates the password matrices array passMatrices
			 * from the string given as argument
			 */
			void initializePassword(std::string password);

			/* encrypt
			 * this function will try to encrypt as much data from the input buffer
			 * as possible and add it to the output buffer. this function is called
			 * by any of the input functions as well as by close().
			 */
	    void encrypt();

			/* encrypt matrix
			 * called by encrypt() to actually encrypt a matrix. it does the encryption
			 * and then adds the result to the output buffer
			 */
			void encryptMatrix(const CMatrix& mesg, const CMatrix& pass);

			/* matrix to string
			 * this function is called by encryptMatrix() to convert 2 matrices to a
			 * string representation. this also compresses them with deflate and
			 * encodes it with ascii86 before appending it to the output buffer.
			 */
			void matricesToOutbuf(const CMatrix& m1, const CMatrix& m2);

			/* addToOutbuf
			 * this function adds some (encrypted) string to the output buffer. 
			 * this is done in a seperate function to allow for special things 
			 * like XOR encryption, compression and/or encoding (the latter two
			 * are done by default)
			 */
			void stringToOutbuf(const std::string& add);

			/* get the next password matrix
			 * password matrices are stored in a vector and they are used by rotation.
			 * this function will take a password matrix from the index of the 
			 * password matrix index and then increase the index to prepare for the 
			 * next call.
			 */
			CMatrix nextPassMatrix() const;

			/* getMesgMatrices
			 * returns a matrix array of as many message matrices as can be
			 * extracted from inbuf. the number of matrices extracted is 
			 * always a multiple of 2, like that they can be turned into
			 * a string representation right away. 
			 */
			marry getMesgMatrices();
		public:
			/* constructor
			 * needs a password as argument, this is used to construct the password
			 * matrix array. 
			 */
			explicit Encrypter(std::string pass);

			// destructor
			~Encrypter();

			/* write
			 * adds data to the input buffer and calls encrypt() to encrypt it all. 
			 */
			void write(std::string data);

			/* data
			 * gets all data that is currently in the output buffer
			 */
			std::string data();

			/* close
			 * closes the input and finishes encrypting the whole input buffer.
			 * this finalizes the encryption, thus after calling this you can't
			 * add any more data.
			 */
			void close();

			// clears everything (except the passMatrices)
			void clear();
	};

	typedef Encrypter encrypter;
}
