#include "mops.hpp"
using namespace std;

// turn a password into a matrices array usable for encryption
marry makePassMatrix(const string& pass) {
  // matrices array
  marry mats((pass.size()+3)/4);

  // copy the pass string over into marry
  for(int i = 0; i < pass.size(); ++i) {
    mats[i/4][i%4] = pass[i];
  }

  // escape every matrix
  for(int i = 0; i < marry.size(); ++i) {
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
marry encryptMatrices(const marry mesg, const marry& pass) {
  // now the fun part: let's crypt :D
  for(int i = 0; i < mesg.size(); ++i) {  // loop thru mesg
    mesg[i] *= pass[i%pass.size()];       // multiply and store result in the mesg matrix array
  }

  // whoa, we're done!
  return(mesg);
}

// decrypt marry mesg with marry pass
marry decryptMatrices(const marry mesg, const marry& pass) {
  // now the fun part: let's decrypt :D
  for(int i = 0; i < mesg.size(); ++i) {  // loop thru mesg
    mesg *= pass[i%pass.size()].inverse();// multiply with inverse
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
    binpool += intToBinString(m1[i], 17);
  }
  // same with the second matrix
  for(int i = 0; i < 4; ++i) {
    binpool += intToBinString(m2[i], 17);
  }
  // convert it into a string
  for(int i = 0; i < 17; ++i) {
    result.push_back((char)binStringToInt(bin_tmp.substr(i*8, 8))); // extract 8 chars and convert
  }

  // done :D
  return(result);
}

// turn encrypted matrices into a string
string cryptedMatrixToString(const marry& crypt) {
  // place to store the result
  string result;
  // make space to store all the stuff
  result.reserve(((crypt.size()+1)/2) * 17);
  
  for(int i = 0; (i+1) < crypt.size(); i+=2) {
    result.push_back(twoMatricesToString(crypt[0], crypt[1]));
  }

  if((crypt.size()%2) == 1) {
    CMatrix mat(2,2,0);
    result += twoMatricesToString(crypt[crypt.size()-1], mat);
  }

  return(result);
}

