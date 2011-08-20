#include <iostream>
#include <string>
#pragma once

template <typename T>
T bitrotl(T value, int shift) {
  int bits = sizeof(T) * 8;
  shift = shift % bits;
  if(shift == T(0)) {
    return value;
  }
  return (value << shift) | (value >> (bits - shift));
}

template <typename T>
T bitrotr(T value, int shift) {
  int bits = sizeof(T) * 8;
  shift = shift % bits;
  if(shift == T(0)) {
    return value;
  }
  return (value >> shift) | (value << (bits - shift));
}
