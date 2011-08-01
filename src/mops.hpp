#include <iostream>
#include <string>
#include <vector>
#include "cmatrix.hpp"
#include "ascii86.hpp"
#include "gzip.hpp"
#include "util.hpp"

namespace cme {
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
}
