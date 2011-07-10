#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <string>
using namespace std;

// PROTOTYPES
string encode86(string data); //encodes a string of any length
string decode86(string code); //decodes a string of any length
string encode86_tuple(string tuple); //encodes a tuple (of 4 characters)
string decode86_tuple(string tuple); //decodes a tuple (of 5 characters)

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
