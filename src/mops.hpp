#include <iostream>
#include <string>
#include <vector>
#include "cmatrix.hpp"
#include "ascii86.hpp"
#include "gzip.hpp"
#include "util.hpp"

// bloody mary
typedef std::vector<CMatrix> marry;

// turn string into list of pass matrices
marry makePassMatrix(const std::string);
// turn string into list of mesg matrices
marry makeMesgMatrix(const std::string);
// encrypt the mesg and pass matrices
marry cryptMatrices(const marry mesg, const marry pass);
// turn that encrypted shiba into a string
std::string crypedMatrixToString(const marry crypt);
