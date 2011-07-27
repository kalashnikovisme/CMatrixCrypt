#include "ascii86.hpp"
/*
// IMPLEMENTATIONS
// encode a tuple, which is a string of 4 characters
string encode86_tuple(string tuple) {
  string enc = "";      // encoded string
  long int int_rep = 0; // integer representation of the string, this is a LONG int so that it's min. 32bit

  long int multiplier = 256*256*256;                // turn the tuple into an int, this is, too, a LONG int
  for(int i = 0; i < tuple.length(); i++) {         // loop thru the characters
    int_rep += multiplier * (unsigned char)tuple[i];// add the byte to the int_representation. coerce this to unsigned char because 
    multiplier /= 256;                              // increase the multiplier
  }

  unsigned char chr;           // store current character
  for(int i = 0; i < 5; i++) { // loop 5 times
    chr = (int_rep%85)+33;     // extract a character
    enc.insert(0, 1, chr);     // insert the character at the beginning (the extraction process is in reverse)
    int_rep /= 85;             // prime the int for the next extraction
  }

  return(enc);                 // return the encoded tuple
}

// encode a whole string by splitting it up and encoding it's tuples
string encode86(string data) {
  string enc = "<~";                        // string to be filled up
  int padded_chars = (4-(data.size()%4))%4; // number of chars the had to be padded
  
  for(int i = 0; i < padded_chars; i++) {   // do the padding
    data.append(string("\0"));              // add NULL characters to string
  }

  for(int i = 0; i < data.size(); i+=4) {           // do the encoding
    enc.append(encode86_tuple(data.substr(i, 4)));  // encode the tuple and add it to the encoded string
  }

  enc.erase(enc.size()-padded_chars, padded_chars); // erase the superflous characters at the end
  enc.append("~>");                                 // append the arrow thingy

  return(enc);                                      // return the encoded string
}

// decode a tuple, which is a string of 5 characters
string decode86_tuple(string tuple) {
  string dec = "";      // decoded string
  long int int_rep = 0; // integer representation of the tuple, long int so that it's at least 32bit

  long int multiplier = 85*85*85*85;        // turn the tuple into an int, long so that it's at least 32bit
  for(int i = 0; i < tuple.length(); i++) { // loop thru the characters
    int_rep += (((unsigned char)tuple[i])-33) * multiplier; // add the byte to the int_representation, subtracting 33
                                                            // coerced to unsigned char to correctly multiply chars >128
    multiplier /= 85;                       // increase the multiplier
  }

  unsigned char chr;           // store current unsigned character
  for(int i = 0; i < 4; i++) { // loop 4 times
    chr = (int_rep%256);       // extract a character
    dec.insert(0, 1, chr);     // insert the character at the beginning (the extraction process is in reverse)
    int_rep /= 256;            // prime the int for the next extraction
  }

  return(dec);                 // return the encoded tuple
}

// decode a whole string by splitting it up and decoding its tuples
string decode86(string data) {
  string dec = "";                          // string to be filled up
  data.erase(0, 2);                         // erase the first 2 chars (<~)
  data.erase(data.size()-2, 2);             // erase the last 2 chars (~>)
  int padded_chars = (5-(data.size()%5))%5; // number of chars the had to be padded

  for(int i = 0; i < padded_chars; i++) {   // do the padding
    data.append(string("u"));               // add 'u' characters to string
  }

  for(int i = 0; i < data.size(); i+=5) {           // do the encoding
    dec.append(decode86_tuple(data.substr(i, 5)));  // encode the tuple and add it to the encoded string
  }

  dec.erase(dec.size()-padded_chars, padded_chars); // erase the superflous characters at the end

  return(dec);                                      // return the encoded string
}
*/

namespace cme {
  /* CONSTRUCTORS */
  Encode86::Encode86() {
    reset();
  }

  Encode86::Encode86(std::string data) {
    reset();
    write(data);
  }

  /* ENCODING FACILITIES */
  void Encode86::encode_tuple(std::string tuple) {
    std::string enc = "";      // encoded string
    long int int_rep = 0; // integer representation of the string, this is a LONG int so that it's min. 32bit

    long int multiplier = 256*256*256;                // turn the tuple into an int, this is, too, a LONG int
    for(int i = 0; i < tuple.length(); i++) {         // loop thru the characters
      int_rep += multiplier * (unsigned char)tuple[i];// add the byte to the int_representation. coerce this to unsigned char because 
      multiplier /= 256;                              // increase the multiplier
    }

    unsigned char chr;           // store current character
    for(int i = 0; i < 5; i++) { // loop 5 times
      chr = (int_rep%85)+33;     // extract a character
      enc.insert(0, 1, chr);     // insert the character at the beginning (the extraction process is in reverse)
      int_rep /= 85;             // prime the int for the next extraction
    }

    outbuf += enc;  // add the encoded stuff to the outbuffer
  }

  void Encode86::encode() {
    if(!closed_input) {
      int amount = inbuf.size()/4;
      int pos;
      for(pos = 0; pos < amount; ++pos) {
        encode_tuple(inbuf.substr(pos*4, 4));
      }
      inbuf.erase(0, amount*4);
    }
  }

  /* OUTPUT FACILITIES */
  // get a single character
  char Encode86::get() const {
    if(outpos < outbuf.size()) {
      ++outpos;
      return(outbuf[outpos]);
    } else {
      return(0);
    }
  }

  // read as much data as there is
  std::string Encode86::read() const {
    if((outbuf.size() - outpos) > 0) {
      return(outbuf.substr(outpos, (outbuf.size() - outpos)));
    } else {
      return("");
    }
  }

  std::string Encode86::data() const {
    return(outbuf);
  }

  /* INPUT FACILITIES */
  // push a single character
  void Encode86::put(char chr) {
    if(!closed_input) {
      inbuf.push_back(chr);
      encode(); // tell the class to try to encode the data we got
    }
  }

  void Encode86::write(std::string data) {
    if(!closed_input) {
      inbuf += data;
      encode();
    }
  }

  /* OPERATOR OVERLOADS */
  // write to a string
  Encode86& Encode86::operator>>(std::string& out) {
    close();
    out += read();
    return(*this);
  }

  Encode86& Encode86::operator<<(const std::string& in) {
    if(!closed_input) {
      write(in);
      encode();
    }
    return(*this);
  }

  /* OTHER STUFF */
  // check if the input is closed
  bool Encode86::closed() const {
    return(closed_input);
  }

  // close the input
  void Encode86::close() {
    if(!closed_input) {
      encode();             // encode the rest of the stuff in the input buffer
      closed_input = true;  // prevent new input from coming in
      
      int appendixes = (inbuf.size() % 4);  // find out how many characters are missing
      if(appendixes != 0) {
        for(int i = 0; i < (4-appendixes); ++i) { 
          inbuf.push_back('\0');              // add the null characters
        }

        // encode the last four characters
        encode_tuple(inbuf.substr(inbuf.size()-4, 4));

        for(int i = 0; i < (4-appendixes); ++i) { // remove the unneeded characters
          outbuf.erase(outbuf.size()-1, 1);
        }
      }

      // reset inbuf
      inbuf = "";

      // finish outbuf
      outbuf += "~>";
    }
  }

  void Encode86::reset() {
    closed_input = false;
    inbuf = "";
    outbuf = "<~";
    outpos = 0;
  }

  void Encode86::debug() {
    std::cout << "inbuf: " << inbuf << std::endl;
    std::cout << "outbuf: " << outbuf << std::endl;
    std::cout << "outpos: " << outpos << std::endl;
    std::cout << "closed: " << closed_input << std::endl;
  }
}
