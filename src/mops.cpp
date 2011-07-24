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


marry makeMesgMatrix(string mesg) {
  // append to the mesg to denote end
  mesg += ";";
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
