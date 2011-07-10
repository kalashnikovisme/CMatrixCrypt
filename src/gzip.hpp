#pragma once

#include <string>
#include <stdexcept>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <zlib.h>

std::string compress_string(const std::string& str, int compressionlevel /*= Z_BEST_COMPRESSION*/);
std::string decompress_string(const std::string& str);
