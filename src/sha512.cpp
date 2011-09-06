#include "sha512.hpp"

namespace cme {
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
			sha4_update(&sha4_ctx, input, ilen);
		}
	}

	void SHA512::sha4_finish() {
		if(closed_input) {
			throw std::runtime_error("sha4 input already closed");
		} else {
			sha4_finish(&sha4_ctx, hash);
		}
	}

	void SHA512::close() {
		sha4_finish();
		closed_input = true;
	}

	bool SHA512::closed() {
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

	void SHA512::write(std::string& input) {
		sha4_update( (const unsigned char *) input.c_str(), input.size() );
	}

	void SHA512::hash(unsigned char data[64]) {
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

	std::string SHA512::hash() {
		// check if the input is closed (and thus the hash is available)
		if(!closed_input) {
			throw std::runtime_error("sha4 input not closed yet");
		} else {
			// package the hash as a string and return
			return(string( (char *) hash, 64));
		}
	}

	std::string SHA512::hexdigest() {
		// check if the input is closed (and thus the hash is available)
		if(!closed_input) {
			throw std::runtime_error("sha4 input not closed yet");
		} else {
			// declare a stringstream to hold the hex data
			std::stringstream hexdigest;
			// tell it that we want hex stuff
			hexdigest << std::hex;

			// add all digest digits
			for(int i = 0; i < 64; ++i) {
				hexdigest << hash[i];
			}

			// return the resulting string
			return(hexdigest.str());
		}
	}

	bool compare(const std::string ohash) {
		if(!closed_input) {
			throw std::runtime_error("sha4 input not closed yet");
		}

		switch(ohash.size()) {
			case 64:
				return(this->hash() == ohash);
				break;
			case 128:
				return(this->hexdigest() == ohash);
				break;
			default:
				throw std::runtime_error("invalid hash provided for comparison");
		}
	}

	bool compare(const unsigned char data[64]) {
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

}
