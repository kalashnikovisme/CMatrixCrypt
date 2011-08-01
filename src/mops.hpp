#include <iostream>
#include <string>
#include <vector>
#include "cmatrix.hpp"
#include "ascii86.hpp"
#include "deflate.hpp"
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

  // this will be a bitch to code...
  class Encrypt {
    private:
      // input buffer
      std::string input;
      // output buffer
      std::string output;
      // current position in the output buffer
      int output_pos;

      // exporting classes
      Deflate deflate;
      Encode86 encode;

      // encryption code
      void encrypt();
      marry getMesgMatrices();
      marry getPassMatrices();
      marry doMatrixEncryption(marry& mesg, const marry& pass);
      std::string matricesToString(const marry& matrices);

      // status tools
      bool closed;
      int pass_offset;
  }
}
