#include "mops.hpp"
using namespace std;

marry makePassMatrix(const string pass) {
  // matrices array
  marry mats;
  mats.reserve((pass.size()/4)+1); // tell mats an estimate of how much space we're going to need
  // temp matrix
  CMatrix mat;
  // iterate thru chars of string
  for(int i = 0; i < pass.size(); i+=4) {
    mat(0,0) = pass[i+0];  // fill up matrix
    mat(0,1) = pass[i+1];  // these chars may or may not exist!!!
    mat(1,0) = pass[i+2];  // we are relying on the standard behavior of std::string to return 0 for an inexisting element. 
    mat(1,1) = pass[i+3];
    mat.escape();         // escape matrix
    mats.push_back(mat);  // append matrix to array
  }

  // return matrices
  return(mats);
}


marry makeMesgMatrix(const string msg) {
  // append to the mesg to denote end
  string mesg = msg + ";";
  // matrices array
  marry mats;
  mats.reserve((mats.size()/4)+1); // tell mats about how much space we'll need
  // temp matrix
  CMatrix mat;
  // iterate thru chars of string
  for(int i = 0; i < mesg.size(); i+=4) {
    mat(0,0) = mesg[i+0];  // fill up matrix
    mat(0,1) = mesg[i+1];  // these chars of mesg may or may not exist!!!
    mat(1,0) = mesg[i+2];
    mat(1,1) = mesg[i+3];
    mats.push_back(mat);   // append matrix to array
  }

  // return matrices
  return(mats);
}

// encrypt marry mesg with marry pass
marry encryptMatrices(const marry mesg, const marry pass) {
  // marry to hold the result
  marry result;
  // make space to hold all the stuff we need to
  result.reserve(mesg.size());

  // now the fun part: let's crypt :D
  for(int i = 0; i < mesg.size(); ++i) {                // loop thru mesg
    result.push_back(mesg[i] * pass[i % pass.size()]);  // multiply mesg with a pass matrix
  }

  // whoa, we're done!
  return(result);
}

// decrypt marry mesg with marry pass
marry decryptMatrices(const marry mesg, const marry pass) {
  // marry to hold the result
  marry result;
  // make space to hold all the stuff we need to
  result.reserve(mesg.size());

  // now the fun part: let's decrypt :D
  for(int i = 0; i < mesg.size(); ++i) {                          // loop thru mesg
    result.push_back(mesg[i] * pass[i % pass.size()].inverse());  // multiply mesg with a pass matrix's inverse
  }

  // whoa, we're done!
  return(result);
}

// turn two matrices into a string representation
string twoMatricesToString(const CMatrix m1, const CMatrix m2) {
  // the conversion temporarily converts the numbers in the matrices into a base 2 representation
  // this will be the buffer and the result
  std::string bin_tmp, result;

  // tell both strings how much space to allocate
  bin_tmp.reserve(136);
  result.reserve(17);

  // fill up the temp string
  bin_tmp += intToBinString(m1.get(0,0), 17);
  bin_tmp += intToBinString(m1.get(0,1), 17);
  bin_tmp += intToBinString(m1.get(1,0), 17);
  bin_tmp += intToBinString(m1.get(1,1), 17);
  bin_tmp += intToBinString(m2.get(0,0), 17);
  bin_tmp += intToBinString(m2.get(0,1), 17);
  bin_tmp += intToBinString(m2.get(1,0), 17);
  bin_tmp += intToBinString(m2.get(1,1), 17);
  cout << bin_tmp << endl;
  // convert it into a string
  for(int i = 0; (i+8) <= 136; i+=8) {
    result.push_back((char)binStringToInt(bin_tmp.substr(i, 8)));
  }

  return(result);
}
