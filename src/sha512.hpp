#include "polarssl/sha4.h"
#include <string>
#include <sstream>
#include <iostream>
#include <stdexcept>
#pragma once

namespace cme {
	class SHA512 {
		private:
			// used by polarssl for hashing
			polarssl::sha4_context sha4_ctx;
			// store the final hash (kinda like caching)
			unsigned char hash[64];
			// is it closed?
			bool closed_input;
			
			// sha4 wrapper functions (these access polarssl)
			void sha4_starts(int is384);
			void sha4_update(const unsigned char* input, size_t ilen);
			void sha4_finish();
		public:
			// default constructor, initializes sha4 hashing
			SHA512();
			// constructor with string argument, initializes and starts hashing the string
			SHA512(std::string);
			~SHA512();

			// add a string to the hash
			void write(std::string);

			// close the input (finishes the hash)
			void close();

			// check if it's closed
			bool closed() const;

			// get the hash as raw data
			void gethash(unsigned char data[64]) const;

			// get the hash as string
			std::string gethash() const;

			// or get it as a hex encoded string
			std::string hexdigest() const;

			// compare this hash to another hash
			bool compare(const std::string) const;
			bool compare(const unsigned char data[64]) const;

			// interaction with cout and cin
			friend std::ostream& operator<<(std::ostream&, const SHA512&);
			// read a link from istream and add it to the SHA512 hex
			friend std::istream& operator>>(std::istream&, SHA512&);

			// string input and output
			SHA512& operator<<(const std::string&);
			SHA512& operator>>(std::string&);

			// comparing hashes
			bool operator==(const SHA512&);
			bool operator==(const std::string&);
			bool operator==(const unsigned char data[64]);
			bool operator!=(const SHA512&);
			bool operator!=(const std::string&);
			bool operator!=(const unsigned char data[64]);
	};
}
