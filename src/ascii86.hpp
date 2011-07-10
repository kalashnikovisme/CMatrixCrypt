#pragma once

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
